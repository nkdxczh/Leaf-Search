package org.jason.lsearch.internet;

import java.io.IOException;
import java.util.Properties;

public class InternetConfig {
	
	private static final Properties PROPERTIES = new Properties();
	static{
		try {
			PROPERTIES.load(InternetConfig.class.getResourceAsStream("inertnet.cfg.properties"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static final String IP = PROPERTIES.getProperty("IP");
	public static final String PORT = PROPERTIES.getProperty("PORT");
	public static final String PROJECT = PROPERTIES.getProperty("PROJECT");
}
