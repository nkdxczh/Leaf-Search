package com.jason.lsearch.dispose;

import java.util.ArrayList;

import com.jason.lsearch.po.leaf;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class DBAdapter {
	public static final String KEY_ROWID = "id";
	public static final String KEY_NAME = "name";
	public static final String KEY_CLASS = "classification";
	public static final String KEY_DESC = "desc";
	public static final String[] KEY_FEATURE=new String[18];
	private static final String DATABASE_NAME = "lsearch";
	private static final String DATABASE_TABLE = "leaves";
	private static final int DATABASE_VERSION = 1;
	private static final String DATABASE_CREATE = "CREATE TABLE IF NOT EXISTS leaves (id integer primary key autoincrement, "
			+ "name text not null, classification integer not null, "
			+ "desc text not null, "
			+ "feature1 float not null,feature2 float not null,"
			+ "feature3 float not null,feature4 float not null,"
			+ "feature5 float not null,feature6 float not null);";
	private final Context context;
	private DatabaseHelper DBHelper;
	private SQLiteDatabase db;
	
	
	public DBAdapter(Context ctx) {
		this.context = ctx;
		DBHelper = new DatabaseHelper(context);
		db = DBHelper.getWritableDatabase();
	}

	public DBAdapter open() throws SQLException {
		db.execSQL(DATABASE_CREATE);
		return this;
	}

	// ---¹Ø±ÕÊý¾Ý¿â---
	
	public void deleteTable() {
		db.execSQL("DROP TABLE IF EXISTS leaves");
	}

	public void close() {
		DBHelper.close();
	}

	public void insert(leaf leaves) {
		ContentValues initialValues = new ContentValues();
		initialValues.put(KEY_NAME, leaves.getName());
		initialValues.put(KEY_CLASS, leaves.getClassification());
		initialValues.put(KEY_DESC, leaves.getDesc());
		
		float[] feature=new float[6];
		feature=leaves.getFeature();
		for(int i=1;i<7;i++)initialValues.put("feature"+i,feature[i-1]);
		
		db.insert(DATABASE_TABLE, null, initialValues);
	}

	public ArrayList<leaf> query(String column, String value) {
		Cursor cursor = db
				.query("leaves",
						new String[] { "id,name,classification,desc,"
								+ "feature1,feature2,feature3,feature4,"
								+ "feature5,feature6" },
						column + "=?", new String[] { value }, null, null, null);
		ArrayList<leaf> leaves = new ArrayList<leaf>();
		boolean canContinue=cursor.moveToFirst();
		while(canContinue) {
			leaf teml=new leaf();
			teml.setId(cursor.getInt(0));
			teml.setName(cursor.getString(1));
			teml.setClassification(cursor.getInt(2));
			teml.setDesc(cursor.getString(3));
			
			float[] feature=new float[6];
			for(int i=0;i<6;i++){
				feature[i]=cursor.getFloat(4+i);
			}
			teml.setFeature(feature);
			
			leaves.add(teml);
			canContinue=cursor.moveToNext();
		}
		cursor.close();
		return leaves;
	}
	
	public ArrayList<leaf> getAll() {
		Cursor cursor = db
				.query("leaves",
						new String[] { "id,name,classification,desc,"
								+ "feature1,feature2,feature3,feature4,"
								+ "feature5,feature6" },
						null,null, null, null, null);
		ArrayList<leaf> leaves = new ArrayList<leaf>();
		boolean canContinue=cursor.moveToFirst();
		while(canContinue) {
			leaf teml=new leaf();
			teml.setId(cursor.getInt(0));
			teml.setName(cursor.getString(1));
			teml.setClassification(cursor.getInt(2));
			teml.setDesc(cursor.getString(3));
			
			float[] feature=new float[6];
			for(int i=0;i<6;i++){
				feature[i]=cursor.getFloat(4+i);
			}
			teml.setFeature(feature);
			
			leaves.add(teml);
			canContinue=cursor.moveToNext();
		}
		cursor.close();
		return leaves;
	}

	private static class DatabaseHelper extends SQLiteOpenHelper {
		DatabaseHelper(Context context) {
			super(context, DATABASE_NAME, null, DATABASE_VERSION);
		}

		@Override
		public void onCreate(SQLiteDatabase db) {
			db.execSQL(DATABASE_CREATE);
		}

		@Override
		public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
			db.execSQL("DROP TABLE IF EXISTS leaves");
			onCreate(db);
		}
	}
}