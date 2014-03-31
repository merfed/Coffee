package com.synaptik.jWoWModelView.models.m2;

import java.nio.ByteBuffer;

import com.synaptik.jWoWModelView.models.Piece;

public class M2Triangle extends Piece {
	public short vertex1;
	public short vertex2;
	public short vertex3;
	
	protected void loadSelf(ByteBuffer bb) {
		vertex1 = bb.getShort();
		vertex2 = bb.getShort();
		vertex3 = bb.getShort();
	}
	
	public String toString() {
		return "(" + vertex1 + ", " + vertex2 + ", " + vertex3 + ")";
	}

	public int getSize() {
		return 6;
	}
}
