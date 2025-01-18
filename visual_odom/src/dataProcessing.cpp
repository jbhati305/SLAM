#include <eigen3/Eigen/Dense>

#include <iostream>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/core/eigen.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/calib3d.hpp>

using namespace std;
using namespace Eigen;

cv::Mat compute_disparity(const cv::Mat& left, const cv::Mat& right, const string& matcher)
{
  int sad_window = 36;
  int num_disparities = 6 * 16;
  int block_size = 11;

  // Parameters for SGBM
  int p1 = 8 * 3 * sad_window;
  int p2 = 32 * 3 * sad_window;

  // Declare pointer here
  cv::Ptr<cv::StereoMatcher> stereo;

  if (matcher == "bm")
  {
    stereo = cv::StereoBM::create(num_disparities, block_size);
  }
  else if (matcher == "sgbm")
  {
    stereo = cv::StereoSGBM::create(0,                       // minDisparity
                                    num_disparities,         // numDisparities
                                    block_size,              // blockSize
                                    p1,                      // P1
                                    p2,                      // P2
                                    0,                       // disp12MaxDiff
                                    0,                       // preFilterCap
                                    0,                       // uniquenessRatio
                                    0,                       // speckleWindowSize
                                    0,                       // speckleRange
                                    cv::StereoSGBM::MODE_HH  // mode
    );
  }

  cv::Mat disparity;
  stereo->compute(left, right, disparity);

  cv::Mat disparity_normalized;
  cv::normalize(disparity, disparity_normalized, 0, 255, cv::NORM_MINMAX, CV_8U);

  return disparity_normalized;
}