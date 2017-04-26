#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

#ifndef ASSIST_H
#define ASSIST_H

void distancePL(Point center, vector<cv::Point> contour, double angle, double* min, double* max);
void symmetry(Point center, vector<cv::Point> contour, double angle, double* interval);
float rotateAngle(vector<cv::Point> contour);

#endif