// File: main.cc 
// ./test [.xml]  [reject_level] [level_weights] [min_neighbors] 
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
//#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
# include "opencv2/nonfree/features2d.hpp"
#include <string>
using namespace std;
using namespace cv;

struct rectangles_Info{
   
   Point centre;
   Point v1,v2,v3,v4;
   rectangles_Info(Point centre,Point v1,Point v2,Point v3,Point v4);
   
};

rectangles_Info::rectangles_Info(Point Centre, Point a, Point b, Point c, Point d)
{
  centre= Centre ;
  v1=a;
  v2=b;
  v3=c;
  v4=d;

}


vector <rectangles_Info*> rectangles;

int surfFunc(Mat img_1,Mat img_2){
 if( !img_1.data || !img_2.data )
  { printf(" --(!) Error reading images \n"); return -1; }

  //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 400;
  cvtColor(img_1,img_1,CV_RGB2GRAY);
 cvtColor(img_2,img_2,CV_RGB2GRAY);
 Size size(640,480);
 resize(img_1,img_1,size);
  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_1, keypoints_2;

  detector.detect( img_1, keypoints_1 );
  detector.detect( img_2, keypoints_2 );

  //-- Step 2: Calculate descriptors (feature vectors)
  SurfDescriptorExtractor extractor;
  Ptr<DescriptorExtractor> descriptorExtractor = DescriptorExtractor::create( "SURF");
  Mat descriptors_1, descriptors_2;
 

 
  if(descriptors_1.type()!=CV_32F) {
    descriptors_1.convertTo(descriptors_1, CV_32F);
  }
   if(descriptors_2.type()!=CV_32F) {
    descriptors_2.convertTo(descriptors_2, CV_32F);
  }
  if( descriptors_1.empty()|| descriptors_2.empty()){
   cvError(0,"MatchFinder","2nd descriptor empty",__FILE__,__LINE__);
  
    return 0;
  }
 cout << "yesSurf";
 descriptorExtractor->compute( img_1, keypoints_1, descriptors_1 );
  descriptorExtractor->compute( img_2, keypoints_2, descriptors_2 );


  //-- Step 3: Matching descriptor vectors using FLANN matcher
 // FlannBasedMatcher matcher;
  FlannBasedMatcher matcher(new cv::flann::LshIndexParams(20, 10, 2));
  std::vector< DMatch > matches;
  matcher.match( descriptors_1, descriptors_2, matches );

  double max_dist = 0; double min_dist = 100;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_1.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }

  printf("-- Max dist : %f \n", max_dist );
  printf("-- Min dist : %f \n", min_dist );

  //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
  //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
  //-- small)
  //-- PS.- radiusMatch can also be used here.
  std::vector< DMatch > good_matches;

  for( int i = 0; i < descriptors_1.rows; i++ )
  { if( matches[i].distance <= max(2*min_dist, 0.02) )
    { good_matches.push_back( matches[i]); }
  }

  //-- Draw only "good" matches
  Mat img_matches;
  drawMatches( img_1, keypoints_1, img_2, keypoints_2,
               good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

  //-- Show detected matches
  imshow( "Good Matches", img_matches );

  for( int i = 0; i < (int)good_matches.size(); i++ )
  { printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx ); }

  
  
}
int main(int argc, char **argv) {
   

    CascadeClassifier cascade;
    const float scale_factor(1.2f);
    const int min_neighbors(24);
	cascade.load(argv[1]); 
	VideoCapture cap(-1);
	if(!cap.isOpened()){
	cout << "Cam cannot be opened!" <<endl;
	return -1;
	}
	Mat frameTemp;
	Mat input_image =  imread("SensorB.jpg", CV_LOAD_IMAGE_COLOR);
	Mat frame; Mat img_gray ;
      for(;;){
 
	    cap >> frame;
	    frameTemp=frame.clone();
	    cvtColor(frame,img_gray,CV_RGB2GRAY);
	    equalizeHist(img_gray, img_gray);
            vector<Rect> objs;
            Rect RectTemp;Point aTemp,bTemp,cTemp,dTemp,eTemp;
            vector<int> reject_levels;
            vector<double> level_weights;
            cascade.detectMultiScale(img_gray, objs, reject_levels, level_weights, scale_factor, min_neighbors, 0, Size(), Size(), true);
 
           
            for (int n = 0; n < objs.size(); n++) {
		if(reject_levels[n]>stoi(argv[2])  && level_weights[n] >stoi(argv[3])){
                	rectangle(frame, objs[n], Scalar(255,0,0), 8);
			RectTemp.x=objs[n].x;
			RectTemp.y=objs[n].y;
			RectTemp.width=objs[n].width;
			RectTemp.height=objs[n].height;
			aTemp.x=RectTemp.x;                             aTemp.y=RectTemp.y;
			bTemp.x = RectTemp.x + RectTemp.width;          bTemp.y=RectTemp.y;
			cTemp.x = RectTemp.x;                           cTemp.y = RectTemp.y + RectTemp.height;
			dTemp.x = RectTemp.x + RectTemp.width ;	        dTemp.y = RectTemp.y + RectTemp.height;
			eTemp.x = (dTemp.x-aTemp.x)/2;                   eTemp.y = (dTemp.y-aTemp.y)/2;
			rectangles.push_back(new rectangles_Info(Point (eTemp),Point (aTemp),Point (bTemp),Point (cTemp),Point (dTemp)));
			Mat frameROI = frameTemp (Rect(RectTemp.x,RectTemp.y,RectTemp.width,RectTemp.height));
			//if((RectTemp.width*RectTemp.height)>1000)
			
			//  surfFunc( input_image,frameROI);
            		putText(frame, std::to_string(reject_levels[n]),Point(objs[n].x, objs[n].y), 1, 1, Scalar(0,0,255));
		       // putText(frame, std::to_string(level_weights[n]),Point(objs[n].x+100, objs[n].y), 1, 1, Scalar(0,0,255));
			cout << level_weights[n] << "\n\n" <<reject_levels[n]<<endl;
            }
}
            imshow("VJ Face Detector", frame);
	    char key = (char)waitKey(200);
		if(key==27)break;          
        }
 
 
    return 0;
}
