#include <iostream>
#include <cstdlib>
#include <fstream>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double mean(Mat block){
  Size s = block.size();
  int h = s.height;
  cout<<h<<endl;
  int w = s.width;
  cout<<w<<endl;
  int sum = 0;
  for (int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      sum += block.at<uchar>(i,j);
    }
  }
  cout<<sum / (h*w)<<endl;
  return sum / (h*w);
}

void
process(const char* ims, int radius, int cst)
{
  Mat image = imread(ims,0);
  Size s = image.size();
  int h = s.height;
  int w = s.width;
  Mat imageMoy(h,w,CV_8UC1);
  for (int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      //cout<<"i= "<<i-radius<<endl;
      //cout<<"j= "<<j-radius<<endl;
      Mat block;
      if(j+radius < w && i+radius < h && j-radius >=0 && i-radius >= 0){
        //cout<<"i-rad= "<<i-radius<<endl;
        //cout<<"j-rad "<<j-radius<<endl;
        block = image(Rect(j-radius,i-radius,2*radius+1,2*radius+1));
      }
      else{
        int deltaJ = 0;
        int deltaI = 0;
        while(j - radius + deltaJ < 0){
        //cout<<"j - radius + deltaJ "<<j - radius + deltaJ <<endl;
          deltaJ++;
        }
        while(i - radius + deltaI < 0){
          deltaI++;
          //cout<<"i - radius + deltaI "<<i - radius + deltaI <<endl;
        }
        while(j + radius + deltaJ > w-1){
          deltaJ--;
          //cout<<"j + radius + deltaJ "<<j - radius + deltaJ <<endl;
        }
        while(i + radius + deltaI > h-1){
          deltaI--;
          //cout<<"i + radius + deltaI "<<i - radius + deltaI <<endl;
        }
        block = image(Rect(j-radius+deltaJ,i-radius+deltaI,2*radius+1-abs(deltaJ),2*radius+1-abs(deltaI)));
      }
      int val = mean(block) - cst;
      if(imageMoy.at<uchar>(i,j) <= val)
        imageMoy.at<uchar>(i,j) = 0;
      else
        imageMoy.at<uchar>(i,j) = 255;
      //cout<<mean(block)<<endl;
    }
  }
  imshow("img moy",imageMoy);
  imwrite("th.png",imageMoy);
  cout<<"h ="<<h<<endl;
  cout<<"w ="<<w<<endl;
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
