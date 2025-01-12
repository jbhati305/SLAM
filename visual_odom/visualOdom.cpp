#include "libReadData.h"
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <iostream>
#include <pangolin/pangolin.h>
#include <string>
#include <vector>

using namespace std;
using namespace Eigen;

// Use CMake-defined path
const string kDatasetPath(DATASET_PATH);
const string SEQUENCE = "00";
string calibFile = kDatasetPath + "/data_odometry_calib/dataset/sequences/" +
                   SEQUENCE + "/calib.txt";

string poseFile =
    kDatasetPath + "/data_odometry_poses/dataset/poses" + "/00.txt";

void DrawTrajectory(vector<Matrix4d> &poses) {
  pangolin::CreateWindowAndBind("Trajectory Viewer", 1024, 768);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Set background color to light gray
  glClearColor(0.75f, 0.75f, 0.75f, 1.0f);

  // Define camera render object (OpenGl)
  pangolin::OpenGlRenderState s_cam(
      pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
      pangolin::ModelViewLookAt(0, -10, -15, 0, 0, 0, 0.0, -1.0, 0.0));

  // Create viewport
  pangolin::View &d_cam = pangolin::CreateDisplay()
                              .SetBounds(0.0, 1.0, 0.0, 1.0, -1024.0f / 768.0f)
                              .SetHandler(new pangolin::Handler3D(s_cam));

  while (!pangolin::ShouldQuit()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    d_cam.Activate(s_cam);

    // Draw coordinate axes
    glLineWidth(3);
    glBegin(GL_LINES);
    // X axis - red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    // Y axis - green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);
    // Z axis - blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glEnd();

    // Draw grid
    glLineWidth(1);
    glColor3f(0.5f, 0.5f, 0.5f);
    float sz = 20.0;
    float step = 1.0;

    glBegin(GL_LINES);
    for (float i = -sz; i <= sz; i += step) {
      glVertex3f(i, -sz, 0);
      glVertex3f(i, sz, 0);
      glVertex3f(-sz, i, 0);
      glVertex3f(sz, i, 0);
    }
    glEnd();

    // Draw trajectory
    glColor3f(0.0f, 1.0f, 0.0f);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    for (const auto &pose : poses) {
      Vector3d pos = pose.block<3, 1>(0, 3);
      glVertex3d(pos[0], pos[1], pos[2]);
    }
    glEnd();

    // Draw current position
    glPointSize(4);
    glBegin(GL_POINTS);
    for (const auto &pose : poses) {
      Vector3d pos = pose.block<3, 1>(0, 3);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex3d(pos[0], pos[1], pos[2]);
    }
    glEnd();

    pangolin::FinishFrame();
  }
}

int main() {
  vector<MatrixXd> projectionMatrices;
  Matrix4d transformation;
  vector<Matrix4d> poses;

  if (readCalibrationFile(calibFile, projectionMatrices, transformation)) {
    // Print results for verification
    for (size_t i = 0; i < projectionMatrices.size(); i++) {
      cout << "P" << i << ":\n" << projectionMatrices[i] << "\n\n";
    }
    cout << "Tr:\n" << transformation << endl;
  }

  if (readPoseFile(poseFile, poses)) {
    DrawTrajectory(poses);
  }

  return 0;
}