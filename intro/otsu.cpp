#include <iostream>
#include <cstdlib>
#include <fstream>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

bool exists_file (const char *filename) {
  //check if the file "filename" exists in the current repository
    ifstream f(filename);
    return f.good();
}

vector<int> calculateHistogram(Mat image, int h, int w){
  vector<int> histogram = vector<int>(256,0);
  for(int i = 0; i < h; i++){
    uchar* pointer = image.ptr<uchar>(i);
    for(int j = 0; j < w; j++){
      int k = pointer[j];
      histogram.at(k) += 1;
    }
  }
  return histogram;
}

void manuelOtsuThreshold(Mat image, Mat imageOtsu, int h, int w){
  int threshold = 0;
  double varMax = 0;
  double sum = 0;
  double sumB = 0;
  double q1 = 0;
  double q2 = 0;
  double mu1 = 0;
  double mu2 = 0;
  double vb = 0;
  vector<int> histogram = calculateHistogram(image,h,w);
  int Imax = 255;
  for(int i=0; i<=Imax; i++){
    sum += i*histogram.at(i);
  }
  for(int t=0; t<=Imax; t++){
    q1 += histogram.at(t);
    if(q1 == 0)
      continue; //goes to the next iteration
    q2 = h*w - q1;

    sumB += t*histogram.at(t);
    mu1 = sumB / q1;
    mu2 = (sum - sumB) / q2;

    vb = q1*q2*(mu1 - mu2)*(mu1 - mu2);

    if(vb > varMax){
      threshold = t;
      varMax = vb;
    }
  }
  //Binarizing the image
  for(int i = 0; i < h; i++){
    uchar* pointer = image.ptr<uchar>(i);
    uchar* pointerOtsu = imageOtsu.ptr<uchar>(i);
    for(int j = 0; j < w; j++){
      if(pointer[j] > threshold){
        pointerOtsu[j] = 255;
      }
      else{
        pointerOtsu[j] = 0;
      }
    }
  }
  cout<<"manually-obtained threshold = "<<threshold<<endl;   
}

void
process(const char* ims)
{
  bool exist = exists_file(ims);
  if(!exist){
    std::cerr<<"The file doesn't exist, check its location.\n"<<std::endl;
    exit(EXIT_FAILURE);
  }
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
  waitKey(0);
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
