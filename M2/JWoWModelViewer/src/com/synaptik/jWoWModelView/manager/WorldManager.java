package com.synaptik.jWoWModelView.manager;

import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.jme.scene.Node;
import com.jme.util.Timer;
import com.synaptik.jWoWModelView.models.ModelException;

/**
 * This guy keeps track of all of the models loaded within this "world".
 *	- ModelManager
 *		(Keeps track of Models needed / used)
 *		(Talks to TextureManager for model textures)
 *	- TextureManager
 *		(Keeps track of textures needed / used)
 *	- MapManager
 *		(keeps track of map information, world location, music)
 *	- PlayerManager
 *		(keeps track of player location? and player stats)
 *  - ActorManager
 *  	(keeps track of individual objects)
 *  - CameraManager
 *  - InputManager
 *  - SoundManager
 *  - MusicManager
 *  
 *  TODO:
 *  	- textures
 *  	- maps
 *      - playermanager
 *  
 */
public class WorldManager {
	public ModelManager modelManager = null;
	public TextureManager textureManager = null;
	public CameraManager cameraManager = null;
	public ActorManager actorManager = null;
	public SoundManager soundManager = null;
	public InputManager inputManager = null;
	public Timer timer = null;
	
	public static WorldManager globalWorldManager = null;
	
	public Node world;
	
	public WorldManager(String dataDirectory) throws ModelException, IOException {
		modelManager = new ModelManager(dataDirectory);
		modelManager.loadModels();
		textureManager = new TextureManager(dataDirectory);
		cameraManager = new CameraManager();
		actorManager = new ActorManager();
		soundManager = new SoundManager();
		inputManager = new InputManager();
		
		world = new Node("World");

		globalWorldManager = this;
		timer = Timer.getTimer();
	}
	
	public void update() {
		timer.update();
		
		try {
			actorManager.animateActors((int)timer.getTime());
		} catch (ModelException ex) {
			Logger.getAnonymousLogger().log(Level.SEVERE, "Caught ModelException while rendering animation.", ex);
		}
		
		soundManager.update();
		inputManager.update(timer.getTimePerFrame());
	}
}
