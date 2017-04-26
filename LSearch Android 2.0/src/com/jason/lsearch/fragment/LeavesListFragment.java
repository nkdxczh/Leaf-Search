package com.jason.lsearch.fragment;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.opencv.core.CvType;
import org.opencv.core.Mat;

import com.jason.lsearch.activity.ProcActivity;
import com.jason.lsearch.activity.R;
import com.jason.lsearch.dispose.DBAdapter;
import com.jason.lsearch.po.Species;

import android.annotation.SuppressLint;
import android.app.Fragment;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

@SuppressLint("NewApi")
public class LeavesListFragment extends Fragment implements OnItemClickListener {

	private ListView itemlist;

	List<Map<String, Object>> list;
	//private mySVM svm;

	//private File mCascadeFile;
	//private File mCascadeFile1;
	private View view;
	private List<View> views;

	DBAdapter dp;

	private int featureNum = 6;
	private float data[] = new float[9];
	private Mat dataMat = new Mat(1,9,CvType.CV_32FC1);
	private Mat featureMat = new Mat(1,3,CvType.CV_32FC1);

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		view = inflater
				.inflate(R.layout.fragment_leaveslist, container, false);

		itemlist = (ListView) view.findViewById(R.id.list_leaveslist_leaves);

		this.getActivity().setTitle("识别结果");
		
		List<Species> lstSpecies=((ProcActivity)this.getActivity()).lstSpecies;
		List<Bitmap> lstBitmap=((ProcActivity)this.getActivity()).lstBitmap;
		
		List<View> views=new ArrayList<View>();
		for(int i=0;i<lstSpecies.size();i++){
			View view=inflater.inflate(R.layout.item_leaf, container, false);
			TextView t;
			t=(TextView) view.findViewById(R.id.txt_leaf_name);
			t.setText(lstSpecies.get(i).getName());
			t=(TextView) view.findViewById(R.id.txt_leaf_desc);
			t.setText(lstSpecies.get(i).getDescription());
			ImageView image=(ImageView) view.findViewById(R.id.img_leaf_leaf);
			image.setImageBitmap(lstBitmap.get(i));
			views.add(view);
		}
		System.out.println("dddddddd"+views.size());
		itemlist.setAdapter(new leafitem(this.getActivity(),lstSpecies, lstBitmap));
		System.out.println("weeee");
		//itemlist.setOnItemClickListener(this);
		//itemlist.setSelection(0);

		//refreshListItems(((ProcActivity)this.getActivity()).lstSpecies,((ProcActivity)this.getActivity()).lstBitmap);

