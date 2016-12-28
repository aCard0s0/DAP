
//	CANNY FIND CARS
//			Algorithm to detect cars in a single image or in an image sequence based on Canny Edge Detector
//			Ideal for non homogeneous pavements (Macadam, Portuguese Pavement, Block Pavement)


#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

int bw=40, bh=40, step=4, edgeThresh=10, nreg, area, lineth=100,blurline=7;
int hmin=100, hmax=150, smin=65,smax=255,vmin=65,vmax=160;
bool undist=0,verb=0, debug=0,hist=0, adjust=0, refresh=1;
vector<Point2d> stain;
vector<vector<Point2d> > regions; 
int regionarea[50];
Mat visited;

//Prints out usage instuctions
void usage()
{
	cout<<"Usage:\n\n";
	cout<<"'-f' [file]   Specify Source file to be analysed"<<endl;
	cout<<"'-u' [file]   Undistort images given the file with Camera Parameters"<<endl;
	cout<<"'-v'          Verbose Option"<<endl;
	cout<<"'-d'          Debug Option (images used to obtain final result)"<<endl;
	cout<<"'-a'          Adjust parameters Mode"<<endl;
	cout<<"'-h'          Histogram calculation and display"<<endl<<endl;
	
	cout<<"Keys:"<<endl;
	cout<<"'SpaceBar' next Photo"<<endl;
}

//Method called by changing trackbars, used to reprocess image with different parameters
void on_trackbar( int, void* )
{
	refresh=1;
}

//Computes and Displays 3 channel Histograms
void Histogram(Mat im)
{
	clock_t begin,end;
	vector<Mat> channels;
	split(im, channels);
	int histSize = 256;
	float range[] = { 0, 256 } ;
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;
	Mat h_hist, s_hist, v_hist;
	begin=clock();
	calcHist( &channels[0], 1, 0, Mat(), h_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &channels[1], 1, 0, Mat(), s_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &channels[2], 1, 0, Mat(), v_hist, 1, &histSize, &histRange, uniform, accumulate );
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
	normalize(h_hist, h_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(s_hist, s_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(v_hist, v_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	end=clock();
	for( int i = 1; i < histSize; i++ )
	{
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(h_hist.at<float>(i-1)) ) ,
				Point( bin_w*(i), hist_h - cvRound(h_hist.at<float>(i)) ),
				Scalar( 255, 0, 0), 2, 8, 0  );
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(s_hist.at<float>(i-1)) ) ,
				Point( bin_w*(i), hist_h - cvRound(s_hist.at<float>(i)) ),
				Scalar( 0, 255, 0), 2, 8, 0  );
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(v_hist.at<float>(i-1)) ) ,
				Point( bin_w*(i), hist_h - cvRound(v_hist.at<float>(i)) ),
				Scalar( 0, 0, 255), 2, 8, 0  );
	}
	imshow("Histogram", histImage);
}

//Analizes 4connected neighbours to perform region growing algorithm
void findNeighbours(Mat imblocks, int x,int y,int nreg)
{
	visited.at<uchar>(y,x)=1;
	Point2d up(y-1,x);
	Point2d down(y+1,x);
	Point2d left(y,x-1);
	Point2d right(y,x+1);
	int upvisit=1, downvisit=1,rightvisit=1,leftvisit=1;
	int uppix=0,downpix=0,leftpix=0,rightpix=0;
	
	if(y>0)
	{
		uppix=(int)imblocks.at<uchar>(y-1,x);
		upvisit=(int)visited.at<uchar>(y-1,x);
		visited.at<uchar>(y-1,x)=1;
	}
	if(y<imblocks.rows-1)
	{
		downpix=(int)imblocks.at<uchar>(y+1,x);
		downvisit=(int)visited.at<uchar>(y+1,x);
		visited.at<uchar>(y+1,x)=1;
	}
	if(x>0)
	{
		leftpix=(int)imblocks.at<uchar>(y,x-1);
		leftvisit=(int)visited.at<uchar>(y,x-1);
		visited.at<uchar>(y,x-1)=1;
	}
	if(y<imblocks.cols-1)
	{
		rightpix=(int)imblocks.at<uchar>(y,x+1);
		rightvisit=(int)visited.at<uchar>(y,x+1);
		visited.at<uchar>(y,x+1)=1;
	}
	if(uppix==255 && !upvisit)
	{stain.push_back(Point2d(x,y-1)); area++;}
	if(downpix==255 && !downvisit)
	{stain.push_back(Point2d(x,y+1)); area++;}
	if(leftpix==255 && !leftvisit)
	{stain.push_back(Point2d(x-1,y)); area++;}
	if(rightpix==255 && !rightvisit)
	{stain.push_back(Point2d(x+1,y)); area++;}
}

