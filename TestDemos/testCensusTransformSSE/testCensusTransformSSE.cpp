#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Census.hpp"

using namespace correspondence;
using namespace cv;

int main(int argc, char* argv)
{
  Mat img = imread("../../../Resources/ImageData/TeddyLeft.png", CV_LOAD_IMAGE_GRAYSCALE);
  //Mat img = imread("../../../Resources/ImageData/CensusTestImage.png", CV_LOAD_IMAGE_GRAYSCALE);
  double t = (double)cv::getTickCount();
  Image image(img.rows, img.cols, 8, (byte*)img.data, img.cols * img.step[1] + img.rows * img.step[0]);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"memcpy align image "<<t/1.0<<std::endl;

  CensusCfg cfg;

  cfg.type = eSamplingWindow::SPARSE_16;
  prepOffsetsLUT(cfg.type, cfg.pattern, cfg.windowSize, image.stride);

  Image scalarResult(img.rows, img.cols, 2);//pxStep = 2 bytes because descriptor length is 16
 
  t = (double)cv::getTickCount();
  censusTransformScalar(image, cfg, scalarResult);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"Scalar Census Transform "<<t/1.0<<std::endl;

  namedWindow("Original Image", CV_WINDOW_KEEPRATIO);
  namedWindow("Census Image", CV_WINDOW_KEEPRATIO);
  Mat censusImg(Size(img.cols, img.rows), CV_16U, scalarResult.data, scalarResult.stride);

  imshow("Census Image", censusImg);
  imshow("Original Image", img);
  waitKey();

  //Image sseResult(img.rows, img.cols, 2);
  //censusTransformSSE(image, cfg, sseResult);

  return 0;
}