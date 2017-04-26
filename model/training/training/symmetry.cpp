#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

void symmetry(Point center, vector<cv::Point> contour, double angle, double* interval){
	double k = tan(angle);
	//double mu = pow(pow(k, 2) + 1, 0.5);
	int x0 = center.x;
	int y0 = center.y;
	double sum[2];
	sum[0] = 0;
	sum[1] = 0;
	for (int i = 0; i < contour.size(); i++){
		int x = contour.at(i).x;
		int y = contour.at(i).y;
		double d = (y-y0)-k*(x-x0);
		if (d < 0)sum[0] += d;
		if (d > 0)sum[1] += d;
	}
	//cout << sum[0] <<","<< sum[1] << endl;
	*interval = abs(sum[0] + sum[1]);
}