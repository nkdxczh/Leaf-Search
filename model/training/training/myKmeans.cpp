#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>

#define N 3

using namespace std;
using namespace cv;

void countAverage(string in, string out){
	ifstream fin(in);
	ofstream fout(out); 
	string line;
	string type=" ";
	string features[N];
	double feature[N];
	int count = 0;
	while (getline(fin,line)){
		int devide = line.find(' ');
		string presenttype = line.substr(0, devide);
		if (presenttype != type){
			if (type != " "){
				fout << type << " ";
				cout << type << " ";
				for (int i = 0; i < N; i++){
					fout << feature[i] / count << " ";
					cout << feature[i] / count << " ";
				}
				fout << endl;
				cout << endl;
			}
			type = presenttype;
			for (int i = 0; i < N; i++){
				feature[i]=0;
			}
			count = 0;
			int devide1;
			for (int i = 0; i < N; i++){
				devide1 = line.find(' ', devide + 1);
				features[i] = line.substr(devide + 1, devide1 - devide - 1);
				feature[i] += atof(features[i].c_str());
				devide = devide1;
			}
			count++;
		}
		else{
			int devide1;
			for (int i = 0; i < N; i++){
				devide1 = line.find(' ', devide + 1);
				features[i] = line.substr(devide + 1, devide1 - devide - 1);
				feature[i] += atof(features[i].c_str());
				devide = devide1;
			}
			count++;
		}
	}
	fout << type << " ";
	cout << type << " ";
	for (int i = 0; i < N; i++){
		fout << feature[i] / count << " ";
		cout << feature[i] / count << " ";
	}
	fout << endl;
	cout << endl;
}

void kmean(string in, string out){
	ifstream fin(in);
	ofstream fout(out);
	string line;

	int count = 0;
	while (getline(fin, line))count++;
	fin.clear();
	fin.seekg(0);
	Mat input(count, N, CV_32FC1);
	Mat inputLabel(count, 1, CV_32FC1);
	count = 0;

	string features[N];
	double feature[N];

	while (getline(fin, line)){
		//cout << line << endl;
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
		inputLabel.at<int>(count, 0) = type;
		for (int i = 0; i < N; i++){
			input.at<float>(count, i) = feature[i];
		}
		count++;
	}
	Mat output;
	Mat outputLabel;
	const int kinds = 6;
	kmeans(input, kinds, outputLabel, TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 3, 1.0), 5, KMEANS_PP_CENTERS);
	int kind[kinds];
	for (int i = 0; i < kinds; i++)kind[i] = 0;
	for (int row = 0; row < outputLabel.rows; row++){
		for (int col = 0; col < outputLabel.cols; col++){
			fout << outputLabel.at<int>(row, col);
			kind[outputLabel.at<int>(row, col)]++;
		}
		fout << endl;
	}
	for (int i = 0; i < kinds; i++)cout << kind[i] << ",";
	cout << endl;
	fout.close();
	fin.close();
}

void rebuildData(string in, string modify, string out, string out1){
	ifstream fin(modify);
	ofstream fout(out);
	ofstream fout1(out1);
	string line;

	string features[N];
	double feature[N];

	int count = 0;

	while (getline(fin, line))count++;
	fin.clear();
	fin.seekg(0);

	int* type=new int[count];
	count = 0;
	while (getline(fin, line)){
		type[count++] = atoi(line.c_str());
	}
	cout << count << endl;
	fin.close();
	fin.open(in);
	count = -1;
	int t=-1;
	while (getline(fin, line)){
		int devide = line.find(' ');
		int devide1 = line.find(' ', devide + 1);
		string presenttype = line.substr(0, devide);
		int presentt = atoi(presenttype.c_str());
		if (presentt != t){
			count++;
			t = presentt;
		}
		for (int i = 0; i < N; i++){
			devide1 = line.find(' ', devide + 1);
			features[i] = line.substr(devide + 1, devide1 - devide - 1);
			feature[i] = atof(features[i].c_str());
			devide = devide1;
		}
		fout << presentt << " " << type[count] << " ";
		fout1 << type[count] << " ";
		for (int i = 0; i < N; i++){
			fout << feature[i] << " ";
			fout1 << feature[i] << " ";
		}
		fout << endl;
		fout1 << endl;
	}
}

void generateFile(string in, string modify, string out){
	
	ifstream fin(modify);
	string line;
	
	int count = 0;

	while (getline(fin, line))count++;
	fin.clear();
	fin.seekg(0);

	
	int* typel = new int[count];
	count = 0;
	while (getline(fin, line)){
		typel[count++] = atoi(line.c_str());
	}
	cout << count << endl;
	fin.close();

	fin.open(in);
	ofstream fout(out);
	string type = " ";
	string features[N];
	double feature[N];
	Vector<float> data[N];
	count = 0;
	int count1 = 0;
	while (getline(fin, line)){
		int devide = line.find(' ');
		string presenttype = line.substr(0, devide);
		if (presenttype != type){
			if (type != " "){
				fout << type << " ";
				cout << type << " ";
				fout << typel[count1] << " ";
				cout << typel[count1] << " ";
				count1++;
				for (int i = 0; i < N; i++){
					fout << feature[i] / count << " ";
					cout << feature[i] / count << " ";

					float tem = 0;
					for (int j = 0; j < data[i].size(); j++){
						tem += pow(data[i][j] - feature[i] / count,2);
					}
					tem = sqrt(tem/count);
					fout << tem << " ";
					cout << tem << " ";
					data[i].clear();
				}
				fout << endl;
				cout << endl;
			}
			type = presenttype;
			for (int i = 0; i < N; i++){
				feature[i] = 0;
			}
			count = 0;
			int devide1;
			for (int i = 0; i < N; i++){
				devide1 = line.find(' ', devide + 1);
				features[i] = line.substr(devide + 1, devide1 - devide - 1);
				feature[i] += atof(features[i].c_str());
				data[i].push_back(atof(features[i].c_str()));
				devide = devide1;
			}
			count++;
		}
		else{
			int devide1;
			for (int i = 0; i < N; i++){
				devide1 = line.find(' ', devide + 1);
				features[i] = line.substr(devide + 1, devide1 - devide - 1);
				feature[i] += atof(features[i].c_str());
				data[i].push_back(atof(features[i].c_str()));
				devide = devide1;
			}
			count++;
		}
	}
	fin.clear();
	fin.seekg(0);



	fout << type << " ";
	cout << type << " ";
	fout << typel[count1] << " ";
	cout << typel[count1] << " ";
	for (int i = 0; i < N; i++){
		fout << feature[i] / count << " ";
		cout << feature[i] / count << " ";

		float tem = 0;
		for (int j = 0; j < data[i].size(); j++){
			cout << data[i][j] << endl;
			tem += pow(data[i][j] - feature[i] / count, 2);
		}
		tem = sqrt(tem / count);
		fout << tem << " ";
		cout << tem << " ";
		data[i].clear();
	}
	fout << endl;
	cout << endl;
}
