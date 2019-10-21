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

vector<int> calculateHistogramCumul(vector<int> histogram){
  vector<int> histogramCumule = vector<int>(256,0);
  for(int l = 0; l < (int)histogramCumule.size(); l++){
    for(int p= 0; p <= l; p++){
      histogramCumule.at(l) += histogram.at(p);
    }
  }
  return histogramCumule;
}

void enhance(Mat image, Mat imageRehaussee, int h, int w, int Imax, vector<int> histogramCumule){
  for(int i = 0; i < h; i++){
    uchar* pointer = image.ptr<uchar>(i);
    uchar* pointerRehaussee = imageRehaussee.ptr<uchar>(i);
    for(int j = 0; j < w; j++){
      int k = pointer[j];
      pointerRehaussee[j] = Imax * histogramCumule.at(k) / (h*w);
    }
  }
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
  vector<int> histogram = calculateHistogram(image,h,w);
  vector<int> histogramCumule = calculateHistogramCumul(histogram);
  int Imax = 255;
  Mat imageRehaussee(h,w,CV_8UC1);
  enhance(image, imageRehaussee, h, w, Imax, histogramCumule);
  
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
