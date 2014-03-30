package com.synaptik.jWoWModelView.models.m2;

import java.nio.ByteBuffer;

import com.synaptik.jWoWModelView.models.Piece;

/**
 * SubMesh = group of triangles that describe a specific piece of the model.
 * For character models they describe a hair style, a cloak, boots, armor, shoulders, etc.
 * For other models (e.g. Murlocs), they a just pieces of the whole model.
 * 
 * @author Dan Watling (dwatling@synaptik.com)
 */
public class M2SubMesh extends Piece {

	public short id;
	public short unk0;
	public short startVertex;
	public short nVertexes;
	public short startTriangle;
	public short nTriangles;
	public short nBlockY;
	public short startBlockY;
	public short unk1;
	public short unk2;
	public float[] unk3 = new float[3];
	public float[] unk4 = new float[4];
	
	/**
	    0x00    uint32 	Mesh part ID
		0x04 	uint16 	Starting vertex number
		0x06 	uint16 	Number of vertices
		0x08 	uint16 	Starting triangle index (that's 3* the number of triangles drawn so far)
		0x0A 	uint16 	Number of triangle indices
		0x0C 	uint16 	Number of elements in Block Y
		0x0E 	uint16 	Starting index in Block Y
		0x10 	uint16 	Unknown
		0x12 	uint16 	Unknown
		0x14 	float[3] 	3 float values?
		0x20 	float[4] 	4 float values? (only in Client 2.0 M2s) 
	 */
	protected void loadSelf(ByteBuffer bb) {
		id = bb.getShort();
		unk0 = bb.getShort();
		startVertex = bb.getShort();
		nVertexes = bb.getShort();
		startTriangle = bb.getShort();
		nTriangles = bb.getShort();
		
		// These are really vertex offsets, I already coded it to be triangles, so I figured I
		//  would just leave them that way. Makes more sense to me.
		startTriangle /= 3;
		nTriangles /= 3;
		
		nBlockY = bb.getShort();
		startBlockY = bb.getShort();
		unk1 = bb.getShort();
		unk2 = bb.getShort();
		
		unk3[0] = bb.getFloat();
		unk3[1] = bb.getFloat();
		unk3[2] = bb.getFloat();
		unk4[0] = bb.getFloat();
		unk4[1] = bb.getFloat();
		unk4[2] = bb.getFloat();
		unk4[3] = bb.getFloat();
	}
	
	public String toString() {
		return "";
	}

	public int getSize() {
		return 48;
	}
	
}
