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
process(int h, int s, int v, const char* ims, const char* imd)
{
  bool exist = exists_file(ims);
  if(!exist){
    std::cerr<<"The file doesn't exist, check its location.\n"<<std::endl;
    exit(EXIT_FAILURE);
  }
  //Read the image "imsname"
  Mat image = imread(ims, CV_LOAD_IMAGE_COLOR);
  Mat imageHSV;
  cvtColor(image, imageHSV, CV_BGR2HSV);
  //imshow("HSV",imageHSV);

  Mat hsvchannel[3];
  // The actual splitting.
  split(imageHSV, hsvchannel);
  
  Mat hChannel = hsvchannel[0];
  namedWindow("h",CV_WINDOW_AUTOSIZE);
  imshow("h", hChannel);

  Mat sChannel = hsvchannel[1];
  namedWindow("s",CV_WINDOW_AUTOSIZE);
  imshow("s", sChannel);

  Mat vChannel = hsvchannel[2];
  namedWindow("v",CV_WINDOW_AUTOSIZE);
  imshow("v", vChannel);

  hChannel += h;
  sChannel += s;
  vChannel += v;

  vector<Mat> channels;

  channels.push_back(hChannel);
  channels.push_back(sChannel);
  channels.push_back(vChannel);

  merge(channels, imageHSV);
  
  Mat hsvModified;
  cvtColor(imageHSV, hsvModified, CV_HSV2BGR);
  imwrite(imd, hsvModified);
  waitKey(0);
}

void
usage (const char *s)
{
  std::cerr<<"Usage: "<<s<<" h s v ims imd\n"<<std::endl;
  exit(EXIT_FAILURE);
}

#define param 5
int
main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), argv[4], argv[5]);
  return EXIT_SUCCESS;
}
