#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cxcore.h"
#include <stdio.h>
#include <math.h>
#include <vector>
#include "Windows.h" 

#define M_PI       3.14159265358979323846

using namespace cv;	// OpenCV�����ռ�
using namespace std;

struct MyCircle
{
	int x;
	int y;
	int r;
};
//MyCircle detectInCircles(CvSeq *contour);


IplImage *dst;


//MyCircle bc = {0, 0, 2};

double length; // �����ܳ�
double area;  // �������
CvPoint pt_c;  // ��������
double UR;//���ĵ�������ƽ������   
double PR;//���ĵ�������ľ�����   
double r_out = 0;//����Բ�뾶
double r_in = 500;//�ڽ�Բ�뾶
double circle_area_out;  // ��С���Բ���
double circle_circum_out;  // ��С���Բ�ܳ�
double circle_area_in;  // �������Բ���
double circle_circum_in;  // �������Բ�ܳ�
double rect_w;  // ��С��Ӿ��ο�
double rect_h;  // ��С��Ӿ��γ�
double rect_area;  // ��С��Ӿ������
double rect_length;  // ��С��Ӿ����ܳ�
double hull_area;  // ͹�����
double hull_length;  // ͹���ܳ�
double long_axis;  // ��������
double short_axis;  // ��������
double l_c;  // ���ĵ�����������̾���

double rectangularity;  // ���ζ�
double complexity;  // ���Ӷ�
double circularity;  // Բ�ζ�
double sphericity;  // ���ζ�
double aspect_radio;  // �������
double a_convexity;  // �����͹��
double p_convexity;  // �ܳ���͹��
double eccentricity;  //ƫ����
double lobation;  // Ҷ״��


void contour(CvSeq *cont)//��������ܳ������
{
	length = cvArcLength(cont);
	area = cvContourArea(cont);

	cout << "Length = " << length << endl;
	cout << "Area = " << area << endl;

}

void center(CvSeq *cont)//�����������
{
	CvMoments moments;
	CvMat *region;
	CvPoint pt1, pt2;
	double m00 = 0, m10, m01, mu20, mu11, mu02, inv_m00;
	double a, b, c;
	int xc, yc; //���ĺ�������

	region = (CvMat*)cont;
	cvMoments(region, &moments, 0);

	m00 = moments.m00;
	m10 = moments.m10;
	m01 = moments.m01;
	mu11 = moments.mu11;
	mu20 = moments.mu20;
	mu02 = moments.mu02;

	inv_m00 = 1. / m00;
	xc = cvRound(m10 * inv_m00);
	yc = cvRound(m01 * inv_m00);
	a = mu20 * inv_m00;
	b = mu11 * inv_m00;
	c = mu02 * inv_m00;
	pt_c.x = xc;  pt_c.y = yc;


	cvCircle(dst, pt_c, int(2), CV_RGB(255, 0, 0));
	cout << "xc = " << pt_c.x << endl << "yc = " << pt_c.y << endl;


	//���������ĵ���̾�����ƽ������   
	double sum = 0;
	int count = 0;
	l_c = 500;
	CvPoint * contourPoint;
	for (int m = 0; m< cont->total; m++)
	{
		contourPoint = (CvPoint*)cvGetSeqElem(cont, m);
		double temp1 = sqrt(pow(contourPoint->x - pt_c.x, 2) + pow(contourPoint->y - pt_c.y, 2));
		l_c = l_c<temp1 ? l_c : temp1;
		sum += temp1;
		count++;
	}
	UR = sum / count;
	cout << "l_c = " << l_c << endl;
	cout << "mean distance to center of gravity" << UR << endl;



	//���������ĵ�������ľ�����   
	sum = 0;
	for (int m = 0; m< cont->total; m++)
	{
		contourPoint = (CvPoint*)cvGetSeqElem(cont, m);
		double temp1 = sqrt(pow(contourPoint->x - pt_c.x, 2) + pow(contourPoint->y - pt_c.y, 2));
		sum += pow(temp1 - UR, 2);
	}
	PR = sum / count;
	cout << "mean square error of distance to center of gravity" << PR << endl;



}

