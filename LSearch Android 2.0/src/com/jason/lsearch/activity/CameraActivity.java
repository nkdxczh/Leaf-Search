package com.jason.lsearch.activity;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.Button;

public class CameraActivity extends Activity implements OnClickListener,
		CvCameraViewListener2 {
	private static final String TAG = "OCVSample::Activity";

	private Mat photo;

	private CameraBridgeViewBase mOpenCvCameraView;
	private Button toNextBtn;

	private final static String ALBUM_PATH = Environment
			.getExternalStorageDirectory() + "/lsearch/";

	private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
		@Override
		public void onManagerConnected(int status) {
			switch (status) {
			case LoaderCallbackInterface.SUCCESS: {
				Log.i(TAG, "OpenCV loaded successfully");
				mOpenCvCameraView.enableView();
			}
				break;
			default: {
				super.onManagerConnected(status);
			}
				break;
			}
		}
	};

	public CameraActivity() {
		Log.i(TAG, "Instantiated new " + this.getClass());
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.i(TAG, "called onCreate");
		super.onCreate(savedInstanceState);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		setContentView(R.layout.activity_camera);

		mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.view_camera_camera);
		mOpenCvCameraView.setVisibility(View.VISIBLE);
		mOpenCvCameraView.setCvCameraViewListener(this);

		toNextBtn = (Button) findViewById(R.id.btn_camera_takePhoto);
		toNextBtn.setOnClickListener(this);
	}

	/*
	 * @Override public boolean onCreateOptionsMenu(Menu menu) {
	 * 
	 * // Inflate the menu; this adds items to the action bar if it is present.
	 * getMenuInflater().inflate(R.menu.main, menu); return true; }
	 * 
	 * 
	 * @Override public boolean onOptionsItemSelected(MenuItem item) { // Handle
	 * action bar item clicks here. The action bar will // automatically handle
	 * clicks on the Home/Up button, so long // as you specify a parent activity
	 * in AndroidManifest.xml. int id = item.getItemId(); if (id ==
	 * R.id.action_settings) { return true; } return
	 * super.onOptionsItemSelected(item); }
	 */

	@Override
	public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
		// TODO Auto-generated method stub
		photo = inputFrame.rgba();
		return photo;
	}

	@Override
	public void onCameraViewStarted(int width, int height) {
		// TODO Auto-generated method stub
	}

	@Override
	public void onCameraViewStopped() {
		// TODO Auto-generated method stub
		photo.release();
	}

	@Override
	public void onPause() {
		super.onPause();
		if (mOpenCvCameraView != null)
			mOpenCvCameraView.disableView();
	}

	@Override
	public void onResume() {
		super.onResume();
		OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_3, this,
				mLoaderCallback);
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		mOpenCvCameraView.disableView();
	}

	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		if (photo != null) {
			// Imgproc.resize(photo, photo, new Size(photo.cols()*2,
			// photo.rows()*2));
			Point center = new Point(photo.rows() / 2, photo.rows() / 2);
			Mat rotImage = Imgproc.getRotationMatrix2D(center, -90, 1.0);
			Imgproc.warpAffine(photo, photo, rotImage, new Size(photo.rows(),
					photo.cols()));
			File dirFile = new File(ALBUM_PATH);
			if (!dirFile.exists()) {
				dirFile.mkdir();
			}

			Bitmap bmp = Bitmap.createBitmap(photo.width(), photo.height(),
					Config.RGB_565);
			Utils.matToBitmap(photo, bmp);

			String path = ALBUM_PATH + System.currentTimeMillis() + ".jpg";
			File myCaptureFile = new File(path);
			BufferedOutputStream bos;
			try {
				myCaptureFile.createNewFile();
				bos = new BufferedOutputStream(new FileOutputStream(
						myCaptureFile));
				bmp.compress(Bitmap.CompressFormat.JPEG, 80, bos);
				bos.flush();
				bos.close();
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			Intent intent = new Intent(CameraActivity.this, ProcActivity.class);

			Bundle bundle = new Bundle();
			bundle.putString("path", path);
			intent.putExtras(bundle);

			startActivity(intent);
		}
	}

}
