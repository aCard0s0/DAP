
//	Find Cars Contours
//			Algorithm to detect cars using contours instead of region growing

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

struct car
{
	Point2f pos;
	int color[3];
};

int bsize=40, bs=10, step=5, edgeThresh=10, lineth=150,blurline=7;
int hmin=100, hmax=150, smin=65,smax=255,vmin=65,vmax=160;
int minP=350, maxP=1200, colorTh=10;
int d=3;
bool undist=0,verb=0, debug=0, adjust=0, refresh=1;
car cars[200];
int ncars=0;
Point2d measPoint1, measPoint2;
bool measure1=1;
//Prints out usage instuctions
void usage()
{
	cout<<"Usage:\n\n";
	cout<<"'-f' [file]   Specify Source file to be analysed"<<endl;
	cout<<"'-u' [file]   Undistort images given the file with Camera Parameters"<<endl;
	cout<<"'-v'          Verbose Option"<<endl;
	cout<<"'-d'          Debug Option (images used to obtain final result)"<<endl;
	cout<<"'-a'          Adjust parameters Mode"<<endl;
	
	cout<<"Keys:"<<endl;
	cout<<"'SpaceBar' next Photo"<<endl;
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
		 if  ( event == EVENT_LBUTTONDOWN && measure1)
		{
			cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
			 measPoint1.x=x; measPoint1.y=y;
			measure1=0;
			//cout<<"H:"<<(int)imhsv.at<Vec3b>(y*3,x*3)[0]<<endl;
			//cout<<"S:"<<(int)imhsv.at<Vec3b>(y*3,x*3)[1]<<endl;
			//cout<<"V:"<<(int)imhsv.at<Vec3b>(y*3,x*3)[2]<<endl;
		}
		else if(event == EVENT_LBUTTONDOWN && !measure1) 
		{
			measPoint2.x=x; measPoint2.y=y;
			double dist= sqrt(pow(measPoint1.x-measPoint2.x, 2) + pow(measPoint1.y-measPoint2.y,2) );
			cout<<"Distancia entre pontos: "<<dist<<endl;
			measure1=1;
		}
		
}

//Method called by changing trackbars, used to reprocess image with different parameters
void on_trackbar( int, void* )
{
	refresh=1;
}

void getCarColor(Mat im, Point2f mc)
{
	int r,g,b,count;
	r=g=b=count=0;
	for(int i=mc.x-2; i<=mc.x+2; i++)
	for(int j=mc.y-2; j<=mc.y+2; j++)
	{
		count++;
		b+=im.at<Vec3b>(j,i)[0];
		g+=im.at<Vec3b>(j,i)[1];
		r+=im.at<Vec3b>(j,i)[2];
	}
	cars[ncars].color[0]=b/count;
	cars[ncars].color[1]=g/count;
	cars[ncars].color[2]=r/count;
}

Mat getCars(Mat blocks, Mat im)
{
	RNG rng(12345);
	Mat res(im.rows,im.cols,CV_8UC3);
	ncars=0;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(blocks, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0) );
	Moments mu;
	Point2f mc;
	for( int i = 0; i< contours.size(); i++ )
		 {
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			drawContours( res, contours, i, color, 2, 8, hierarchy, 0, Point() );
			if(contours[i].size()>minP && contours[i].size()<maxP)
			{
				mu = moments( contours[i], false );
				mc = Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
				cars[ncars].pos=mc;
				//getCarColor(im, mc);
				ncars++;
			}
		 }
		return res;
		resize(res,res,Size(im.cols/d, im.rows/d));
		imshow("Contornos",res);
}

//Creates a B/W Image where White represents low edges density area
Mat carBlocks(Mat src)
{
	Mat imblocks = Mat::zeros(src.rows, src.cols, CV_8UC1);
	int total=bsize*bsize;												//Total Area of each block
	int val,count;
	double percent;
	for(int x=0; x<(src.cols-bsize);x+=step)
	for(int y=0; y<(src.rows-bsize);y+=step)
	{
		count=0;
		for(int i=x; i<x+bsize; i++)
		for(int j=y; j<y+bsize; j++)
		{
			val=(int)src.at<uchar>(j,i);
			if(val==255)
			count++;													//count every pixel that represents an edge
		}
		percent=(double)count/(double)total*100;						//compute edge/non-edge ratio
		if(percent<=edgeThresh)											//draw white filled rectangles in low edge density areas
		rectangle(imblocks, Point2d(x,y), Point2d(x+bsize,y+bsize), Scalar(255), CV_FILLED);
	}
	return imblocks;
}

