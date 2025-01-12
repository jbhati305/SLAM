// #include "libReadData.h"
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <iostream>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace Eigen;
// using namespace cv;

MatrixXd readProjectionMatrix(const string& line)
{
  MatrixXd P(3, 4);
  stringstream ss(line.substr(line.find(":") + 1));
  for (int i = 0; i < 12; i++)
  {
    double val;
    ss >> val;
    P(i / 4, i % 4) = val;
  }
  return P;
}

bool readCalibrationFile(const string& filename, vector<MatrixXd>& projectionMatrices, Matrix4d& transformation)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    cerr << "Error opening file: " << filename << endl;
    return false;
  }

  string line;
  while (getline(file, line))
  {
    if (line.empty())
      continue;

    if (line.substr(0, 2) == "P0" || line.substr(0, 2) == "P1" || line.substr(0, 2) == "P2" ||
        line.substr(0, 2) == "P3")
    {
      projectionMatrices.push_back(readProjectionMatrix(line));
    }
    else if (line.substr(0, 2) == "Tr")
    {
      Matrix4d tr = Matrix4d::Identity();
      stringstream ss(line.substr(line.find(":") + 1));
      for (int i = 0; i < 12; i++)
      {
        double val;
        ss >> val;
        tr(i / 4, i % 4) = val;
      }
      transformation = tr;
    }
  }

  file.close();
  return true;
}

bool readPoseFile(const string& filename, vector<Matrix4d>& poses)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    cerr << "Error opening file: " << filename << endl;
    return false;
  }

  string line;
  while (getline(file, line))
  {
    if (line.empty())
      continue;

    Matrix4d pose = Matrix4d::Identity();
    stringstream ss(line);
    for (int i = 0; i < 12; i++)
    {
      double val;
      ss >> val;
      pose(i / 4, i % 4) = val;
    }
    poses.push_back(pose);
  }

  file.close();
  return true;
}

bool readImageFile(const string& filename, cv::Mat& image)
{
  image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
  if (image.data == nullptr)
  {
    cerr << "File " << filename << " does not exist." << endl;
    return false;
  }
  return true;
}
