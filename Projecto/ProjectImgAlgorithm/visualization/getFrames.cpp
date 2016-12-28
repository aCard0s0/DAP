#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
    Mat intrinsics, distcoeffs;
//    bool toUndistort=false;
//	FileStorage fs("../calibration/CamParams.xml", cv::FileStorage::READ);
//    if (fs.isOpened() )
//    {
//        fs["cameraMatrix"] >>  intrinsics;
//        fs["distCoeffs"] >>  distcoeffs;
//        toUndistort=true;     
//    }
    
    VideoCapture capture(argv[1]);
	if(!capture.isOpened())
    {
    cout<<"Invalid Input Video.\n";
    return -1;
    }
    Mat frame, preview;
    char path[100];
    double rate = capture.get(CV_CAP_PROP_FPS);
    cout<< "Frames por segundo: "<<rate<<endl;
    int div=4, i, nframes=0, count=0;
    bool pause=false;
  
    while(true)
    {
        i = waitKey(1000/rate);
        switch(i)
        {
            case 27: cout<<"EXIT \n"; return -1; break;
            case -1: break;
            //case 102: cout<<"Fast Forward"<<endl; for(int n=0;n<300;n++) capture.read(frame);break;
            case 32: pause=!pause; if(pause) cout<<"Pause"<<endl; else cout<<"Resume"<<endl; break;
            //case 13:sprintf(path,"../imgs/frame%02d.jpg",nframes);imwrite(path, frame); nframes++; cout<<"Frame Saved"<<endl;break;
            case 63232: div+=1; break;
            case 63233: div-=1; break;
            default: cout<<"Tecla: "<<i<<endl; break;
        }
        if(!pause)
        {
            if(!capture.read(frame))
            break;
//            if(toUndistort)
//            undistort(frame,preview, intrinsics, distcoeffs);
            if(nframes%15==0)
            {
                resize(frame,preview, Size(frame.cols/div, frame.rows/div));
                imshow("Preview",preview);
                sprintf(path,"../imgs/Bebop/CIAQ_vid/frame%03d.jpg",nframes/15);imwrite(path, frame); 
            }
            nframes++;
            
        }
        
    }
    capture.release();
    return 0;
}