Mat mixIm(Mat bl, Mat ced)
{
	int val,count;
	for(int x=0; x<(ced.cols-5);x+=10)
	for(int y=0; y<(ced.rows-3);y+=3)
	{
		count=0;
		for(int i=x; i<x+bsize; i++)
		for(int j=y; j<y+bsize; j++)
		{
			val=(int)ced.at<uchar>(j,i);
			if(val==255)
			count++;
		}
		if(count>12)
		rectangle(bl, Point2d(x,y), Point2d(x+20,y+4), Scalar(0), CV_FILLED);
	}
	return bl;
}


//Removes Orange Color from bricked walls
Mat removeColor(Mat hsv, Mat edges)
{
	int h,s,v; 
	Mat preview;
	for(int i=0; i<hsv.rows;i++)
	for(int j=0; j<hsv.cols;j++)
	{
		h=(int)hsv.at<Vec3b>(i,j)[0];
		s=(int)hsv.at<Vec3b>(i,j)[1];
		v=(int)hsv.at<Vec3b>(i,j)[2];
		if(h<hmax && h>hmin && s>smin && s<smax && v>vmin && v<vmax)
		edges.at<uchar>(i,j)=255;
	}
	return edges;
}

Mat removeBorder(Mat im, double m,double b, bool side)
{
	
	for(int x=0; x<im.cols;x++)
	for(int y=0; y<im.rows;y++)
	{
		if(m==0 && side && x>=b)
		im.at<uchar>(y,x)=255;
		else if(m==0 && !side && x<=b)
		im.at<uchar>(y,x)=255;
		else if( (y-m*x-b>0) && ((m<0 && side) || (m>0 && !side)) )
		im.at<uchar>(y,x)=255;
		else if( y-m*x-b<0 && ((m>0 && side) || (m<0 && !side)) )
		im.at<uchar>(y,x)=255;
	}
		
	return im;
}
Mat drawLines(Mat src, Mat cenas)
{
		Mat edges;
		cvtColor(cenas, edges, CV_BGR2GRAY);
		Mat drLines = Mat::zeros(edges.rows,edges.cols, CV_8UC1);
		vector<Vec2f> lines;
		int nlinesLeft=0, nlinesRight=0;
		Point pt1, ptUL, ptDL, ptUR, ptDR;
		double a,b, x0, y0;
		HoughLines(edges, lines, 1, CV_PI/180, lineth, 0, 0 );
		for( size_t i = 0; i < lines.size(); i++ )
		{
			float rho = lines[i][0], theta = lines[i][1];
			a=cos(theta); b=sin(theta);
			x0 = a*rho; y0 = b*rho;
			pt1.x = cvRound(x0 + 1000*(-b));
			if(theta<0.3 || theta>2.8)
			{
				if(pt1.x<edges.cols/2 && (ptUL.x==0 || pt1.x>ptUL.x))
				{
					ptUL.x=pt1.x;
					ptUL.y = cvRound(y0 + 1000*(a));
					ptDL.x = cvRound(x0 - 1000*(-b));
					ptDL.y = cvRound(y0 - 1000*(a));
					nlinesLeft++;
				}
				else if(pt1.x>edges.cols/2 && (ptUR.x==0 || pt1.x<ptUR.x))
				{
					ptUR.x=pt1.x;
					ptUR.y = cvRound(y0 + 1000*(a));
					ptDR.x = cvRound(x0 - 1000*(-b));
					ptDR.y = cvRound(y0 - 1000*(a));				
					nlinesRight++;
				}
			}

		}
		if(nlinesLeft>0 && nlinesRight>0)
		{
			line( src, ptUL, ptDL, Scalar(255, 255,255), 5, CV_AA);
			line( src, ptUR, ptDR, Scalar(255,255,255), 5, CV_AA);}
			/*int mid=src.cols/2, up=src.rows/4, down=3*src.rows/4;
			int val=0, i=0, upright,upleft,downright,downleft;
			while(val!=255)
			{	val=drLines.at<uchar>(up, mid+i);	i++;}
			upright=mid+i;
			val=i=0;
			while(val!=255)
			{	val=drLines.at<uchar>(up, mid-i);	i++;}
			upleft=mid-i;
			val=i=0;
			while(val!=255)
			{	val=drLines.at<uchar>(down, mid+i);	i++;}
			downright=mid+i;
			val=i=0;
			while(val!=255)
			{	val=drLines.at<uchar>(down, mid-i);	i++;}
			downleft=mid-i;
			val=i=0;
			int dist=((upright-upleft)+(downright-downleft))/2;
			Point r1(upright+dist, up);
			Point r2(downright+dist,down);
			Point l1(upleft-dist,up);
			Point l2(downleft-dist,down);
		}
		
		
		if(r2.x-r1.x!=0)
		{
			double mr=(r2.y-r1.y)/(r2.x-r1.x);
			double br=r1.y-(mr*r1.x);
			edges=removeBorder(edges, mr, br,1);
		}
		else
		edges=removeBorder(edges,0,r1.x,1);
		if(l2.x-l1.x!=0)
		{
		double ml=(l2.y-l1.y)/(l2.x-l1.x);
		double bl=l1.y-(ml*l1.x);
		edges=removeBorder(edges, ml, bl,0);
		
		}
		else
		edges=removeBorder(edges,0,l1.x,0);*/
		Mat tm;
		resize(src,tm, Size(src.cols/d, src.rows/d));
		imshow("Lines", tm);
	return edges;
}