		return view;
		/*
		Bundle bundle = this.getIntent().getExtras();
		data[0] = (float) bundle.getDouble("rectangularity");
		data[1] = (float) bundle.getDouble("complexity");
		data[2] = (float) bundle.getDouble("circularity");
		data[3] = (float) bundle.getDouble("sphericity");
		data[4] = (float) bundle.getDouble("aspect_radio");
		data[5] = (float) bundle.getDouble("a_convexity");
		data[6] = (float) bundle.getDouble("p_convexity");
		data[7] = (float) bundle.getDouble("eccentricity");
		data[8] = (float) bundle.getDouble("lobation");
		

		try {
			// load cascade file from application resources
			InputStream is = getResources().openRawResource(R.raw.svmdataset);
			File cascadeDir = getDir("cascade", Context.MODE_PRIVATE);
			mCascadeFile = new File(cascadeDir, "svmdataset.xml");
			FileOutputStream os = new FileOutputStream(mCascadeFile);

			byte[] buffer = new byte[4096];
			int bytesRead;
			while ((bytesRead = is.read(buffer)) != -1) {
				os.write(buffer, 0, bytesRead);
			}
			is.close();
			os.close();

			cascadeDir.delete();

		} catch (IOException e) {
			e.printStackTrace();
		}

		 svm=new mySVM(mCascadeFile.getAbsolutePath());

		Mat eigenvectors = new Mat(9,3,CvType.CV_32FC1);
		InputStream myFile = null;
		myFile = getResources().openRawResource(R.raw.eigenvectors);// cet4为一个TXT文件
		BufferedReader br = null;
		try {
			br = new BufferedReader(new InputStreamReader(myFile, "gbk"));// 注意编码
			String tmp;
			int i=0;
			while ((tmp = br.readLine()) != null) {
				System.out.println(tmp);
				String[] data=tmp.split(" ");
				for(int j=0;j<featureNum/2;j++)eigenvectors.put(i, j, new float[]{Float.parseFloat(data[j])});
				i++;
			}
		} catch (Exception e1) {
			// TODO Auto-generated catch block

		}
		
		myFile = null;
		myFile = getResources().openRawResource(R.raw.allaveravg);// cet4为一个TXT文件
		br = null;
		try {
			br = new BufferedReader(new InputStreamReader(myFile, "gbk"));// 注意编码
			String tmp;
			tmp = br.readLine();
			String[] avg=tmp.split(" ");
			for(int i=0;i<9;i++)
				dataMat.put(0,i,new float[]{data[i]-Float.parseFloat(avg[i])});
		} catch (Exception e1) {
			// TODO Auto-generated catch block
			System.out.println("error!");
		}
		
		myFile = null;
		myFile = getResources().openRawResource(R.raw.name);// cet4为一个TXT文件
		br = null;
		ArrayList<String> name=new ArrayList();
		try {
			br = new BufferedReader(new InputStreamReader(myFile, "gbk"));// 注意编码
			String tmp;
			while ((tmp = br.readLine()) != null) {
				name.add(tmp);
			}
		} catch (Exception e1) {
			// TODO Auto-generated catch block
			System.out.println("error!");
		}


		dp = new DBAdapter(this);
		dp.deleteTable();
		dp.open();

		myFile = null;
		myFile = getResources().openRawResource(R.raw.generateresult);// cet4为一个TXT文件
		br = null;
		try {
			br = new BufferedReader(new InputStreamReader(myFile, "gbk"));// 注意编码
			String tmp;
			while ((tmp = br.readLine()) != null) {
				String[] data=tmp.split(" ");
				leaf teml = new leaf();
				int id=Integer.parseInt(data[0]);
				if(id>=96)id-=5;
				teml.setName(name.get(id-1));
				teml.setClassification(Integer.parseInt(data[1]));
				teml.setDesc("杨树（拉丁语学名：Populus），包含了胡杨、白杨、棉白杨等，通称“杨树”。");
				float[] feature = new float[featureNum];
				for (int j = 0; j <featureNum; j++){
					feature[j] = Float.parseFloat(data[2+j]);
				}
				teml.setFeature(feature);
				//System.out.println("llllllll");
				dp.insert(teml);
				//System.out.println("ppppppp");
			}
		} catch (Exception e1) {
			// TODO Auto-generated catch block
			System.out.println("error!");
		}
		
		for(int i=0;i<3;i++){
			float tem=0;
			for(int ii=0;ii<9;ii++){
				float d[]=new float[1];
				dataMat.get(0,ii,d);

				float e[]=new float[1];
				eigenvectors.get(ii,i,e);
				
				tem+=d[0]*e[0];
			}
			featureMat.put(0,i,new float[]{tem});
		}
		
		 int kind=svm.predict(data,featureNum/2);
		 
		 ArrayList<leaf> leaves=dp.getAll();
		
		System.out.println(leaves.size());

		if (leaves.size() != 0) {
			refreshListItems(leaves);
		}

		dp.deleteTable();
		*/
		
	}

