package com.synaptik.jWoWModelView.ui;

import com.jme.scene.Node;
import com.jme.scene.SceneElement;
import com.jme.scene.Text;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.TextureState;
import com.jme.util.Timer;

public class WorldStateNode extends Node {
	private static final long serialVersionUID = 1L;
	public Text text;
	private StringBuffer buffer = null;
	
	public WorldStateNode() {
        super("State Node");
        
        text = Text.createDefaultTextLabel("State label");
        text.setCullMode( SceneElement.CULL_NEVER );
        text.setTextureCombineMode( TextureState.REPLACE );
        
        setRenderState( text.getRenderState( RenderState.RS_ALPHA ) );
        setRenderState( text.getRenderState( RenderState.RS_TEXTURE ) );
        attachChild( text );
        setCullMode( SceneElement.CULL_NEVER );
        
        updateGeometricState( 0.0f, true );
        updateRenderState();
        
        buffer = new StringBuffer();
    }
	
	public void update(Timer timer, String extraText) {
        buffer.setLength(0);
        buffer.append("FPS: ").append( (int) timer.getFrameRate() ).append(" - Ticks: ").append(timer.getTime()).append(" - Mem: ");
        buffer.append(Runtime.getRuntime().freeMemory() / 1024).append("KB / ").append(Runtime.getRuntime().totalMemory() / 1024).append("KB");
        if (extraText != null) {
        	buffer.append(" - ").append(extraText);
        }
        text.print( buffer );
	}
}
