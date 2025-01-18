#ifndef LIBREADDATA_H_
#define LIBREADDATA_H_

#include <eigen3/Eigen/Dense>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;
using namespace Eigen;

// Declaration of the function in the library
bool readCalibrationFile(const string& filename, vector<MatrixXd>& projectionMatrices, Matrix4d& transformation);
vector<string> getImageFilesList(const string& directory_path);
bool readPoseFile(const string& filename, vector<Matrix4d>& poses);
bool readImageFile(const string& filename, cv::Mat& images);
vector<double> readTimestamps(const string& filename);

#endif  // LIBHELLOSLAM_H_