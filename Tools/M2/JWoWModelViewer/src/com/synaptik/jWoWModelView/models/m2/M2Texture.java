package com.synaptik.jWoWModelView.models.m2;

import java.nio.ByteBuffer;

import com.synaptik.jWoWModelView.models.Piece;

public class M2Texture extends Piece {
	public int type;
	public short unk1;
	public short flags;
	public int lenName;
	public int ofsName;
	
	public String name;
	
	protected void loadSelf(ByteBuffer bb) {
		type = bb.getInt();
		unk1 = bb.getShort();
		flags = bb.getShort();
		lenName = bb.getInt();
		ofsName = bb.getInt();
		
		int oldPosition = bb.position();
		
		bb.position(ofsName);
		byte[] bytes = new byte[lenName-1];
		bb.get(bytes);
		name = new String(bytes);
		
		bb.position(oldPosition);
	}
	
	public String toString() {
		return name + " (Type: " + getTypeString() + ")";
	}
	
	protected String getTypeString() {
		String result = "Unknown";
		switch (type) {
			case 0:
				result = "Filename";
				break;
			case 1:
				result = "Body";
				break;
			case 2:
				result = "Cape";
				break;
			case 6:
				result = "Hair";
				break;
			case 8:
				result = "Fur";
				break;
			case 11:
			case 12:
			case 13:
				result = "Creature Skin [" + type + "]";
				break;
		}
		
		return result;
	}

	public int getSize() {
		return 16;
	}
}
