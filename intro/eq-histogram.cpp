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

void
process(const char* ims)
{
  bool exist = exists_file(ims);
  if(!exist){
    std::cerr<<"The file doesn't exist, check its location.\n"<<std::endl;
    exit(EXIT_FAILURE);
  }
  //Mat image = imread(ims,1);
  Mat image = imread(ims,0);
  imshow(ims,image);
  Size s = image.size();
  int h = s.height;
  int w = s.width;
  vector<int> histogram = vector<int>(256,0);
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      int k = image.at<uchar>(i,j);
      histogram.at(k) += 1;
    }
  }
  vector<int> histogramCumule = vector<int>(256,0);
  for(int l = 0; l < (int)histogramCumule.size(); l++){
    for(int p= 0; p <= l; p++){
      histogramCumule.at(l) += histogram.at(p);
    }
  }
  int Imax = 255;
  Mat imageRehaussee(h,w,CV_8UC1);
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      int k = image.at<uchar>(i,j);
      imageRehaussee.at<uchar>(i,j) = Imax * histogramCumule.at(k) / (h*w);
    }
  }
  
  imwrite("eq.png",imageRehaussee);
  Mat imageRehausseeOcv;
  equalizeHist(image, imageRehausseeOcv);
  imwrite("eq-ocv.png",imageRehausseeOcv);

  Mat diff = imageRehausseeOcv - imageRehaussee;
  imwrite("diff.png",diff);
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
