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
  int sum = 0;
  for (int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      sum += block.at<uchar>(i,j);
    }
  }
  return sum / (h*w);
}

Mat manuelAdaptativeThreshold(Mat image, Mat imageMoy,int radius, int cst){
  Size s = image.size();
  int h = s.height;
  int w = s.width;
  for (int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      //Extracting the submatrix to average
      Mat block;
      if(j+radius < w && i+radius < h && j-radius >=0 && i-radius >= 0){
        block = image(Rect(j-radius,i-radius,2*radius+1,2*radius+1));
      }
      else{
        int deltaJ = 0;
        int deltaI = 0;
        while(j - radius + deltaJ < 0){
          deltaJ++;
        }
        while(i - radius + deltaI < 0){
          deltaI++;
        }
        while(j + radius + deltaJ > w-1){
          deltaJ--;
        }
        while(i + radius + deltaI > h-1){
          deltaI--;
        }
        block = image(Rect(j-radius+deltaJ,i-radius+deltaI,2*radius+1-abs(deltaJ),2*radius+1-abs(deltaI)));
      }
      //Calculating the average of the submatrix (minus the constant)
      int val = mean(block) - cst;
      //Binarizing the averaged image
      if(image.at<uchar>(i,j) <= val){
        imageMoy.at<uchar>(i,j) = 0;
      }
      else{
        imageMoy.at<uchar>(i,j) = 255;
      }
    }
  }
  return imageMoy;
}

void
process(const char* ims, int radius, int cst)
{
  Mat image = imread(ims,0);
  Size s = image.size();
  int h = s.height;
  int w = s.width;
  Mat imageMoy(h,w,CV_8UC1);

  //Manual threshold
  imageMoy = manuelAdaptativeThreshold(image,imageMoy,radius,cst);
  imwrite("th.png",imageMoy);

  //OpenCV threshold with mean method
  Mat imageMoyOcv(h,w,CV_8UC1);
  adaptiveThreshold(image, imageMoyOcv, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 2*radius+1, cst);
  imwrite("th-ocv-mean.png",imageMoyOcv);
  Mat diff = imageMoy - imageMoyOcv;
  imwrite("diff2.png",diff);

  //OpenCV threshold with gaussian method
  Mat imageGaussOcv(h,w,CV_8UC1);
  adaptiveThreshold(image, imageGaussOcv, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 2*radius+1, cst);
  imwrite("th-ocv-gauss.png",imageGaussOcv);
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
