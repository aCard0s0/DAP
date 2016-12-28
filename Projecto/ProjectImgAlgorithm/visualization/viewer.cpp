#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <string>
using namespace cv;
using namespace std;

void usage()
{
	cout<<"KEYS TO USE:\n\n";
	cout<<"'U'        to set Undistortion ON or OFF"<<endl;
	cout<<"'C'        to change color space displayed [RGB, H, S, V, HSV]"<<endl;
	cout<<"'SpaceBar' to Pause"<<endl;
	cout<<"'ENTER'    to save frame"<<endl;
}

int isJPG(string s)        //Check if file format matches JPG
{
	string formats[4]={".jpg", ".JPG", ".png", ".PNG"};
	for(int i=0;i<4;i++)
	{
		if(s.find(formats[i])!=string::npos)
			return 1;
	}

	return 0;
}

//Function to Calculate and Display 3 channel Histograms
void Histogram(Mat im, int verb, int nframes, int freq)
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
	if(verb && nframes%freq==0)            //Print information every (freq) frames;
	cout<<"Histogram Time:"<<double(end-begin)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
}

int main(int argc, char** argv )
{
	Mat image, imhsv,preview,intrinsics,distcoeffs, undistorted;
	Mat channels[3];
	string colorName [5]={"Original","H","S","V","HSV"};
	namedWindow("ImageDisplay", CV_WINDOW_AUTOSIZE);
	moveWindow("ImageDisplay", 0, 0);
	int color=0,key=0,nframes=0,ratio=3, still=0;
	int opt,hist=0,undist=0, undON=0,verb=0,isVideo=0,pause=0;
	clock_t begin, end, totalb;
	string file="", calibFile="";
	VideoCapture cap;
	FileStorage fs;
	char path[100];

	while ((opt = getopt(argc, argv, "f:vhu:")) != -1) {
		switch (opt) {
		case 'f':                                                        //File path
			file = optarg;
			break;
		case 'v':                                                        //Verbose option
			verb = 1;
			break;
		case 'h':                                                        //Ask Histogram
			hist=1;
			namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
			moveWindow("Histogram", 650, 0);
			break;
		case 'u':                                                        //Undistort images
			undist=1;
			calibFile=optarg;
			break;
		default:
			fprintf(stderr, "Usage: %s [-f file] [-v (verbose)] [-h (histogram)] [-u calibration.xml] \n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	usage();
	
	//Open Source File
	if(file=="")
	{cout<<"\n\n\nERROR: Must specify Source File!\n\n\n"; exit(EXIT_FAILURE);}
	cap.open(file);
	if(!cap.isOpened())
	{cout<<"\n\n\nERROR: Couldn't open Source File!\n\n\n"; exit(EXIT_FAILURE);}

	//Get Calibration parameters
	if(calibFile=="" && undist)
	{cout<<"\n\n\nERROR: Must specify Calibration File!\n\n\n"; exit(EXIT_FAILURE);}
	else if(undist)
	fs.open(calibFile, cv::FileStorage::READ);
	if (undist && fs.isOpened() )
	{ fs["cameraMatrix"] >>  intrinsics; fs["distCoeffs"] >>  distcoeffs; undON=1;}
	else if(undist)
	{cout<<"\n\n\nERROR: Couldn't find Calibration file!\n\n\n"; exit(EXIT_FAILURE);}

	
	if (isJPG(file))                                               //Check whether file is Image or Video
	{still=1; cap>>image;}
	else
	{isVideo=1; cap>>image;}
		
	while(1)
	{
		totalb=clock();
		begin=clock();
		switch(key)
		{
		case 27: 
			cout<<"Exit!\n"; 
			return 0; 
			break;
		case 99: 
			color=(color+1)%5; 
			cout<<"Color Display: "<<colorName[color]<<endl; 
			break;
		case 32: 
			pause=!pause; 
			if(pause) 
			cout<<"Pause"<<endl; 
			else 
			cout<<"Resume"<<endl; 
			break;
		case 'f':
			sprintf(path,"../imgs/frame%02d.jpg",nframes);
			imwrite(path, image); 
			cout<<"Frame Saved"<<endl;
			break;
		case 117: 
			if (undist)
			undON=!undON; 
			if(undON)
			cout<<"Undistort:ON"<<endl; 
			else cout<<"Undistort:OFF"<<endl;
			break;
		case 63233: 
			ratio+=1; 
			break;
		case 63232: 
			ratio-=1; 
			break;
		}
		end=clock();
		if(verb && !pause && !still && nframes%30==0)
		cout<<"Switch Time: "<< double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
		
		begin=clock();                                             		//Initiate execution time counter
		if(isVideo && !pause)
			cap >> image;                                              //Capture Next Frame
		end=clock();
		if(verb && !pause && !still && nframes%30==0)
		cout<<"  Read Time: "<< double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
		begin=clock();
		if(undON && !pause &&!still)
		{
			undistort(image,undistorted, intrinsics, distcoeffs);      //Undistort Frame
			image=undistorted.clone();
		}
				
		cvtColor(image, imhsv, CV_RGB2HSV);                        		//Convert to HSV
		
		end=clock();
		if(verb && !pause && !still && nframes%30==0)
		cout<<"Undist/HSV Time: "<< double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;

		begin=clock();
		if(hist && !pause)                               //Calculate Histogram
			Histogram(imhsv, verb, nframes, 30);

		if(color==0)
			preview=image;                                             //Display Original Image
		else if(color==4)
			preview=imhsv;                                             //Display HSV Image
		else
		{
			split(imhsv,channels);
			preview=channels[color-1];                                //Display H S or V channel.
		}

		//Resize Preview Frame to fit screen
		resize(preview, preview, Size(preview.cols/ratio, preview.rows/ratio));
		imshow("ImageDisplay", preview);
		key=waitKey(10);
		end=clock();
		if(verb && !pause && !still && nframes%30==0)
		cout<<"Display Time: "<< double(end-begin)/CLOCKS_PER_SEC*1000 <<"ms"<<endl;
		
		
		end=clock();
		if(verb && !pause && !still && nframes%30==0)
		cout<<"TOTAL Time: "<< double(end-totalb)/CLOCKS_PER_SEC*1000 <<"ms"<<endl<<endl<<endl;
		nframes++;
	}
	return 0;
}


