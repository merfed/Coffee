package com.synaptik.jWoWModelView.manager;

import java.util.HashMap;
import java.util.Map;

/**
 * TODO:
 * 	- Load textures
 *  - Free textures when no longer used
 *   
 * @author Dan Watling (dwatling@synaptik.com)
 */
public class TextureManager {
	public Map<String, String> textures = null;
	public String basePath = null;
	
	public TextureManager(String basePath) {
		textures = new HashMap<String, String>();
		this.basePath = basePath;
	}
}
