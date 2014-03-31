package com.synaptik.jWoWModelView.models.m2;

import java.nio.ByteBuffer;

import com.jme.math.Matrix3f;
import com.jme.math.Matrix4f;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.synaptik.jWoWModelView.models.ModelException;
import com.synaptik.jWoWModelView.models.Piece;

public class M2Bone extends Piece {
	public static final int FLAG_BILLBOARD = 8;
	public static final int FLAG_TRANSFORMED = 512;
	
	public int animID;
	public int flags;
	public short parent;
	public short geoid;
	public int unknown;
	public M2AnimationBlock translation;
	public M2AnimationBlock rotation;
	public M2AnimationBlock scaling;
	public Vector3f pivot;
	
	// -----
	public Matrix3f matrix = null;
	public Matrix4f matrixRotation = null;
	
	public M2Bone() {
		translation = new M2AnimationBlock(Vector3f.class);
		rotation = new M2AnimationBlock(Quaternion.class);
		scaling = new M2AnimationBlock(Vector3f.class);
		pivot = new Vector3f();
	}
	
	public int getSize() {
		return 112;
	}
	
	protected void loadSelf(ByteBuffer bb) throws ModelException {
		animID = bb.getInt();
		flags = bb.getInt();
		parent = bb.getShort();
		geoid = bb.getShort();
		unknown = bb.getInt();
		
		translation.load(bb);
		rotation.load(bb);
		scaling.load(bb);
		
		pivot.x = bb.getFloat();
		pivot.y = bb.getFloat();
		pivot.z = bb.getFloat();
		M2Helper.fixCoordSystem(pivot);
	}
	
	public String toString() {
		return "Type: " + getTypeAsString();
	}
	
	private String getTypeAsString() {
		String result = "Unknown";
		switch (flags) {
			case FLAG_BILLBOARD:
				result = "Billboard";
				break;
			case FLAG_TRANSFORMED:
				result = "Transformed";
				break;
		}
		return result;
	}
}
