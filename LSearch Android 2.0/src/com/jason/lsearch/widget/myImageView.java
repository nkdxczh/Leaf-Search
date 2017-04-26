package com.jason.lsearch.widget;

import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

import com.jason.lsearch.dispose.feature;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Rect;
import android.os.Handler;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.widget.ImageView;

public class myImageView extends ImageView {
	
	double pi=3.1415927;
	
	private Vector<Point> inP;
	private Vector<Point> exP;
	private boolean isin;
	private boolean hasDetect;

	private Mat image;
	private Mat mask;
	private org.opencv.core.Rect area;
	private Mat fgdModel;
	private Mat bgdModel;
	private List<MatOfPoint> contours;

	private Paint paint;// 声明画笔
	private Bitmap bitmap;// 位图
	private Bitmap result;
	private Rect srcRect;
	private Rect dstRect;
	
	private static Handler handler=new Handler();

	public myImageView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub

		paint = new Paint(Paint.DITHER_FLAG);// 创建一个画笔

		paint.setStyle(Style.STROKE);// 设置非填充
		paint.setStrokeWidth(10);// 笔宽5像素
		paint.setColor(Color.GREEN);// 设置为红笔
		paint.setAntiAlias(true);// 锯齿不显示
		
		srcRect=new Rect();
		dstRect=new Rect();
		
		isin=true;
		hasDetect=false;
		inP=new Vector<Point>();
		exP=new Vector<Point>();

		fgdModel = new Mat(1,13*5,CvType.CV_64FC1,new Scalar(0));
		bgdModel = new Mat(1,13*5,CvType.CV_64FC1,new Scalar(0));
		
