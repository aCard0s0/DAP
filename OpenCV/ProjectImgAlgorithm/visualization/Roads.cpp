//============================================================================
// Name          : roads.cpp
// Author        : Manuel Camarneiro
// Modifications : André Cajús
// Description   : C++
//============================================================================

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <string>
#include <vector>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <dirent.h>
#include "restclient-cpp/restclient.h" //é necessario installar no servidor!!
#include <cstring> // for std::strlen

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
//para base64
#define _BASE64_H_

//encode para as fotos
static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

using namespace cv;
using namespace std;

//verificação de .mp4
bool has_suffix(const std::string &str, const std::string &suffix);
//apaga todas as imagens na directoria
void remove();
//divide o video em frames
void getFrames(const std::string &str);
//tratamento de dados do nome do vid
std::string getInfo(int Option, const std::string &filename);
//fazer o post
std::string postCapture(const std::string &idPark, const std::string &idDrone, int carsCount);
std::string base64_encode(char const* bytes_to_encode, unsigned int in_len);
//current date
std::string currentTime();
char* readFileBytes(const char *name);
void associatePhotosWithCapture();

//ja existentes
void findRoad();
void CallBackDist(int, int, int, int, void*);
void MouseLines(int, int, int, int, void*);
void getLDzones();
void drawLDzones();
void LowPass();
void getCars();
void drawCars();
void findFramePos();
void saveRoad();
int getCarColor(int, int, int, bool);
void writeFile();
bool checkRoadPosition();
void refreshDebug(int, void*);

//ja existentes------
const char* imfile[10];
Mat image, imgray, imhsv, edges, road, temp, graph, roadcalib, toDisplay, last, dil, dd, filtered, tmpcanny,novam;
int key=32, windDiv=3,savedFrames=0, h=30, carsCount=0, ncars=0,lastcount=0 ,opt, imset=0;
//Config Variables
int rbs=60, rstep=60, roadTh=80,cannymin=10, cannymax=100;
int rmin=180,gmin=180,bmin=180;
FileStorage fr;
int roadBorder[2][1080];
int meanVals[2][1080];
int LDzones[2][1080];
int LP[2][1080];
Point p1;
char filepath[200];
clock_t begin,end,begint,endt;
bool calibFrames=0, refresh=1, debug=1, verbose=0, secClick=0, adjust=0;
bool findLeft=1, findRight=0;
//------

//escrita do doc de contagem
struct stat statbuf;
int isDir = 0;
//watchfolder
int length, i = 0, fd, wd;
char buffer[BUF_LEN];
//inicializar a string
std::string fn = ".mp4";
//uso de inotify events (documentação online)
struct inotify_event *event;
//para post das fotos
std::string idCapture;

struct car {
	int ycent;			//Car center regarding y axis
	int size;			//Car height
	bool onLeft;
	Scalar color;
};
car found[10];
car lastfound[10];


