#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <string>
#include <vector>
using namespace cv;
using namespace std;

void findRoad();
void MouseLines(int,int,int,int,void*);
void dist(int,int,int,int,void*);
void getLDzones();
void drawLDzones();
void LowPass();
void getCars();
void drawCars();
void findFramePos();
void getCarColor(int, int, int, bool);
bool checkRoadPosition();
void refreshDebug( int, void* );

Mat image, imgray, imhsv, edges, road, carBlocks, debugim, temp, roadcalib, m, toDisplay, last, dil, dd;
int cannymin=10, cannymax=200, key=32, windDiv=2, h=30, carsCount=0, ncars=0,lastcount=0;
int rbs=60, rstep=60, roadTh=90;
int percBlacks=25, band=20;
int roadBorder[2][1080];
int LDzones[2][1080];
int LP[2][1080];
Point p1;
char filepath[200];
clock_t begin,end;
bool refresh=1, debug=1, verbose=0, secClick=0;
struct car
{
	int ycent;	//Car center regarding y axis
	int size;	//Car height
	bool onLeft;
	Scalar color;
};
car	found[10];
car lastfound[10];
int main(int argc, char** argv )
{
	FileStorage fr;
	int nframe=-1;
	namedWindow("Window", 1);
	namedWindow("Canny", 1);
	//Prepare Debug Windows
	if(debug)
	{
		namedWindow("EdgeDensity Graph", 1);
		namedWindow("Grelhas", 1);
		setMouseCallback("Grelhas", dist, NULL);
		moveWindow("EdgeDensity Graph", 0, 500);
		setMouseCallback("EdgeDensity Graph", MouseLines, NULL);
	}
	//Get Road Parameters Matrix
		fr.open("..//config//RoadLibrary10m.xml", cv::FileStorage::READ);
		fr["roadMatrix"] >> roadcalib;
	
	while(1)
	{
		//Get New Frame
		if(key==32) 
		{
			if(nframe!=-1)
				{
					image.copyTo(last);
					cout<<endl<<"Carros da imagem anterior: "<<endl;
					for(int i=0;i<ncars;i++)
					{
						lastfound[i]=found[i];
						cout<<"Carro "<<i<<": "<<lastfound[i].ycent<<" Color:"<<lastfound[i].color;
						if(lastfound[i].onLeft)cout<<" LEFT"<<endl;
						else cout<<" RIGHT"<<endl;
					}
					lastcount=ncars;
				}
			nframe++;
			sprintf(filepath,"../imgs/Bebop/Library_10m/frame%03d.jpg",nframe);
			image=imread(filepath);
			image.copyTo(toDisplay);
			refresh=1;
		}
		if(refresh)
		{
			begin=clock();
			cvtColor(image, imgray, CV_RGB2GRAY);
			Canny(imgray, edges, cannymin, cannymax,3);
			//Canny(imgray, edges, 200, 300,3);
			findRoad();								//Find Road and Restrict Parking Zones
			if(checkRoadPosition())					//Check Image is Alligned with road
			{
				getLDzones();						//Compute and Edges Density
				LowPass();							//Low Pass Filter on Density Array
				getCars();							//Compare Size of filtered LD zones with expected Car Size 
				drawCars();
				if(debug)
				{
					resize(dil, dd, edges.size()/3);
					imshow("Dilated", dd);
				}
			}
			else
			cout<<"ROAD NOT FOUND"<<endl;			
		
			resize(toDisplay, toDisplay, Size(image.cols/windDiv, image.rows/windDiv));
			imshow("Window", toDisplay);
			if(debug)
			{
				drawLDzones();
				line(m, Point(0,150-2*h), Point(edges.rows,150-2*h), Scalar(255,255,0));
				imshow("EdgeDensity Graph",m);
				resize(edges,dd,edges.size()/3);
				imshow ("Canny", dd);
			}
			refresh=0;
			end=clock();
			if(verbose)
			cout<<"Time to Process: "<<double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
		}
		key=waitKey(10);
	}
}

