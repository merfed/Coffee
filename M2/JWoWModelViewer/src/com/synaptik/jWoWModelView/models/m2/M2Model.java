package com.synaptik.jWoWModelView.models.m2;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.List;

import com.jme.animation.AnimationController;
import com.jme.animation.Bone;
import com.jme.animation.BoneAnimation;
import com.jme.animation.BoneTransform;
import com.jme.animation.SkinNode;
import com.jme.math.Matrix4f;
import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.jme.scene.Controller;
import com.jme.scene.Node;
import com.jme.scene.TriMesh;
import com.jme.scene.batch.TriangleBatch;
import com.jme.util.geom.BufferUtils;
import com.synaptik.jWoWModelView.models.Model;
import com.synaptik.jWoWModelView.models.ModelException;

/**
 * My attempt at porting WoWModelViewer's M2 model loading to Java/jME. Many
 * thanks to the author(s). WoWModelViewer can be found at:
 *     http://www.wowmodelviewer.org
 * 
 * TODO:
 *	- Textures
 *		+ Apply textures
 *  - Bones
 *  	+ Understand jME's bone animation structure and convert to jME's
 *  - Cleaner classes
 *  	+ Make the classes much cleaner and easier to understand. 
 *      + Model should only contain model information. Actor class should contain
 *        "current animation" information.
 *      + All "frames" should be precalculated. (Is this possible?)
 * 
 *  References:
 *  	http://wowdev.org/wiki/index.php/M2
 *  	http://www.madx.dk/wowdev/wiki/index.php?title=M2
 *  
 *  @author Dan Watling (dwatling@synaptik.com)
 */
public class M2Model extends Model {
	
	public int[] globalSequences = null;
	public List<M2Vertex> vertexes = null;
//	public List<M2View> views = null;
	public M2View view = null;
	public List<M2Texture> textures = null;
	public List<M2Bone> bones = null;
	public List<M2Animation> animations = null;
	public short[] boneLookup = null;
	public List<Bone> jmeBones = null;
	public boolean[] displayedMeshes = null;


	public M2Model() throws IOException, ModelException {
		this.type = "M2";
		header = new M2Header();
		
		vertexes = new ArrayList<M2Vertex>();
//		views = new ArrayList<M2View>();
		view = new M2View();
		textures = new ArrayList<M2Texture>();
		bones = new ArrayList<M2Bone>();
		animations = new ArrayList<M2Animation>();
		jmeBones = new ArrayList<Bone>();
	}
	
	public void load(ByteBuffer bb) throws IOException, ModelException {
		header.load(bb);
		
		loadName(bb);
		loadGlobalSequences(bb);
		loadVertexes(bb);
		loadViews(bb);
		loadTextures(bb);
		loadBones(bb);
		loadAnimations(bb);
		loadBoneLookupTable(bb);
	}
	
	protected void loadName(ByteBuffer bb) {
		M2Header m2Header = (M2Header)header;
		
		bb.position(m2Header.nameOffset);
		byte[] bytes = new byte[m2Header.nameLength-1];
		bb.get(bytes);
		this.name = new String(bytes);
	}
	
	protected void loadGlobalSequences(ByteBuffer bb) throws ModelException {
		M2Header m2Header = (M2Header)header;
		
		globalSequences = new int[m2Header.nGlobalSequences];
		bb.position(m2Header.ofsGlobalSequences);
		for (int index = 0; index < globalSequences.length; index ++) {
			globalSequences[index] = bb.getInt();
		}
	}
	
	protected void loadVertexes(ByteBuffer bb) throws ModelException {
		M2Header m2Header = (M2Header)header;
		
		bb.position(m2Header.ofsVertices);
		for (int index = 0; index < m2Header.nVertices; index ++) {
			M2Vertex vertex = new M2Vertex();
			vertex.load(bb);
			vertexes.add(vertex);
		}
	}
	
	protected void loadViews(ByteBuffer bb) throws ModelException {
		M2Header m2Header = (M2Header)header;
		
		bb.position(m2Header.ofsViews);
		for (int index = 0; index < 1; index ++) {
			view.load(bb);
//			views.add(view);
		}
	}
	
	protected void loadTextures(ByteBuffer bb) throws ModelException {
		M2Header m2Header = (M2Header)header;
		
		bb.position(m2Header.ofsTextures);
		for (int index = 0; index < m2Header.nTextures; index ++) {
			M2Texture texture = new M2Texture();
			texture.load(bb);
			textures.add(texture);
		}
	}
	
	protected void loadBones(ByteBuffer bb) throws ModelException {
		M2Header m2Header = (M2Header)header;
		
		bb.position(m2Header.ofsBones);
		for (int index = 0; index < m2Header.nBones; index ++) {
			M2Bone bone = new M2Bone();
			bone.load(bb);
			bones.add(bone);
		}
	}
	
	protected void loadAnimations(ByteBuffer bb) throws ModelException {
		M2Header m2Header = (M2Header)header;
		
		bb.position(m2Header.ofsAnimations);
		for (int index = 0; index < m2Header.nAnimations; index ++) {
			M2Animation animation = new M2Animation();
			animation.load(bb);
			animations.add(animation);
		}
	}
	
	protected void loadBoneLookupTable(ByteBuffer bb) throws ModelException {
		M2Header m2Header = (M2Header)header;
		boneLookup = new short[m2Header.nF];
		
		bb.position(m2Header.ofsF);
		bb.asShortBuffer().get(boneLookup);
	}
	