int main(int argc, char **argv) {

	//apagar todas as imagens da directoria (para efeitos de espaço)
	remove();

	fd = inotify_init();
	if (fd < 0) {
		perror("inotify_init");
	}

	//evento IN_MOVE accionado quando algo é movido para o watchfolder
	wd = inotify_add_watch(fd, "/home/andrecajus/Desktop/ProjectImgAlgorithm", IN_MOVE);

	//está sempre a correr (watchfolder)
	LOOP: while (1) {

		length = read(fd, buffer, BUF_LEN);
		if (length < 0) {
			perror("read");
		}

		event = (struct inotify_event *) &buffer[i];
		if (event->len) {
			//so estamos a usar o in_move_to visto que o filme é movido para o directorio
			if (event->mask & IN_MOVED_TO) {
				if (event->mask & IN_ISDIR) {
					printf("Novo directorio %s introduzido!\n", event->name);
				} else {
					//inicializar string 
					std::string filename(event->name);

					//condição para que o algoritmo de imagem seja inicializado
					if (has_suffix(filename, fn) == true) {

						//para partir em frames
						getFrames(filename);

						FileStorage fr;
						int nframe = -1;
						namedWindow("Window", 1);
						moveWindow("Window", 0, 0);

						//Prepara a janela de Debug
						if (debug) {
							//para mostar graficamente
							namedWindow("EdgeDensity Graph", 1);
							moveWindow("EdgeDensity Graph", 0, 500);
							setMouseCallback("Window", CallBackDist, NULL);
							setMouseCallback("EdgeDensity Graph", MouseLines, NULL);
						}
						//recebe parametros da matriz da estrada
						if (!calibFrames) {
							fr.open("..//config//RoadCIAQ10m.xml", cv::FileStorage::READ);
							fr["roadMatrix"] >> roadcalib;
						}

						cout << carsCount;

						while (1) {
							if (1) {
								nframe++;
								if (nframe != -1) {
									image.copyTo(last);
									cout << endl << "Carros da imagem anterior: " << endl;
									for (int i = 0; i < ncars; i++) {
										lastfound[i] = found[i];
										cout << "Carro " << i << ": " << lastfound[i].ycent << " Color:" << lastfound[i].color;
										if (lastfound[i].onLeft)
											cout << " LEFT" << endl;
										else
											cout << " RIGHT" << endl;
									}
									lastcount = ncars;
								}
								//ter em atençao ficheiros de calibração
								sprintf(filepath, "../imgs/Bebop/CIAQ_vid/frame%03d.jpg", nframe);

								//else para escrita de ficheiro de contagem
								if (access(filepath, F_OK) != -1) {
									image = imread(filepath);
									image.copyTo(toDisplay);
								} else {
							
									//dados do nome do ficheiro enviado
									std::string parkNumb = getInfo(1, filename);
									std::string droneNumb = getInfo(2, filename);

									//postCapture da contagem, park e drone
									idCapture = postCapture(parkNumb, droneNumb, carsCount);

									//arranjar forma de enviar imagens 5 a 5

									//apagar todas as imagens da directoria (para efeitos de espaço)
									remove();

									//voltar a correr while e reset numero de carros
									carsCount = 0;
									goto LOOP;
								}
								refresh = 1;
							}
							if (refresh) {
								begin=clock();
								cvtColor(image, imgray, CV_BGR2GRAY);
								cvtColor(image, imhsv, CV_BGR2HSV);
								Canny(imgray, edges, cannymin, cannymax,3);
								findRoad();
								getLDzones();																		//Compute and Edges Density
								LowPass();																			//Low Pass Filter on Density Array
								getCars();																			//Compare Size of filtered LD zones with expected Car Size
								drawCars();
								if(debug)
									{
										//para mostar GRAFICAMENTE
										resize(dil, dd, edges.size()/3);
										imshow("Dilated", dd);
									}
									else{
										cout << "ROAD NOT FOUND" << endl;
									}
									//para mostar GRAFICAMENTE
									resize(toDisplay, toDisplay, Size(image.cols / windDiv, image.rows / windDiv));
									imshow("Window", toDisplay);
									
																												//Check if detectedCars were already counted before
								if(debug)
								{
									drawLDzones();
									line(graph, Point(0,150-2*h), Point(edges.rows,150-2*h), Scalar(255,255,0));
									imshow("EdgeDensity Graph",graph);
									resize(dil,dil,image.size()/windDiv);
									imwrite("../imgs/final.jpg", toDisplay);
									imshow("Dilated", dil);
								}

							}
							key = waitKey(10);
						}
						
						cout << "Captura concluida!\n";
						printf("Novo ficheiro %s introduzido!\n", event->name);
					}
				}
			}
		}
	}

	(void) inotify_rm_watch(fd, wd);
	(void) close(fd);
	exit(0);
}


//novas adicionadas-------------------------------------------------------

