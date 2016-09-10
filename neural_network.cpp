#include "opencv2/highgui/highgui_c.h"
#include "opencv2/core.hpp"
#include "opencv2/ml/ml.hpp"
#include <iostream>
using namespace cv;
using namespace std;

cv::Ptr<cv::ml::ANN_MLP> mlp = cv::ml::ANN_MLP::create(); 

void train(list<IplImage*> image_list, vector<char> v){
   
   int size = image_list.size();
   Mat layers = cv::Mat(5, 1, CV_32SC1);
  
   layers.row(0) = cv::Scalar(375);
   layers.row(1) = cv::Scalar(10);
   layers.row(2) = cv::Scalar(15);
   layers.row(3) = cv::Scalar(20);
   layers.row(4) = cv::Scalar(36);
    
   int data[36] = {0};
   int value;
   list<IplImage*>::iterator it = image_list.begin();
   for(int i = 0; i < size; i++){
   	 value = v[i];
   	 if(value >= 65 && value <= 90){
   	 	value = value - 55;
   	 }
   	 else if(value >= 30 && value <= 39){
   	 	value = value - 30;
   	 }
   	 else{
   	 	cerr << "Ouput dataset contains illegal data"<< endl;
   	 	return;
   	 }
     data[value] = 1;
     Mat outputlayers(1,1,CV_8UC3,&data);
     mlp->setLayerSizes(layers);
     Mat mat_image = cvarrToMat(*it);
     it ++;
     mlp->train(mat_image, ml::ROW_SAMPLE, outputlayers );
   }
}

vector<char> predict(list<IplImage*> image_list){
	int size = image_list.size();
    list<IplImage*>::iterator it = image_list.begin();
    Mat predict;
    vector<int> v;
    vector<char> result;
    char letter;
    for(int i = 0; i < size; i++){
        Mat mat_image = cvarrToMat(*it);
        it++;
        mlp->predict(mat_image, predict ,ml::ANN_MLP::RAW_OUTPUT);
        v.assign(predict.datastart, predict.dataend);
        int k = 0;
        int o = 0;
        for(int j = 0; j < 36; j++){
           if(v[j] > k){
           	 k = v[j];
           	 o = i;
           }
        }
        if(o <=10){
   	 	   letter = o + 30;
   	    }
   	    else if(o <=36){
   	 	   letter = o + 55;
   	    }  
        result[i] = letter;
    }
    return result;
}