		contours = new ArrayList<MatOfPoint>();
	}
	
	
	
	public myImageView(Context context, AttributeSet attrs){
		super(context,attrs);

		paint = new Paint(Paint.DITHER_FLAG);// 创建一个画笔

		paint.setStyle(Style.STROKE);// 设置非填充
		paint.setStrokeWidth(10);// 笔宽5像素
		paint.setColor(Color.GREEN);// 设置为红笔
		paint.setAntiAlias(true);// 锯齿不显示
		
		srcRect=new Rect();
		dstRect=new Rect();
		
		isin=true;
		hasDetect=false;
		inP=new Vector<Point>();
		exP=new Vector<Point>();

		fgdModel = new Mat(1,13*5,CvType.CV_64FC1,new Scalar(0));
		bgdModel = new Mat(1,13*5,CvType.CV_64FC1,new Scalar(0));
		
		contours = new ArrayList<MatOfPoint>();
		
	}
	
	public myImageView(Context context, AttributeSet attrs, int defStyle){
		super(context, attrs, defStyle);

		paint = new Paint(Paint.DITHER_FLAG);// 创建一个画笔

		paint.setStyle(Style.STROKE);// 设置非填充
		paint.setStrokeWidth(10);// 笔宽5像素
		paint.setColor(Color.GREEN);// 设置为红笔
		paint.setAntiAlias(true);// 锯齿不显示
		
		srcRect=new Rect();
		dstRect=new Rect();
		
		isin=true;
		hasDetect=false;
		inP=new Vector<Point>();
		exP=new Vector<Point>();

		fgdModel = new Mat(1,13*5,CvType.CV_64FC1,new Scalar(0));
		bgdModel = new Mat(1,13*5,CvType.CV_64FC1,new Scalar(0));
		
		contours = new ArrayList<MatOfPoint>();
	}

	public void setPhoto(Bitmap bitmap) {
		image=new Mat();
		Utils.bitmapToMat(bitmap, image);
		Imgproc.cvtColor(image, image, Imgproc.COLOR_RGBA2RGB);
		System.out.println(image.width());
		System.out.println(image.height());
		int width=image.width();
		int height=image.height();
		float w=(float)width/(float)(width+height);
		float h=(float)height/(float)(width+height);
		Imgproc.resize(image, image, new Size((int)(400*w),(int)(400*h)));
		mask = new Mat(image.rows(),image.cols(),CvType.CV_8UC1,new Scalar(Imgproc.GC_PR_BGD));
		area = new org.opencv.core.Rect(50, 50, mask.cols()-100,mask.rows()-100);
		this.bitmap=bitmap;
		System.out.println(image.rows());
		System.out.println(image.cols());
		//Utils.matToBitmap(image, this.bitmap);
		//mask = new Mat();
	}
	
	public void include(){
		isin=true;
	}
	
	public void exclude(){
		isin=false;
	}

	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub

		srcRect.set(0, 0, this.bitmap.getWidth(), this.bitmap.getHeight());
		dstRect.set(0, 0, this.getWidth(), this.getHeight());
		if(hasDetect){
			canvas.drawBitmap(bitmap, srcRect, dstRect, paint);
			paint.setAlpha(80);
			canvas.drawBitmap(result, srcRect, dstRect, paint);
			paint.setAlpha(100);
		}
		else canvas.drawBitmap(bitmap, srcRect, dstRect, paint);
		paint.setColor(Color.RED);
		for(int i=0;i<inP.size();i++){
			canvas.drawPoint((float)inP.get(i).x, (float)inP.get(i).y, paint);
		}
		paint.setColor(Color.BLUE);
		for(int i=0;i<exP.size();i++){
			canvas.drawPoint((float)exP.get(i).x, (float)exP.get(i).y, paint);
		}
		
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		
        switch(event.getAction()){  
        case MotionEvent.ACTION_MOVE:
        //case MotionEvent.ACTION_DOWN:  
       // case MotionEvent.ACTION_UP:
    		Point temP=new Point(event.getX(), event.getY());
    		int row=(int)((event.getY()/this.getHeight())*image.rows());
    		int col=(int)((event.getX()/this.getWidth())*image.cols());
    		System.out.println((int)event.getX()+","+(int)event.getY());
    		System.out.println(row+","+col);
    		System.out.println(mask.rows()+","+mask.cols());
    		byte b[] = new byte[1];
    		mask.get(row, col,b);
    		System.out.println((int)b[0]+"   fffff");
    		int location;
    		if (isin) {
        		byte[] data={Imgproc.GC_FGD};
    			mask.put(row,col,data);
    			if((location=exP.indexOf(temP))!=-1){
    				exP.remove(location);
    			}
    			inP.add(new Point(event.getX(), event.getY()));
    			invalidate();
    		}
    		else{
        		byte[] data={Imgproc.GC_BGD};
    			mask.put(row,col,data);
    			if((location=inP.indexOf(temP))!=-1){
    				inP.remove(location);
    			}
    			exP.add(new Point(event.getX(), event.getY()));
    			invalidate();
    		}
    		mask.get(row, col,b);
    		System.out.println((int)b[0]+"   fffffff");
        }
		return true;
	}
	
	public boolean detectOutline(){
		try{
			new Thread(new Runnable() {      
				@Override
				public void run() {
					handler.post(new Runnable() {  
						@Override
						public void run() {
							}
						});            
					}
				}).start();
		}
		catch(Exception e){
			return false;
		}
	//	Core.convertScaleAbs(mask, mask, 100, 0);
	//	Imgproc.cvtColor(mask, mask, Imgproc.COLOR_GRAY2RGBA);
		Imgproc.grabCut(image, mask, area, bgdModel, fgdModel, 1,Imgproc.GC_EVAL);
		for(int i=0;i<mask.rows();i++){
			for(int j=0;j<mask.cols();j++){
				byte data[]=new byte[1];
				mask.get(i, j, data);
				if(data[0]==Imgproc.GC_PR_FGD||data[0]==Imgproc.GC_FGD){
					mask.put(i, j, new byte[]{(byte) 255});
				}
			}
		}
		hasDetect=true;
		result = Bitmap.createBitmap(mask.cols(), mask.rows(),Bitmap.Config.ARGB_8888);
		Utils.matToBitmap(mask, result);
		
		Mat temMat =mask;
		for(int i=0;i<temMat.rows();i++){
			for(int j=0;j<temMat.cols();j++){
				byte data[]=new byte[1];
				temMat.get(i, j, data);
				if(data[0]==(byte) 255){
					temMat.put(i, j, new byte[]{1});
				}
				else temMat.put(i, j, new byte[]{0});
			}
		}
		
		contours.clear();
		Mat hierarchy = new Mat(mask.rows(), mask.cols(), CvType.CV_8UC1, new Scalar(0));
		Mat result1 = new Mat(mask.rows(), mask.cols(), CvType.CV_8UC1, new Scalar(0));
		Imgproc.findContours(temMat, contours, hierarchy, Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_NONE);
		
		int maxS=0;
		int maxC=0;
		for(int i=0;i<contours.size();i++){
			System.out.println("size:"+contours.get(i).rows());
			if(contours.get(i).cols()>maxS){
				maxS=contours.get(i).cols();
				maxC=i;
			}
		}
		MatOfPoint teml=contours.get(maxC);
		contours.clear();
		contours.add(teml);
		Imgproc.drawContours(result1, contours, -1, new Scalar(255));
		result = Bitmap.createBitmap(mask.cols(), mask.rows(),Bitmap.Config.ARGB_8888);
		Utils.matToBitmap(result1, result);
		
		invalidate();
		
		mask = new Mat(image.rows(),image.cols(),CvType.CV_8UC1,new Scalar(Imgproc.GC_PR_BGD));
		for(int i=0;i<inP.size();i++){
    		int row=(int)((inP.get(i).y/this.getHeight())*image.rows());
    		int col=(int)((inP.get(i).x/this.getWidth())*image.cols());
    		byte[] data={Imgproc.GC_FGD};
			mask.put(row,col,data);
		}
		for(int i=0;i<exP.size();i++){
    		int row=(int)((exP.get(i).y/this.getHeight())*image.rows());
    		int col=(int)((exP.get(i).x/this.getWidth())*image.cols());
    		byte[] data={Imgproc.GC_BGD};
			mask.put(row,col,data);
		}
		return true;
	}
	
	public void resume(){
		inP.clear();
		exP.clear();
		mask = new Mat(image.rows(),image.cols(),CvType.CV_8UC1,new Scalar(3));
		hasDetect=false;
		invalidate();
	}
	
	public float[] getData(){
		float data[]=new float[10];
		feature f=new feature();
		data=f.getFeature(contours.get(0));
		
		
		
		//for(int i=0;i<9;i++)System.out.println("data"+i+":"+data[i]);
		/*
		Point right = new Point();
		right.x = 0;
		Point left = new Point();
		left.x = mask.cols();
		Point top = new Point();
		top.y = mask.rows();
		Point bottom = new Point();
		bottom.y = 0;
		MatOfPoint pmat = contours.get(0);
		//System.out.println(pmat.cols());
		//System.out.println(pmat.rows());
		for (int i = 0; i < pmat.rows(); i++){
			double[] present=pmat.get(i, 0);
			if (present[0]>right.x){
				right.x = present[0];
				right.y = present[1];
			}
			if (present[0]<left.x){
				left.x = present[0];
				left.y = present[1];
			}
			if (present[1]<top.y){
				top.x = present[0];
				top.y = present[1];
			}
			if (present[1]>bottom.y){
				bottom.x = present[0];
				bottom.y = present[1];
			}
		}
		
		System.out.println(right.x+","+right.y);
		System.out.println(left.x+","+left.y);
		System.out.println(top.x+","+top.y);
		System.out.println(bottom.x+","+bottom.y);
		
		double area=Imgproc.contourArea(contours.get(0));
		data[0]=(float) (java.lang.Math.pow(contours.get(0).rows(),2) / (4 * pi*area));
		data[1]=(float) ((bottom.y-top.y)/(right.x-left.x));*/
		
		return data;
	}

}
