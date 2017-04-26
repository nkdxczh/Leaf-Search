package com.jason.lsearch.activity;

import android.support.v4.app.Fragment;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;

public class MainActivity extends Activity implements OnClickListener {
	
	Button takePhotoBtn;	
	Button selectPhotoBtn;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		this.takePhotoBtn = (Button) findViewById(R.id.btn_main_takePhoto);
		this.takePhotoBtn.setOnClickListener(this);

		this.selectPhotoBtn = (Button) findViewById(R.id.btn_main_selectPhoto);
		this.selectPhotoBtn.setOnClickListener(this);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	/**
	 * A placeholder fragment containing a simple view.
	 */
	public static class PlaceholderFragment extends Fragment {

		public PlaceholderFragment() {
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container,
				Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_main, container,
					false);
			return rootView;
		}
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		if(v.getId()==R.id.btn_main_takePhoto){
			Intent intent = new Intent();
			intent.setClass(MainActivity.this,
					CameraActivity.class);
			startActivity(intent);
		}
		if(v.getId()==R.id.btn_main_selectPhoto){
			Intent intent = new Intent();
			intent.setClass(MainActivity.this,
					FileViewerActivity.class);
			startActivity(intent);
		}
		
	}

}
