package com.synaptik.jWoWModelView.input;

import com.jme.input.Mouse;
import com.jme.input.action.InputAction;
import com.jme.input.action.InputActionEvent;
import com.jme.input.action.MouseInputAction;
import com.jme.renderer.Camera;
import com.jme.scene.Spatial;

public class RotateCameraLeftAction extends MouseInputAction {

	protected Camera camera;
	protected Spatial target;
	protected Mouse mouse;
	
	public RotateCameraLeftAction(Spatial target, Camera camera, Mouse mouse) {
		this.target = target;
		this.camera = camera;
		this.mouse = mouse;
	}
	
	public void performAction(InputActionEvent evt) {
		if (mouse.getLocalTranslation().x < 0) {
			camera.update();
		}
	}

}