//para verificar os ficheiros que terminam em .mp4
bool has_suffix(const std::string &str, const std::string &suffix) {
	return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

//para selecionar imagens de 5 a 5
void selectPictures() {

}

//para trabalhar com leitura do nome do ficheiro de video
std::string getInfo(int Option, const std::string &filename) {

	std::size_t pos = filename.find("park");
	std::size_t pos2 = filename.find("_d");
	//string aux
	std::string filenameaux = filename.substr(pos2 + 1);
	std::size_t pos3 = filenameaux.find("drone");
	std::size_t pos4 = filenameaux.find("_");
	//para return
	std::string droneOrPark;

	//park number
	if (Option == 1) {
		droneOrPark = filename.substr(4, (pos2 - 4));
		return droneOrPark;
	}
	//drone number
	if (Option == 2) {
		droneOrPark = filenameaux.substr(5, (pos4 - 5));
		;
		return droneOrPark;
	}
	return "";
}

//para remover todas as imagens depois de enviadas pelo service
void remove() {

	DIR *theFolder = opendir("../imgs/Bebop/CIAQ_vid/");
	struct dirent *next_file;
	char filepath[256];

	while ((next_file = readdir(theFolder)) != NULL) {
		// constroi o caminho para cada ficheiro na directoria
		sprintf(filepath, "%s/%s", "../imgs/Bebop/CIAQ_vid/",
				next_file->d_name);
		remove(filepath);
	}
	closedir(theFolder);
}

//para seprar frames do video recebido 
void getFrames(const std::string &str) {

	Mat intrinsics, distcoeffs;
	VideoCapture capture("../" + str + "");
	if (!capture.isOpened()) {
		cout << "Invalid Input Video.\n";
		//return -1;
	}
	Mat frame, preview;
	char path[100];
	double rate = capture.get(CV_CAP_PROP_FPS);
	cout << "Frames por segundo: " << rate << endl;
	int div = 4, j, nframes = 0, count = 0;
	bool pause = false;

	while (true) {
		//delay diminuido para que nao incrave e seja rapido
		j = waitKey(300 / rate);
		switch (j) {
		case 27:
			cout << "EXIT \n"; /*return -1;*/
			break;
		case -1:
			break;
		case 32:
			pause = !pause;
			if (pause)
				cout << "Pause" << endl;
			else
				cout << "Resume" << endl;
			break;
		case 63232:
			div += 1;
			break;
		case 63233:
			div -= 1;
			break;
		default:
			cout << "Tecla: " << j << endl;
			break;
		}
		if (!pause) {
			if (!capture.read(frame))
				break;
			if (nframes % 15 == 0) {
				resize(frame, preview,
						Size(frame.cols / div, frame.rows / div));
				imshow("Preview", preview);
				sprintf(path, "../imgs/Bebop/CIAQ_vid/frame%03d.jpg",
						nframes / 15);
				imwrite(path, frame);
			}
			nframes++;
		}
	}
	destroyWindow("Preview");
	cout << "Acabou algoritmo de getFrames!\n";
	capture.release();
	cout << "Vai iniciar algoritmo de imagem!\n";

}

std::string postCapture(const std::string &idPark, const std::string &idDrone, int carsCount){

	//para converter para string
	string carCount2 = static_cast<ostringstream*>( &(ostringstream() << carsCount) )->str();
	//link do webservice
	std::string capturePost = "http://192.168.160.174:8080/api/capture/park/"+ idPark +"/drone/"+ idDrone ;
	cout << "capturePost: " + capturePost + "\n";

	//body (json)
	std::string bodyCapture ("{ \"Park_idPark\" : "+ idPark +", \"Drone_idDrone\" : "+ idDrone +", \"startedTime\" : \"" +currentTime() + "\", \"finishTime\" : \"" +currentTime() + "\", \"noccupiedLots\" : "+ carCount2 +", \"valid\" : \"true\" }");
	cout << "bodyCapture: " + bodyCapture + "\n";

	//POST capture
    RestClient::Response capture = RestClient::post( capturePost, "application/json", bodyCapture );
    //cout << "Capture: " + capture + "\n";
    // get idcapture
    std::string idCapture = capture.body.substr(13, capture.body.find(",") - 13);
    //cout << idCapture << "\n";

    return idCapture;
}

//recebe tempo actual para ser usado no post da capture
std::string currentTime(){

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,80,"%Y-%m-%dT%H:%M:%S",timeinfo);
	std::string str(buffer);

	std::cout << str;

	return str;
}