/*	private void refreshListItems(List<Species> Species,List<Bitmap> bitmap) {
		list = buildListForSimpleAdapter(Species,bitmap);
		SimpleAdapter notes = new SimpleAdapter(this.getActivity(), list, R.layout.item_leaf,
				new String[] { "name", "desc", "img" },
				new int[] { R.id.txt_leaf_name, R.id.txt_leaf_desc,
						R.id.img_leaf_leaf });
		System.out.println(itemlist);
		System.out.println(notes);
		itemlist.setAdapter(notes);
		itemlist.setOnItemClickListener(this);
		itemlist.setSelection(0);
	}

	private List<Map<String, Object>> buildListForSimpleAdapter(
			List<Species> species,List<Bitmap> bitmap) {
		
		List<Map<String, Object>> list = new ArrayList<Map<String, Object>>(
				species.size());
		
		for (int i = 0; i < species.size(); i++) {

			Map<String, Object> map = new HashMap<String, Object>();
			map.put("name", species.get(i).getName());
			map.put("desc", species.get(i).getDescription());
			map.put("img",bitmap.get(i));
			list.add(map);
		}
		
		return list;
		/*
		float temdata[]=new float[featureNum/2];
		for(int i=0;i<featureNum/2;i++){
			float tem[]=new float[1];
			featureMat.get(0, i, tem);
			temdata[i]=tem[0];
		}
		
		ArrayList<Float> pro=new ArrayList<Float>();
		for (int i = 0; i < species.size(); i++){
			float[] feature = new float[featureNum];
			float[] mean = new float[featureNum / 2];
			float[] dev = new float[featureNum / 2];
			feature = species.get(i).getFeature();
			for (int j = 0; j < featureNum / 2; j++) {
				mean[j] = feature[2 * j];
				dev[j] = feature[2 * j + 1];
			}
			pro.add(dataDispose.probability(mean, dev, temdata,(int) featureNum / 2));
		}
		
		List<Map<String, Object>> list = new ArrayList<Map<String, Object>>(
				species.size());
		
		System.out.println("size:"+pro.size());
		
		float max=0;
		int[] index=new int[10];
		boolean no=false;
		for(int i=0;i<10;i++){
			for(int j=0;j<pro.size();j++){
				if(pro.get(j)>max){
					for(int z=0;z<i;z++){
						if(j==index[z]){
							no=true;
							break;
						}
					}
					if(!no){
						index[i]=j;
						max=pro.get(j);
					}
					no=false;
				}
			}
			max=0;
		}
		
		int base=R.raw.o1;
		for (int i = 0; i < 10; i++) {
			System.out.println("index:"+index[i]);

			Map<String, Object> map = new HashMap<String, Object>();
			map.put("name", species.get(index[i]).getName());
			map.put("desc", species.get(index[i]).getDescription());
			map.put("probability",
					"可能概率："+ species.get(index[i]).getPossibility());
			
			//map.put("img",R.raw.o1);
			list.add(map);
		}
		

		for (int i = 0; i < leaves.size(); i++) {
			float[] feature = new float[featureNum];
			float[] mean = new float[featureNum / 2];
			float[] dev = new float[featureNum / 2];
			feature = leaves.get(i).getFeature();
			for (int j = 0; j < featureNum / 2; j++) {
				mean[j] = feature[2 * j];
				dev[j] = feature[2 * j + 1];
			}

			Map<String, Object> map = new HashMap<String, Object>();
			map.put("name", leaves.get(i).getName());
			map.put("desc", leaves.get(i).getDesc());
			map.put("probability",
					"可能概率："
							+ dataDispose.probability(mean, dev, temdata,
									(int) featureNum / 2));
			list.add(map);
		}
	}*/

	@Override
	public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
		// TODO Auto-generated method stub

	}
	
	class leafitem extends BaseAdapter{
		
		private List<View> views;
		private List<Species> lstSpecies;
		private List<Bitmap> lstBitmap;
		private Context  context;
		
		public leafitem(Context  context,List<Species> lSpecies,List<Bitmap> lstBitmap){
			super();
			this.lstSpecies=lSpecies;
			this.lstBitmap=lstBitmap;
			this.context=context;
		}

		@Override
		public int getCount() {
			// TODO Auto-generated method stub
			return lstSpecies.size();
		}

		@Override
		public Object getItem(int arg0) {
			// TODO Auto-generated method stub
			return lstSpecies.get(arg0);
		}

		@Override
		public long getItemId(int arg0) {
			// TODO Auto-generated method stub
			return arg0;
		}

		@Override
		public View getView(int arg0, View view, ViewGroup arg2) {
			// TODO Auto-generated method stub
			ViewHolder holder;
			if(view==null){
				view=LayoutInflater.from(context).inflate(R.layout.item_leaf, null);
				holder=new ViewHolder();
				view.setTag(holder);
				holder.name=(TextView)view.findViewById(R.id.txt_leaf_name);
				holder.des=(TextView)view.findViewById(R.id.txt_leaf_desc);
				holder.image=(ImageView)view.findViewById(R.id.img_leaf_leaf);
			}
			else{
				holder=(ViewHolder)view.getTag();
			}
			holder.name.setText(lstSpecies.get(arg0).getName());
			holder.des.setText(lstSpecies.get(arg0).getDescription());
			holder.image.setImageBitmap(lstBitmap.get(arg0));;
			return view;
		}
		
	}
	static class ViewHolder{
		TextView name;
		TextView des;
		ImageView image;
	}
}
