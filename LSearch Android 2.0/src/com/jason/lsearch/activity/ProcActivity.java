package com.jason.lsearch.activity;

import java.util.ArrayList;
import java.util.List;

import com.jason.lsearch.fragment.PhotoFragment;
import com.jason.lsearch.po.Species;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.Fragment;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.Window;

public class ProcActivity extends Activity {
	private FragmentManager FM;
	private FragmentTransaction FT;
	private PhotoFragment photo;
	
	public List<Species> lstSpecies;
	public List<Bitmap> lstBitmap;
	public String path;
	@SuppressLint("NewApi")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setTitle("ä¯ÀÀÎÄ¼þ");

		Bundle bundle = this.getIntent().getExtras();
		path = bundle.getString("path");
		
		lstBitmap=new ArrayList<Bitmap>();

		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.activity_proc);
		photo = new PhotoFragment();
		FM = getFragmentManager();
		FT = FM.beginTransaction();
		FT.add(R.id.mainfragment, photo);
		FT.commit();
	}

	@SuppressLint("NewApi")
	public void setNewFragment(Fragment fragment) {
		FT = FM.beginTransaction();
		FT.replace(R.id.mainfragment, fragment);
		FT.commit();
	}
}
