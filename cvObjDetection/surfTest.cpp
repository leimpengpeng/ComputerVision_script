#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
# include "opencv2/nonfree/features2d.hpp"
using namespace cv;
using namespace std;

char key = 'a';
int framecount = 0;

SurfFeatureDetector detector( 400 );
SurfDescriptorExtractor extractor;
FlannBasedMatcher matcher;

Mat frame, des_object, image;
Mat des_image, img_matches, H;

std::vector<KeyPoint> kp_object;
std::vector<Point2f> obj_corners(4);
std::vector<KeyPoint> kp_image;
std::vector<vector<DMatch > > matches;
std::vector<DMatch > good_matches;
std::vector<Point2f> obj;
std::vector<Point2f> scene;
std::vector<Point2f> scene_corners(4);

int main(int argc, char **argv)
{             
                //reference image
    Mat object = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
    Size size(640,480);
    resize(object,object,size);	
//	Canny(object,object,10,350);

    if( !object.data )
    {
        std::cout<< "Error reading object " << std::endl;
        return -1;
    }

                //compute detectors and descriptors of reference image
    detector.detect( object, kp_object );
    extractor.compute( object, kp_object, des_object );     

                //create video capture object
    VideoCapture cap(0);

    //Get the corners from the object
    obj_corners[0] = cvPoint(0,0);
    obj_corners[1] = cvPoint( object.cols, 0 );
    obj_corners[2] = cvPoint( object.cols, object.rows );
    obj_corners[3] = cvPoint( 0, object.rows );

                //wile loop for real time detection
  for(;;){
                                //capture one frame from video and store it into image object name 'frame'
        cap >> frame;

        if (framecount < 5)
        {
            framecount++;
            continue;
        }      

                                //converting captured frame into gray scale
        cvtColor(frame, image, CV_RGB2GRAY);

          detector.detect( image, kp_image );
       extractor.compute( image, kp_image, des_image );

                                //find matching descriptors of reference and captured image
                                matcher.knnMatch(des_object, des_image, matches, 2);

                                //finding matching keypoints with Euclidean distance 0.6 times the distance of next keypoint
                                //used to find right matches
                                for(int i = 0; i < min(des_image.rows-1,(int) matches.size()); i++)
                                {
                                                if((matches[i][0].distance < 0.6*(matches[i][1].distance)) && ((int) matches[i].size()<=2 && (int) matches[i].size()>0))
                                                {
                                                                good_matches.push_back(matches[i][0]);
                                                }
                                }                             

                                //Draw only "good" matches
                                drawMatches( object, kp_object, frame, kp_image, good_matches, img_matches,
                                                Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

                                //3 good matches are enough to describe an object as a right match.
                      if (good_matches.size() >= 3)
        {                                     
	
            for( int i = 0; i < good_matches.size(); i++ )
            {
                //Get the keypoints from the good matches
                obj.push_back( kp_object[ good_matches[i].queryIdx ].pt );
                scene.push_back( kp_image[ good_matches[i].trainIdx ].pt );
            }
            try{
               H = findHomography( obj, scene, CV_RANSAC );
                 }
              catch(Exception e){}

            perspectiveTransform( obj_corners, scene_corners, H);

            //Draw lines between the corners (the mapped object in the scene image )
            line( img_matches, scene_corners[0] + Point2f( object.cols, 0), scene_corners[1] + Point2f( object.cols, 0), Scalar(0, 255, 0), 4 );
            line( img_matches, scene_corners[1] + Point2f( object.cols, 0), scene_corners[2] + Point2f( object.cols, 0), Scalar( 0, 255, 0), 4 );
            line( img_matches, scene_corners[2] + Point2f( object.cols, 0), scene_corners[3] + Point2f( object.cols, 0), Scalar( 0, 255, 0), 4 );
            line( img_matches, scene_corners[3] + Point2f( object.cols, 0), scene_corners[0] + Point2f( object.cols, 0), Scalar( 0, 255, 0), 4 );
        }

        //Show detected matches
        imshow( "Good Matches", img_matches );

                                //clear array
                                good_matches.clear();

        key = waitKey(1);
    }
    return 0;
}
