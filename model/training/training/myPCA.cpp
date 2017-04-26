#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cxcore.h"
#include <stdio.h>
#include <math.h>
#include <vector>
#include "Windows.h" 
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <iostream>
#include<iomanip>
#include<fstream>

using namespace cv;	// OpenCV命名空间
using namespace std;


#define N1 9 //叶片样本数  
#define N2 3 //叶片样本数  


void myPCA(string in, string out)
{
	ifstream fin(in);
	ofstream fout(out);

	int count = 0;
	string line;
	float avg[N1];
	for (int n = 0; n < N1; n++)avg[n] = 0;
	while (getline(fin, line))count++;
	fin.clear();
	fin.seekg(0);

	Mat pcaSet = Mat(count, N1, CV_64F);
	Mat type = Mat(count, 1, CV_32S);

	int count1 = 0;
	while (getline(fin, line)){
		int devide = line.find(' ');
		int devide1;
		string types = line.substr(0, devide);
		int typei = atoi(types.c_str());
		type.at<int>(count1, 0) = typei;
		for (int i = 0; i < N1; i++){
			devide1 = line.find(' ', devide + 1);
			string features = line.substr(devide + 1, devide1 - devide - 1);
			float feature = atof(features.c_str());
			avg[i] += feature;
			pcaSet.at<double>(count1, i) = feature;
			devide = devide1;
		}
		count1++;
	}

	ofstream fout2("G:\\LSearchData\\allaveragr.txt");
	for (int n = 0; n < N1; n++){
		avg[n] = avg[n] / count1;
		fout2<<avg[n]<<" ";
	}
	fout2.close();

	PCA pca(pcaSet, Mat(), 0/*CV_PCA_DATA_AS_ROW*/);
	cout << pca.eigenvalues << endl;
	cout << endl;
	cout << pca.eigenvectors << endl << endl;

	Mat eigenvectors = pca.eigenvectors;

	int dim = N2;

	Mat post_eigenvectors = eigenvectors.rowRange(0, dim).clone();

	Mat real_post_eigenvectors = post_eigenvectors.t();

	cout << "-----------------post_eigenvectors -----------------" << endl << real_post_eigenvectors << endl;

	for (int i = 0; i < count; i++){
		for (int j = 0; j < N1; j++){
			 pcaSet.at<double>(i, j)-=avg[j];
		}
	}

	//cout << "-----------------pcaSet-----------------" << endl << pcaSet << endl;

	Mat output = pcaSet * real_post_eigenvectors;

	cout << "-----------------output-----------------" << endl << real_post_eigenvectors << endl;

	count1 = 0;
	for (int i = 0; i < count; i++){
		fout << type.at<int>(count1, 0) << " ";
		for (int j = 0; j < N2; j++){
			fout << output.at<double>(i,j) << " ";
		}
		fout << endl;
		count1++;
	}
	fout.close();

	ofstream fout1("G:\\LSearchData\\real_post_eigenvectors.txt");
	for (int i = 0; i < N1; i++){
		for (int j = 0; j < N2; j++){
			fout1 << real_post_eigenvectors.at<double>(i, j) << " ";
		}
		fout1 << endl;
	}
	//fout1 << real_post_eigenvectors << endl;
	fout1.close();

	//FileStorage fs("G:\\LSearchData\\real_post_eigenvectors.xml",FileStorage::WRITE);
	//fs << "real_post_eigenvectors" << real_post_eigenvectors;
	//fs.release();

	/*
	FileStorage fs(".\\vocabulary.xml", FileStorage::READ);
	Mat mat_vocabulary;
	fs["vocabulary"] >> mat_vocabulary;*/

	system("pause");
}

void avgPCA(string in, string out){
	FileStorage fs("G:\\LSearchData\\real_post_eigenvectors.xml", FileStorage::READ);
	Mat real_post_eigenvectors;
	fs["real_post_eigenvectors"] >> real_post_eigenvectors;

	ifstream fin(in);
	ofstream fout(out);

	int count = 0;
	string line;
	while (getline(fin, line))count++;
	fin.clear();
	fin.seekg(0);

	Mat pcaSet = Mat(count, N1, CV_64F);
	Mat type = Mat(count, 1, CV_32S);

	int count1 = 0;
	while (getline(fin, line)){
		int devide = line.find(' ');
		int devide1;
		string types = line.substr(0, devide);
		int typei = atoi(types.c_str());
		type.at<int>(count1, 0) = typei;
		for (int i = 0; i < N1; i++){
			devide1 = line.find(' ', devide + 1);
			string features = line.substr(devide + 1, devide1 - devide - 1);
			float feature = atof(features.c_str());
			pcaSet.at<double>(count1, i) = feature;
			devide = devide1;
		}
		count1++;
	}

	Mat output = pcaSet * real_post_eigenvectors;

	count1 = 0;
	for (int i = 0; i < count; i++){
		fout << type.at<int>(count1, 0) << " ";
		for (int j = 0; j < N2; j++){
			fout << output.at<double>(i, j) << " ";
		}
		fout << endl;
		count1++;
	}
	fout.close();
}