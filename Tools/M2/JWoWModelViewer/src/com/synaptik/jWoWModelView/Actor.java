package com.synaptik.jWoWModelView;

import com.jme.math.Matrix4f;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.scene.Node;
import com.jme.scene.TriMesh;
import com.jme.scene.batch.TriangleBatch;
import com.jme.util.geom.BufferUtils;
import com.synaptik.jWoWModelView.models.ModelException;
import com.synaptik.jWoWModelView.models.m2.M2AnimationBlock;
import com.synaptik.jWoWModelView.models.m2.M2Model;
import com.synaptik.jWoWModelView.models.m2.M2Range;
import com.synaptik.jWoWModelView.models.m2.M2Vertex;

/**
 * @author Dan Watling (dwatling@synaptik.com)
 */
public class Actor {
	public Vector3f pos;
	public Vector3f velocity;
	public Quaternion heading;
	public Quaternion headingVelocity;
	public String modelName;
	
	public M2Model model;
	public Node node;
	
	public int animationTime;
	public int animationNumber;
	
	private Matrix4f[] boneMatrix;
	private Matrix4f[] boneRotation;

/* -------------------------------------------------------------------------------- */
	
	public void initialize(String name) {
		node = model.createNode(name);
		
		node.setLocalTranslation(pos);
		heading = new Quaternion(0.0f, -0.7f, 0.0f, 0.7f);
		node.setLocalRotation(heading);
		
		velocity = new Vector3f(0.0f, 0.0f, 0.0f);
	}
	
/* -------------------------------------------------------------------------------- */
	
	public void animate() throws ModelException {
		TriangleBatch tb = ((TriMesh)node.getChild(model.getName() + "_mesh")).getBatch(0);
		Vector3f[] vertexes = model.clonePosVectors();
		Vector3f[] normals = model.cloneNormalVectors();
		
		boneMatrix = new Matrix4f[model.bones.size()];
		boneRotation = new Matrix4f[model.bones.size()];
		
		calcBoneMatrix(animationNumber, animationTime);
		
		/**
		 * See model.cpp : animate -- Line ~1178
		 */
		for (int index = 0; index < model.vertexes.size(); index ++) {
			M2Vertex vertex = model.vertexes.get(index);
			Vector3f v = new Vector3f();
			Vector3f n = new Vector3f();
			
			for (int boneIndex = 0; boneIndex < 4; boneIndex ++) {
				if (vertex.weights[boneIndex] > 0) {
					Vector3f tv = boneMatrix[vertex.bones[boneIndex]].mult(vertex.pos);
					Vector3f tn = boneRotation[vertex.bones[boneIndex]].mult(vertex.normal);
					v = v.add(tv.mult((float)vertex.weights[boneIndex] / 255.0f));
					n = tn;
				}
			}

			vertexes[index] = v;
			normals[index] = n;
		}
		
		tb.setVertexBuffer(BufferUtils.createFloatBuffer(vertexes));
		tb.setNormalBuffer(BufferUtils.createFloatBuffer(normals));
		
		pos = pos.add(velocity);
		node.setLocalTranslation(pos);
		node.setLocalRotation(heading);
	}
	
/* -------------------------------------------------------------------------------- */
	
	private void calcBoneMatrix(int anim, int time) throws ModelException {
		for (int index = 0; index < model.bones.size(); index ++) {
			calcBoneMatrix(index, anim, time);
		}
	}
	
/* -------------------------------------------------------------------------------- */
	
	private void calcBoneMatrix(int boneIndex, int anim, int time) throws ModelException {
		Vector3f pivot = model.bones.get(boneIndex).pivot; 
		Matrix4f m = new Matrix4f();
		m.setTranslation(pivot);
		
		int parentID = model.bones.get(boneIndex).parent;
		
		Quaternion q = null;
		
		if (model.bones.get(boneIndex).translation.nRanges > 0) {
			Vector3f tr = (Vector3f)getValue(model.bones.get(boneIndex).translation, anim, time);
			Matrix4f temp = new Matrix4f();
			temp.setTranslation(tr);
			m = m.mult(temp);
		}

		if (model.bones.get(boneIndex).rotation.nRanges > 0) {
			q = (Quaternion)getValue(model.bones.get(boneIndex).rotation, anim, time);
			Matrix4f temp = new Matrix4f();
			temp.setRotationQuaternion(q);
			m = m.mult(temp);
		}

		if (model.bones.get(boneIndex).scaling.nRanges > 0) {
			Vector3f sc = (Vector3f)getValue(model.bones.get(boneIndex).scaling, anim, time);
			m.mult(sc);
		}
		
		{
			Matrix4f temp = new Matrix4f();
			temp.setTranslation(pivot.mult(-1.0f));
			m = m.mult(temp);
		}
		
		boneMatrix[boneIndex] = m;
		
		if (parentID >= 0) {
			if (boneRotation[parentID] == null) {
				// just in case...
				calcBoneMatrix(parentID, anim, time);
			}
			boneMatrix[boneIndex] = boneMatrix[parentID].mult(m);
		}

		/**
		 * Line 1757  model.cpp
		 */
		if (model.bones.get(boneIndex).rotation.nRanges > 0 && parentID >= 0) {
			Matrix4f temp = new Matrix4f();
			temp.setRotationQuaternion(q);
			boneRotation[boneIndex] = boneRotation[parentID].mult(temp);
		} else {
			boneRotation[boneIndex] = new Matrix4f();
		}
	}
	
/* -------------------------------------------------------------------------------- */
	
	/** See animated.h : getValue -- Line 114 **/
	public Object getValue(M2AnimationBlock block, int anim, int time) throws ModelException {
		M2Range range = null;
		Object result = null;
		if (block.type != M2AnimationBlock.TYPE_NONE && block.values.size() > 1) {
			if (block.seq == M2AnimationBlock.SEQ_NONE) {
				range = block.ranges.get(anim);
				time %= block.times[block.times.length-1];
			}
			
			if (range.start != range.end) {
				int t1, t2;
				int pos = 0;
				for (int index = range.start; index < range.end; index++) {
					if (time >= block.times[index] && time < block.times[index+1]) {
						pos = index;
						break;
					}
				}
				t1 = block.times[pos];
				t2 = block.times[pos+1];
				float r = (time - t1) / (float)(t2 - t1);
	
				switch (block.type) {
					case M2AnimationBlock.TYPE_LINEAR:
						if (Quaternion.class.getName().equals(block.valueClass.getName())) {
							result = block.values.get(pos);
							result = new Quaternion((Quaternion)result);
							((Quaternion)result).slerp((Quaternion)block.values.get(pos+1), r);
						} else if (Vector3f.class.getName().equals(block.valueClass.getName())) {
							result = block.values.get(pos);
							result = new Vector3f((Vector3f)result);
							((Vector3f)result).interpolate((Vector3f)block.values.get(pos+1), r);
						} else {
							throw new ModelException("Unsupported AnimationBlock type: " + block.valueClass.getName());
						}
						break;
					case M2AnimationBlock.TYPE_NONE: 
						result = block.values.get(pos);
						break;
					default:
						throw new ModelException("Hermite animation block type is not supported.");
				}
			} else {
				result = block.values.get(range.start);
			}
		} else {
			if (block.values.size() == 0) {
				result = 0;
			} else {
				result = block.values.get(0);
			}
		}
		
		return result;
	}
}
