package com.jason.lsearch.activity;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;


import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

public class FileViewerActivity extends Activity implements OnItemClickListener {

    ListView itemlist = null;  
    String path = Environment.getExternalStorageDirectory()+"";  
    List< Map<String, Object>> list;  

	 private BaseLoaderCallback  mLoaderCallback = new BaseLoaderCallback(this) {
		    @Override
		    public void onManagerConnected(int status) {
		      switch (status) {
		        case LoaderCallbackInterface.SUCCESS:
		        {
		        } break;
		        default:
		        {
		          super.onManagerConnected(status);
		        } break;
		      }
		    }
	    };
    
    @Override  
    public void onCreate(Bundle savedInstanceState) {  
        super.onCreate(savedInstanceState);  
        
		OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_3, this, mLoaderCallback);
		
        setContentView(R.layout.activity_fileviewer);  
        setTitle("浏览文件");  
        itemlist = (ListView) findViewById(R.id.list_fileviewer_file);  
        refreshListItems(path);  
    }  
    
    /*根据path更新路径列表*/  
    private void refreshListItems(String path) {  
        //setTitle("文件浏览器 > "+path);  
        list = buildListForSimpleAdapter(path);  
        SimpleAdapter notes = new SimpleAdapter(this, list, R.layout.item_file,  
                new String[] { "name", "path" ,"img"}, new int[] { R.id.txt_file_name,  
                        R.id.txt_file_desc ,R.id.img_file_file});  
        itemlist.setAdapter(notes);  
        itemlist.setOnItemClickListener(this);  
        itemlist.setSelection(0);  
    }  
    
    /*根据路径生成一个包含路径的列表*/  
    private List<Map<String, Object>> buildListForSimpleAdapter(String path) {  
        File[] files = new File(path).listFiles();  
        List<Map<String, Object>> list = new ArrayList<Map<String, Object>>(files.length);  
        List<Map<String, Object>> jpglist = new ArrayList<Map<String, Object>>(files.length);  
        Map<String, Object> root = new HashMap<String, Object>();  
        root.put("name", "/");  
        root.put("img", R.drawable.etc);  
        root.put("path", "返回根目录");  
        list.add(root);  
        Map<String, Object> pmap = new HashMap<String, Object>();  
        pmap.put("name", "返回");  
        pmap.put("img", R.drawable.etc);  
        pmap.put("path", "返回父级目录");  
        list.add(pmap);  
        for (File file : files){  
            Map<String, Object> map = new HashMap<String, Object>();  
            map.put("name", file.getName());  
            map.put("path", file.getPath());  
            if(file.isDirectory()){  
                map.put("img", R.drawable.folder); 
                list.add(map);   
            }else{  
            	if(!file.getName().endsWith(".jpg"))continue;
                map.put("img", R.drawable.jpg);  
                jpglist.add(map);  
            }  
        }  
        for(int i=0;i<jpglist.size();i++){
        	list.add(jpglist.get(i));
        }
        return list;  
    }  
    
    /*跳转到上一层*/  
    private void goToParent() {  
        File file = new File(path);  
        File str_pa = file.getParentFile();  
        if(path.equals(Environment.getExternalStorageDirectory()+"")){  
            Toast.makeText(FileViewerActivity.this,  
                    "已经是根目录",  
                    Toast.LENGTH_SHORT).show();  
            refreshListItems(path);   
        }
        else{  
            path = str_pa.getAbsolutePath();  
            refreshListItems(path);   
        }  
    }  
    
    /*实现OnItemClickListener接口*/  
    @Override  
    public void onItemClick(AdapterView<?> parent, View v, int position, long id) {  
        if (position == 0) {  
            path = Environment.getExternalStorageDirectory()+"";  
            refreshListItems(path);  
        }else if(position == 1){  
            goToParent();  
        } else {  
            path = (String) list.get(position).get("path");  
            File file = new File(path);  
            if (file.isDirectory())  
                refreshListItems(path);  
            else  
            {  
    			Intent intent =new Intent(FileViewerActivity.this,ProcActivity.class);
    		    
    		    Bundle bundle=new Bundle();
    		    bundle.putString("path", this.path);
    		    intent.putExtras(bundle);
    		    
    		    startActivity(intent);  
            }  
              
        }  
  
    }  

}
