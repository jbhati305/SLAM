#ifndef LIBDATAPROCESSING_H
#define LIBDATAPROCESSING_H

#include <eigen3/Eigen/Dense>
#include <fstream>
#include <iostream>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/core/eigen.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/calib3d.hpp>
#include <pangolin/pangolin.h>
#include <string>
#include <vector>

cv::Mat compute_disparity(const cv::Mat& left, const cv::Mat& right, const string& matcher);

#endif  // LIBDATAPROCESSING_H