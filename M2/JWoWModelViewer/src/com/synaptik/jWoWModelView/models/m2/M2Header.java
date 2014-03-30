package com.synaptik.jWoWModelView.models.m2;

import java.nio.ByteBuffer;

import com.synaptik.jWoWModelView.models.ModelException;
import com.synaptik.jWoWModelView.models.Piece;

public class M2Header extends Piece {
	
	public byte[] id = new byte[4];
	public byte[] version = new byte[4];
	public int nameLength;
	public int nameOffset;
	public int type;
	
	public int nGlobalSequences;
	public int ofsGlobalSequences;
	public int nAnimations;
	public int ofsAnimations;
	public int nC;
	public int ofsC;
	public int nD;
	public int ofsD;
	public int nBones;
	public int ofsBones;
	public int nF;
	public int ofsF;

	public int nVertices;
	public int ofsVertices;
	public int nViews;
	public int ofsViews;

	public int nColors;
	public int ofsColors;

	public int nTextures;
	public int ofsTextures;

	public int nTransparency; // H
	public int ofsTransparency;
	public int nI;   // always unused ?
	public int ofsI;
	public int nTexAnims;	// J
	public int ofsTexAnims;
	public int nTexReplace;
	public int ofsTexReplace;

	public int nTexFlags;
	public int ofsTexFlags;
	public int nY;
	public int ofsY;

	public int nTexLookup;
	public int ofsTexLookup;

	public int nTexUnitLookup;		// L
	public int ofsTexUnitLookup;
	public int nTransparencyLookup; // M
	public int ofsTransparencyLookup;
	public int nTexAnimLookup;
	public int ofsTexAnimLookup;

	float[] floats = new float[14];

	public int nBoundingTriangles;
	public int ofsBoundingTriangles;
	public int nBoundingVertices;
	public int ofsBoundingVertices;
	public int nBoundingNormals;
	public int ofsBoundingNormals;

	public int nAttachments; // O
	public int ofsAttachments;
	public int nAttachLookup; // P
	public int ofsAttachLookup;
	public int nQ; // Q
	public int ofsQ;
	public int nLights; // R
	public int ofsLights;
	public int nCameras; // S
	public int ofsCameras;
	public int nT;
	public int ofsT;
	public int nRibbonEmitters; // U
	public int ofsRibbonEmitters;
	public int nParticleEmitters; // V
	public int ofsParticleEmitters;

	public int getSize() {
		return 324;
	}
	
	protected void loadSelf(ByteBuffer bb) throws ModelException {
		bb.get(id);
		if (id[0] != 'M' && id[1] != 'D' && id[2] != '2') {
			throw new ModelException("Not a valid M2 file.");
		}
		bb.get(version);
		if (version[0] != 4 && version[1] != 1 && version[2] != 0 && version[3] != 0) {
			throw new ModelException("This program does not support this model version.");
		}
		
		nameLength = bb.getInt();
		nameOffset = bb.getInt();
		
		type = bb.getInt();
		nGlobalSequences = bb.getInt();
		ofsGlobalSequences = bb.getInt();
		nAnimations = bb.getInt();
		
		ofsAnimations = bb.getInt();
		nC = bb.getInt();
		ofsC = bb.getInt();
		nD = bb.getInt();
		
		ofsD = bb.getInt();
		nBones = bb.getInt();
		ofsBones = bb.getInt();
		nF = bb.getInt();
		
		ofsF = bb.getInt();
		nVertices = bb.getInt();
		ofsVertices = bb.getInt();
		nViews = bb.getInt();
		
		ofsViews = bb.getInt();
		nColors = bb.getInt();
		ofsColors = bb.getInt();
		nTextures = bb.getInt();
		
		ofsTextures = bb.getInt();

		nTransparency = bb.getInt(); // H
		ofsTransparency = bb.getInt();
		nI = bb.getInt();   // always unused ?
		ofsI = bb.getInt();
		nTexAnims = bb.getInt();	// J
		ofsTexAnims = bb.getInt();
		nTexReplace = bb.getInt();
		ofsTexReplace = bb.getInt();

		nTexFlags = bb.getInt();
		ofsTexFlags = bb.getInt();
		nY = bb.getInt();
		ofsY = bb.getInt();

		nTexLookup = bb.getInt();
		ofsTexLookup = bb.getInt();

		nTexUnitLookup = bb.getInt();		// L
		ofsTexUnitLookup = bb.getInt();
		nTransparencyLookup = bb.getInt(); // M
		ofsTransparencyLookup = bb.getInt();
		nTexAnimLookup = bb.getInt();
		ofsTexAnimLookup = bb.getInt();
		
		for (int index = 0; index < floats.length; index ++) {
			floats[index] = bb.getFloat();
		}
		
		nBoundingTriangles = bb.getInt();
		ofsBoundingTriangles = bb.getInt();
		nBoundingVertices = bb.getInt();
		ofsBoundingVertices = bb.getInt();
		nBoundingNormals = bb.getInt();
		ofsBoundingNormals = bb.getInt();

		nAttachments = bb.getInt(); // O
		ofsAttachments = bb.getInt();
		nAttachLookup = bb.getInt(); // P
		ofsAttachLookup = bb.getInt();
		nQ = bb.getInt(); // Q
		ofsQ = bb.getInt();
		nLights = bb.getInt(); // R
		ofsLights = bb.getInt();
		nCameras = bb.getInt(); // S
		ofsCameras = bb.getInt();
		nT = bb.getInt();
		ofsT = bb.getInt();
		nRibbonEmitters = bb.getInt(); // U
		ofsRibbonEmitters = bb.getInt();
		nParticleEmitters = bb.getInt(); // V
		ofsParticleEmitters = bb.getInt();
	}
}
