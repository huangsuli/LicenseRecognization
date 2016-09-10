#include "opencv2/highgui/highgui_c.h"
#include "opencv2/core.hpp"
#include <iostream>
#include <list>
using namespace cv;
using namespace std;

IplImage* findLicense(IplImage* img){
   int x, y;
   int width, height;
   list<CvRect> rects;
   CvSize size = cvGetSize(img);
   IplImage* copy = cvCloneImage(img);

   //Initialize the image variable
   IplImage* gray = cvCreateImage( size, IPL_DEPTH_8U,1 );  
   IplImage* canny = cvCreateImage( size, IPL_DEPTH_8U,1 );  
   
   //Convert to gray scale
   cvCvtColor(img, gray, CV_RGB2GRAY); 

   //Edge detection
   cvCanny(gray, canny, 255,255,3);
   
   //Binarize
   //IplImage* sobel = cvCreateImage(cvGetSize(gray), IPL_DEPTH_16S,1);
   //cvSobel(gray,sobel,0,5,7);
   //IplImage* temp = cvCreateImage(cvGetSize(sobel), IPL_DEPTH_8U,1);
   //cvConvertScale(sobel, temp, 0.00390625, 0.00390625);
   //IplImage* threshold = cvCreateImage(cvGetSize(temp), IPL_DEPTH_8U,1);
   //cvThreshold(temp, threshold, 0, 255, CV_THRESH_BINARY| CV_THRESH_OTSU);
   
   //Dilation and erosion
   IplConvKernel* kernal = cvCreateStructuringElementEx(2,2, 1, 0, CV_SHAPE_RECT);
   IplImage* erode_dilate = cvCreateImage(size, IPL_DEPTH_8U,1);
   cvDilate(canny, erode_dilate, kernal, 4);
   cvErode(erode_dilate, erode_dilate, kernal, 4);
   
   //kernal = cvCreateStructuringElementEx(3, 5, 0, 1, CV_SHAPE_RECT);
   //cvErode(erode_dilate, erode_dilate, kernal, 1);

    
    //Find appropriate contour
    CvMemStorage* storage = cvCreateMemStorage();
    CvSeq* contours;
    //IplImage* copy = cvCloneImage(canny);
    cvFindContours(canny, storage, &contours);
    
    while(contours!=nullptr)
    {
        rects.push_back(cvBoundingRect(contours));
        contours = contours->h_next;
    }     
    
    
    for(list<CvRect>::iterator it = rects.begin(); it != rects.end(); ++it){
    	//cout << "x: " << (*it).x << " " << "y: " << (*it).y << " " << "width: " << (*it).width << " " <<  "height: " << (*it).height << endl;
    	if( size.width * 2/3 > (*it).width && (*it).width > size.width/2  && size.height * 1/20 < (*it).height && (*it).height < size.height * 1/10 ){
    		x = (*it).x;
    		y = (*it).y;
    		width = (*it).width;
    		height = (*it).height;
    	}
    }
    cvSetImageROI(img,cvRect(x,y,width,height));
    
    cvNamedWindow("window0", CV_WINDOW_AUTOSIZE);
    cvShowImage("window0", img);

    cvNamedWindow("window111", CV_WINDOW_AUTOSIZE);
    cvShowImage("window111", copy);
    return erode_dilate;
       
}