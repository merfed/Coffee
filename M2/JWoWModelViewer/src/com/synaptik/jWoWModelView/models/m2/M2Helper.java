package com.synaptik.jWoWModelView.models.m2;

import com.jme.math.Quaternion;
import com.jme.math.Vector3f;

public class M2Helper {

	/**
	 * WoW has a weird coordinate system. This fixes it.
	 * @param vec
	 */
	public static void fixCoordSystem(Vector3f vec) {
		float tempX = vec.x;
		float tempY = vec.y;
		float tempZ = vec.z;
		
		vec.x = tempX;
		vec.y = tempZ;
		vec.z = -tempY;
	}
	
	public static void fixCoordSystem(Quaternion q) {
		float tempX = q.x;
		float tempY = q.y;
		float tempZ = q.z;
		float tempW = q.w;
		
		q.x = -tempX;
		q.y = -tempZ;
		q.z = tempY;
		q.w = -tempW;
	}
}
