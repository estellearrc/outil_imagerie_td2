#include <iostream>
#include <cstdlib>
#include <math.h>
#include <stdio.h>
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
  Mat image = imread(ims, CV_LOAD_IMAGE_COLOR);   // Read the file
 
  namedWindow( "Original Image", CV_WINDOW_AUTOSIZE );
  imshow( "Original Image", image );
    
  Mat rgbchannel[3];
  // The actual splitting.
  split(image, rgbchannel);
    
  namedWindow("Blue",CV_WINDOW_AUTOSIZE);
  imshow("Blue", rgbchannel[0]);

  namedWindow("Green",CV_WINDOW_AUTOSIZE);
  imshow("Green", rgbchannel[1]);
        
  namedWindow("Red",CV_WINDOW_AUTOSIZE);
  imshow("Red", rgbchannel[2]);

  Mat imageGrey;
  cvtColor(image, imageGrey, CV_BGR2GRAY);
  imshow("BGR to gray scale", imageGrey);

  Mat imageBGR2YCrCb;
  cvtColor(image, imageBGR2YCrCb, CV_BGR2YCrCb);
  imshow("BGR to YCrCb", imageBGR2YCrCb);

  Mat imageYCrCb2BGR;
  cvtColor(imageBGR2YCrCb, imageYCrCb2BGR, CV_YCrCb2BGR);
  imshow("YCrCb to BGR", imageYCrCb2BGR);

  Mat diffBGRYCrCb = imageYCrCb2BGR - image;
  imshow("diffBGRYCrCb",diffBGRYCrCb);

  Mat yCrCbChannel[3];
  // The actual splitting.
  split(imageBGR2YCrCb, yCrCbChannel);
    
  namedWindow("Y",CV_WINDOW_AUTOSIZE);
  imshow("Y", yCrCbChannel[0]);

  namedWindow("Cr",CV_WINDOW_AUTOSIZE);
  imshow("Cr", yCrCbChannel[1]);
        
  namedWindow("Cb",CV_WINDOW_AUTOSIZE);
  imshow("Cb", yCrCbChannel[2]);

  Mat diffYGray = yCrCbChannel[0] - imageGrey;
  imshow("diffYGray",diffYGray);

  waitKey(0);//Wait for a keystroke in the window
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
