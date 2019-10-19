#include <iostream>
#include <cstdlib>
#include <fstream>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

vector<int> calculateHistogram(Mat image, int h, int w){
  vector<int> histogram = vector<int>(256);
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      int k = image.at<uchar>(i,j);
      histogram.at(k) += 1;
    }
  }
  return histogram;
}

void
process(const char* ims)
{
  Mat image = imread(ims,0);
  Size s = image.size();
  int h = s.height;
  int w = s.width;
  Mat imageThreshold(h,w,CV_8UC1);

  int threshold, varMax, sum, sumB, q1, q2, mu1, mu2 = 0;
  vector<int> histogram = calculateHistogram(image,h,w);
  int Imax = 255;
  for(int i=0; i<=Imax; i++){
    sum += i*histogram.at(i);
  }
  for(int t=0; t<=Imax; t++){
    q1 += histogram.at(t);
    if(q1 == 0)
      continue;
    q2 = h*w - q1;

    sumB += t*histogram.at(t);
    mu1 = sumB / q1;
    mu2 = (sum - sumB) / q2;

    int vb = q1*q2*(mu1 - mu2)^2;

    if(vb > varMax){
      threshold = t;
      varMax = vb;
    }
  }
  //Binarizing the image
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
    if(image.at<uchar>(i,j) > threshold){
        imageThreshold.at<uchar>(i,j) = 255;
      }
      else{
        imageThreshold.at<uchar>(i,j) = 0;
      }
    }
  }   
}

void
usage (const char *s)
{
  std::cerr<<"Usage: "<<s<<" ims\n"<<std::endl;
  exit(EXIT_FAILURE);
}

#define param 2
int
main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1]);
  return EXIT_SUCCESS;
}
