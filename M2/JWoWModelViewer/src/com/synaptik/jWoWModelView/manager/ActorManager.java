package com.synaptik.jWoWModelView.manager;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import com.jme.math.Vector3f;
import com.synaptik.jWoWModelView.Actor;
import com.synaptik.jWoWModelView.models.ModelException;
import com.synaptik.jWoWModelView.models.m2.M2Model;

/**
 * TODO:
 * 	- Collision detection between actors and the world
 *  
 * @author Dan Watling (dwatling@synaptik.com)
 */
public class ActorManager {
	public Map<String, Actor> actors = null;
	
	public ActorManager() {
		actors = new HashMap<String, Actor>();
	}
	
	public void addActor(String name, String modelName, Vector3f position,
			boolean[] displaySubMesh) throws ManagerException {
		Actor actor = new Actor();
		actor.modelName = modelName;
		actor.pos = position;
		actor.model = (M2Model)WorldManager.globalWorldManager.modelManager.getModel(modelName);
		actor.model.displayedMeshes = displaySubMesh;
		if (actor.model != null) {
			actor.initialize(name);
			
			actors.put(name, actor);
		
			WorldManager.globalWorldManager.world.attachChild(actor.node);
		} else {
			throw new ManagerException("There is no actor with that name: " + modelName);
		}
	}
	
	public Actor getActor(String name) {
		return actors.get(name);
	}
	
	public void removeActor(String name) {
		actors.remove(name);
	}
	
	public void animateActors(int ticks) throws ModelException {
		Iterator<Actor> iter = actors.values().iterator();
		while (iter.hasNext()) {
			Actor actor = iter.next();
			actor.animationTime = ticks % (actor.model.animations.get(actor.animationNumber).end - actor.model.animations.get(actor.animationNumber).start);
			actor.animationTime += actor.model.animations.get(actor.animationNumber).start;
			actor.animate();
		}
	}
}
