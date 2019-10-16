#include <iostream>
#include <cstdlib>
#include <math.h>
#include <stdio.h>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void
process(const char* imsname, int i0, int j0, int w, int h)
{
}

void
usage (const char *s)
{
  std::cerr<<"Usage: "<<s<<" imsname i0 j0 w h\n"<<std::endl;
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
