#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp> 
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

class mySVM{

private:
	ifstream* fin;
	CvSVM svm;

public:
	mySVM(string in);
	int identify(Mat result);
};