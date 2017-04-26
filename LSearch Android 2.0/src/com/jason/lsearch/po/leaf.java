package com.jason.lsearch.po;

public class leaf {
	private int id,classification;
	private String name,desc;
	private float[] feature=new float[6];
	
	public leaf() {
		super();
		// TODO Auto-generated constructor stub
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public int getClassification() {
		return classification;
	}

	public void setClassification(int classification) {
		this.classification = classification;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getDesc() {
		return desc;
	}

	public void setDesc(String desc) {
		this.desc = desc;
	}

	public float[] getFeature() {
		return feature;
	}

	public void setFeature(float[] feature) {
		this.feature = feature;
	}
	
	
}