void getCarColor(int st, int end, int ncars, bool carOnLeft)
{
	int b=0, g=0, r=0, count=0, width;
	if(carOnLeft)
	{
		for(int y=st; y<end; y++)
		{
			width=roadBorder[0][y]+roadBorder[1][y];
			for(int x=roadBorder[0][y]; x<edges.cols/2 && x<width+roadBorder[0][y]; x++)
			{
				if((int)dil.at<uchar>(y,edges.cols/2-x)==0)
				{
					count++;
					b+=image.at<Vec3b>(y,edges.cols/2-x)[0];
					g+=image.at<Vec3b>(y,edges.cols/2-x)[1];
					r+=image.at<Vec3b>(y,edges.cols/2-x)[2];
				}
			}
		}
	}
	else
	{
		for(int y=st; y<end; y++)
		{
			width=roadBorder[0][y]+roadBorder[1][y];
			for(int x=roadBorder[0][y]; x<edges.cols/2 && x<width+roadBorder[0][y]; x++)
			{
				count++;
				b+=image.at<Vec3b>(y,edges.cols/2+x)[0];
				g+=image.at<Vec3b>(y,edges.cols/2+x)[1];
				r+=image.at<Vec3b>(y,edges.cols/2+x)[2];
			}
		}
	}
	if(count>0)
	{
		b/=count;	g/=count;	r/=count;	
	}
	found[ncars].color=Scalar(b,g,r);
}


void getCars()
{
	int thr=30, count=0, ignoreLimit=20, center, parkSize;
	ncars=0;
	for(int i=edges.rows/2; i<edges.rows; i++)
	{
		if(LP[0][i]>=thr)
		{
			count++;
			while(LP[0][i]>=thr && i<edges.rows)
			{ 
				count++; i++;
				if(LP[0][i]<thr)
				{
					int tmp=i;
					while(LP[0][tmp]<thr && tmp-i<edges.rows)
					tmp++;
					if(tmp-i<=ignoreLimit)
					i=tmp;
				}
			 }
		}
		center=i-count/2;
		parkSize=center*1885/10000+137;
		if(count>parkSize/3 && count<5*parkSize/3)
		{
			found[ncars].ycent=center;
			found[ncars].size=count;
			found[ncars].onLeft=1;
			getCarColor(i-count, i, ncars, 1);
			ncars++;
		}
		else if(count>=4*parkSize/3 && count<3*parkSize)
		{
			found[ncars].ycent=i-3*count/4;
			found[ncars].size=count/2;
			found[ncars].onLeft=1;
			getCarColor(i-count, i-count/2, ncars, 1);
			ncars++;
			found[ncars].ycent=i-count/4;
			found[ncars].size=count/2;
			found[ncars].onLeft=1;
			getCarColor(i-count/2, i, ncars, 1);
			ncars++;
		}
		else if(count>=parkSize*3)
		cout<<endl<<endl<<endl<<endl<<"Car Size Out Of RANGE"<<endl;
		count=0;
	}

	for(int i=edges.rows/2; i<edges.rows; i++)
	{
		if(LP[1][i]>=thr)
		{
			count++;
			while(LP[1][i]>=thr && i<edges.rows)
			{ 
				count++; i++;
				if(LP[1][i]<thr)
				{
					int tmp=i;
					while(LP[1][tmp]<thr && tmp-i<edges.rows)
					tmp++;
					if(tmp-i<=ignoreLimit)
					i=tmp;
				}
			 }
		}
		center=i-count/2;
		parkSize=center*1885/10000+137;
		if(count>parkSize/3 && count<5*parkSize/3)
		{
			found[ncars].ycent=center;
			found[ncars].size=count;
			found[ncars].onLeft=0;
			getCarColor(i-count, i, ncars, 0);
			ncars++;
		}
		else if(count>=4*parkSize/3 && count<3*parkSize)
		{
			cout<<"SAO 2 CARROS!!!!"<<endl;
			found[ncars].ycent=i-3*count/4;
			found[ncars].size=count/2;
			found[ncars].onLeft=0;
			getCarColor(i-count, i-count/2, ncars, 0);
			ncars++;
			found[ncars].ycent=i-count/4;
			found[ncars].size=count/2;
			found[ncars].onLeft=1;
			getCarColor(i-count/2, i, ncars, 0);
			ncars++;
		}
		else if(count>=parkSize*3)
		cout<<endl<<endl<<endl<<endl<<"Car Size Out Of RANGE"<<endl;
		count=0;
	}
}
//Draw and find repeated cars
void drawCars()
{
	stringstream ss;
	string str;
	bool match;
	for(int i=0; i<ncars;i++)
	{
		int top = (found[i].ycent-found[i].size/2);
		int bottom = (found[i].ycent+found[i].size/2);
		int val=0, dist=0;
		match=0;
		for(int j=0; j<lastcount;j++)
		{
			if(lastfound[j].onLeft==found[i].onLeft && lastfound[j].ycent<found[i].ycent)
			{
				val=abs(found[i].color[0]-lastfound[j].color[0]);
				val=val + abs(found[i].color[1]-lastfound[j].color[1]);
				val=val + abs(found[i].color[2]-lastfound[j].color[2]);
				dist=abs(found[i].ycent-lastfound[j].ycent);
				if(val<60 && dist<300)
				{
					match=1;
					cout<<"Repeated: LastPos:"<<lastfound[j].ycent<<" Color:"<<lastfound[j].color<<endl;
					str="Already COUNTED";
					if(found[i].onLeft) 
					{
						rectangle(toDisplay, Point(edges.cols/10, top), Point(2*edges.cols/10, bottom), Scalar(255,0,0), 20);
						putText(toDisplay, str, Point(edges.cols/10, top+50), FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0,0,255), 3);
					}
					else
					{
						rectangle(toDisplay, Point(8*edges.cols/10, top), Point(9*edges.cols/10, bottom), Scalar(255,0,0), 20);
						putText(toDisplay, str, Point(8*edges.cols/10, top+50), FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0,255,0), 3);
					} 
				}
			}
		}
		if(!match)
		{
			ss.str("");
			ss << "NEW CAR: "<<(carsCount++);
			str=ss.str();
			if(found[i].onLeft)
			{
				rectangle(toDisplay, Point(edges.cols/10, top), Point(2*edges.cols/10, bottom), Scalar(0,0,255), 20);
				putText(toDisplay, str, Point(edges.cols/10, top+50), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(0,0,255), 3);
			} 
			else
			{
				rectangle(toDisplay, Point(8*edges.cols/10, top), Point(9*edges.cols/10, bottom), Scalar(0,255,0), 20);
				putText(toDisplay, str, Point(8*edges.cols/10, top+50), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(0,255,0), 3);						
			} 
		}
	}
}	


