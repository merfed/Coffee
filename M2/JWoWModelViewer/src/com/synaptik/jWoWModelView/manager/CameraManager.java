package com.synaptik.jWoWModelView.manager;

import com.jme.math.Vector3f;
import com.jme.renderer.Camera;
import com.jme.system.DisplaySystem;

/**
 * TODO
 * 	- Ability to create multiple cameras (Is this really necessary?)
 */
public class CameraManager {
	public Camera camera;

	public void createCamera(DisplaySystem display, float fovY) {
		camera = display.getRenderer().createCamera(display.getWidth(), display.getHeight());
		camera.setFrustumPerspective(fovY, (float)display.getWidth() / (float)display.getHeight(), 0.1f, 1000.0f);
		
		Vector3f locVec = new Vector3f(0.0f, 0.0f, 0.0f); 
		Vector3f leftVec = new Vector3f(-1.0f, 0.0f, 0.0f); 
		Vector3f upVec = new Vector3f(0.0f, 1.0f, 0.0f); 
		Vector3f dirVec = new Vector3f(0.0f, 0.0f, -1.0f); 
		
		camera.setFrame(locVec, leftVec, upVec, dirVec);
		camera.update();
		
	}
	
	public void useCamera(DisplaySystem display) {
		display.getRenderer().setCamera(camera);
	}
	
	public Camera getCamera() {
		return camera;
	}
}