//Separates White parts of ImBlocks in different regions and calculate their areas;
void getCars(Mat imblocks)
{
	int vis, pix;
	Point2d point;
	vector<Point2d> tempstain;
	visited = Mat::zeros(imblocks.rows, imblocks.cols, CV_8UC1);
	nreg=0;
	regions.clear();
	for(int r=0;r<visited.cols;r++)
	for(int c=0;c<visited.rows;c++)
	{
		pix=imblocks.at<uchar>(c,r);
		vis=visited.at<uchar>(c,r);
		if(vis==0 && pix==255)
		{
			area=0;
			tempstain.clear();
			stain.push_back(Point2d(r,c));
			while(stain.size()>0)
			{
				point=stain.back();
				stain.pop_back();
				tempstain.push_back(point);
				findNeighbours(imblocks,point.x,point.y,nreg);
			}
			regions.push_back(tempstain);
			regionarea[nreg]=area;
			nreg++;
		}
	}
}

//Creates a B/W Image where White represents low edges density area
Mat carBlocks(Mat edges)
{
	Mat imblocks = Mat::zeros(edges.rows, edges.cols, CV_8UC1);
	int total=bw*bh;													//Total Area of each block
	int val,count;
	double percent;
	for(int x=0; x<(edges.cols-bw);x+=bw/step)
	for(int y=0; y<(edges.rows-bh);y+=bh/step)
	{
		count=0;
		for(int i=x; i<x+bw; i++)
		for(int j=y; j<y+bh; j++)
		{
			val=(int)edges.at<uchar>(j,i);
			if(val==255)
			count++;													//count every pixel that represents an edge
		}
		percent=(double)count/(double)total*100;						//compute edge/non-edge ratio
		if(percent<=edgeThresh)											//draw white filled rectangles in low edge density areas
		rectangle(imblocks, Point2d(x,y), Point2d(x+bw,y+bh), Scalar(255), CV_FILLED);
	}
	return imblocks;
}

//Removes Green from Gardens and Orange from bricked walls
Mat removeGreen(Mat hsv, Mat edges)
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