Mat drawCars( Mat image)
{
	Mat final;
	stringstream ss;
	string str;
	image.copyTo(final);
	for(int i=0; i<ncars; i++)
	{
		ss.str("");
		ss << i+1;
		str = ss.str();
		circle(final, cars[i].pos, 50, Scalar(0,0,255), CV_FILLED);
		putText(final, str, cars[i].pos, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(cars[i].color[0],cars[i].color[1],cars[i].color[2]), 3);
	}	
	cout<<"Found "<<ncars<<" cars!"<<endl;
	return final;
}

bool getBlock(Mat edges, int x, int y)
{
	for(int i=x; i<x+bs; i++)
	for(int j=y; j<y+bs; j++)
	{
		if((int)edges.at<uchar>(j,i)==255)
		return false;
	}
	return true;
}

Mat blocos(Mat edges)
{
	Mat cenas = Mat::zeros(edges.rows, edges.cols, CV_8UC1);
	int n=0;
	for(int x=0; x<(edges.cols-bs);x+=step)
	for(int y=0; y<(edges.rows-bs);y+=step)
	{
		if(getBlock(edges,x,y) )
		{
			n++;
			for(int i=x; i<x+bs; i++)
			for(int j=y; j<y+bs; j++)
			cenas.at<uchar>(j,i)==255;
		}
	}
	cout<<"NBlocos: "<<n<<endl;
	Mat res;
	resize(cenas,res, Size(edges.cols/d, edges.rows/d));
	imshow("Blocage", res);
	return res;
}

Mat colorCarFind(Mat imhsv)
{
	Mat result = Mat::zeros(imhsv.rows, imhsv.cols, CV_8UC1);
	Mat res;
	int up, left, pix;
	for(int i=0; i<imhsv.rows;i++)
	for(int j=0;j<imhsv.cols;j++)
	{
		up=-1;left=-1;
		pix = (int)imhsv.at<Vec3b>(i,j)[0];
		if(i!=0)
		{
			up=(int)imhsv.at<Vec3b>(i-1,j)[0];
			if(abs(pix-up)>colorTh)
			result.at<uchar>(i,j)=255;
		}
		if(j!=0)
		{
			left=(int)imhsv.at<Vec3b>(i,j-1)[0];
			if(abs(pix-left)>colorTh)
			result.at<uchar>(i,j)=255;
		}
	}
	resize(result,res, Size(imhsv.cols/d, imhsv.rows/d));
	imshow("imm", res);
	return result;
}

