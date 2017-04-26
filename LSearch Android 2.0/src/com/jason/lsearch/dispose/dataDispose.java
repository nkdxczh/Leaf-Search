package com.jason.lsearch.dispose;

public class dataDispose {
	private static float Gaussian(float average,float deviation,float aim){
		double tem=java.lang.Math.sqrt(2*java.lang.Math.PI)*deviation;
		double tem1=java.lang.Math.exp(-((aim-average)*(aim-average))/(2*deviation*deviation));
		return (float) (tem1/tem);
	}
	
	static public float probability(float[] sampleAvg,float[] sampleDev,float[] detect,int n){
		float probability=1;
		for(int i=0;i<n;i++){
			probability*=Gaussian(sampleAvg[i],sampleDev[i],detect[i]);
		}
		return probability;
		
	}
}
