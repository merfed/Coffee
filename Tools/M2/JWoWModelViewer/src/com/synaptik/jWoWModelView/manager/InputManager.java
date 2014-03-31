package com.synaptik.jWoWModelView.manager;

import com.jme.input.FirstPersonHandler;
import com.jme.input.InputHandler;
import com.jme.renderer.Camera;
import com.synaptik.jWoWModelView.Actor;

/**
 * TODO:
 *	- Need to create my own input handler
 *	 	+ Mouse movement should rotate around the object
 *		+ WSAD should make the model move (forward, backword, strafe left, strafe right)
 *			and animate appropriately.
 *	 
 * @author Dan Watling (dwatling@synaptik.com)
 */
public class InputManager {
	private InputHandler input;
	
	public void update(float timePerFrame) {
		input.update(timePerFrame);
	}
	
	public void init(Actor actor, Camera cam) {
//		input = new MouseInputHandler(actor.node, cam, 1.0f);
		input = new FirstPersonHandler(cam, 2.0f, 1.0f);
	}
}
