#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <iostream>
#include<iomanip>
#include<fstream>


// OpenCV 库
#pragma comment( lib, "opencv_core249d.lib" ) 
#pragma comment( lib, "opencv_imgproc249d.lib" )
#pragma comment( lib, "opencv_highgui249d.lib" ) 

using namespace cv;	// OpenCV命名空间
using namespace std;


//需要的库cv210.lib cxcore210.lib highgui210.lib  cvcam.lib
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cxcore.h"
#include <stdio.h>
#include <math.h>
#include <vector>
#include "Windows.h" 


//CvPoint pt_c;  // 轮廓重心
//IplImage *dst;
//double rect_w;  // 最小外接矩形宽
//double rect_h;  // 最小外接矩形长
//double rect_area;  // 最小外接矩形面积
//double rect_length;  // 最小外接矩形周长

//void center(CvSeq *cont)//获得轮廓重心
//{
//	CvMoments moments; 
//    CvMat *region; 
//    CvPoint pt1,pt2; 
//    double m00 = 0, m10, m01, mu20, mu11, mu02, inv_m00; 
//    double a, b, c; 
//    int xc, yc; //重心横纵坐标
//
//    region=(CvMat*)cont; 
//    cvMoments( region, &moments,0 ); 
//
//	m00 = moments.m00; 
//    m10 = moments.m10; 
//    m01 = moments.m01; 
//    mu11 = moments.mu11; 
//    mu20 = moments.mu20; 
//    mu02 = moments.mu02; 
//
//    inv_m00 = 1. / m00; 
//    xc = cvRound( m10 * inv_m00 ); 
//    yc = cvRound( m01 * inv_m00 ); 
//    a = mu20 * inv_m00; 
//    b = mu11 * inv_m00; 
//    c = mu02 * inv_m00; 
//	pt_c.x = xc;  pt_c.y = yc;
//
//
//	cvCircle(dst,pt_c,int(2),CV_RGB(255,0,0),-1);    
//	cout<<"xc = "<<pt_c.x<<endl<<"yc = "<<pt_c.y<<endl;
//
//}

//CvPoint2D32f p1, p2; //短轴两端点 
//CvPoint2D32f p3, p4; //另一短轴两端点

//void rect (CvSeq *cont)//获得最小外接矩形周长和面积
//{
//	CvBox2D box = cvMinAreaRect2(cont,NULL);  
//    CvPoint2D32f pt[4];  
//    cvBoxPoints(box,pt);  
//	double l1 = sqrt(pow((pt[1].x-pt[0].x),2)+pow((pt[1].y-pt[0].y),2));
//	double l2 = sqrt(pow((pt[2].x-pt[1].x),2)+pow((pt[2].y-pt[1].y),2));
//	if (l1<l2)
//	{
//		p1 = pt[0];
//		p2 = pt[1];
//		p3 = pt[2];
//		p4 = pt[3];
//		rect_w = l1;  rect_h = l2;
//	}
//	else
//	{
//		p1 = pt[1];
//		p2 = pt[2];
//		p3 = pt[3];
//		p4 = pt[0];
//		rect_w = l2;  rect_h = l1;
//
//	}
//	cvLine(dst,cvPointFrom32f(p1),cvPointFrom32f(p2),CV_RGB(255,255,255));  
//	cvLine(dst,cvPointFrom32f(p3),cvPointFrom32f(p4),CV_RGB(255,255,255));  
//
//	//rect_h = l1>l2?l1:l2;
//	//rect_w = l1<l2?l1:l2;
//	rect_area = rect_h *rect_w;
//	rect_length = (rect_h+rect_w)*2;
//	
//	cout<<"rect_h = "<<rect_h<<endl;    
//    cout<<"rect_w = "<<rect_w<<endl;   	
//	cout<<"rect_area = "<<rect_area<<endl;    
//    cout<<"rect_length = "<<rect_length<<endl;   
// //   for(int i = 0;i<4;++i)
//	//{  
// //       cvLine(dst,cvPointFrom32f(pt[i]),cvPointFrom32f(pt[((i+1)%4)?(i+1):0]),CV_RGB(100,100,100));  
//	//}  
//
//}

