
// FUNÇõES NAO UTILIZADAS


void clusterCars()
{
	int st, pix, stpos, last;
	bool change=0;

	last=stripes.at<uchar>(edges.rows,0);
	for(int y=edges.rows-1; y>edges.rows/2; y--)
	{
		pix=stripes.at<uchar>(y,0);
		if(pix!=last && pix==255)
		{
			st=y;
		}
		else if(pix!=last && pix==120)
		{
			if(st-y<40)
			for(int p=st; p>y;p--)
			stripes.at<uchar>(p,0)=120;
		}
		last=pix;
	}
	last=stripes.at<uchar>(edges.rows,edges.cols);
	for(int y=edges.rows-1; y>edges.rows/2; y--)
	{
		pix=stripes.at<uchar>(y,0);
		if(pix!=last && pix==255)
		{
			st=y;
		}
		else if(pix!=last && pix==120)
		{
			if(st-y<40)
			for(int p=st; p>y;p--)
			stripes.at<uchar>(p,edges.cols)=120;
		}
		last=pix;
	}		
	//LEFT PART
	stpos=edges.rows;
	st=(int)stripes.at<uchar>(stpos,edges.cols);
	cout<<"Valor inicial "<<st<<endl;
	for(int y=edges.rows; y>0; y--)
	{
		pix=(int)stripes.at<uchar>(y,0);
		if(st!=pix && stpos-y>20) //Ignore thin lines
		{
			if(st==120)
			circle(image, Point(edges.cols/8, (stpos+y)/2), 30, Scalar(0,0,255), CV_FILLED);
			
			st=pix;stpos=y;
			
		}
	}	
}






void getCarsHorizontal()
{
	Mat element=getStructuringElement(MORPH_RECT,Size(5,5), Point(2,2));
	Mat aux;
	dilate( edges, aux, element, Point(-1,-1), 2);
	toAnalyze = aux|road;
	stripes=Mat::ones(edges.rows, edges.cols, CV_8UC1)*255;
	
	int width, count, blacks;
	for(int b=0; b<image.rows; b+=band)
	{
		
		width=roadBorder[0][b]+roadBorder[1][b];
		count=0; blacks=0;
		for(int y=b; y<b+band; y++)
		for(int x=roadBorder[0][y]+50; x<edges.cols/2 && x<width+roadBorder[0][y]; x++)
		{
			count++;
			if((int)toAnalyze.at<uchar>(y,edges.cols/2-x)==0)
			blacks++;
		}
		if(blacks>=(count*percBlacks/100))
		{
			rectangle(stripes, Point(0,b), Point(edges.cols/2-roadBorder[0][b],b+band), 120, CV_FILLED);
			rectangle(toAnalyze, Point(0,b), Point(edges.cols/2-roadBorder[0][b],b+band), 120, CV_FILLED);
			
		}
		count=0; blacks=0;
		for(int y=b; y<b+band; y++)
		for(int x=roadBorder[1][y]; x<edges.cols/2 && x<width+roadBorder[1][y]; x++)
		{
			count++;
			if((int)toAnalyze.at<uchar>(y,edges.cols/2+x)==0)
			blacks++;
		}
		if(blacks>=(count*percBlacks/100))
		{
			rectangle(toAnalyze, Point(edges.cols/2+roadBorder[1][b],b), Point(edges.cols,b+band), 120, CV_FILLED);
			rectangle(stripes, Point(edges.cols/2+roadBorder[1][b],b), Point(edges.cols,b+band), 120, CV_FILLED);
		}
	}	
}





void findCarBlocks()
{
	carBlocks = Mat::zeros(edges.rows, edges.cols, CV_8UC1);
	int count, st;
	int threshold=carTh*cbs*cbs/100;
	for(int y=0; y<edges.rows;y+=cstep)
	{
		st=edges.cols/2+roadBorder[1][y];
		for(int x=st; x<edges.cols && (int)road.at<uchar>(y,x)==0;x+=cstep)
		{
			count=0;
			for(int i=x; i<x+cbs && i<edges.cols; i++)
			for(int j=y; j<y+cbs && j<edges.rows; j++)
			{
				if((int)edges.at<uchar>(j,i)==0)
				count++;
			}
			if(count>=threshold)
			rectangle(carBlocks, Point2d(x,y), Point2d(x+cbs,y+cbs), Scalar(255), CV_FILLED);
		}
		st=edges.cols/2-roadBorder[0][y];
		for(int x=st; x>0 && (int)road.at<uchar>(y,x)==0;x-=cstep)
		{
			count=0;
			for(int i=x-cbs; i<x && i>0; i++)
			for(int j=y; j<y+cbs && j<edges.rows; j++)
			{
				if((int)edges.at<uchar>(j,i)==0)
				count++;
			}
			if(count>=threshold)
			rectangle(carBlocks, Point2d(x,y), Point2d(x+cbs,y+cbs), Scalar(255), CV_FILLED);
		}
	}
}


void findFramePos()
{
	int lastpix[3];
	int actpix[3];
	long diff, min;
	int mdelta=0;
	Mat hh;
	last.copyTo(hh);
	for(int delta=0; delta<300; delta++)
	{
		diff=0;
		for(int r=1000; r<edges.rows; r++)
		for(int c=0; c<edges.cols; c++)
		for(int ch=0; ch<3; ch++)
		{
			lastpix[ch]=(int)last.at<Vec3b>(r-delta,c)[ch];
			actpix[ch]=(int)image.at<Vec3b>(r,c)[ch];
			diff+=abs(lastpix[ch]-actpix[ch]);
		}
		if(diff<min || delta==0)
		{min=diff;  mdelta=delta;}
	}
	for(int r=1000; r<edges.rows; r++)
	for(int c=0; c<edges.cols; c++)
	{
		hh.at<Vec3b>(r-mdelta,c)[2]=255;
		toDisplay.at<Vec3b>(r,c)[2]=255;
	}
	
	resize(hh, hh, Size(image.cols/3, image.rows/3));
	imshow("LAST", hh);
}




//		createTrackbar("CannyMin", "Window", &cannymin, 500);
//		createTrackbar("CannyMax", "Window", &cannymax, 500);
//		createTrackbar("RoadBlockSize", "Window", &rbs, 100);
//		createTrackbar("RoadBlockStep", "Window", &rstep, 100);
//		createTrackbar("RoadThreshold", "Window", &roadTh, 100);
//		setMouseCallback("Window", CallBackFunc, NULL);
//		createTrackbar("Line", "EdgeDensity Graph", &h, 100,refreshDebug);
//		createTrackbar("CarBlockSize", "Window", &cbs, 100,refreshDebug);
//		createTrackbar("CarBlockStep", "Window", &cstep, 100,refreshDebug);
//		createTrackbar("CarRoadThreshold", "Window", &carTh, 100,refreshDebug);
//		createTrackbar("MaxCarContour", "Window", &maxcont, 5000,refreshDebug);
//		createTrackbar("CarBlockStep", "Window", &mincont, 2000,refreshDebug);
//		createTrackbar("PercentageForCarBand", "Window", &percBlacks, 100,refreshDebug);
//		createTrackbar("BandWidth", "Window", &band, 200,refreshDebug);
