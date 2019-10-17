#include <iostream>
#include <cstdlib>
#include <fstream>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void
process(const char* ims, int radius, int cst)
{
  Mat image = imread(ims,0);
  (void)radius;
  (void)cst;
  //Extracting the cropped image from the image "imsname" with 'at' method
	/*Mat imgcrop(h, w, CV_8UC3);
	for (int i = 0; i < h; i++){
		for(int j = 0; j < w; j++){
			imgcrop.at<Vec3b>(i, j) = image.at<Vec3b>(i0 + i, j0 + j);
		}
	}*/
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
