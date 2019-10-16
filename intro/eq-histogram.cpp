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
  Mat image = imread(ims,0);
  imshow(ims,image);
  Size s = image.size();
  int h = s.height;
  int w = s.width;
  std::cout<<"h = "<<h<<" w = "<<w<<std::endl;
  vector<int> histogram = vector<int>(256);
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      int k = image.at<Vec3b>(i,j)[0];
      histogram.at(k) += 1;
    }
  }
  vector<int> histogramCumule = vector<int>(256);
  for(int l = 0; l < (int)histogramCumule.size(); l++){
    for(int p= 0; p < l; p++){
      histogramCumule.at(l) += histogram.at(p);
    }
    std::cout<<"hC = "<<histogramCumule.at(l)<<std::endl;
  }
  int Imax = 200;
  Mat imageRehaussee(image.size(), CV_8UC1);
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      int k = image.at<Vec3b>(i,j)[0];
      imageRehaussee.at<Vec3b>(i,j)[0] = Imax * histogramCumule.at(k) / (h*w);
    }
  }
  imshow("image réhaussée",imageRehaussee);
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
