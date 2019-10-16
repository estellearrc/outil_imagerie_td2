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
process(const char* imsname, const char* imdname)
{
  std::cout<<"\n\n\n ===================================================\n EXERCICE 1 : READWRITESHOW \n ===================================================" <<std::endl;

  bool exist = exists_file(imsname);
  if(!exist){
    std::cerr<<"The file doesn't exist, check its location.\n"<<std::endl;
    exit(EXIT_FAILURE);
  }
  //Read the image "imsname"
  Mat image = imread(imsname, CV_LOAD_IMAGE_COLOR);
  int rows = image.rows;
  int cols = image.cols;
  std::cout<<"Number rows = " << rows << " number columns = " << cols << "\n" <<std::endl;

  //Create and show a magenta picture
  Mat magenta(100, 200, CV_8UC3, Scalar(255,0,255));
  imshow("magenta", magenta);
  imshow(imsname, image);
  waitKey(0);
  imwrite(imdname, magenta);
}

void
usage (const char *s)
{
  std::cerr<<"Usage: "<<s<<" imsname imdname\n"<<std::endl;
  exit(EXIT_FAILURE);
}

#define param 2
int
main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1], argv[2]);
  return EXIT_SUCCESS;
}