//void circle_out (CvSeq *cont)//�����С���Բ�ܳ������
//{
//	CvPoint2D32f center;    
//    float radius_out;    
//
//    int a0 =  cvMinEnclosingCircle(cont,&center,&radius_out); 
//
//	circle_area_out = M_PI * pow(radius_out,2);
//	circle_circum_out = 2 * M_PI * radius_out;
//
//
//	cout<<"center_out.x = "<<center.x<<endl;    
//    cout<<"center_out.y = "<<center.y<<endl;   
//    cout<<"radius_out = "<<radius_out<<endl;   
//	cout<<"circle_area_out = "<<circle_area_out<<endl;    
//    cout<<"circle_circum_out = "<<circle_circum_out<<endl;   
//    cvCircle(dst,cvPointFrom32f(center),int(radius_out),CV_RGB(255,255,255));    
//}


//MyCircle detectInCircles(CvSeq *contour) //�������Բ
//{
//    CvRect r = cvBoundingRect(contour);
//	
//	int dx = r.x + r.width;            
//	int dy = r.y + r.height;
//
//
//
//	for (int x = r.x; x < dx; x += 5)
//		for (int y = r.y; y < dy; y += 5)
//		{
//            float d = cvPointPolygonTest(contour, cvPoint2D32f(x, y), 1);
//			if (d > 0 &&  bc.r  < d)
//			{
//                bc.x = x;  
//				bc.y = y;  
//				bc.r = d;
//			}
//		}
//
//		//second
//        dx = bc.x + 5;  
//		dy = bc.y + 5;
//		for (int x = bc.x; x < dx; x += 1)      
//			for (int y = bc.y; y < dy; y += 1)
//			{
//                float d = cvPointPolygonTest(contour, cvPoint2D32f(x, y), 1);
//                if (d > 0 &&  bc.r  < d)
//				{
//					bc.x = x;  
//					bc.y = y;  
//					bc.r = d;
//				}
//			}
//
//          return bc;
//}

//void circle_in (CvSeq *cont)//����������Բ������ܳ�
//{
//	MyCircle pCircle = detectInCircles(cont);
//
//	double radius_in = pCircle.r;    
//	double circle_area_in = M_PI * pow(radius_in,2);
//	double circle_circum_in = 2 * M_PI * radius_in;
//
//
//	cout<<"center_in.x = "<<pCircle.x<<endl;    
//    cout<<"center_in.y = "<<pCircle.y<<endl;   
//    cout<<"radius_in = "<<radius_in<<endl;  
//	cout<<"circle_area_in = "<<circle_area_in<<endl;    
//    cout<<"circle_circum_in = "<<circle_circum_in<<endl;   
//	cvCircle(dst,cvPoint(pCircle.x,pCircle.y),pCircle.r,CV_RGB(255,255,255),1,8);
//
////}



void rect(CvSeq *cont)//�����С��Ӿ����ܳ������
{
	CvBox2D box = cvMinAreaRect2(cont, NULL);
	CvPoint2D32f pt[4];
	cvBoxPoints(box, pt);
	double l1 = sqrt(pow((pt[1].x - pt[0].x), 2) + pow((pt[1].y - pt[0].y), 2));
	double l2 = sqrt(pow((pt[2].x - pt[1].x), 2) + pow((pt[2].y - pt[1].y), 2));
	if (l1<l2)
	{
		rect_w = l1;  rect_h = l2;
	}
	else
	{
		rect_w = l2;  rect_h = l1;
	}

	rect_area = rect_h *rect_w;
	rect_length = (rect_h + rect_w) * 2;

	cout << "rect_h = " << rect_h << endl;
	cout << "rect_w = " << rect_w << endl;
	cout << "rect_area = " << rect_area << endl;
	cout << "rect_length = " << rect_length << endl;
	for (int i = 0; i<4; ++i)
	{
		cvLine(dst, cvPointFrom32f(pt[i]), cvPointFrom32f(pt[((i + 1) % 4) ? (i + 1) : 0]), CV_RGB(100, 100, 255));
	}

}



