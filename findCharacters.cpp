#include "opencv2/highgui/highgui_c.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

list<IplImage*> findCharacters(IplImage* img){
   vector<int> x(20,0);
   vector<int> y(20,0);
   vector<int> width(20,0);
   vector<int> height(20,0);

   list<IplImage*> image_list;
   list<CvRect> rects;

   //Get the size of the image
   CvSize size = cvGetSize(img);
  
   //Initialize the image variable
   IplImage* gray = cvCreateImage( size, IPL_DEPTH_8U,1 );  
   IplImage* canny = cvCreateImage( size, IPL_DEPTH_8U,1 );  
   
   //Convert to gray scale
   cvCvtColor(img, gray, CV_RGB2GRAY); 
   //Edge detection
   cvCanny(gray, canny, 70,70,3);
   
   IplConvKernel* kernal = cvCreateStructuringElementEx(2,2, 1, 0, CV_SHAPE_RECT);
   IplImage* erode_dilate = cvCreateImage(size, IPL_DEPTH_8U,1);
   cvDilate(canny, erode_dilate, kernal, 1);
   cvErode(erode_dilate,erode_dilate, kernal, 2);
   //cvDilate(erode_dilate,erode_dilate,kernal,2);
   //Find appropriate contour
   CvMemStorage* storage = cvCreateMemStorage();
   CvSeq* contours;
   IplImage* copy = cvCloneImage(canny);
   cvFindContours(canny, storage, &contours);
    
   while(contours!=nullptr)
   {
       rects.push_back(cvBoundingRect(contours));
       contours = contours->h_next;
   }     
       
   int i = 0;
   for(list<CvRect>::iterator it = rects.begin(); it != rects.end(); ++it){
   	  //cout << "x: " << (*it).x << " " << "y: " << (*it).y << " " << "width: " << (*it).width << " " <<  "height: " << (*it).height << endl;
    	if((*it).width > 0.01 * size.width && (*it).width < 0.05 * size.width && (*it).height > 0.7 * size.height && (*it).height < size.height){
    	x[i] = (*it).x;
    	y[i] = (*it).y;
        width[i] = (*it).width;
    	height[i] = (*it).height;
    	i ++;
   	    cout << "x: " << (*it).x << " " << "y: " << (*it).y << " " << "width: " << (*it).width << " " <<  "height: " << (*it).height << endl;
        }
    }
    
    for(int g = 0; g < i; g ++){
    	IplImage* copy2 = cvCloneImage(copy);
    	cvSetImageROI(copy2,cvRect(x[g],y[g],width[g],height[g]));
        image_list.push_back(copy2);
    }
    
    int size1 = image_list.size();
    char windowname[20];
    list<IplImage*>::iterator it = image_list.begin();

    for(int i = 0; i < size1; i ++){
      sprintf(windowname, "Window%d", i);
      cvNamedWindow(windowname, CV_WINDOW_AUTOSIZE);
      cvShowImage(windowname, *it);
      cvWaitKey(60);
      it ++;
   }

   cvNamedWindow("window1", CV_WINDOW_AUTOSIZE);
   cvShowImage("window1", copy);
   //cvNamedWindow("window2", CV_WINDOW_AUTOSIZE);
   //cvShowImage("window2", gray);
   //cvWaitKey(0);
   //cvDestroyWindow( "window1" );

   return image_list;
}