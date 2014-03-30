package com.synaptik.jWoWModelView.manager;

public class ManagerException extends Exception {
	private static final long serialVersionUID = 1L;

	public ManagerException(String msg) {
		super (msg);
	}
	
	public ManagerException (String msg, Throwable t) {
		super(msg, t);
	}
}
