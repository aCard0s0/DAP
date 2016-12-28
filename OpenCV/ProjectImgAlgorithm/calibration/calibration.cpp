
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>        
#include <opencv2/highgui/highgui.hpp>  
#include <stdio.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

int FindAndDisplayChessboard(cv::Mat image,int board_w,int board_h, std::vector<cv::Point2f> *corners)
{
  int board_size = board_w * board_h;
  CvSize board_sz = cvSize(board_w,board_h);
  Mat grey_image;
  cvtColor(image, grey_image, CV_BGR2GRAY);
  
  bool found = cv::findChessboardCorners(grey_image, board_sz, *corners,0);
  
  if (true)
  {
	drawChessboardCorners(image, board_sz, cv::Mat(*corners), found);
	resize(image, image, Size(image.cols/3, image.rows/3) );
  imshow("Calibration",image);
  waitKey (200);
	printf("Number of corners: %d\n",corners->size()); 
  }
  return corners->size();
}

int main(int argc, char **argv)
{
  int n_boards = 14;
  int board_w = 9;
  int board_h = 6;
  int board_sz = board_w * board_h;
  char filename[200];
  vector<vector<Point3f> > object_points;
  vector<vector<Point2f> > image_points;
  vector<Point2f> corners;
  int corner_count, i,sucesses = 0;
  Mat image;

  vector<cv::Point3f> obj;
  for(int j=0;j<board_sz;j++)
    obj.push_back(cv::Point3f(float(j/board_w), float(j%board_w), 0.0));
  
  for (i=1;i<n_boards;i++)
  {
    sprintf(filename,"..//imgs//calib//Rollei//frame%02d.jpg",i);
    printf("Reading %s\n",filename);
    image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
    
    
    if(!image.data) 
    {
      printf("\nCould not load image file: %s\n",filename);
	    getchar();
      return 0;
    }
    
    corner_count = FindAndDisplayChessboard(image,board_w,board_h,&corners);
    
	if (corner_count == board_w * board_h)
    {
      image_points.push_back(corners);
      object_points.push_back(obj);
      sucesses++;
    }
  }
	Mat intrinsics;
	Mat distcoef, rvecs, tvecs;	
	calibrateCamera(object_points, image_points, image.size(),intrinsics,distcoef,rvecs,tvecs,CV_CALIB_RATIONAL_MODEL);//,CV_CALIB_USE_INTRINSIC_GUESS);
	FileStorage fs("..//config//Rollei//CamParams.xml", FileStorage::WRITE);
  fs << "cameraMatrix" << intrinsics << "distCoeffs" << distcoef;
  fs.release();

  return 0;
}
