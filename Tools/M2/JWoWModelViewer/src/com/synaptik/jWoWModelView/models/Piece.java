package com.synaptik.jWoWModelView.models;

import java.nio.ByteBuffer;
import java.util.logging.Logger;

/**
 * TODO:
 * 	- Need a better name for this guy. "Piece" is vague. It really ensures a part of
 *    the model (e.g. header, triangle, vertex) is loaded correctly.
 *  
 * @author Dan Watling (dwatling@synaptik.com)
 */
public abstract class Piece {
	public abstract int getSize();
	protected abstract void loadSelf(ByteBuffer bb) throws ModelException;
	
	public void load(ByteBuffer bb) throws ModelException {
		int startPosition = bb.position();
		
		loadSelf(bb);
		
		int endPosition = bb.position();
		if ( (endPosition - startPosition) != getSize()) {
			String thisClass = this.getClass().getName();
			Logger.getAnonymousLogger().warning("(" + thisClass + ") Read " + (endPosition - startPosition) + " bytes! Should have read " + getSize() + " bytes.");
		}
	}
}
