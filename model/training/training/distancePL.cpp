#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

void distancePL(Point center, vector<cv::Point> contour, double angle, double* min, double* max){
	*min = 0;
	*max = 0;
	double k = tan(angle);
	double mu = pow(pow(k,2)+1,0.5);
	int x0 = center.x;
	int y0 = center.y;
	for (int i = 0; i < contour.size(); i++){
		int x = contour.at(i).x;
		int y = contour.at(i).y;
		double d = (y - k*x - y0 + k*x0) / mu;
		if (d < 0)if (d < *min)*min = d;
		if (d > 0)if (d > *max)*max = d;
	}
}