//���͹�����ܳ������
//�����С����Բ�ܳ������
//�������ڽ�Ե�ܳ������
//�����������
void hull(CvSeq *cont)
{

	hull_area = 0.0;
	hull_length = 0.0;

	CvSeq *hull = NULL;
	hull = cvConvexHull2(cont, 0, CV_CLOCKWISE, 0);

	CvPoint pt0 = **(CvPoint**)cvGetSeqElem(hull, hull->total - 1);

	for (int i = 0; i<hull->total; ++i)
	{
		CvPoint pt1 = **(CvPoint**)cvGetSeqElem(hull, i);

		cvLine(dst, pt0, pt1, CV_RGB(200, 100, 150));

		//����͹�����		
		//�����ÿ�������ε����
		hull_area += abs(((pt1.x - pt_c.x) * (pt0.y - pt_c.y) - (pt1.y - pt_c.y) * (pt0.x - pt_c.x)) / 2.0);

		//����͹���ܳ�
		hull_length += sqrt(pow((pt0.x - pt1.x), 2) + pow((pt0.y - pt1.y), 2));

		pt0 = pt1;
	}

	cout << "hull_area = " << hull_area << endl;
	cout << "hull_Length = " << hull_length << endl;



	r_out = 0.0;
	r_in = 500.0;

	CvSeq *defect = cvConvexityDefects(cont, hull);

	for (int i = 0; i<defect->total; ++i)
	{
		CvConvexityDefect df = *(CvConvexityDefect*)cvGetSeqElem(defect, i);
		cvCircle(dst, *df.start, 2, CV_RGB(255, 255, 0), -1);
		cvCircle(dst, *df.end, 2, CV_RGB(255, 255, 0), -1);
		cvCircle(dst, *df.depth_point, 2, CV_RGB(0, 255, 255), -1);

		CvPoint s = *df.start;  /* point of the contour where the defect begins */
		CvPoint e = *df.end;  /* point of the contour where the defect ends */
		CvPoint d = *df.depth_point;  /* the farthest from the convex hull point within the defect */

		double s_t = sqrt(pow((s.x - pt_c.x), 2) + pow((s.y - pt_c.y), 2));
		double e_t = sqrt(pow((e.x - pt_c.x), 2) + pow((e.y - pt_c.y), 2));
		double r_in_t = sqrt(pow((d.x - pt_c.x), 2) + pow((d.y - pt_c.y), 2));
		double r_out_t = s_t>e_t ? s_t : e_t;

		r_out = r_out>r_out_t ? r_out : r_out_t; //��С���Բ�뾶Ϊ���ĵ���������Զ�ĵ�ľ���
		r_in = r_in<r_in_t ? r_in : r_in_t;  //�������Բ�뾶Ϊ���ĵ�����������ĵ�ľ���

	}

	cout << "r_out = " << r_out << endl << "r_in = " << r_in << endl;
	cvCircle(dst, pt_c, r_out, CV_RGB(255, 255, 0));
	cvCircle(dst, pt_c, r_in, CV_RGB(0, 255, 255));

	circle_area_out = M_PI * pow(r_out, 2);
	circle_circum_out = 2 * M_PI * r_out;
	cout << "circle_area_out = " << circle_area_out << endl;
	cout << "circle_circum_out = " << circle_circum_out << endl;

	circle_area_in = M_PI * pow(r_in, 2);
	circle_circum_in = 2 * M_PI * r_in;
	cout << "circle_area_in = " << circle_area_in << endl;
	cout << "circle_circum_in = " << circle_circum_in << endl;



}