void LowPass()
{
	for(int i=0; i<1080; i++)
	{
		if(i<2 || i>edges.rows-2)
		LP[0][i]=LDzones[0][i];
		else
		LP[0][i]=(LDzones[0][i-2]+LDzones[0][i-1]+LDzones[0][i]+LDzones[0][i+1]+LDzones[0][i+2])/5;
		
		if(i<2 || i>edges.rows-2)
		LP[1][i]=LDzones[1][i];
		else
		LP[1][i]=(LDzones[1][i-2]+LDzones[1][i-1]+LDzones[1][i]+LDzones[1][i+1]+LDzones[1][i+2])/5;
	}
}

void drawLDzones()
{
	m=Mat::zeros(150, 1080, CV_8UC3);
	for(int i=1; i<1080; i++)
	{
		line(m, Point(i-1,150-2*LP[0][i-1]), Point(i,150-2*LP[0][i]), Scalar(0,0,255));
		line(m, Point(i-1,150-2*LP[1][i-1]), Point(i,150-2*LP[1][i]), Scalar(0,255,0));	
	}
	line(m, Point(edges.rows/2,0), Point(edges.rows/2, 200), Scalar(255,255,255));
}

void getLDzones()
{	
	Mat element=getStructuringElement(MORPH_RECT,Size(5,5), Point(2,2));
	dilate( edges, dil, element, Point(-1,-1), 2);
	dil = dil|road;
	int width, count, blacks;
	for(int y=0; y<image.rows; y++)
	{
		width=roadBorder[0][y]+roadBorder[1][y];
		count=0; blacks=0;
		for(int x=roadBorder[0][y]; x<edges.cols/2 && x<width+roadBorder[0][y]; x++)
		{
			count++;
			if((int)dil.at<uchar>(y,edges.cols/2-x)==0)
			blacks++;
		}
		LDzones[0][y]=blacks*100/count;
		count=0; blacks=0;
		for(int x=roadBorder[1][y]; x<edges.cols/2 && x<width+roadBorder[1][y]; x++)
		{
			count++;
			if((int)dil.at<uchar>(y,edges.cols/2+x)==0)
			blacks++;
		}
		LDzones[1][y]=blacks*100/count;
	}
}