int main(int argc, char** argv )
{
	namedWindow("DetectedCars",1);
	Mat image, imgray, final;
	Mat intrinsics,distcoeffs, undistorted;
	Mat edges, cedges, imblocks, debug1, debug2, debug3, mix;
	int key=0, opt, nframe=0;
	int blursize=3, cannymin=20, cannymax=60;
	bool isDistorted=1;
	clock_t begin, end, totalbegin,totalend;
	string file="",calibFile="";
	char filepath[200];
	VideoCapture cap;
	FileStorage fs;
	
	while ((opt = getopt(argc, argv, "af:vhu:d")) != -1) {
		switch (opt) {
		case 'f':                                                      //File path
			file = optarg;
			break;
		case 'v':                                                      //Verbose option
			verb = 1;
			break;
		case 'u':														//Undistort images
			undist=1;
			calibFile=optarg;
			break;
		case 'd':                                                      //Verbose option
			debug = 1;
			break;
		case 'a':														//Adjust Mode
			adjust=1;
			break;
		default:
			fprintf(stderr, "Usage: %s [-f file] [-v (verbose)] \n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	usage();
	setMouseCallback("DetectedCars", CallBackFunc, NULL);
	if(adjust)
	{
		namedWindow("Adjusts",1);
		//createTrackbar("Color Edge Threshold", "Adjusts", &colorTh, 50, on_trackbar);
		createTrackbar("Block Edges Percentage", "Adjusts", &edgeThresh, 30, on_trackbar);
		createTrackbar("BlockSize", "Adjusts", &bsize, 60, on_trackbar);
		createTrackbar("BlockSteps", "Adjusts", &step, 60, on_trackbar);
		createTrackbar("Max Perimeter", "Adjusts", &maxP, 6000, on_trackbar);
		createTrackbar("Min Perimeter", "Adjusts", &minP, 6000, on_trackbar);
		createTrackbar("CannyMin", "Adjusts", &cannymin, 500, on_trackbar);
		createTrackbar("CannyMax", "Adjusts", &cannymax, 500, on_trackbar);
		createTrackbar("BlurSize", "Adjusts", &blursize, 15, on_trackbar);
		createTrackbar("Lines Thresh", "Adjusts", &lineth, 1500, on_trackbar);
	}
	
	//Open Source File
	if(file!="")
	{
		cap.open(file);
		if(!cap.isOpened())
		{
			cout<<"\n\n\nERROR: Couldn't open Source File!\n\n\n"; 
			exit(EXIT_FAILURE);
		}
		cap>>image;
	}
	else 
	{
		cout<<"\nOpening "<< filepath<<endl;
		sprintf(filepath,"../imgs/Bebop/CIAQ_vid/frame%03d.jpg",nframe);
		image=imread(filepath);
	}

	//Get Calibration parameters
	if(calibFile=="" && undist)
	{cout<<"\n\n\nERROR: Must specify Calibration File!\n\n\n"; exit(EXIT_FAILURE);}
	else if(undist)
	fs.open(calibFile, cv::FileStorage::READ);
	if (undist && fs.isOpened() )
	{ fs["cameraMatrix"] >>  intrinsics; fs["distCoeffs"] >>  distcoeffs;}
	else if(undist)
	{cout<<"\n\n\nERROR: Couldn't find Calibration file!\n\n\n"; exit(EXIT_FAILURE);}
	
	while(1)
	{
		totalbegin=clock();
		switch(key)
		{
		case 27: 
			cout<<"Exit!\n"; 
			return 0; 
			break;
		case 32: 
			refresh=1;	//FIND NEXT FRAME
			nframe++;
			isDistorted=1;
			sprintf(filepath,"../imgs/Bebop/CIAQ_vid/frame%03d.jpg",nframe);
			image=imread(filepath);
			break;
		}

		if(undist && isDistorted)
		{	
			begin=clock();
			undistort(image,undistorted, intrinsics, distcoeffs);
			isDistorted=0;
			image=undistorted.clone();
			end=clock();
			if(verb && refresh)
			cout<<"Undistortion Time: "<< double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
		}
		
		if(refresh)
		{
			begin=clock();
			cvtColor(image, imgray, CV_RGB2GRAY);
			//cvtColor(image, imhsv, CV_RGB2HSV);
			blur( imgray, edges, Size(blursize,blursize) );
			Canny( edges, edges, cannymin, cannymax, 3 );
			//edges=removeColor(imhsv,edges);
			edges.copyTo(debug1);
			
			//cedges=colorCarFind(imhsv);
			//edges=drawLines(image,edges);
			imblocks=carBlocks(edges);
			//imblocks=blocos(edges);
			imblocks.copyTo(debug2);
			//mix=mixIm(imblocks,cedges);
			Mat nova=getCars(imblocks, image);
			Mat nova2=drawLines(image, nova);
			end=clock();
			if(verb)
			cout<<"Car Detection Time: "<< double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
					
			//Draw Cars Found
			begin=clock();
			final=drawCars(image);
			end=clock();
			if(verb)
			cout<<"Car Draw and Display Time: "<< double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
		}		
		
		if(debug && refresh)
		{
			begin=clock();
			resize(debug1, debug1, Size(image.cols/d, image.rows/d));
			imshow("Canny",debug1);
			resize(debug2, debug2, Size(image.cols/d, image.rows/d));
			imshow("Blocks Image",debug2);
			//resize(cedges, cedges, Size(image.cols/d, image.rows/d));
			//imshow("Color Edges",cedges);
			//resize(mix,mix, Size(image.cols/3, image.rows/3));
			//imshow("MIX",mix);
			if(verb)
			cout<<"Debug Images Display Time: "<< double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
		}
		resize(final,final, Size(image.cols/d, image.rows/d));
		imshow("DetectedCars", final);
		totalend=clock();
		if(verb && refresh)
		cout<<"TOTAL Time: "<< double(totalend-totalbegin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
		
		refresh=0;
		key=waitKey(50);
		
		}
	return 0;
}


