package com.synaptik.jWoWModelView.manager;

import java.net.URL;

import com.jmex.audio.AudioSystem;
import com.jmex.audio.AudioTrack;
import com.jmex.audio.MusicTrackQueue;
import com.jmex.audio.MusicTrackQueue.RepeatType;

/**
 * TODO:
 * 	- Multiple sounds at same time
 * 		+ Music
 * 		+ Ambience
 * 		+ Many sound effects (weapon attacks, spells, etc.)
 *  - Seems to be a problem with OGG files.
 *  - Support MP3
 * 
 * @author Dan Watling (dwatling@synaptik.com)
 */
public class SoundManager {
	public String music;
	public String ambience;
	public String[] sounds;
	
	public SoundManager() {
	}
	
	public void setMusic(String music) throws Exception {
		this.music = music;
		
		AudioTrack track = AudioSystem.getSystem().createAudioTrack(
				new URL("file:///" + this.music), false);
		MusicTrackQueue queue = AudioSystem.getSystem().getMusicQueue();
		queue.setCrossfadeinTime(0);
		queue.setRepeatType(RepeatType.ALL);
		queue.addTrack(track);
		queue.play();
	}
	
	public void setAmbience(String ambience) {
		this.ambience = ambience;
	}
	
	public void update() {
		AudioSystem.getSystem().update();
	}
}
