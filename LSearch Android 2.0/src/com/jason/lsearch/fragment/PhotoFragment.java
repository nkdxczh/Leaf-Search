package com.jason.lsearch.fragment;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.List;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;
import org.jason.lsearch.internet.Visitor;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.reflect.TypeToken;
import com.jason.lsearch.activity.ProcActivity;
import com.jason.lsearch.activity.R;
import com.jason.lsearch.po.Feature;
import com.jason.lsearch.po.Species;

import android.annotation.SuppressLint;
import android.app.Fragment;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.Button;

@SuppressLint("NewApi")
public class PhotoFragment extends Fragment implements OnClickListener {

	private Button includeBtn;
	private Button excludeBtn;
	private Button okBtn;
	private Button detectBtn;
	private Button resumeBtn;
	private com.jason.lsearch.widget.myImageView myImageView;

	private View view;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		this.getActivity().setTitle("ҶƬѡȡ");

		view = inflater.inflate(R.layout.fragment_photo, container, false);

		FileInputStream fis = null;
		try {
			fis = new FileInputStream(((ProcActivity) this.getActivity()).path);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Bitmap bitmap = BitmapFactory.decodeStream(fis);

		includeBtn = (Button) view.findViewById(R.id.btn_photo_include);
		includeBtn.setOnClickListener(this);

		excludeBtn = (Button) view.findViewById(R.id.btn_photo_exclude);
		excludeBtn.setOnClickListener(this);

		okBtn = (Button) view.findViewById(R.id.btn_photo_ok);
		okBtn.setEnabled(false);
		okBtn.setOnClickListener(this);

		detectBtn = (Button) view.findViewById(R.id.btn_photo_detect);
		detectBtn.setOnClickListener(this);

		resumeBtn = (Button) view.findViewById(R.id.btn_photo_resume);
		resumeBtn.setOnClickListener(this);

		myImageView = (com.jason.lsearch.widget.myImageView) view
				.findViewById(R.id.img_photo_photo);
		myImageView.setPhoto(bitmap);
		myImageView.setImageBitmap(bitmap);

		return view;
	}

	@Override
	public void onClick(View view) {
		// TODO Auto-generated method stub
		switch (view.getId()) {
		case R.id.btn_photo_include:
			myImageView.include();
			break;
		case R.id.btn_photo_exclude:
			myImageView.exclude();
			break;
		case R.id.btn_photo_detect:
			okBtn.setEnabled(true);
			myImageView.detectOutline();
			break;
		case R.id.btn_photo_resume:
			myImageView.resume();
			break;
		case R.id.btn_photo_ok:
			float[] data = myImageView.getData();

			Feature feature = new Feature();
			
			feature.setCentroid(data[0]); feature.setComplexity(data[1]);
			feature.setCurvature(data[2]); feature.setGrayavg(data[3]);
			feature.setGraydev(data[4]); feature.setHu_3(data[5]);
			feature.setHueavg(data[6]); feature.setLpr(data[7]);
			feature.setSaturationavg(data[8]); feature.setTexture_0(data[9]);
			

			Gson gson = new GsonBuilder().setDateFormat("yyyy-MM-ddhh:mm:ss")
					.create();
			String feature_data = gson.toJson(feature);
			List<NameValuePair> lstNameValuePairs = new ArrayList<NameValuePair>();
			lstNameValuePairs.add(new BasicNameValuePair("feature",
					feature_data));

			Visitor visitor = new Visitor("AGetSpeciesServlet",
					lstNameValuePairs);
			visitor.start();
			try {
				visitor.join();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			String response = visitor.response;

			Type ListMessages = new TypeToken<ArrayList<Species>>() {
			}.getType();
			List<Species> lstSpecies = gson.fromJson(response, ListMessages);
			((ProcActivity) this.getActivity()).lstSpecies = lstSpecies;

			for (int i = 0; i < lstSpecies.size(); i++) {
				Visitor visitor1=new Visitor(lstSpecies
						.get(i).getSmall_image(), null);
				visitor1.flag=1;
				visitor1.start();
				try {
					visitor1.join();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				System.out.println("kkkkkk"+visitor1.bitmap.getAllocationByteCount());
				Bitmap bitmap = visitor1.bitmap;
				((ProcActivity) this.getActivity()).lstBitmap.add(bitmap);
			}
			
			System.out.println(((ProcActivity) this.getActivity()).lstBitmap.size());

			LeavesListFragment leavesListFragment = new LeavesListFragment();

			((ProcActivity) this.getActivity())
					.setNewFragment(leavesListFragment);

			break;
		}
	}

}
