#ifndef LIBREADDATA_H_
#define LIBREADDATA_H_

#include <eigen3/Eigen/Dense>
#include <string>
#include <vector>

using namespace std;
using namespace Eigen;

// Declaration of the function in the library
bool readCalibrationFile(const string &filename,
                         vector<MatrixXd> &projectionMatrices,
                         Matrix4d &transformation);

bool readPoseFile(const string &filename, vector<Matrix4d> &poses);

#endif // LIBHELLOSLAM_H_