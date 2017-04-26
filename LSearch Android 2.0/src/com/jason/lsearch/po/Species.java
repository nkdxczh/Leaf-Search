package com.jason.lsearch.po;

public class Species {
	private int id;
	private String name;
	private String description;
	private String small_image;
	private float possibility;
	
	public Species(){
	}
	
	public Species(int id, String name, String description, String small_image) {
		super();
		this.id = id;
		this.name = name;
		this.description = description;
		this.small_image = small_image;
	}
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getDescription() {
		return description;
	}
	public void setDescription(String description) {
		this.description = description;
	}
	public String getSmall_image() {
		return small_image;
	}
	public void setSmall_image(String small_image) {
		this.small_image = small_image;
	}
	public float getPossibility() {
		return this.possibility;
	}
	public void setPosibility(int possibility) {
		this.possibility = possibility;
	}
	@Override
	public String toString() {
		return "species [id=" + id + ", name=" + name + ", description="
				+ description + ", small_image=" + small_image + "]";
	}

}
