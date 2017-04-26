#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "Windows.h" 

using namespace std;
using namespace cv;

void dispose(int type, int id, string filename){
	string outputfilename;
	if (id < 25){
		outputfilename="g:\\LSearchData\\outlineForTraining.txt"; // 此处写入文件名 
	}
	else{
		outputfilename="g:\\LSearchData\\outlineForTesting.txt"; // 此处写入文件名 
	}
	ofstream fout(outputfilename,ios::out | ios::app);
	fout << type <<endl;

	std::cout << filename << endl;
	Mat image = imread(filename);
	float n = image.rows < image.cols ? image.cols : image.rows;
	n = n / 200;
	resize(image, image, Size(image.cols / n, image.rows/n), 0, 0, CV_INTER_LINEAR);

	Mat gray(image.size(), CV_8U);

	cvtColor(image, gray, CV_BGR2GRAY); 

	//Sobel(gray, gray, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	threshold(gray, gray, 145, 255, cv::THRESH_BINARY_INV);

	Mat element(5, 5, CV_8U, cv::Scalar(255));

	morphologyEx(gray, gray, CV_MOP_CLOSE, element);
	vector<std::vector<cv::Point>> contours;

	cv::findContours(gray,
		contours, // a vector of contours   
		CV_RETR_EXTERNAL, // retrieve the external contours  
		CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours  

	// Print contours' length  
	//std::cout << "Contours: " << contours.size() << std::endl;
	vector<std::vector<cv::Point>> contour;
	int maxsize = 0;
	for (int i = 0; i<contours.size(); i++){
		if (contours.at(i).size() > maxsize){
			maxsize = contours.at(i).size();
			contour.clear();
			contour.push_back(contours.at(i));
		}
	}

	fout << contour.at(0) << endl;

	Mat result(image.size(), CV_8U, cv::Scalar(0));
	cv::drawContours(result, contour,
		0, // draw all contours  
		cv::Scalar(255), // in black  
		CV_FILLED); // with a thickness of 2  

	string name;
	std::ostringstream oss;
	if (id < 25){
		oss << "g:\\LSearchData\\cutLeaveForTraining\\" << type << "\\" << id << ".jpg";
	}
	else{
		oss << "g:\\LSearchData\\cutLeaveForTesting\\" << type << "\\" << id-25 << ".jpg";
	}
	name = oss.str();
	imwrite(name, result);

	fout.close();
	//float angle = rotateAngle(contour.at(0));
/*	RotatedRect mr = minAreaRect(contour.at(0));

	Point right;
	right.x = 0;
	Point left;
	left.x = gray.cols;
	Point top;
	top.y = gray.rows;
	Point bottom;
	bottom.y = 0;
	for (int i = 0; i < contour.at(0).size(); i++){
		Point present = contour.at(0).at(i);
		if (present.x>right.x)right = present;
		if (present.x<left.x)left = present;
		if (present.y<top.y)top = present;
		if (present.y>bottom.y)bottom = present;
	}
	
    

	Size size;
	size.height = bottom.y - top.y;
	size.width=right.x - left.x;
	Mat cutResult(size, CV_8U, cv::Scalar(0));
	for (int row = 0; row < size.height; row++){
		for (int col = 0; col < size.width; col++){
			cutResult.at<bool>(row, col) = result.at<bool>(row + top.y, col + left.x);
		}
	}


	int area = contourArea(contour.at(0));
	fout << pow(contour.at(0).size(),2)/(4*pi*area)<<" ";

	fout << (double)(bottom.y-top.y)/(right.x-left.x)<<endl;

	fout.close();

	*/

	/*
	cv::Mat rotateMat;
	std::cout <<"rotate:"<< angle << endl;
	rotateMat = cv::getRotationMatrix2D(mr.center, -angle, 0.6);

	cv::Mat rotateImg;
	cv::warpAffine(result, result, rotateMat, result.size());
	
	cv::namedWindow("Contours");
	cv::imshow("Contours", result);

	cv::waitKey(10);*/
}