package org.jason.lsearch.internet;

import java.util.List;
import org.apache.http.NameValuePair;
import android.annotation.SuppressLint;
import android.graphics.Bitmap;

public class Visitor extends Thread {
	String command;
	List<NameValuePair> lstNameValuePairs;
	public String response;
	public Bitmap bitmap;
	public int flag=0;
	
	public Visitor(String command,List<NameValuePair> lstNameValuePairs){
		this.command=command;
		this.lstNameValuePairs=lstNameValuePairs;
	}
	
	@SuppressLint("NewApi")
	@Override 
	public void run(){
		if(flag==1){ 
			bitmap=WebAccessUtils.getBitmapFromServer(command);
			System.out.println("mmmmmmm"+bitmap.getAllocationByteCount());
			return;
		}
		response = WebAccessUtils.httpRequest(command,
				lstNameValuePairs);
		System.out.println(response);
	}
}
