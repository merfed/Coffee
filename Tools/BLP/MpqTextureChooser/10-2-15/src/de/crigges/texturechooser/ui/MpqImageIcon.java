package de.crigges.texturechooser.ui;

import java.awt.Image;

import javax.swing.ImageIcon;

public class MpqImageIcon extends ImageIcon {
	private String path;

	public MpqImageIcon(Image image, String path) {
		super(image);
		this.path = path;
	}
	
	public String getPath() {
		return path;
	}
	
}
