#include "libReadData.h"
#include "libDraw.h"
#include "libdataProcessing.h"

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

using namespace std;
using namespace Eigen;

// Use CMake-defined path
const string kDatasetPath(DATASET_PATH);
const string SEQUENCE = "00";
string calibFile = kDatasetPath + "/data_odometry_calib/dataset/sequences/" + SEQUENCE + "/calib.txt";
string poseFile = kDatasetPath + "/data_odometry_poses/dataset/poses" + "/00.txt";
string imageFile = kDatasetPath + "/data_odometry_gray/dataset/sequences/" + SEQUENCE + "/image_0/000000.png";
string image_0 = kDatasetPath + "/data_odometry_gray/dataset/sequences/" + SEQUENCE + "/image_0";
string image_1 = kDatasetPath + "/data_odometry_gray/dataset/sequences/" + SEQUENCE + "/image_1";
string timestampFile = kDatasetPath + "/data_odometry_gray/dataset/sequences/" + SEQUENCE + "/times.txt";

int main()
{
  vector<MatrixXd> projectionMatrices;
  Matrix4d transformation;
  vector<Matrix4d> poses;
  cv::Mat left_image, right_image;
  vector<string> image_0_list = getImageFilesList(image_0);
  vector<string> image_1_list = getImageFilesList(image_1);
  vector<double> timestamps = readTimestamps(timestampFile);

  left_image = cv::imread(image_0_list[0], cv::IMREAD_GRAYSCALE);
  right_image = cv::imread(image_1_list[0], cv::IMREAD_GRAYSCALE);
  cv::Mat disparity = compute_disparity(left_image, right_image, "bm");
  cv::imshow("disparity", disparity);
  cv::waitKey(0);
  return 0;
}

// if (readCalibrationFile(calibFile, projectionMatrices, transformation))
// {
//   // Convert Eigen matrix to cv::Mat
//   cv::Mat projMat(3, 4, CV_64F);
//   cv::eigen2cv(projectionMatrices[0], projMat);
//   // Declare output matrices
//   cv::Mat k, R, t, rotMatrix, rotVec, eulerAngles;
//   // Decompose projection matrix
//   cv::decomposeProjectionMatrix(projMat, k, R, t, rotMatrix, rotVec, eulerAngles);
//   cout << "K: " << k << endl;
//   cout << "R: " << R << endl;
//   cout << "t: " << t << endl;
// }

// if (readPoseFile(poseFile, poses))
// {
//   DrawTrajectory(poses);
// }

// if (readImageFile(imageFile, image))
// {
//   cv::imshow("image", image);
//   cv::waitKey(0);
// }

// for (const auto& timestamp : timestamps)
// {
//   cout << timestamp << endl;
// }