//Uses region vector to draw car containers and numbers
Mat drawCars( Mat image, Mat imblocks)
{
	Mat final;
	int ncars=0;
	stringstream ss;
	string str;
	image.copyTo(final);
	int xmin, xmax, ymin, ymax, Xm,Ym,XM,YM, x, y;
	Point2d p1,p2;
	for(int i=0; i<nreg; i++)
	{
		if(regionarea[i]>15000 && regionarea[i]<80000)
		{
			for(int j=0; j<regions[i].size();j++)
			{				
				x=regions[i].at(j).x;
				y=regions[i].at(j).y;
				if(j==0)
				{
					xmax=x; ymax=y; 
					xmin=x; ymin=y;
				}
				if(x>=xmax)
				{XM=j; xmax=x;}
				if(x<=xmin)
				{Xm=j; xmin=x;}
				
				if(y>=ymax)
				{YM=j; ymax=y;}
				if(y<=ymin)
				{Ym=j; ymin=y;}
			}
			p1.x=regions[i].at(Xm).x; 
			p1.y=regions[i].at(Ym).y;
			p2.x=regions[i].at(XM).x; 
			p2.y=regions[i].at(YM).y;
			rectangle(final, p1, p2, Scalar(0,0,255), 2);
			ss.str("");
			ncars++;
			ss << ncars;
			str = ss.str();
			putText(final, str, (p1+p2)*0.5, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0,0,255), 3);
		}
	}	
	cout<<"Found "<<ncars<<" cars!"<<endl;
	return final;
}
Mat removeBorder(Mat im, int m,int b, bool side)
{
	for(int x=0; x<im.cols;x++)
	for(int y=0; y<im.rows;y++)
	{
		if((y-m*x-b)>0 && side)
		im.at<uchar>(y,x)=255;
		if((y-m*x-b)<0 && !side)
		im.at<uchar>(y,x)=255;
	}
	
	return im;
}
Mat drawLines(Mat src, Mat edges)
{
		Mat blured, dst, cdst, disp;
		int cannymin=50,cannymax=150;
		double m, b;
		
		blur(src, blured, Size(blurline,blurline));
		Canny(blured, dst, cannymin, cannymax, 3);
		cvtColor(dst, cdst, CV_GRAY2BGR);
		vector<Vec2f> lines;
		HoughLines(dst, lines, 1, CV_PI/180, lineth, 0, 0 );

		for( size_t i = 0; i < lines.size(); i++ )
		{
			float rho = lines[i][0], theta = lines[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1.x = cvRound(x0 + 1000*(-b));
			pt1.y = cvRound(y0 + 1000*(a));
			pt2.x = cvRound(x0 - 1000*(-b));
			pt2.y = cvRound(y0 - 1000*(a));
			if(pt2.x-pt1.x != 0)
			{
				m=(pt2.y-pt1.y)/(pt2.x-pt1.x);
				b=pt1.y-(m*pt1.x);
				if(theta<0.3  &&  pt1.x>3*src.cols/4) 
					edges=removeBorder(edges, m, b,1);
				if(theta>2.8 &&  pt1.x>3*src.cols/4)
					edges=removeBorder(edges,m,b,0);
			} 
			if(theta<0.3 || theta>2.8)
			line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
		}
		resize(cdst,disp,Size(src.cols/3, src.rows/3));
		imshow("Window", disp);
		
	return edges;
}

int main(int argc, char** argv )
{
	namedWindow("DetectedCars",1);
	Mat image, imhsv, imgray, cars;
	Mat intrinsics,distcoeffs, undistorted;
	Mat edges, imblocks, debug1, debug2;
	int key=0, opt, nframe=0;
	int blursize=3, cannymin=20, cannymax=60;
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
		case 'h':                                                      //Histogram option
			hist = 1;
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
	if(adjust)
	{
		namedWindow("RemoveColor",1);
		createTrackbar("HMin", "RemoveColor", &hmin, 255, on_trackbar);
		createTrackbar("SMin", "RemoveColor", &smin, 255, on_trackbar);
		createTrackbar("VMin", "RemoveColor", &vmin, 255, on_trackbar);
		createTrackbar("HMax", "RemoveColor", &hmax, 255, on_trackbar);
		createTrackbar("SMax", "RemoveColor", &smax, 255, on_trackbar);
		createTrackbar("VMax", "RemoveColor", &vmax, 255, on_trackbar);
		/*namedWindow("Canny",1);
		createTrackbar("Block Edges Percentage", "DetectedCars", &edgeThresh, 30, on_trackbar);
		createTrackbar("BlockWidth", "DetectedCars", &bw, 400, on_trackbar);
		createTrackbar("BlockHeight", "DetectedCars", &bh, 200, on_trackbar);
		createTrackbar("BlockSteps", "DetectedCars", &step, 10, on_trackbar);*/
		if(debug)
		createTrackbar("CannyMin", "Canny", &cannymin, 500, on_trackbar);
		createTrackbar("CannyMax", "Canny", &cannymax, 500, on_trackbar);
		createTrackbar("BlurSize", "Canny", &blursize, 15, on_trackbar);


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
		sprintf(filepath,"../imgs/Library_Park/frame%02d.jpg",nframe);
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
			sprintf(filepath,"../imgs/Library_Park/frame%02d.jpg",nframe);
			image=imread(filepath);
			break;
		case 'h': 
			cout<<"histogram"<<endl;
			break;
		}

		if(undist && refresh)
		{	
			begin=clock();
			undistort(image,undistorted, intrinsics, distcoeffs);
			image=undistorted.clone();
			end=clock();
			if(verb && refresh)
			cout<<"Undistortion Time: "<< double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
		}
		
		if(hist && refresh)
		{
			begin=clock();
			cvtColor(image, imhsv, CV_RGB2HSV);
			if(hist)
			Histogram(imhsv);
			end=clock();
			if(verb)
			cout<<"Histogram Time: "<< double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
		}
		
		if(refresh)
		{
			begin=clock();
			cvtColor(image, imgray, CV_RGB2GRAY);
			cvtColor(image, imhsv, CV_RGB2HSV);
			blur( imgray, edges, Size(blursize,blursize) );
			Canny( edges, edges, cannymin, cannymax, 3 );
			edges=drawLines(image,edges);
			edges=removeGreen(imhsv,edges);
			imblocks=carBlocks(edges);
			getCars(imblocks);
			end=clock();
			if(verb)
			cout<<"Car Detection Time: "<< double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
			imshow("DetectedCars", image);
			
			//Draw Cars Found
			begin=clock();
			cars=drawCars(image, imblocks);
			resize(cars,cars, Size(image.cols/2, image.rows/2));
			imshow("DetectedCars", cars);
			end=clock();
			if(verb)
			cout<<"Car Draw and Display Time: "<< double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
		}		
		
		if(debug && refresh)
		{
			begin=clock();
			resize(edges, debug1, Size(image.cols/2, image.rows/2));
			imshow("Canny",debug1);
			resize(imblocks, debug2, Size(image.cols/3, image.rows/3));
			imshow("Blocks Image",debug2);
			if(verb)
			cout<<"Debug Images Display Time: "<< double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
		}
		
		totalend=clock();
		if(verb && refresh)
		cout<<"TOTAL Time: "<< double(totalend-totalbegin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
		
		refresh=0;
		key=waitKey(50);
		
		}
	return 0;
}

