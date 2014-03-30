package com.synaptik.jWoWModelView;

import com.jme.app.BaseGame;
import com.jme.input.KeyBindingManager;
import com.jme.input.KeyInput;
import com.jme.light.DirectionalLight;
import com.jme.math.Vector3f;
import com.jme.renderer.ColorRGBA;
import com.jme.scene.state.LightState;
import com.jme.scene.state.ZBufferState;
import com.jme.system.DisplaySystem;
import com.synaptik.jWoWModelView.manager.WorldManager;
import com.synaptik.jWoWModelView.ui.WorldStateNode;

/**
 * jME stuff
 * 
 * Mouse to change the direction of the camera.
 * WSAD to move the camera
 * Press 'e' and 'r' to change animation frames.
 * 
 * @author Dan Watling (dwatling@synaptik.com)
 */
public class Game extends BaseGame {

	private WorldManager wm = null;
	private WorldStateNode infoText = null;
	
	public Game(WorldManager wm) {
		this.wm = wm;
	}
	
	protected void initGame() {
        ZBufferState buf = display.getRenderer().createZBufferState();
        buf.setEnabled(true);
        buf.setFunction(ZBufferState.CF_LEQUAL);
        wm.world.setRenderState(buf);
        
        addLights();
		
        wm.world.updateGeometricState(0.0f, true);
        wm.world.updateRenderState();
        
        infoText = new WorldStateNode();
	}
	
	protected void addLights() {
	    DirectionalLight light = new DirectionalLight();
	    light.setDiffuse(new ColorRGBA(0.8f, 0.8f, 0.8f, 0.8f));
	    light.setAmbient(new ColorRGBA(0.5f, 0.5f, 0.5f, 1.0f));
	    light.setDirection(new Vector3f(-1,-1,-1));
	    light.setEnabled(true);

	    DirectionalLight light2 = new DirectionalLight();
	    light2.setDiffuse(new ColorRGBA(0.0f, 0.0f, 0.5f, 1.0f));
	    light2.setAmbient(new ColorRGBA(0.5f, 0.5f, 0.5f, 1.0f));
	    light2.setDirection(new Vector3f(1,1,1));
	    light2.setEnabled(true);

	    LightState lightState = display.getRenderer().createLightState();
	    lightState.setEnabled(true);
	    lightState.attach(light);
	    lightState.attach(light2);
	    wm.world.setRenderState(lightState);
	}
	
	protected void initSystem() {
		display = DisplaySystem.getDisplaySystem(properties.getRenderer());
		display.createWindow(properties.getWidth(), properties.getHeight(), properties.getDepth(), properties.getFreq(), properties.getFullscreen());

		wm.cameraManager.createCamera(display, 45.0f);
		wm.inputManager.init(wm.actorManager.getActor("Player"), wm.cameraManager.camera);
		
		display.getRenderer().setBackgroundColor(ColorRGBA.black);
		display.setVSyncEnabled(true);
		
		wm.cameraManager.useCamera(display);
		
		KeyBindingManager.getKeyBindingManager().set("exit", KeyInput.KEY_ESCAPE);
		KeyBindingManager.getKeyBindingManager().set("decAnimation", KeyInput.KEY_E);
		KeyBindingManager.getKeyBindingManager().set("incAnimation", KeyInput.KEY_R);
	}

	protected void reinit() {
		display.recreateWindow(properties.getWidth(), properties.getHeight(), properties.getDepth(), properties.getFreq(), properties.getFullscreen());
	}

	protected void render(float interpolation) {
		display.getRenderer().clearBuffers();
		display.getRenderer().draw(wm.world);
		display.getRenderer().draw(infoText);
//		Debugger.drawBounds( wm.world, display.getRenderer(), true );
//        Debugger.drawNormals( wm.world, display.getRenderer() );
	}

	protected void update(float interpolation) {
		infoText.update(wm.timer, "Animation # " + wm.actorManager.actors.get("Player").animationNumber + " / " + (wm.actorManager.actors.get("Player").model.animations.size()-1));
		
		if (KeyBindingManager.getKeyBindingManager().isValidCommand("exit")) {
			finished = true;
		}
		if( KeyBindingManager.getKeyBindingManager().isValidCommand( "decAnimation", false ) ) {
			if (wm.actorManager.actors.get("Player").animationNumber > 0) {
				wm.actorManager.actors.get("Player").animationNumber--;
			}
		}
		if( KeyBindingManager.getKeyBindingManager().isValidCommand( "incAnimation", false ) ) {
			if (wm.actorManager.actors.get("Player").animationNumber < wm.actorManager.actors.get("Player").model.animations.size()-1) {
				wm.actorManager.actors.get("Player").animationNumber++;
			}
		}
		
		wm.update();
	}
	
	protected void cleanup() {
	}
}
