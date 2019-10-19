#include <iostream>
#include <cstdlib>
#include <fstream>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

vector<int> calculateHistogram(Mat image, int h, int w){
  vector<int> histogram = vector<int>(256,0);
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      int k = image.at<uchar>(i,j);
      histogram.at(k) += 1;
    }
  }
  return histogram;
}

void manuelOtsuThreshold(Mat image, Mat imageOtsu, int h, int w){
  int threshold = 0;
  int varMax = 0;
  int sum = 0;
  int sumB = 0;
  int q1 =0;
  int q2 =0;
  int mu1 = 0;
  int mu2 = 0;
  vector<int> histogram = calculateHistogram(image,h,w);
  int Imax = 255;
  for(int i=0; i<=Imax; i++){
    sum += i*histogram.at(i);
  }
  for(int t=0; t<=Imax; t++){
    cout<<"t= "<<t<<endl;
    q1 += histogram.at(t);
    cout<<"q1 ="<<q1<<endl;
    if(q1 == 0)
      continue; //goes to the next iteration
    q2 = h*w - q1;
    if(q2 == 0)
      continue;

    sumB += t*histogram.at(t);
    mu1 = sumB / q1;
    mu2 = (sum - sumB) / q2;

    int vb = q1*q2*(mu1 - mu2)^2;

    if(vb > varMax){
      cout<<t<<endl;
      threshold = t;
      varMax = vb;
    }
  }
  //Binarizing the image
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      if(image.at<uchar>(i,j) > threshold){
        imageOtsu.at<uchar>(i,j) = 255;
      }
      else{
        imageOtsu.at<uchar>(i,j) = 0;
      }
    }
  }
  cout<<"manually-obtained threshold = "<<threshold<<endl;   
}

void
process(const char* ims)
{
  Mat image = imread(ims,0);
  Size s = image.size();
  int h = s.height;
  int w = s.width;
  Mat imageOtsu(h,w,CV_8UC1);

  //Manual threshold
  manuelOtsuThreshold(image,imageOtsu,h,w);
  imshow("otsu-th.png",imageOtsu);
  imwrite("otsu-th.png",imageOtsu);
  
  //OpenCV threshold with otsu method
  Mat imageOtsuOcv(h,w,CV_8UC1);
  double t = threshold(image, imageOtsuOcv,0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
  cout<<"openCV-obtained threshold = "<<t<<endl;
  imwrite("otsu-th-ocv.png",imageOtsuOcv);
  Mat diff = imageOtsu - imageOtsuOcv;
  imwrite("diff3.png",diff);
}

void
usage (const char *s)
{
  std::cerr<<"Usage: "<<s<<" ims\n"<<std::endl;
  exit(EXIT_FAILURE);
}

#define param 1
int
main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1]);
  return EXIT_SUCCESS;
}
