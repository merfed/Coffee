package com.synaptik.jWoWModelView.models.m2;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import com.synaptik.jWoWModelView.models.ModelException;
import com.synaptik.jWoWModelView.models.Piece;

/**
 * Very important piece.
 *  Describes how to turn all of the vertexes into triangles, also describes textures
 *  and submeshes (configurability for the model [e.g. hair styles, clothing, etc.])
 * 
 * @author Dan Watling (dwatling@synaptik.com)
 */
public class M2View extends Piece {

	public int nVertexes;
	public int ofsVertexes;
	public int nTriangles;
	public int ofsTriangles;
	public int nVertexProps;
	public int ofsVertexProps;
	public int nSubmesh;
	public int ofsSubmesh;
	public int nTexture;
	public int ofsTexture;
	public int unk;
	
	public short[] globalVertexLookup = null;
	public M2Triangle[] triangles = null;
	public M2SubMesh[] submeshes = null;
	public M2ViewTexture[] textures = null;
			
	protected void loadSelf(ByteBuffer bb) throws ModelException {
		nVertexes = bb.getInt();
		ofsVertexes = bb.getInt();
		nTriangles = bb.getInt();
		nTriangles = nTriangles / 3;
		ofsTriangles = bb.getInt();
		
		nVertexProps = bb.getInt();
		ofsVertexProps = bb.getInt();
		nSubmesh = bb.getInt();
		ofsSubmesh = bb.getInt();
		
		nTexture = bb.getInt();
		ofsTexture = bb.getInt();
		unk = bb.getInt();
		
		int oldPosition = bb.position();
		
		loadIndexes(bb);
		loadTriangles(bb);
		loadSubMeshes(bb);
		loadTextures(bb);

		bb.position(oldPosition);
	}
	
	public void loadIndexes(ByteBuffer bb) {
		globalVertexLookup = new short[nVertexes];
		bb.position(ofsVertexes);
		bb.asShortBuffer().get(globalVertexLookup);
	}
	
	public void loadTriangles(ByteBuffer bb) throws ModelException {
		triangles = new M2Triangle[nTriangles];
		bb.position(ofsTriangles);
		for (int index = 0; index < triangles.length; index ++) {
			triangles[index] = new M2Triangle();
			triangles[index].load(bb);
			
			// Do the global vertex lookup here
			triangles[index].vertex1 = globalVertexLookup[triangles[index].vertex1];
			triangles[index].vertex2 = globalVertexLookup[triangles[index].vertex2];
			triangles[index].vertex3 = globalVertexLookup[triangles[index].vertex3];
		}
	}
	
	public void loadSubMeshes(ByteBuffer bb) throws ModelException {
		submeshes = new M2SubMesh[nSubmesh];
		bb.position(ofsSubmesh);
		for (int index = 0; index < submeshes.length; index ++) {
			submeshes[index] = new M2SubMesh();
			submeshes[index].load(bb);
		}
	}
	
	public void loadTextures(ByteBuffer bb) throws ModelException {
		textures = new M2ViewTexture[nTexture];
		bb.position(ofsTexture);
		for (int index = 0; index < textures.length; index ++) {
			textures[index] = new M2ViewTexture();
			textures[index].load(bb);
		}
	}
	
	/**
	 * model.cpp -- Line 660
	 */
	public int[] buildIndexListForSubMesh(int submeshNumber) {
		IntBuffer result = IntBuffer.allocate(submeshes[submeshNumber].nTriangles * 3);
		
		int startTriangle = submeshes[submeshNumber].startTriangle;

		for (int index = 0; index < submeshes[submeshNumber].nTriangles; index ++) {
			result.put(triangles[startTriangle + index].vertex1);
			result.put(triangles[startTriangle + index].vertex2);
			result.put(triangles[startTriangle + index].vertex3);
		}

		return result.array();
	}
	
	public String toString() {
		return "nIndex: " + nVertexes + "; nTriangles: " + nTriangles + "; nSubmesh: " + nSubmesh;
	}

	public int getSize() {
		return 44;
	}

}