void axis(CvSeq *cont)
{
	long_axis = 0;
	short_axis = 0;
	CvPoint *contourPoint1, *contourPoint2;
	CvPoint pt_l1, pt_l2; //�������˵�
	CvPoint pt_s1, pt_s2; // �������˵�

	for (int m = 0; m< cont->total; m++)
	{
		contourPoint1 = (CvPoint*)cvGetSeqElem(cont, m);
		for (int n = 0; n< cont->total; n++)
		{
			contourPoint2 = (CvPoint*)cvGetSeqElem(cont, n);
			double p1_p2_t = sqrt(pow(contourPoint1->x - contourPoint2->x, 2) + pow(contourPoint1->y - contourPoint2->y, 2));
			if (p1_p2_t>long_axis)
			{
				long_axis = p1_p2_t;
				pt_l1 = *contourPoint1;
				pt_l2 = *contourPoint2;
			}
		}
	}

	double k_l = double(pt_l1.y - pt_l2.y) / double(pt_l1.x - pt_l2.x);
	double k_s;

	for (int m = 0; m< cont->total; m++)
	{
		contourPoint1 = (CvPoint*)cvGetSeqElem(cont, m);
		for (int n = 0; n< cont->total; n++)
		{
			contourPoint2 = (CvPoint*)cvGetSeqElem(cont, n);
			if (contourPoint1->x == contourPoint2->x)
				continue;
			k_s = double(contourPoint1->y - contourPoint2->y) / double(contourPoint1->x - contourPoint2->x);
			double x = abs(k_s*k_l + 1);
			if (x < 0.005)
			{
				double short_temp = sqrt(pow(contourPoint1->x - contourPoint2->x, 2) + pow(contourPoint1->y - contourPoint2->y, 2));
				if (short_temp>short_axis)
				{
					short_axis = short_temp;
					pt_s1 = *contourPoint1;
					pt_s2 = *contourPoint2;
				}
			}
		}
	}


	cvLine(dst, pt_l1, pt_l2, CV_RGB(30, 200, 100));
	cvLine(dst, pt_s1, pt_s2, CV_RGB(30, 200, 100));
	cout << "long_axis = " << long_axis << endl;
	cout << "short_axis = " << short_axis << endl;

}




void feature()
{
	rectangularity = area / rect_area;
	complexity = pow(length, 2) / area;
	circularity = UR / PR;
	sphericity = r_in / r_out;
	aspect_radio = rect_w / rect_h;
	a_convexity = area / hull_area;
	p_convexity = length / hull_length;
	eccentricity = long_axis / short_axis;
	lobation = l_c / short_axis;

	cout << "rectangularity = " << rectangularity << endl;
	cout << "complexity = " << complexity << endl;
	cout << "circularity = " << circularity << endl;
	cout << "sphericity = " << sphericity << endl;
	cout << "aspect_radio = " << aspect_radio << endl;
	cout << "a_convexity = " << a_convexity << endl;
	cout << "p_convexity = " << p_convexity << endl;
	cout << "eccentricity = " << eccentricity << endl;
	cout << "lobation = " << lobation << endl;

}










/*
int main()
{

	IplImage * src_Img = NULL;
	src_Img = cvLoadImage("01.jpg", 1);//-1 ����ԭͼ��1����bgr

	if (!src_Img)
		return -1;

	//�ҶȻ�
	IplImage * temp_Img = cvCreateImage(cvGetSize(src_Img), IPL_DEPTH_8U, 1);
	cvCvtColor(src_Img, temp_Img, CV_BGR2GRAY);//תΪ�ҽ�ͼ��temp_Img

	//��ֵ��
	IplImage * binary_Img = cvCreateImage(cvGetSize(src_Img), IPL_DEPTH_8U, 1);;
	cvThreshold(temp_Img, binary_Img, 200, 255, CV_THRESH_BINARY);

	cvNamedWindow("1");
	cvShowImage("1", src_Img);


	////��ֵ�˲�
	//IplImage * median_Img = cvCreateImage(cvGetSize(src_Img), IPL_DEPTH_8U, 1);   
	//   cvSmooth(binary_Img,median_Img,CV_MEDIAN,5,2);  

	// �ҵ���������
	CvMemStorage *stor = cvCreateMemStorage(0);  //�ڴ�洢����
	CvSeq *cont = 0;     //ָ��storage�е�����
	int cnt = cvFindContours(binary_Img, stor, &cont);  //cnt = 5;
	cout << "number = " << cnt << endl;
	//h_nextΪ��һ������
	//���ĸ�����ΪҶƬ����
	cont = cont->h_next;
	cont = cont->h_next;
	cont = cont->h_next;

	// ���Ƶ�ǰ����
	dst = cvCreateImage(cvGetSize(src_Img), 8, 3);
	cvZero(dst);
	cvDrawContours(dst, cont, CV_RGB(255, 255, 0), CV_RGB(255, 255, 255), 0, 1, 8, cvPoint(0, 0));


	contour(cont);

	center(cont);

	rect(cont);

	hull(cont);

	axis(cont);

	feature();


	cvShowImage("dst", dst);
	cvWaitKey();


	waitKey();
	return 0;
	}*/