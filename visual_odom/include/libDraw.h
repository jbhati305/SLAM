#ifndef LIBDRAW_H_
#define LIBDRAW_H_

#include <iostream>
#include <pangolin/pangolin.h>
#include <vector>
#include <eigen3/Eigen/Dense>

using namespace std;

void DrawTrajectory(std::vector<Eigen::Matrix4d>& poses);

#endif  // LIBDRAW_H_