//double distan(CvPoint2D32f pt1, CvPoint2D32f pt2, CvPoint pt3)
//{
//    double A = (pt1.y-pt2.y)/(pt1.x- pt2.x);
//    double B = (pt1.y-A*pt1.x);
//    /// > 0 = ax +b -y;
//    return abs(A*pt3.x + B -pt3.y)/sqrt(A*A + 1);
//}

//int main()
//{
//	char path_r [10];//读取图像的位置
//
//for(int n = 1; n<=5; n++)//循环处理80张图像
//{
//
//	sprintf(path_r,"0%d.jpg",n);	
//	IplImage * src_Img=NULL;
//	src_Img=cvLoadImage(path_r ,1);//-1 代表原图，1代表bgr
//	
//	if(!src_Img)
//		return -1;
//
//	//IplImage * src_Img=NULL;
//	//src_Img=cvLoadImage("04.jpg",1);//-1 代表原图，1代表bgr
//	//
//	//if(!src_Img)
//	//	return -1;
//
//	//灰度化
//	IplImage * temp_Img=cvCreateImage(cvGetSize(src_Img),IPL_DEPTH_8U,1);
//	cvCvtColor(src_Img,temp_Img,CV_BGR2GRAY);//转为灰阶图像temp_Img
//	
//	//二值化
//	IplImage * binary_Img = cvCreateImage(cvGetSize(src_Img), IPL_DEPTH_8U, 1); ;
//	cvThreshold(temp_Img, binary_Img, 200, 255, CV_THRESH_BINARY); 
//
//	cvNamedWindow("1");
//    cvShowImage("1",binary_Img);
//
//	dst = cvCreateImage(cvGetSize(src_Img),8,3);
//	cvZero(dst);
//	CvMemStorage *storage = cvCreateMemStorage();
//	CvSeq *contour = NULL , *hull = NULL;
//	CvSeq *cont = NULL;
//
//    cvFindContours( binary_Img, storage, &contour);  //cnt = 5;
//	double area = 0;
//
//	for ( ;contour->h_next != NULL; contour = contour->h_next)
//	{
//		double area_t = cvContourArea(contour); 
//		if (area_t>area)
//		{
//			area = area_t;
//			cont = contour;
//		}
//	}
//
//
//	//h_next为下一个轮廓
//    //第四个轮廓为叶片轮廓
//    //contour = contour->h_next;
// //   contour = contour->h_next;
//	//contour = contour->h_next;
//
//	cvDrawContours(dst,cont,CV_RGB(255,0,0),CV_RGB(0,255,0),0,4);
//
//	//center(contour);
//		
//	//rect(contour);
//
//	//double long_axis = 0;   
//	//double short_axis = 0;
// //   CvPoint *contourPoint1, *contourPoint2;   
//	//CvPoint pt_l1, pt_l2; // 长轴两端点
//	//CvPoint pt_s1, pt_s2; // 短轴两端点
// //   for (int m = 0 ; m< contour->total;m++)   
// //   {   
// //       contourPoint1 = (CvPoint*)cvGetSeqElem(contour,m);  
//	//	for (int n = 0; n< contour->total; n++)
//	//	{
//	//		contourPoint2 = (CvPoint*)cvGetSeqElem(contour,n);
//	//		double long_temp = sqrt(pow(contourPoint1->x - contourPoint2->x,2)+ pow(contourPoint1->y - contourPoint2->y,2)); 
//	//		if (long_temp>long_axis)
//	//		{
//	//			long_axis = long_temp;
//	//			pt_l1 = *contourPoint1;
//	//			pt_l2 = *contourPoint2;
//	//		}
//	//	}
// //   }   
//	//double k_l = double(pt_l1.y - pt_l2.y) / double(pt_l1.x - pt_l2.x);
//	//double k_s;
//
//	//for (int m = 0 ; m< contour->total;m++)   
// //   {   
// //       contourPoint1 = (CvPoint*)cvGetSeqElem(contour,m);  
//	//	for (int n = 0; n< contour->total; n++)
//	//	{
//	//		contourPoint2 = (CvPoint*)cvGetSeqElem(contour,n);
//	//		if (contourPoint1->x == contourPoint2->x)
//	//			continue;
//	//		k_s = double(contourPoint1->y - contourPoint2->y) / double(contourPoint1->x - contourPoint2->x);
//	//		double x = abs(k_s*k_l+1);
//	//		if (x < 0.005)
//	//		{
//	//			double short_temp = sqrt(pow(contourPoint1->x - contourPoint2->x,2)+ pow(contourPoint1->y - contourPoint2->y,2)); 
//	//			if (short_temp>short_axis)
//	//			{
//	//				short_axis = short_temp;
//	//			    pt_s1 = *contourPoint1;
//	//			    pt_s2 = *contourPoint2;
//	//			}
//	//		}
//	//	}
// //   }   
//
// //   cvLine(dst,pt_l1,pt_l2,CV_RGB(30,200,100));
// //   cvLine(dst,pt_s1,pt_s2,CV_RGB(30,200,100));
//	//cout<<"long_axis = "<<long_axis<<endl;
//	//cout<<"short_axis = "<<short_axis<<endl;
//
//
//		
//		//hull = cvConvexHull2(contour,0,CV_CLOCKWISE,0);
//		//
//		//CvPoint pt0 = **(CvPoint**)cvGetSeqElem(hull,hull->total - 1);
//		//for(int i = 0;i<hull->total;++i)
//		//{
//		//	CvPoint pt1 = **(CvPoint**)cvGetSeqElem(hull,i);
//		//	cvLine(dst,pt0,pt1,CV_RGB(0,0,255));
//
//		//	pt0 = pt1;
//		//}
//
//
//		//double r_out = 0;//外切圆半径
//		//double r_in = 500;//内接圆半径
//		//CvPoint pt_l1, pt_l2; //长轴两端点
//		//double l1_min = 500, l2_min = 500;
//
//		//CvSeq *defect = cvConvexityDefects(contour,hull);
//		//
//		//for(int i = 0;i<defect->total;++i)
//		//{
//		//	CvConvexityDefect df = *(CvConvexityDefect*)cvGetSeqElem(defect,i);
//		//	cvCircle(dst,*df.start,2,CV_RGB(255,255,0),-1);
//		//	cvCircle(dst,*df.end,2,CV_RGB(255,255,0),-1);
//		//	cvCircle(dst,*df.depth_point,2,CV_RGB(0,255,255),-1);
//		//	
//		//	CvPoint s = *df.start;
//		//	CvPoint e = *df.end;
//		//	CvPoint d = *df.depth_point;
//
//		//	
//		//	double s_t = sqrt(pow((s.x-pt_c.x),2)+pow((s.y-pt_c.y),2));
//		//	double e_t = sqrt(pow((e.x-pt_c.x),2)+pow((e.y-pt_c.y),2));
//		//	double r_in_t = sqrt(pow((d.x-pt_c.x),2)+pow((d.y-pt_c.y),2));
//		//	double r_out_t = s_t>e_t?s_t:e_t;
//
//		//	r_out = r_out>r_out_t?r_out:r_out_t;
//		//	r_in = r_in<r_in_t?r_in:r_in_t;
//		//}
//
//		//cout<<"r_out = "<<r_out<<endl<<"r_in = "<<r_in<<endl;
//		//cvCircle(dst,pt_c,r_out,CV_RGB(255,255,0));
//		//cvCircle(dst,pt_c,r_in,CV_RGB(0,255,255));
//
//		//cvLine(dst,pt_l1,pt_l2,CV_RGB(30,200,100));
//
//
//
//
//   	    sprintf(path_r,"%d_dst",n);	
//		cvShowImage(path_r,dst);
//		cvWaitKey();
//
//
//}
//
//
//
//}

