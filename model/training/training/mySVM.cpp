#include "mySVM.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream>
#include <math.h>

using namespace std;
using namespace cv;

#define pi 3.1415927
#define N 3

mySVM::mySVM(string in){
	fin = new ifstream(in);
	string line;
	int count=0;

	while (getline(*fin, line))count++;
	fin->clear();
	fin->seekg(0);

	Mat trainingDataMat(count, N, CV_32FC1);
	Mat labelsMat(count, 1, CV_32SC1);

	string features[N];
	double feature[N];

	count = 0;
	while (getline(*fin, line)){
		int devide = line.find(' ');
		string presenttype = line.substr(0, devide);
		int devide1 = line.find(' ', devide + 1);
		string typestr = line.substr(0, devide);
		int type = atoi(typestr.c_str());
		for (int i = 0; i < N; i++){
			devide1 = line.find(' ', devide + 1);
			features[i] = line.substr(devide + 1, devide1 - devide - 1);
			feature[i] = atof(features[i].c_str());
			devide = devide1;
		}
		labelsMat.at<int>(count, 0) = type;
		for (int i = 0; i < N; i++){
			trainingDataMat.at<float>(count, i) = feature[i];
		}
		count++;
	}

	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;//SVM类型：使用C支持向量机  
	params.kernel_type = CvSVM::LINEAR;//核函数类型：线性  
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);//终止准则函数：当迭代次数达到最大值时终止  


	svm.train_auto(trainingDataMat, labelsMat, Mat(), Mat(), params);
	svm.save("G:\\LSearchData\\svmdataset.xml");
}

int mySVM::identify(Mat input){

	vector<std::vector<cv::Point>> contours;
	Mat gray;
	cvtColor(input, gray, CV_BGR2GRAY);
	findContours(gray, contours, // a vector of contours   
		CV_RETR_EXTERNAL, // retrieve the external contours  
		CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours  

	Mat result(input.size(), CV_8U, cv::Scalar(0));
	cv::drawContours(result, contours,
		0, // draw all contours  
		cv::Scalar(255), // in black  
		2); // with a thickness of 2  



	vector<std::vector<cv::Point>> contour;
	int maxsize = 0;
	for (int i = 0; i<contours.size(); i++){
		if (contours.at(i).size() > maxsize){
			maxsize = contours.at(i).size();
			contour.clear();
			contour.push_back(contours.at(i));
		}
	}
	RotatedRect mr = minAreaRect(contour.at(0));

	Point right;
	right.x = 0;
	Point left;
	left.x = input.cols;
	Point top;
	top.y = input.rows;
	Point bottom;
	bottom.y = 0;
	for (int i = 0; i < contour.at(0).size(); i++){
		Point present = contour.at(0).at(i);
		if (present.x>right.x)right = present;
		if (present.x<left.x)left = present;
		if (present.y<top.y)top = present;
		if (present.y>bottom.y)bottom = present;
	}

	int area = contourArea(contour.at(0));
	//std::cout << "area:" << area << endl;
	float acrsumvalue=pow(contour.at(0).size(), 2) / (4 * pi*area);

	float whrvalue=(float)(bottom.y - top.y) / (right.x - left.x);

	cout << acrsumvalue << "," << whrvalue << endl;
	Mat sampleMat = (Mat_<float>(1, 2) << acrsumvalue, whrvalue);
	//Mat sampleMat = (Mat_<float>(1, 2) << 46, 4);
	return svm.predict(sampleMat);


	cv::namedWindow("Contours");
	cv::imshow("Contours", result);

	cv::waitKey(10);
}