void findRoad()
{
	Mat roadlines = Mat::zeros(edges.rows, edges.cols, CV_8UC1);
	road = Mat::zeros(edges.rows, edges.cols, CV_8UC1);
	
	int val,count,width, leftborder, rightborder;
	
}

bool checkRoadPosition()
{
	Mat grids, disp;
	bool limit;
	Canny(imgray, grids, 200,300, 3);
	Mat element=getStructuringElement(MORPH_RECT,Size(5,5), Point(2,2));
	dilate( grids, grids, element, Point(-1,-1), 2);
	
	for(int r=0; r<grids.rows; r+=4)
	for(int c=grids.cols/2; c<grids.cols; c++)
	{
		limit=1;
		for(int l=0;l<4;l++ )
		{
			if(grids.at<uchar>(r+l, c)==255)
			limit=0;
			if(l==3 && limit)
			{
				line(toDisplay, Point(c,r), Point(c,r+l), Scalar(0,255,0), 5 );
				c=grids.cols;
			}
		}
	}
	for(int r=0; r<grids.rows; r+=4)
	for(int c=grids.cols/2; c>0; c--)
	{
		limit=1;
		for(int l=0;l<4;l++ )
		{
			if(grids.at<uchar>(r+l, c)==255)
			limit=0;
			if(l==3 && limit)
			{
				line(toDisplay, Point(c,r), Point(c,r+l), Scalar(0,0,255), 5 );
				c=0;
			}
		}
	}
	resize(grids, disp, image.size()/3);
	imshow("Grelhas", disp);
	
	int left=0,right, wrong=0,blacks;
	int mid=edges.cols/2;
	for(int y=0; y<1080; y++)
	{
		cout<<"Y:"<<y<<endl<<endl;
		left=roadcalib.at<ushort>(0,y);
		right=roadcalib.at<ushort>(1,y);
		circle(toDisplay, Point(right+edges.cols/2, y), 4, Scalar(255,0,0));
		circle(toDisplay, Point(edges.cols/2-left, y), 4, Scalar(255,0,0));
		cout<<"Left:"<<left<<"  Right:"<<right<<endl;
//		for(int x=mid-calibleft; x<mid+calibright; x+=20)
//		{
//			
//			blacks=0;
//			for(int i=x; i<(x+20); i++)
//			for(int j=y; j<(y+20); j++)
//			{
//				if((int)edges.at<uchar>(j,i)==0)
//				blacks++;
//				toDisplay.at<Vec3b>(j,i)[0]=255;
//			}
//			
//			if (blacks<300)
//			wrong++;
//		}
	}
//	if (wrong<100)
//	return 1;
//	else
	return 0;
}


void refreshDebug( int, void* )
{
	refresh=1;
}

void MouseLines(int event, int x, int y, int flags, void* userdata)
{
		if  ( event == EVENT_LBUTTONDOWN)
		{
			image.copyTo(toDisplay);
			cout<<"X:"<<x<<"  Y:"<<y<<endl;
			cout<<"Left (red)"<<LP[0][x]<<endl;
			cout<<"Right (green)"<<LP[1][x]<<endl;
			line(toDisplay, Point(0, x), Point(edges.cols, x), Scalar(255,0,0), 2);
			refresh=1;
		}
}
void dist(int event, int x, int y, int flags, void* userdata)
{
	if  ( event == EVENT_LBUTTONDOWN)
	cout<<"X:"<<x<<" :Y"<< y <<endl;
}
