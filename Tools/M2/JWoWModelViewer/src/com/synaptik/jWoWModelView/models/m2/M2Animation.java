package com.synaptik.jWoWModelView.models.m2;

import java.nio.ByteBuffer;

import com.jme.math.Vector3f;
import com.synaptik.jWoWModelView.models.ModelException;
import com.synaptik.jWoWModelView.models.Piece;

public class M2Animation extends Piece {

	public int animID;
	public int start;
	public int end;
	public float moveSpeed;
	public int loopType;
	public int flags;
	public int unk;
	public int unk1;
	public int playbackSpeed;
	public Vector3f corner1;
	public Vector3f corner2;
	public float radius;
	public short nextAnim;
	public short unk2;

	public int getSize() {
		return 68;
	}
	
	protected void loadSelf(ByteBuffer bb) throws ModelException {
		animID = bb.getInt();
		start = bb.getInt();
		end = bb.getInt();
		moveSpeed = bb.getFloat();
		loopType = bb.getInt();
		flags = bb.getInt();
		unk = bb.getInt();
		unk1 = bb.getInt();
		playbackSpeed = bb.getInt();
		corner1 = new Vector3f();
		corner1.x = bb.getFloat();
		corner1.y = bb.getFloat();
		corner1.z = bb.getFloat();
		M2Helper.fixCoordSystem(corner1);
		corner2 = new Vector3f();
		corner2.z = bb.getFloat();
		corner2.x = bb.getFloat();
		corner2.y = bb.getFloat();
		M2Helper.fixCoordSystem(corner2);
		
		radius = bb.getFloat();
		nextAnim = bb.getShort();
		unk2 = bb.getShort();
	}
}
