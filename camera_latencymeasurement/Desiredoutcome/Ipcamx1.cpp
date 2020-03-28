#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <time.h> 
#include <fstream>
#include <chrono>
#include "mqtt.h"
//#include <conio.h>
using namespace std;
using namespace cv;
using namespace std::chrono;

clock_t begin2;
int a=0;
int first_time=1;
int count_start=0;
double elapsed_secs1;
auto t_start = std::chrono::high_resolution_clock::now();
struct rectangles_Info{ 
	Point centre;
	Point P1;
	Point P2;
	int width;
	int height;
rectangles_Info(Point  centre, Point P1,Point P2,int width,int height);
};

vector<rectangles_Info*>rectangles;
vector<double>timeStamp;
vector<int>id;
vector<double>timeStampa;
vector<int>ida;
vector<double>timeStampb;
vector<int>idb;

vector<double>timeStampc;
vector<int>idc;


rectangles_Info:: rectangles_Info(Point c, Point p1,Point p2,int w,int h){

	centre=c;
	P1=p1;
	P2=p2;
	width=w;
	height=h;
}
bool track1(rectangles_Info* ROI,Mat COI){
	int x=ROI->P1.x;
	int y=ROI->P1.y;
	int countWPixel=0;
	float percentage;

	for(int i=y;i < (ROI->P1.y+ROI->height);i++){
		for(int j=x;j<(ROI->P1.x+ROI->width);j++){
		 
		  if(COI.at<uchar>(i,j)>200)countWPixel++ ;
		 }
	}
	percentage=float(countWPixel)/(ROI->height*ROI->width)*100;

	if(percentage > 60)
		return true;
	else return false;
		  
}
bool trackblack(rectangles_Info* ROI,Mat COI){
	int x=ROI->P1.x+(ROI->width/4);
	int y=ROI->P1.y+(ROI->height/4);
	int x1=x+(ROI->width/2);
	int y1=y+(ROI->height/2);
	int countWPixel=0;
	int countBPixel=0;
	for(int i=y;i < y1;i++){
		for(int j=x;j<x1;j++){
		//  s = cvGet2D(COI,i,j);
		  	
		  if(COI.at<uchar>(i,j)<100)countBPixel++ ;
		  else if (COI.at<uchar>(i,j)>150)countWPixel++;
		 }
	}
		
	if(countBPixel <200)
		return true;
	else return false;
		  
}
void regionOfInterest(Mat ori){
	rectangles.clear();
	//	Mat ori(Original);
		//imshow("ori",ori);
		Mat NoiseReduction = Mat::zeros( ori.size(), CV_8UC1 );
		Mat Output = Mat::zeros( ori.size(), CV_8UC3 );
		morphologyEx(ori,NoiseReduction,MORPH_CLOSE,Mat(),Point(-1,-1),1);
		morphologyEx(NoiseReduction,NoiseReduction,MORPH_OPEN,Mat(),Point(-1,-1),2);	
	//	imshow("NoiseReduction",NoiseReduction);

		vector< vector<Point> > contours;
		vector<Vec4i> hierarchy;
	

	findContours( NoiseReduction, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	//drawContours( Output, contours, -1, Scalar(0, 255, 255),CV_FILLED, 8, hierarchy, 0, Point() );
	

	if (!contours.empty())
	{
		//--- Find 2 largest contours
		double contourSize;
		double largestSize1 = arcLength(contours[0], true);
		double largestSize2 = 0;
		int largestId1 = 0, largestId2 = 0;
		Rect bounding_rect;
		for( unsigned int i = 1; i<contours.size(); i++ )
		{
			contourSize = arcLength(contours[i], true);
			if (contourSize > largestSize1) 
			{
				largestSize2 = largestSize1;
				largestId2 = largestId1;
				largestSize1 = contourSize;
				largestId1 = i;
			}
			else if (contourSize > largestSize2) 
			{
				largestSize2 = contourSize;
				largestId2 = i;
			}
		}

				//drawContours( Output, contours, largestId1, Scalar(0, 0, 255), 1, 8, hierarchy, 0, Point() );
				drawContours( Output, contours, largestId2, Scalar(255, 0, 255), CV_FILLED, 8, hierarchy, 0, Point() );
				 bounding_rect=boundingRect(contours[largestId2]);
				  rectangle(Output, bounding_rect,  Scalar(0,255,0),1, 8,0); 
				  //cout <<  "  X:" << bounding_rect.x << "  " << bounding_rect.y << "  " <<  bounding_rect.width <<  "  " <<  bounding_rect.height<<endl;
				  float lengthOf_R= bounding_rect.width/5;
				  Point c,p1 ,p2; int width,height;
				  c.x=bounding_rect.x+(lengthOf_R/2);
				  c.y=bounding_rect.y+(bounding_rect.height/2);
				  p1.x= bounding_rect.x;p1.y=bounding_rect.y;
				  p2.x=bounding_rect.x+ lengthOf_R;
				  p2.y=bounding_rect.y+bounding_rect.height;
				  width=lengthOf_R;
				height=bounding_rect.height;
				  rectangles.push_back(new rectangles_Info(Point(c.x,c.y),Point(p1),Point(p2),width,height));
				  circle(ori, c, 2,  Scalar(255,125,125),3, 8, 0);
				   rectangle( ori, p1, p2, Scalar(255,0,0), 4, 8, 0);
				  for(int i=1;i<5;i++){
					c.x=c.x+lengthOf_R;
					c.y=bounding_rect.y+(bounding_rect.height/2);
					p1.x= p1.x+lengthOf_R;
					p2.x= p2.x+lengthOf_R;
					width=lengthOf_R;
					height=bounding_rect.height;
					 rectangle( ori, p1, p2, Scalar(255,0,0), 4, 8, 0);
					  circle(ori, c, 2,  Scalar(255,125,125),3, 8, 0);
				  rectangles.push_back(new rectangles_Info(Point(c.x,c.y),Point(p1),Point(p2),width,height));
				  }
				  
					
		
	}
	imshow("Output",Output);
/*	imshow("ori",ori);*/

}
void printlogfile(vector<int> id ,vector<double> time,string filename){

	ofstream outdata;
	outdata.open(filename, ios::app); 
	for(unsigned int i=0;i<time.size();i++){
	outdata << std::fixed<< id.at(i) << "\t" << time.at(i) <<endl;
	cout << std::fixed<< id.at(i) << "\t" << time.at(i) <<endl;

	}
}
void object_recognition(Mat image,string name,int& event_id){
		Mat hsvframe ;
		Mat gray = Mat::zeros(image.size(),CV_8UC1);
		Mat threshyellow ;
		Mat threshred ;
		Mat threshcyan;
		Mat threshblue ;
		Mat threshgreen ;
		Mat threshorange;
		Mat threshviolet ;
		Mat dst ;
		Mat dst1;
		Mat dst2;
		Mat dst3 ;
		Mat dst4;

	cvtColor( image, gray, CV_BGR2GRAY);
	cvtColor(image, hsvframe, COLOR_BGR2HSV);
	inRange(hsvframe, Scalar(0, 100, 0), Scalar(22, 255, 255),threshorange);
	inRange(hsvframe, Scalar(21, 50, 0), Scalar(38, 244, 255),threshyellow);
	inRange(hsvframe,Scalar(38, 100, 0), Scalar(75, 255, 255),threshgreen);// green 
	inRange(hsvframe,Scalar(75, 100, 0), Scalar(100, 255, 255),threshcyan);//cyan
	inRange(hsvframe,Scalar(101, 100, 0), Scalar(130, 255, 255),threshblue);//blue
	inRange(hsvframe,Scalar(130, 100, 0), Scalar(159, 255, 255),threshviolet);//purple
	inRange(hsvframe,Scalar(160, 50, 0), Scalar(179, 255, 255),threshred);//red
	//dst =  Mat::zeros(image.size(),CV_8UC1);
	add(threshorange,threshyellow,dst);
	add(threshgreen, threshcyan, dst1);
	add(threshblue, threshviolet, dst2);
	add(threshred, dst, dst3);
	add(dst1, dst2, dst2);
	add(dst2, dst3, dst4);

  for(int j=0;j<dst4.rows;j++) {
    for (int i=0;i<dst4.cols;i++) {
        if (dst4.at<uchar>(j,i)==255)dst4.at<uchar>(j,i)=0;
		else if (dst4.at<uchar>(j,i)==0)dst4.at<uchar>(j,i)=255;
      }
    }
 
  regionOfInterest(dst4);
 char countV[10];unsigned int i;
  for( i =0;i< rectangles.size();i++){
	  if(track1(rectangles.at(i),threshorange)){
		  if(trackblack(rectangles.at(i),threshorange) ){
			 
			  countV[i]='1'; 
		  }
		  else { 
		  countV[i]='5';}
	 
	  }
	  else if(track1(rectangles.at(i),threshyellow)){
	  //cout << "2= true" <<endl;
		countV[i]='2';
	  }
	  else if(track1(rectangles.at(i),threshgreen)){
		  if(trackblack(rectangles.at(i),threshgreen))
		  {//cout << "4=true";
		   countV[i]='3';}
		  else  {//cout << "3= true" <<endl;
		  
		  countV[i]='4';
		  }
	 
	  }
	   else if(track1(rectangles.at(i),threshcyan)){
	  //cout << "6= true" <<endl;
		countV[i]='6';
	  }
	    else if(track1(rectangles.at(i),threshblue)){
		  if(trackblack(rectangles.at(i),threshblue))
		  {//cout << "0=true";
		   countV[i]='0';}
		  else  {//cout << "7= true" <<endl;
		  countV[i]='7';
		  }
	 
	  }
		  else if(track1(rectangles.at(i),threshviolet)){

		countV[i]='9';
	  }

		    else if(track1(rectangles.at(i),threshred)){
	  //cout << "8= true" <<endl;
		countV[i]='8';
	  }
	//  cout << endl << endl <<endl ; 
	}
for(int j=i;j<20;j++){
countV[j]=' ';

}


event_id = atoi(countV); 
int fontFace=FONT_HERSHEY_SCRIPT_SIMPLEX;
double fontScale=5;
double thickness=5;
Point textOrg(150,400);
putText(image,countV,textOrg,fontFace,fontScale,Scalar(255,255,0),thickness,8);



   imshow(name,image);

       
  

}
void TimerS(high_resolution_clock::time_point& time_start ,int thour,int tmin ){
  time_t t = time(0);
  struct tm * ctime = localtime( & t );  
  if(int(ctime->tm_hour)==thour && int(ctime->tm_min) ==tmin && int(ctime->tm_sec) ==0){
	   count_start=1;
	   time_start = std::chrono::high_resolution_clock::now(); 
	}
  
}
void TimerE(high_resolution_clock::time_point time_start,double &time_on){
  
  


 	high_resolution_clock::time_point  t_end = std::chrono::high_resolution_clock::now();
	time_on=std::chrono::duration<double, std::milli>(t_end-time_start).count();
	time_on=time_on/1000;
	if (time_on<1)
		time_on=0;
		
		//time_t t1= time(0);
		//struct tm * ctime1 = localtime( & t1);	 
	//cout <<  setfill(' ') <<  setw(15) << fixed    <<time_on <<endl  ;


	
  
  
}
void publishData( string cam, int id , double time){

  MqttWrapper *mqttclient = new MqttWrapper(NULL, "localhost", 1883);
    stringstream str;  
    
    str  << id  << "\t\t\t" <<  time;
	mqttclient->publish(NULL, "test", str.str().size(), str.str().c_str());
	
	mqttclient->loop();	
  
  
}
int main(int, char**) {
  VideoCapture vcap;VideoCapture vcapa;VideoCapture vcapb;VideoCapture vcapc;
  Mat image;Mat imagea;Mat imageb;Mat imagec;
  int event_pre,event_prea,event_preb,event_prec;
  int event_id, event_ida,event_idb,event_idc;
  double time_on,time_ona,time_onb,time_onc; 	
  int first_time=1; int first_timea=1;int first_timeb=1;int first_timec=1;
  string name="cam1"; string namea="cam2";string nameb="cam3";string namec="cam4";
  string logfile="log1.log";string logfilea="loga.log";string logfileb="logb.log";string logfilec="logc.log";
  int print_file=0;
	high_resolution_clock::time_point time_start;
	   
	int thour,tmin;
	int thourE,tminE;
	time_t t = time(0);
        struct tm * now = localtime( & t );
        cout <<endl << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' <<
              now->tm_mday << "  "<< now->tm_hour <<":"<<now->tm_min << ":" <<now->tm_sec << endl;
	cout << "Enter time to start(hour min):";
	cin >> thour >> tmin;
      cout << "Enter time to stop(hour min):";
	cin >> thourE >> tminE;

 //const std::string videoStreamAddress = "http://root:pass@192.168.0.93/axis-cgi/mjpg/video.cgi?resolution=640x480&req_fps=30&.mjpg";

 const std::string videoStreamAddress = "http://root:root@192.168.0.98/axis-cgi/mjpg/video.cgi?resolution=640x480&req_fps=30&.mjpg";

    //open the video stream and make sure it's opened
    if(!vcap.open(videoStreamAddress)) {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }


   for(;;) {
	time_t tE = time(0);
        struct tm * nowE = localtime( & tE);
        if(!vcap.read(image)) {}
 
		
	if(count_start==0)
		TimerS(time_start,thour,tmin);
	else {
	  //cam 1
		object_recognition(image,name,event_id);
		    if(first_time==1){
		      event_pre=event_id;
		      first_time=0;}
		      if(event_id!=event_pre){
		      event_pre=event_id;
			TimerE(time_start,time_on);
			cout << endl<< setfill('0')<< setw(5) << event_pre  ;
			cout <<  setfill(' ') <<  setw(15) << fixed    <<time_on <<endl  ;
			timeStamp.push_back(time_on);
			id.push_back(event_pre);
			publishData(  "cam1",  event_pre,  time_on);
		      }

		     if( int(nowE->tm_hour)==thourE && int(nowE->tm_min) ==tminE && int(nowE->tm_sec) ==0){
			printlogfile( id ,timeStamp, logfile);
		       
			  publishData(  "EndOfEvent",  0,  0);
			  cout << "Printed";
			print_file=1;
		    } 
		   //   imshow("InputFrame",image);
		}
	if(cv::waitKey(1) >= 0) break;

   }
 return 0;
}
