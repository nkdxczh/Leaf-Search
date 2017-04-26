package com.jason.lsearch.dispose;

import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.ml.CvSVM;

public class mySVM {
	
	CvSVM svm;
	
	public mySVM(String path){
		svm=new CvSVM();
		svm.load(path);
	}
	
	public int predict(float[] data,int n){
		Mat predictMat=new Mat(1,n,CvType.CV_32FC1);
		for(int i=0;i<n;i++)predictMat.put(0, i, new float[]{data[i]});
		return (int) svm.predict(predictMat);
	}
}
