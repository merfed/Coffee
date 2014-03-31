package com.synaptik.jWoWModelView.models.m2;

import java.nio.ByteBuffer;

import com.synaptik.jWoWModelView.models.Piece;

public class M2ViewTexture extends Piece {
	public short flags;
	public short renderOrder;
	public short materialID;
	public short materialID2;
	public short colorIndex;
	public short ofsRenderFlags;
	public short textureUnit;
	public short unk1;
	public short textureID;
	public short unk2;
	public short transparencyID;
	public short animationID;
	
	protected void loadSelf(ByteBuffer bb) {
		flags = bb.getShort();
		renderOrder = bb.getShort();
		materialID = bb.getShort();
		materialID2 = bb.getShort();
		colorIndex = bb.getShort();
		ofsRenderFlags = bb.getShort();
		textureUnit = bb.getShort();
		unk1 = bb.getShort();
		textureID = bb.getShort();
		unk2 = bb.getShort();
		transparencyID = bb.getShort();
		animationID = bb.getShort();
	}

	public int getSize() {
		return 24;
	}
}
