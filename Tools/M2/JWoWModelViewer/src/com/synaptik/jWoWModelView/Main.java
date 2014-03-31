package com.synaptik.jWoWModelView;

import com.jme.math.Vector3f;
import com.synaptik.jWoWModelView.manager.WorldManager;

/**
 * jWoWModelView v0.1
 * 
 * @author Dan Watling (dwatling@synaptik.com)   12/22/2007
 * 
 * -----
 * 
 * TODO
 * 		- Migrate bone animation to jME's Bone animation structure
 * 		- Textures!
 * 		- Particle effect animation
 * 		- "Billboarding"
 * 		- WoW-like movement
 * 			- Mouse movement should rotate around model.
 * 			- Keyboard presses should animate model (i.e. WASD for movement)
 * -----
 * 
 * The goal of this whole thing was to learn more about jME and understand how it works.
 * I've still got a ways to go, but already I have learned a lot. Thanks go out to the
 * authors of jME and LWJGL. 
 *  
 * Much of this code was written with the assistance of WoWModelView and WoWDev.
 *    http://www.wowmodelviewer.org
 * 	  http://wowdev.org/wiki/index.php/M2
 * 
 * I used MPQEditor 1.8.1.178 to extract the M2 files and sound files.
 *    http://www.zezula.net/en/mpq/download.html
 *    
 * Many thanks to the authors of the aforementioned websites and applications.
 */
public class Main {
	static WorldManager wm = null; 
	
	public static void main(String[] args) throws Exception {
		wm = new WorldManager("./Data2/");
//System.out.println("Number of meshes for DragonOnyxia: " + ((M2Model)wm.modelManager.getModel("DragonOnyxia")).view.nSubmesh);		
wm.actorManager.addActor("Player", "Murloc", new Vector3f(0.0f, -1.0f, -5.0f), new boolean[]{true, true});
//		wm.actorManager.addActor("Player", "DragonOnyxia", new Vector3f(0.0f, -1.0f, -5.0f), new boolean[]{true, true, true, true, true});
//		wm.actorManager.addActor("Player", "GnomeMale", new Vector3f(0.0f, -1.0f, -5.0f), 
//				new boolean[]{
//					true, true, false, false, true, false, false, false, false, false,
//					false, false, false, false, false, false, true, true, false, false,
//					false, false, true, false, false, false, false, false, false, true,
//					true, false, false, false, false, false, false, false, false, false,
//					false, true, true, false, false, false, false, false, false, false,
//					false,
//					});
		System.err.println(Runtime.getRuntime().freeMemory());
		
//		wm.soundManager.setMusic("E:/projects/jWoWModelView/Data2/Sound/Ambience/WMOAmbience/Ironforge.wav");
//		wm.soundManager.setMusic("E:/projects/jWoWModelView/Data2/Sound/Ambience/WMOAmbience/TavernCrowded.wav");
//		wm.soundManager.setMusic("E:/projects/jWoWModelView/Data2/Sound/Ambience/Weather/RainHeavyLoop.wav");
		wm.soundManager.setMusic("E:/projects/jWoWModelView/Data2/Sound/Ambience/Weather/SnowLight.wav");
//		wm.soundManager.setMusic("E:/projects/jWoWModelView/Data2/Sound/Ambience/ZoneAmbience/ForestNormalDay.wav");
//		wm.soundManager.setMusic("E:/projects/jWoWModelView/Data2/Sound/Music/GlueScreenMusic/wow_main_theme.ogg");

		Game game = new Game(wm);
	    game.start();
	}
}
