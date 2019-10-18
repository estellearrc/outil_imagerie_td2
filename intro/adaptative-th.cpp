#include <iostream>
#include <cstdlib>
#include <fstream>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double mean(Mat block){
  Size s = block.size();
  int h = s.height;
  int w = s.width;
  int sum;
  for (int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      sum += block.at<uchar>(i,j);
    }
  }
  return sum / (h*w);
}

void
process(const char* ims, int radius, int cst)
{
  Mat image = imread(ims,0);
  Size s = image.size();
  int h = s.height;
  int w = s.width;
  cout<<"h ="<<h<<endl;
  cout<<"w ="<<w<<endl;
  (void)cst;
  Mat imageMoy(h,w,CV_8UC1);
  for (int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      Mat block;
      if(j+radius < w && i+radius < h && j-radius >=0 && i-radius >= 0){
        cout<<"i-rad="<<i-radius<<endl;
        cout<<"j-rad"<<j-radius<<endl;
        block = image(Rect(j-radius,i-radius,2*radius+1,2*radius+1));
      }
      else{
        cout<<"hello"<<endl;
        int deltaJ = 0;
        int deltaI = 0;
        while(j + deltaJ < 0){
          deltaJ++;
        }
        while(i + deltaI < 0){
          deltaI++;
        }
        while(j + deltaJ > w-1){
          deltaJ--;
        }
        while(i + deltaI > h-1){
          deltaI--;
        }
        block = image(Rect(j-radius+deltaJ,i-radius+deltaI,2*radius+1-deltaJ,2*radius+1-deltaI));
      }
      imageMoy.at<uchar>(i,j) = mean(block);
      //cout<<mean(block)<<endl;
    }
  }
}

void
usage (const char *s)
{
  std::cerr<<"Usage: "<<s<<" ims radius cst\n"<<std::endl;
  exit(EXIT_FAILURE);
}

#define param 3
int
main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1], atoi(argv[2]), atoi(argv[3]));
  return EXIT_SUCCESS;
}