	public String toString() {
		return "name: " + name + "; nVertices: " + vertexes.size();
	}
	
	public Node createNode(String name) {
		Node result = new Node(name);
		
		TriMesh mesh = generateTriMesh();
		result.attachChild(mesh);
		
//		SkinNode skin = new SkinNode();
//		result.attachChild(skin);
//		skin.setSkin(mesh);
//
//		generateJMEBones();
//		linkBones(result);
//		weightSkin(skin);
//		
//		skin.normalizeWeights();
//		skin.regenInfluenceOffsets();
		
		return result;
	}
	
	protected void generateJMEBones() {
		for (int index = 0; index < bones.size(); index ++) {
			Bone jmeBone = new Bone("bone");
			int nValues = bones.get(index).translation.nValues; 
			if (nValues > 0) {
				BoneAnimation boneAnim = new BoneAnimation();
				BoneTransform boneTrans = new BoneTransform(jmeBone, nValues);
				float[] times = new float[nValues];
				for (int index2 = 0; index2 < nValues; index2++) {
					boneTrans.setTranslation(index2, (Vector3f)bones.get(index).translation.values.get(index2));
					Quaternion q = (Quaternion)bones.get(index).rotation.values.get(index2);
					Matrix4f m = new Matrix4f();
					q.toRotationMatrix(m);
					boneTrans.setTransform(index2, m);
					times[index2] = bones.get(index).translation.times[index2] * (1.0f / 20.0f);
				}
				boneAnim.addBoneTransforms(boneTrans);
				boneAnim.setTimes(times);
				boneAnim.setEndFrame(nValues -1);
				boneAnim.setInterpolate(true);
				
				AnimationController ac = new AnimationController();
				ac.addAnimation(boneAnim);
				ac.setActiveAnimation(boneAnim);
				ac.setRepeatType(Controller.RT_WRAP);
				
				jmeBone.addController(ac);
			}
			jmeBones.add(jmeBone);
		}
	}
	
	protected void linkBones(Node rootNode) {
		for (int index = 0; index < bones.size(); index++) {
			M2Bone bone = bones.get(index);
			if (bone.parent == -1) {
				rootNode.attachChild(jmeBones.get(index));
			} else {
				jmeBones.get(bone.parent).attachChild(jmeBones.get(index));
			}
		}
	}
	
	protected void weightSkin(SkinNode skin) {
		for (int index = 0; index < view.nTriangles; index++) {
			M2Triangle tri = view.triangles[index];

			for (int boneNumber = 0; boneNumber < 4; boneNumber++) {
				int bone = vertexes.get(tri.vertex1).bones[boneNumber];
				float weight = vertexes.get(tri.vertex1).weights[boneNumber] / 255.0f;
				skin.addBoneInfluence(0, tri.vertex1, jmeBones.get(bone), weight);

				bone = vertexes.get(tri.vertex2).bones[boneNumber];
				weight = vertexes.get(tri.vertex2).weights[boneNumber] / 255.0f;
				skin.addBoneInfluence(0, tri.vertex2, jmeBones.get(bone), weight);
				
				bone = vertexes.get(tri.vertex3).bones[boneNumber];
				weight = vertexes.get(tri.vertex3).weights[boneNumber] / 255.0f;
				skin.addBoneInfluence(0, tri.vertex3, jmeBones.get(bone), weight);
			}
		}
	}
	
	protected TriMesh generateTriMesh() {
		TriMesh mesh = new TriMesh(name + "_mesh");
		TriangleBatch tb = mesh.getBatch(0);
		tb.setVertexBuffer(BufferUtils.createVector3Buffer(tb.getVertexBuffer(), this.vertexes.size()));
		Vector3f[] vertexes = new Vector3f[this.vertexes.size()];
		Vector3f[] normals = new Vector3f[this.vertexes.size()];
		
		for (int index = 0; index < vertexes.length; index ++) {
			vertexes[index] = this.vertexes.get(index).pos;
			normals[index] = this.vertexes.get(index).normal;
		}
		
		int totalSize = 0;
		
		for (int index = 0; index < displayedMeshes.length; index ++) {
			totalSize += view.submeshes[index].nTriangles * 3;
		}
		IntBuffer ib = BufferUtils.createIntBuffer(totalSize);
		for (int index = 0; index < displayedMeshes.length; index ++) {
			if (displayedMeshes[index]) {
				int[] indexes = view.buildIndexListForSubMesh(index);
				ib.put(indexes);
			}
		}
		
		tb.setVertexBuffer(BufferUtils.createFloatBuffer(vertexes));
		tb.setNormalBuffer(BufferUtils.createFloatBuffer(normals));
		tb.setIndexBuffer(ib);
		return mesh;
	}
	
	/**
	 * Used for animation purposes. Animation frames need a base point, which is this.
	 */
	public Vector3f[] clonePosVectors() {
		Vector3f[] result = new Vector3f[vertexes.size()];
		
		for (int index = 0; index < vertexes.size(); index ++) {
			result[index] = (Vector3f)vertexes.get(index).pos.clone();
		}
		
		return result;
	}
	
	/**
	 * Used for animation purposes. Animation frames need a base point, which is this.
	 */
	public Vector3f[] cloneNormalVectors() {
		Vector3f[] result = new Vector3f[vertexes.size()];
		
		for (int index = 0; index < vertexes.size(); index ++) {
			result[index] = (Vector3f)vertexes.get(index).normal.clone();
		}
		
		return result;
	}
}
