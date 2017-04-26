#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

#define pi 3.1415927

float rotateAngle(vector<cv::Point> contour){
	double longest=0;
	Point aim[2];
	for (int i = 0; i < contour.size(); i++){
		for (int j = 0; j < contour.size(); j++){
			double tem = pow((contour.at(i).x - contour.at(j).x), 2) + pow((contour.at(i).y - contour.at(j).y), 2);
			if (tem>longest){
				aim[0] = contour.at(i);
				aim[1] = contour.at(j);
			}
		}
	}
	double k = (aim[0].x - aim[1].x) / (aim[0].y - aim[1].y);
	double angle = tanh(k);
	return (float)(angle/pi)*180;
}