//para tratar de envio das fotos escolhidas
void associatePhotosWithCapture(){

	//---------------------------TODO: readphotos with cicle/array
	// Read Photo
    char* oData = readFileBytes("dbDrone.png");
    //encode base64, converte bytes da foto
    std::string encodeData = base64_encode( oData, strlen(oData) );

    /*/ POST photo
    std::string photoPost = "http://192.168.160.174:8080/api/photo/capture/"+ idCapture ;
    std::string bodyPhoto ("{ \"fileName\" : \"teste\", \"latitude\" : \"\", \"longitude\" : \"\", \"heigth\" : \"\", \"photo\" : \""+ encodeData +"\"}");
    //cout << bodyPhoto << "\n";
    RestClient::Response photo = RestClient::post( photoPost, "application/json", bodyPhoto );
    //cout << photo.body << "\n"; */
}

char* readFileBytes(const char *name)  
{  
    ifstream fl(name);  
    fl.seekg( 0, ios::end );  
    size_t len = fl.tellg();  
    char *ret = new char[len];  
    fl.seekg(0, ios::beg);   
    fl.read(ret, len);  
    fl.close();  
    return ret;  
}  

//encode dos bytes das fotos
std::string base64_encode(char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';
  }

  return ret;

}

//------------------------------------------------------------------


void configAdjusts()
{
	// moveWindow("Window", 0, 0);
	// namedWindow("EdgeDensity Graph", 1);
	// moveWindow("EdgeDensity Graph", 0, 500);
	// namedWindow("Dilated", 1);
	// moveWindow("Dilated", 700, 0);
	namedWindow("Adjusts");
	// createTrackbar("RoadBlockSize", "Adjusts", &rbs, 100, refreshDebug);
	createTrackbar("CannyMin", "Adjusts", &cannymin, 100, refreshDebug);
	createTrackbar("CannyMax", "Adjusts", &cannymax, 1080, refreshDebug);
	// setMouseCallback("Window", CallBackDist, NULL);
	// setMouseCallback("EdgeDensity Graph", MouseLines, NULL);

}

void filterColor()
{
	filtered=Mat::zeros(image.rows,image.cols,CV_8UC1);
	int r,g,b;
	for(int i=0; i<image.rows; i++)
	for(int j=0; j<image.cols; j++)
	{
		b=image.at<Vec3b>(i,j)[0];
		g=image.at<Vec3b>(i,j)[1];
		r=image.at<Vec3b>(i,j)[2];
		if(r>rmin && g>gmin && b>bmin)
		{
			filtered.at<uchar>(i,j)=255;
		}
	}
}

