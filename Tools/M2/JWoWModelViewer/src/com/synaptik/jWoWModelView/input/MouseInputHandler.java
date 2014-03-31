package com.synaptik.jWoWModelView.input;

import com.jme.input.ChaseCamera;
import com.jme.input.InputHandler;
import com.jme.input.RelativeMouse;
import com.jme.input.thirdperson.ThirdPersonMouseLook;
import com.jme.renderer.Camera;
import com.jme.scene.Spatial;

public class MouseInputHandler extends InputHandler {

    private ThirdPersonMouseLook mouseLook;
    private Spatial target;
    private ChaseCamera chaseCamera;

    public MouseInputHandler( Spatial target, Camera cam, float speed ) {
        RelativeMouse mouse = new RelativeMouse("Mouse Input");
        mouse.registerWithInputHandler( this );
        
        chaseCamera = new ChaseCamera(cam, target);
        chaseCamera.setMaxDistance(10.0f);
        chaseCamera.setMinDistance(1.0f);

        mouseLook = new ThirdPersonMouseLook(mouse, chaseCamera, target);
        mouseLook.setWorldUpVec(cam.getUp());
        
        addAction(mouseLook);
        
        this.target = target;
    }
    
}

