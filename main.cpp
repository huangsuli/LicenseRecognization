#include "opencv2/highgui/highgui_c.h"
#include "opencv2/core.hpp"
#include "findLicense.cpp"
#include "findCharacters.cpp"
#include "neural_network.cpp"
#include <iostream>
#include <cstdio>

using namespace std;

int main( int argc, char** argv ) {
   IplImage* img = cvLoadImage( argv[1] );
   IplImage* copy = cvCloneImage(img);
   list<IplImage*> image_list;

   //cvNamedWindow( "Example1", CV_WINDOW_AUTOSIZE );
   //cvNamedWindow( "Example2", CV_WINDOW_AUTOSIZE );

   //Find the license location
   IplImage* a = findLicense(img);
   //cvSaveImage("license.png", img);
   //Find the specific characters
   image_list = findCharacters(img);
   
   //train(image_list, )
   //cvShowImage( "Example1", result);
   //cvShowImage( "Example2", img);

   cvWaitKey(0);
   
   cvReleaseImage( &img );
   //cvDestroyWindow( "Example1" );

}