package com.synaptik.jWoWModelView.models.m2;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import com.jme.math.Quaternion;
import com.jme.math.Vector3f;
import com.synaptik.jWoWModelView.models.ModelException;
import com.synaptik.jWoWModelView.models.Piece;

/**
 * TODO 
 * 	- Support Hermite animation block
 *  - Redo structure for easier understandability of ranges.
 *  	? What are they exactly? Are they the ranges for the specific animation type? (e.g. Standing / Swimming / etc)
 *  
 *  @author Dan Watling (dwatling@synaptik.com)
 */
public class M2AnimationBlock extends Piece {
	
	public static final short TYPE_NONE = 0;
	public static final short TYPE_LINEAR = 1;
	public static final short TYPE_HERMITE = 2;
	
	public static final short SEQ_NONE = -1;
	
	public short type;
	public short seq;
	public int nRanges;
	public int ofsRanges;
	public int nTimes;
	public int ofsTimes;
	public int nValues;
	public int ofsValues;
	
	public List<M2Range> ranges;
	public int[] times;
	public List<Object> values;			// can be of varying sizes
	
	public Class valueClass = null;		// Vector3f? Quaternion?
	
	public M2AnimationBlock(Class valueClass) {
		this.valueClass = valueClass;
	}
	
	public int getSize() {
		return 28;
	}
	
	protected void loadSelf(ByteBuffer bb) throws ModelException {
		type = bb.getShort();
		seq = bb.getShort();
		nRanges = bb.getInt();
		ofsRanges = bb.getInt();
		nTimes = bb.getInt();
		ofsTimes = bb.getInt();
		nValues = bb.getInt();
		ofsValues = bb.getInt();
		
		int oldPosition = bb.position();
		
		bb.position(ofsRanges);
		ranges = new ArrayList<M2Range>(nRanges);
		for (int index = 0; index < nRanges; index ++) {
			M2Range range = new M2Range();
			range.load(bb);
			ranges.add(range);
		}

		bb.position(ofsTimes);
		times = new int[nTimes];
		bb.asIntBuffer().get(times);
		
		
		/**
		 * Determine how many bytes we need to read based on the value type
		 * 
		 * See: animated.h : init -- Line ~200
		 */
		bb.position(ofsValues);
		values = new ArrayList<Object>();
		if (Quaternion.class.getName().equals(valueClass.getName())) {
			for (int index = 0; index < nValues; index ++) {
				short tempX = bb.getShort();
				short tempY = bb.getShort();
				short tempZ = bb.getShort();
				short tempW = bb.getShort();
				
				float x = (tempX < 0 ? tempX + 32768 : tempX - 32767) / 32767.0f;
				float y = (tempY < 0 ? tempY + 32768 : tempY - 32767) / 32767.0f;
				float z = (tempZ < 0 ? tempZ + 32768 : tempZ - 32767) / 32767.0f;
				float w = (tempW < 0 ? tempW + 32768 : tempW - 32767) / 32767.0f;
				Quaternion q = new Quaternion(x, y, z, w);
				M2Helper.fixCoordSystem(q);

				values.add(q);
			}
		} else if (Vector3f.class.getName().equals(valueClass.getName())) {
			for (int index = 0; index < nValues; index ++) {
				float x = bb.getFloat();
				float y = bb.getFloat();
				float z = bb.getFloat();
				Vector3f v = new Vector3f(x, y, z);
				M2Helper.fixCoordSystem(v);
				values.add(v);
			}
		} else {
			throw new ModelException("Unsupported AnimationBlock type: " + valueClass.getName());
		}
		
		bb.position(oldPosition);
	}
	
//	public void init() throws ModelException {
//		if (type != TYPE_NONE && seq == SEQ_NONE) {
//			M2Range range = new M2Range();
//			range.end = nValues - 1;
//			ranges.add(range); 
//		}
//
//		if (nTimes != nValues) {
//			throw new ModelException("Expected nTimes == nValues.");
//		}
//
//		switch (type) {
//			case TYPE_NONE:
//			case TYPE_LINEAR:
//				break;
//			case TYPE_HERMITE:
//				throw new ModelException("Hermite animation block type is not supported.");
//		}
//	}
	
	public String toString() {
		return "Range count: " + nRanges + "; Seq: " + seq;
	}
}
