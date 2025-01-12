#include "libReadData.h"
#include "libDraw.h"
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <iostream>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
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

int main()
{
  vector<MatrixXd> projectionMatrices;
  Matrix4d transformation;
  vector<Matrix4d> poses;
  cv::Mat image;
  vector<string> image_files =
      getImageFilesList(kDatasetPath + "/data_odometry_gray/dataset/sequences/" + SEQUENCE + "/image_0");

  // if (readCalibrationFile(calibFile, projectionMatrices, transformation))
  // {
  //   // Print results for verification
  //   for (size_t i = 0; i < projectionMatrices.size(); i++)
  //   {
  //     cout << "P" << i << ":\n" << projectionMatrices[i] << "\n\n";
  //   }
  //   cout << "Tr:\n" << transformation << endl;
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

  for (const auto& image_file : image_files)
  {
    cout << image_file << endl;
  }

  return 0;
}