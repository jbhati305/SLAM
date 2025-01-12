#include <iostream>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>

using namespace std;

int main(int argc, char **argv) {
  // Use absolute path or relative to executable
  string imagePath = "/home/jitesh/SLAM/visual_odom/distorted.png";
  cv::Mat image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);

  if (image.empty()) {
    cerr << "Error: Could not read image at " << imagePath << endl;
    return -1;
  }

  cv::imshow("distorted", image);
  cv::waitKey(0);
  return 0;
}