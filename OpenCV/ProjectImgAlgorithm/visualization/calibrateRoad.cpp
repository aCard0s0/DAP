#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <string>
#include <vector>
using namespace cv;
using namespace std;

void CalibRoad(int,int,int,int,void*);
void saveRoad();
void writeFile();

int key=32, savedFrames=0, windDiv=2;
Mat image, toDisplay, roadLines,temp;
int ptsel=0; Point roadpts[4];
char filepath[200];
int meanVals[2][1080];

int main(int argc, char** argv )
{
	int nframe=-1;
	namedWindow("Window", 1);	
	setMouseCallback("Window", CalibRoad, NULL);
	while(1)
	{
		if(key==32) 
		{
			nframe++;
			sprintf(filepath,"../imgs/Bebop/Library_10m/frame%03d.jpg",nframe);
			image=imread(filepath);
			image.copyTo(toDisplay);
			roadLines=Mat::zeros(image.rows,image.cols,CV_8UC1);
			line(toDisplay, Point(image.cols/2,0), Point(image.cols/2, image.rows), Scalar(0,0,255), 4);
		}
		if(ptsel==4)
		{
			line(roadLines, roadpts[0], roadpts[1], 255, 1);
			line(roadLines, roadpts[2], roadpts[3], 255, 1);
			line(toDisplay, roadpts[0], roadpts[1], Scalar(0,0,255), 1);
			line(toDisplay, roadpts[2], roadpts[3], Scalar(0,0,255), 1);
		}
			resize(toDisplay, temp, Size(image.cols/windDiv, image.rows/windDiv));
			imshow("Window", temp);

			if(key=='y')
			saveRoad();
			if(savedFrames==20)
			{writeFile();savedFrames=0;}
			if(key=='n')
			ptsel=0;
			
			key=waitKey(10);
	}
}

void saveRoad()
{
	for(int y=0; y<image.rows; y++)
	{
		int x=0;
		while((int)roadLines.at<uchar>(y,x)==0)
		{x++;}
		meanVals[0][y]+=(image.cols/2-x);
		cout<<"Left part="<<image.cols/2-x<<endl;
		x=image.cols/2;
		while((int)roadLines.at<uchar>(y,x)==0)
		{x++;}
		meanVals[1][y]+=(x-image.cols/2);
		cout<<"Right part="<<x-image.cols/2<<endl;
	}
	savedFrames++;
	cout<<"Frames Total: "<<savedFrames<<endl;

}

void writeFile()
{
	Mat calibFrames = Mat::zeros(1080,2, CV_16UC1);
	
	for(int y=0;y<1080;y++)
	{
		calibFrames.at<ushort>(y,0)=(meanVals[0][y]/20);
		calibFrames.at<ushort>(y,1)=(meanVals[1][y]/20);
		cout<<"Y:"<<y<<"  Left:"<<(meanVals[0][y]/20)<<"   Right:"<<(meanVals[1][y]/20)<<endl;
	}
	FileStorage fs("..//config//RoadLibrary10m.xml", FileStorage::WRITE);
	fs << "roadMatrix" << calibFrames;
	cout<<"FILE SAVED"<<endl;

}





void CalibRoad(int event, int x, int y, int flags, void* userdata)
{
		if (event == EVENT_LBUTTONDOWN)
			{
				if(ptsel==0 || ptsel==2)
				roadpts[ptsel]=Point(x*windDiv,0);
				else if(ptsel==1 || ptsel==3)
				roadpts[ptsel]=Point(x*windDiv,image.rows);
				
				ptsel++;
				cout<<"Selected Point "<<ptsel<<endl;
			}
			
}