void getParameters(int argc, char** argv)
{

	imfile[0]="../imgs/ieeta/frame%03d.jpg";
	imfile[1]="../imgs/Bebop/p2_return/frame%03d.jpg";
	imfile[2]="../imgs/Bebop/CIAQ2/frame%03d.jpg";
	imfile[3]="../imgs/Bebop/Library_go/frame%03d.jpg";
	imfile[4]="../imgs/Bebop/Library_return/frame%03d.jpg";
	imfile[5]="../imgs/Bebop/tarPark/frame%03d.jpg";

	while ((opt = getopt(argc, argv, "f:vdaclr")) != -1) {
		switch (opt) {
			case 'f':																										//USelect image set
				imset=atoi(optarg);
		case 'v':                                                      //Verbose option
			verbose = 1;
			break;
		case 'd':																											//Show debug images
			debug=1;
			break;
		case 'a':																											//Adjust Mode
			adjust=1;
			break;
		case 'c':																											//Calibrate Road Position
			calibFrames=1;
			break;
		case 'l':																											//Find cars only on left;
			findLeft=1;
			findRight=0;
			break;
		case 'r':																											//Find cars only on left;
			findRight=1;
			findLeft=0;
			break;
		default:
			fprintf(stderr, "Usage: %s [-d debug] [-v (verbose)] \n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
}



int getCarColor(int st, int end, int ncars, bool carOnLeft)
{
	int b=0, g=0, r=0, count=0, width;
	int hmin=15, hmax=50, smin=12, smax=45, vmin=110, vmax=200;

	if(carOnLeft)
	{
		for(int y=st; y<end; y++)
		{
			width=roadBorder[0][y]+roadBorder[1][y];
			for(int x=roadBorder[0][y]; x<edges.cols/2 && x<width+roadBorder[0][y]; x++)
			{
				//if((int)dil.at<uchar>(y,edges.cols/2-x)==0)
				//{
					count++;
					b+=imhsv.at<Vec3b>(y,edges.cols/2-x)[0];
					g+=imhsv.at<Vec3b>(y,edges.cols/2-x)[1];
					r+=imhsv.at<Vec3b>(y,edges.cols/2-x)[2];
				//}
			}
		}
	}
	else
	{
		for(int y=st; y<end; y++)
		{
			width=roadBorder[0][y]+roadBorder[1][y];
			for(int x=roadBorder[1][y]; x<edges.cols/2 && x<width+roadBorder[1][y]; x++)
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

	if (imset==5 && b<hmax && b>hmin && g<smax && g>smin && r<vmax && r>vmin)
	{
		cout<<"H:" <<b<<" S:"<<g<<" V:"<<r<<endl;
		found[ncars].color=Scalar(0,0,0);
	}
	return ncars;
}

void getCars()						//Cluster Low Edge Density zones and find cars
{
	int thr=30, count=0, ignoreLimit=50, center, parkSize;
	parkSize=edges.rows/2*1885/10000+137;

	ncars=0;
	if(findLeft)
	for(int i=edges.rows; i>edges.rows/2; i--)			//LEFT PART
	{
		if(LP[0][i]>=thr)
		{
			count++;
			while(LP[0][i]>=thr && i>0)									//LEFT ignore thin HD zones
			{
				count++; i--;
				if(LP[0][i]<thr)
				{
					int tmp=i;
					while(LP[0][tmp]<thr && tmp>0)
					tmp--;
					if(i-tmp<=ignoreLimit)
					i=tmp;
				}
			 }
		}
		if(i<edges.rows/2)													//LEFT car above half image
		{
			if(count>parkSize)
			{
				center=i+count-(parkSize/2);
				found[ncars].ycent=center;
				found[ncars].size=parkSize;
				found[ncars].onLeft=1;
				ncars=getCarColor(i+count-parkSize, i+count, ncars, 1);
				ncars++;
			}
			else																	//LEFT car below half image
			{
				center=i+(count/2);
				found[ncars].ycent=center;
				found[ncars].size=parkSize;
				found[ncars].onLeft=1;
				ncars=getCarColor(i+count-parkSize, i+count, ncars, 1);
				ncars++;

			}
		}
		else
		{
				center=i+count/2;
				parkSize=center*1885/10000+137;

			if(count>parkSize/3 && count<4*parkSize/3)
			{
				found[ncars].ycent=center;
				found[ncars].size=count;
				found[ncars].onLeft=1;
				getCarColor(i, i+count, ncars, 1);
				ncars++;
			}
			else if(count>=4*parkSize/3 && count<3*parkSize)
			{
				found[ncars].ycent=i+count/4;
				found[ncars].size=count/2;
				found[ncars].onLeft=1;
				getCarColor(i, i+count/2, ncars, 1);
				ncars++;
				found[ncars].ycent=i+3*count/4;
				found[ncars].size=count/2;
				found[ncars].onLeft=1;
				getCarColor(i+count/2, i+count, ncars, 1);
				ncars++;
			}
			else if(count>=parkSize*3)
			cout<<endl<<endl<<endl<<endl<<"Car Size Out Of RANGE"<<endl;
		}
		count=0;
	}

	if(findRight)
	for(int i=edges.rows; i>edges.rows/2; i--)
	{
		if(LP[1][i]>=thr)
		{
			count++;
			while(LP[1][i]>=thr && i>0)
			{
				count++; i--;
				if(LP[1][i]<thr)
				{
					int tmp=i;
					while(LP[1][tmp]<thr && tmp>0)
					tmp--;
					if(i-tmp<=ignoreLimit)
					i=tmp;
				}
			 }
		}
		if(i<edges.rows/2)
		{
			if(count>parkSize)
			{
				cout<<"Carro no MEIO MAIOR DO QUE PARKSIZE"<<endl;
				center=i+count-(parkSize/2);
				found[ncars].ycent=center;
				found[ncars].size=parkSize;
				found[ncars].onLeft=0;
				ncars=getCarColor(i+count-parkSize, i+count, ncars, 0);
				ncars++;
			}
			else
			{
				cout<<"Carro no MEIO TAMANHO CORRECTO"<<endl;
				center=i+(count/2);
				found[ncars].ycent=center;
				found[ncars].size=parkSize;
				found[ncars].onLeft=0;
				ncars=getCarColor(i+count-parkSize, i+count, ncars, 0);
				ncars++;

			}
		}
		else
		{
				center=i+count/2;
			if(count>parkSize/3 && count<4*parkSize/3)
			{
				found[ncars].ycent=center;
				found[ncars].size=count;
				found[ncars].onLeft=0;
				ncars=getCarColor(i, i+count, ncars, 0);
				ncars++;
			}
			else if(count>=4*parkSize/3 && count<3*parkSize)
			{
				found[ncars].ycent=i+count/4;
				found[ncars].size=count/2;
				found[ncars].onLeft=0;
				ncars=getCarColor(i, i+count/2, ncars, 0);
				ncars++;
				found[ncars].ycent=i+3*count/4;
				found[ncars].size=count/2;
				found[ncars].onLeft=0;
				ncars=getCarColor(i+count/2, i+count, ncars, 0);
				ncars++;
			}
			else if(count>=parkSize*3)
			cout<<endl<<endl<<endl<<endl<<"Car Size Out Of RANGE"<<endl;
		}
		count=0;
	}

}

void drawCars()						//Draw and find repeated cars
{
	stringstream ss;
	string str;
	bool match;
	for(int i=0; i<ncars;i++)
	{
		if(found[i].color[0]!=0 || found[i].color[1]!=0 || found[i].color[2]!=0){
		int top = (found[i].ycent-found[i].size/2);
		int bottom = (found[i].ycent+found[i].size/2);
		int valh=0, vals=0, valv=0, dist=0;
		match=0;
		for(int j=0; j<lastcount;j++)
		{
			if(lastfound[j].onLeft==found[i].onLeft && lastfound[j].ycent<found[i].ycent)
			{
				valh=abs(found[i].color[0]-lastfound[j].color[0]);
				vals=abs(found[i].color[1]-lastfound[j].color[1]);
				valv=abs(found[i].color[2]-lastfound[j].color[2]);
				dist=abs(found[i].ycent-lastfound[j].ycent);
				if(valh<60 && vals<70 && valv<100 && dist<400)
				{
					match=1;
					//cout<<"Repeated: LastPos:"<<lastfound[j].ycent<<" Color:"<<lastfound[j].color<<endl;
					str="Already COUNTED";
					if(found[i].onLeft)
					{
						rectangle(toDisplay, Point(edges.cols/10, top), Point(3*edges.cols/10, bottom), Scalar(255,0,0), 20);
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
			}}
		}
	}
}

void LowPass()						//LowPass Filter EdgeDensity Values
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

void drawLDzones()				//Draw Graphic of Edges Density as function of Image Row
{
	graph=Mat::zeros(150, 1080, CV_8UC3);
	for(int i=1; i<1080; i++)
	{
		line(graph, Point(i-1,150-2*LP[0][i-1]), Point(i,150-2*LP[0][i]), Scalar(0,0,255));
		line(graph, Point(i-1,150-2*LP[1][i-1]), Point(i,150-2*LP[1][i]), Scalar(0,255,0));
	}
	line(graph, Point(edges.rows/2,0), Point(edges.rows/2, 200), Scalar(255,255,255));
}

void getLDzones()					//Computes Edges Density for every row
{
	Mat element=getStructuringElement(MORPH_RECT,Size(5,5), Point(2,2));
	dilate( edges, dil, element, Point(-1,-1), 2);
	dil = dil|road;
	int width, count, blacks;
	for(int y=0; y<image.rows; y++)
	{
		if (findLeft&&findRight)
		width=roadBorder[0][y]+roadBorder[1][y];
		else
		width=3*image.cols/10;
		count=0; blacks=0;
		if(findLeft)
		{
			for(int x=roadBorder[0][y]; x<edges.cols/2 && x<width+roadBorder[0][y]; x++)
			{
				count++;
				if((int)dil.at<uchar>(y,edges.cols/2-x)==0)
				blacks++;
			}
			LDzones[0][y]=blacks*100/count;
		}
		count=0; blacks=0;
		if(findRight)
		{
			for(int x=roadBorder[1][y]; x<edges.cols/2 && x<width+roadBorder[1][y]; x++)
			{
				count++;
				if((int)dil.at<uchar>(y,edges.cols/2+x)==0)
				blacks++;
			}
			LDzones[1][y]=blacks*100/count;
		}
	}
}

void findRoadLeft()
{
	Mat roadblocks = Mat::zeros(edges.rows, edges.cols, CV_8UC1);
	road = Mat::ones(edges.rows, edges.cols, CV_8UC1)*255;
	int val,count,width=3*image.cols/10, leftborder, rightborder;
	bool edgeLeft=0;
	int threshold=roadTh*10;
	//DETECT ROAD AND PRINT IT ON MAT ROADBLOCKS
	for(int y=0; y<edges.rows;y+=20)
	for(int x=0; x<edges.cols/2;x+=10)
	{
		count=0;
		for(int i=x; i<x+20 && i<edges.cols/2; i++)
		for(int j=y; j<y+100 && j<edges.rows; j++)
		{
			val=(int)dil.at<uchar>(j,edges.cols/2-i);
			if(val==255)
			count++;
		}
		if(count>=threshold)
		{
			rectangle(roadblocks, Point2d(edges.cols/2-x,y), Point2d(edges.cols/2-(x+20),y+100), Scalar(255), CV_FILLED);
			x=edges.cols/2;
		}
	}

	for(int j=0; j<edges.rows; j++)
	for(int i=0; i<edges.cols/2; i++)
	{
		val=(int)roadblocks.at<uchar>(j,edges.cols/2-i);
		if(val==255 && !edgeLeft)
		{
			road.at<uchar>(j,edges.cols/2-i)=0;
			roadBorder[0][j]=i;
			int x=i;
			while(x<edges.cols/2 && x<i+width)
			{road.at<uchar>(j,edges.cols/2-x)=0; x++;}
			i=edges.cols/2;
		}
	}

	if(debug)
	{
		for(int i=0; i<road.cols;i++)
		for(int j=0; j<road.rows;j++)
		if(road.at<uchar>(j,i)==255)
		toDisplay.at<Vec3b>(j,i)[0]=255;
		resize(road, roadblocks, Size(image.cols/windDiv, image.rows/windDiv));
		imshow("Window3", roadblocks);
	}
}



void findRoad()						//Saves Road Limits to int RoadBorder[][] and Mat road
{
	Mat roadblocks = Mat::zeros(edges.rows, edges.cols, CV_8UC1);
	road = Mat::zeros(edges.rows, edges.cols, CV_8UC1);
	int val,count,width, leftborder, rightborder;
	bool edgeLeft=0,edgeRight=0;
	int threshold=roadTh*rbs*rbs/100;
	//DETECT ROAD AND PRINT IT ON MAT ROADBLOCKS
	for(int x=0; x<edges.cols;x+=rstep)
	for(int y=0; y<edges.rows;y+=rstep)
	{
		count=0;
		for(int i=x; i<x+rbs && i<edges.cols; i++)
		for(int j=y; j<y+rbs && j<edges.rows; j++)
		{
			val=(int)edges.at<uchar>(j,i);
			if(val==0)
			count++;
		}
		if(count>=threshold)
		rectangle(roadblocks, Point2d(x,y), Point2d(x+rbs,y+rbs), Scalar(255), CV_FILLED);
	}

	for(int y=0; y<roadblocks.rows; y++)
	{
		width=0;
		val=(int)roadblocks.at<uchar>(y,roadblocks.cols/2);
		edgeLeft=0;edgeRight=0;
		if(val==0)
		{
			road=Mat::ones(edges.rows,edges.cols,CV_8UC1)*255;
			return;
		}
		else
		{
			for(int x=0; x<edges.cols/2 && (!edgeLeft || !edgeRight); x++)
			{
				if((int)roadblocks.at<uchar>(y,edges.cols/2-x)==255 &&!edgeLeft )
				{road.at<uchar>(y,edges.cols/2-x)=255; width++;}
				if((int)roadblocks.at<uchar>(y,edges.cols/2-x)==0 && !edgeLeft )
				{edgeLeft=1;roadBorder[0][y]=x;}

				if((int)roadblocks.at<uchar>(y,edges.cols/2+x)==255 && !edgeRight )
				{  road.at<uchar>(y,edges.cols/2+x)=255; width++;}
				if((int)roadblocks.at<uchar>(y,edges.cols/2+x)==0 && !edgeRight )
				{edgeRight=1;roadBorder[1][y]=x;}
			}
			leftborder=0; rightborder=0;
			for(int x=0; x<edges.cols/2; x++)
			{
				if((int)road.at<uchar>(y,road.cols/2-x)==0 && leftborder==0)
				leftborder++;
				if(leftborder>0 && leftborder<width)
				leftborder++;
				if(leftborder>=width)
				road.at<uchar>(y,road.cols/2-x)=255;

				if((int)road.at<uchar>(y,road.cols/2+x)==0 && rightborder==0)
				rightborder++;
				if(rightborder>0 && rightborder<width)
				rightborder++;
				if(rightborder>=width)
				road.at<uchar>(y,road.cols/2+x)=255;
			}
		}
	}
	if(debug)
	{
		for(int i=0; i<road.cols;i++)
		for(int j=0; j<road.rows;j++)
		if((int)road.at<uchar>(j,i)==255)
		toDisplay.at<Vec3b>(j,i)[0]=255;
	}
}

bool checkRoadPosition()	//Compares calibration Road with actual RoadBorder[][]
{
	int calibleft,calibright, wrong=0;
	for(int y=roadcalib.rows/2; y<roadcalib.rows; y++)
	{
		calibleft=roadcalib.at<ushort>(0,y);
		calibright=roadcalib.at<ushort>(1,y);
		if(abs(calibleft-roadBorder[0][y])>150 || abs(calibright-roadBorder[1][y])>150)
		wrong++;
	}
	if (wrong<70)
	return 1;
	else
	return 0;
}

void saveRoad()						//Save single frame roadBorder for Calibration
{
	for(int y=0; y<1080; y++)
	{
		meanVals[0][y]+=roadBorder[0][y];
		meanVals[1][y]+=roadBorder[1][y];
	}
	cout<<"SavedFrame"<<savedFrames<<endl;
	savedFrames++;
}

void writeFile()					//Compute Mean of roadBorders and save Calibration File
{
	Mat calibFrames = Mat::zeros(1080,2, CV_16UC1);
	for(int y=0;y<1080;y++)
	{
		calibFrames.at<ushort>(0,y)=(meanVals[0][y]/50);
		calibFrames.at<ushort>(1,y)=(meanVals[1][y]/50);
	}
	FileStorage fs("..//config//RoadCIAQ10m.xml", FileStorage::WRITE);
	fs << "roadMatrix" << calibFrames;
	cout<<"FILE SAVED"<<endl;

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

void CallBackDist(int event, int x, int y, int flags, void* userdata)
{
		if  ( event == EVENT_LBUTTONDOWN && secClick)
			{
				int dist=sqrt(pow(windDiv*x-windDiv*p1.x,2)+pow(windDiv*y-windDiv*p1.y,2));
				cout<<"Second Point X:"<<x*windDiv<<" Y:"<<y*windDiv<<endl;
				cout<<"Dist: "<<dist<<endl;
				secClick=0;
			}
		else if  ( event == EVENT_LBUTTONDOWN && !secClick)
			{ p1=Point(x,y); cout<<"First Point X:"<<windDiv*x<<" Y:"<<windDiv*y<<endl; secClick=1;}
}

void refreshDebug( int, void* )
{
	refresh=1;
}
