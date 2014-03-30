package com.synaptik.jWoWModelView.models.m2;

import java.nio.ByteBuffer;

import com.synaptik.jWoWModelView.models.ModelException;
import com.synaptik.jWoWModelView.models.Piece;

public class M2Range extends Piece {
	public int start;
	public int end;

	public M2Range() {
		start = 0;
		end = 0;
	}
	
	public int getSize() {
		return 8;
	}
	protected void loadSelf(ByteBuffer bb) throws ModelException {
		start = bb.getInt();
		end = bb.getInt();
	}
	
	public String toString() {
		return "start: " + start + "; end: " + end;
	}
}
