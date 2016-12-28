#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

/// Global variables
Mat src, src_gray;
int thresh = 190;
int key=32, nframe=0;

/** @function main */
int main( int argc, char** argv )
{
	char filepath[200];
	stringstream ss;
	string str;
	int count;
	Mat image[2];
	Mat result[2];
	Mat dst, dst_norm, dst_norm_scaled, final;
	image[1]=imread("../imgs/Bebop/CIAQ_vid/frame000.jpg");
	dst = Mat::zeros( image[1].size(), CV_32FC1 );
	vector<Point2f> pts;
	vector<Point2f> newpts;
	vector<uchar> status;
	Mat err;
	Point2f found;
	while(1)
	if(key==32)
	{
		count=0;
		nframe++;
		image[0]=image[1];
		sprintf(filepath,"../imgs/Bebop/CIAQ_vid/frame%03d.jpg",nframe);
		image[1]=imread(filepath);
		image[0].copyTo(result[0]);
		image[1].copyTo(result[1]);
		cvtColor( image[0], src_gray, CV_BGR2GRAY );
		cornerHarris( src_gray, dst, 2, 3, 0.04, BORDER_DEFAULT );
		normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
		convertScaleAbs( dst_norm, dst_norm_scaled );
		/// Drawing a circle around corners
		pts.clear();
		newpts.clear();
		for( int j = 0; j < dst_norm.rows ; j++ )
		for( int i = 0; i < dst_norm.cols; i++ )
			{
				if( (int) dst_norm.at<float>(j,i) > thresh )
				{
					pts.push_back(Point2f(i,j));
					circle( result[0], Point( i, j ), 5,  Scalar(0,0,255), 2, 8, 0 );
					ss.str("");
					ss << count++;
					str = ss.str();
					//putText(result[0], str, Point(i,j), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0,0,255), 2);
				}
			}	
			cout<<"Pontos: "<<count<<endl;
		calcOpticalFlowPyrLK(image[0], image[1], pts, newpts, status, err, Size(70,30));
		count=0;
		for(int i=0; i<newpts.size(); i++)
		{
			if(status.at(i)==1)
			{
				found=newpts.at(i);
				circle( result[1], found, 5,  Scalar(0,255,0), 2, 8, 0 );
				count++;
			}
		}
		cout<<"Encontrados: "<<count<<endl;
		resize(result[0],result[0],Size(image[0].size()/3));
		imshow( "Original", result[0] );
		resize(result[1],result[1],Size(image[1].size()/3));
		imshow( "FINAL", result[1] );

		waitKey(-1);
	}
}

