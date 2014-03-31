package com.synaptik.jWoWModelView.models;

import com.jme.scene.Node;

/**
 * Kind of unnecessary for this application...
 *  
 * @author Dan Watling (dwatling@synaptik.com)
 */
public abstract class Model {

	protected String type = null;
	protected String name = null;
	protected Piece header = null;
	
	public Piece getHeader() {
		return header;
	}
	
	public String getName() {
		return name;
	}
	
	public abstract Node createNode(String name);
}
