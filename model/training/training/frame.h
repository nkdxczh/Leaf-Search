#include "windows.h"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <vector>

using namespace std;
using namespace cv;

#ifndef FRAME_H
#define FRAME_H

void TraverseDirectory(int type,wchar_t Dir[MAX_PATH]);
void dispose(int type,int id, string filename);
void readContours(string filename, vector<int> &types, vector<vector<Point> > &contours);
void Centroid_Radii(string fout, vector<int> &types, vector<vector<Point> > &contours);

#endif