#define N 5 //叶片样本数  必须要大于降维后的维数！！
//double features[N][9];


void printMat(CvMat* matric)
{
  int row = matric->rows,col = matric->cols,i,j;
  for(i = 0;i < row;i++)
  {
    for(j = 0;j < col;j++)
    {
      double num = cvGet2D(matric,i,j).val[0];
      cout << num << " ";
    }
    cout << endl;
  }
}
int main()
{
    int row = N ,col = 9 ,i;
    double data[N*9];
    ifstream fin("features.txt");//打开文件
    //读入数字
    for(i=0;i<col*row;i++)
			fin>>data[i];
    fin.close();
  //CvMat* inputs = &cvMat( row, col, CV_64FC1, data );
  //printf("-------输入原矩阵的行和列-----------\n");  
  //int row,col,i,j;  
  //cin >> row >> col;  
  //float* data = new float [row*col];  
  //printf("-------输入原矩阵的元素-------------\n");  
  //for(i = 0;i < col;i++)  
  //{  
  //  for(j = 0;j < row;j++)cin >> data[j * col + i];   
  //}  

	//中心化 每列的和为0
	double data_c[N*9],mean[9] = {0,0,0,0,0,0,0,0,0};

	for (int j = 0; j<9; j++)
		for (int i = 0; i<N; i++)
			mean[j] += data[i*9+j];

	for (int i = 0; i<N; i++)
		for (int j = 0; j<9; j++)
			data_c[i*9+j] = data[i*9+j]-mean[j]/N;

  CvMat* inputs = cvCreateMat(N,9,CV_64FC1);  
  cvSetData(inputs,data_c,inputs->step);  

  printf("------输入的矩阵为------------------\n");
  printMat(inputs);

  
  CvMat *inputs_c;  
  if (9>N)
  {
	  row = 9; col = N;
	  inputs_c = cvCreateMat(9,N,CV_64FC1);
      cvTranspose(inputs, inputs_c);      //转置  
  }
  else  
      inputs_c = cvCloneMat(inputs);

  printf("------实际运算的的矩阵为------------------\n");
  printMat(inputs_c);






  CvMat* means = cvCreateMat(1,col,CV_64FC1);//均值
  CvMat* EigenValue = cvCreateMat(col,1,CV_64FC1);//特征值
  CvMat* EigenVector = cvCreateMat(col,col,CV_64FC1);//特征向量
  CvMat* EigenVector_before;
  if (9>N)
  {
      EigenVector_before = cvCreateMat(9,col,CV_64FC1);//转置前协方差矩阵的特征向量
  }


  cvCalcPCA(inputs_c,means,EigenValue,EigenVector,CV_PCA_DATA_AS_ROW);//以行作为特征维数计算PCA的参数


  printf("-------------均值为-----------------\n");
  printMat(means);
  printf("-------------特征值为---------------\n");
  printMat(EigenValue);
  printf("-------------特征向量为-------------\n");
  printMat(EigenVector);

  if (9>N)
  {
	  cvmMul(inputs_c,EigenVector,EigenVector_before);
  }

  //printf("-----输入PCA的降维系数，按特征值总和的比例确定PCA的维度，大小在0~1之间------\n");
  //double rate;
  //cin >> rate;
  //double EigenValueSum = cvSum(EigenValue).val[0],curSum = 0.0;
  //for(i = 0;i < col;i++)
  //{
  //  curSum += cvGet2D(EigenValue,i,0).val[0];//cvCalcPCA求出的特征值和特征向量都是排好序的，可以直接使用
  //  if(curSum > EigenValueSum * rate)
		//break;
  //}



  int postDim = 4;
  cout << "----从原来的 " << col <<" 维降到 " << postDim << " 维"<<endl;
  CvMat* postEigenVector = cvCreateMat(9,postDim,CV_64FC1);//取前k个特征向量组成的投影矩阵
  if (9>N)
	  cvGetSubRect(EigenVector_before,postEigenVector,cvRect(0,0,postDim,9));
  else
	  cvGetSubRect(EigenVector,postEigenVector,cvRect(0,0,postDim,9));

  printf("------------投影矩阵为------------\n");
  printMat(postEigenVector);


  //CvMat sub;
  //for(i = 0;i < row;i++)
  //{
  //  cvGetSubRect(inputs,&sub,cvRect(0,i,col,1));
  //  cvSub(&sub,means,&sub);
  //}
  //printf("-----------原矩阵单位化后为----------\n");
  //printMat(inputs);




  CvMat* output = cvCreateMat(N,postDim,CV_64FC1);
//  cvmMul(inputs,postEigenVector,output);//cvMul 是多维向量的点乘,cvmMul 是矩阵乘
  cvMatMulAdd( inputs, postEigenVector, 0, output);

  printf("-----------PCA处理后的结果为---------\n");
  printMat(output);





  //delete[] data;
  //cvReleaseMat(&inputs);
  //cvReleaseMat(&means);
  //cvReleaseMat(&EigenValue);
  //cvReleaseMat(&EigenVector);
  //cvReleaseMat(&postEigenVector);
  //cvReleaseMat(&output);
   system("pause");
  return 0;
}