package com.synaptik.jWoWModelView.models.m2;

import java.nio.ByteBuffer;

import com.jme.math.Vector2f;
import com.jme.math.Vector3f;
import com.synaptik.jWoWModelView.models.Piece;

public class M2Vertex extends Piece {
	
	public Vector3f pos;
	public int[] weights = new int[4];
	public int[] bones = new int[4];
	public Vector3f normal;
	public Vector2f texcoords;
	public int unk1, unk2; // always 0,0 so this is probably unused
	
	public M2Vertex() {
		pos = new Vector3f();
		normal = new Vector3f();
		texcoords = new Vector2f();
	}
	
	public String toString() {
		StringBuffer sb = new StringBuffer();
		
		sb.append("pos(x,y,z) = (")
			.append(pos.x).append(", ")
			.append(pos.y).append(", ")
			.append(pos.z).append("); ");
		sb.append("normal(x,y,z) = (")
			.append(normal.x).append(", ")
			.append(normal.y).append(", ")
			.append(normal.z).append("); ");
		sb.append("weights = [")
			.append((int)weights[0]).append(", ")
			.append((int)weights[1]).append(", ")
			.append((int)weights[2]).append(", ")
			.append((int)weights[3]).append("]; ");
		sb.append("bones = [")
			.append((int)bones[0]).append(", ")
			.append((int)bones[1]).append(", ")
			.append((int)bones[2]).append(", ")
			.append((int)bones[3]).append("]; ");
		sb.append("unk1 = ").append(unk1).append("; ");
		sb.append("unk2 = ").append(unk2).append("; ");
			
		return sb.toString();
	}
	
	protected void loadSelf(ByteBuffer bb) {
		pos.x = bb.getFloat();
		pos.y = bb.getFloat();
		pos.z = bb.getFloat();
		M2Helper.fixCoordSystem(pos);
		
		for (int index = 0; index < weights.length; index ++) {
			byte b = bb.get();
			weights[index] = ((int)b) & 0xFF;
		}
		for (int index = 0; index < bones.length; index ++) {
			byte b = bb.get();
			bones[index] = ((int)b) & 0xFF;
		}
		
		normal.x = bb.getFloat();
		normal.y = bb.getFloat();
		normal.z = bb.getFloat();
		M2Helper.fixCoordSystem(normal);
		
		normal = normal.normalize();
		
		texcoords.x = bb.getFloat();
		texcoords.y = bb.getFloat();
		
		unk1 = bb.getInt();
		unk2 = bb.getInt();
	}

	public int getSize() {
		return 48;
	}
	
}
