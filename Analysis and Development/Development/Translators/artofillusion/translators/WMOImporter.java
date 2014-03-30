package artofillusion.translators;

import artofillusion.*;
import artofillusion.animation.*;
import artofillusion.image.*;
import artofillusion.math.*;
import artofillusion.object.*;
import artofillusion.texture.*;


import artofillusion.ui.*;
import buoy.widget.*;
import starlight.taliis.core.chunks.ChunkNotFoundException;
import starlight.taliis.core.chunks.wmo.group.*;
import starlight.taliis.core.files.*;
import starlight.taliis.helpers.fileLoader;

import java.io.*;
import java.util.*;

import de.taliis.editor.openedFile;

/**
 * A simple importplugin for World of Warcrafts *.wmo-files
 * 
 * @author Tigurius
 */

/** WMOImporter implements the importing of WMO files. */

public class WMOImporter
{
  public static void importFile(BFrame parent)
  {
    BFileChooser bfc = new BFileChooser(BFileChooser.OPEN_FILE, Translate.text("importWMO"));
    if (ArtOfIllusion.getCurrentDirectory() != null)
      bfc.setDirectory(new File(ArtOfIllusion.getCurrentDirectory()));
    if (!bfc.showDialog(parent))
      return;
    File f = bfc.getSelectedFile();
    ArtOfIllusion.setCurrentDirectory(bfc.getDirectory().getAbsolutePath());
    String wmoName = f.getName();
    if (wmoName.lastIndexOf('.') > 0)
    	wmoName = wmoName.substring(0, wmoName.lastIndexOf('.'));
    
    // Create a scene to add objects to.
    
    Scene theScene = new Scene();
    CoordinateSystem coords = new CoordinateSystem(new Vec3(0.0, 0.0, Camera.DEFAULT_DISTANCE_TO_SCREEN), new Vec3(0.0, 0.0, -1.0), Vec3.vy());
    ObjectInfo info = new ObjectInfo(new SceneCamera(), coords, "Camera 1");
    info.addTrack(new PositionTrack(info), 0);
    info.addTrack(new RotationTrack(info), 1);
    theScene.addObject(info, null);
    info = new ObjectInfo(new DirectionalLight(new RGBColor(1.0f, 1.0f, 1.0f), 0.8f), coords.duplicate(), "Light 1");
    info.addTrack(new PositionTrack(info), 0);
    info.addTrack(new RotationTrack(info), 1);
    theScene.addObject(info, null);

    // Open the file and read the contents.
    
    Hashtable groupTable = new Hashtable(), textureTable = new Hashtable();
    Vector vertex = new Vector(), normal = new Vector(), texture = new Vector(), face = new Vector();

    wmo_group obj = null;
    try {
		obj= new wmo_group( fileLoader.openBuffer(f.getAbsolutePath()));
	} catch (InvalidClassException e) {
		// TODO Auto-generated catch block
		System.out.println("Couldn't load File");
	} catch (ChunkNotFoundException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}
	System.out.println("nVertices:\t"+obj.movt.nVertices);
	for(int i=0;i<obj.movt.nVertices;i++){
		vertex.addElement(new Vec3(obj.movt.getVertexPos(i)[0], obj.movt.getVertexPos(i)[1], obj.movt.getVertexPos(i)[2]));
	}
	System.out.println("nNormals:\t"+obj.monr.nNormals);
	for(int i=0;i<obj.monr.nNormals;i++){
			normal.addElement(new Vec3(obj.monr.getNormalPos(i)[0], obj.monr.getNormalPos(i)[1], obj.monr.getNormalPos(i)[2]));
	}
	System.out.println("nTriangles:\t"+obj.movi.nTriangles);
	for(int i=0;i<obj.movi.nTriangles;i++){
		face.addElement(new Vec3(obj.movi.getIndex(i)[0],obj.movi.getIndex(i)[1],obj.movi.getIndex(i)[2]));
	}
	
	// Create a triangle mesh for each group.
   


      
        // Find which vertices are used by faces in this group.
        
        int realIndex[] = new int [obj.movt.nVertices];
        for (int i = 0; i < realIndex.length; i++)
          realIndex[i] = i;
        int numVert = obj.movt.nVertices;
        int fc[][] = new int [obj.movi.nTriangles][];
        for (int i = 0; i < fc.length; i++)
          {
            fc[i] = new int [] {realIndex[obj.movi.getIndex(i)[0]], realIndex[obj.movi.getIndex(i)[1]], realIndex[obj.movi.getIndex(i)[2]]};
          }
        
        // Build the list of vertices and center them.
        
        Vec3 vert[] = new Vec3 [numVert], center = new Vec3();
        for (int i = 0; i < realIndex.length; i++)
          if (realIndex[i] > -1)
            {
              vert[realIndex[i]] = (Vec3) vertex.elementAt(i);
              center.add(vert[realIndex[i]]);
            }
        center.scale(1.0/vert.length);
        for (int i = 0; i < vert.length; i++)
          vert[i] = vert[i].minus(center);

        /*info = new ObjectInfo(new TriangleMesh(vert, fc), coords, (wmoName));
        info.addTrack(new PositionTrack(info), 0);
        info.addTrack(new RotationTrack(info), 1);
        theScene.addObject(info, null);*/
        
        
        
        ObjectInfo submeshes[]=new ObjectInfo[obj.moba.entrys.length];
        for(int k=0;k<obj.moba.entrys.length;k++){
        coords = new CoordinateSystem(center, Vec3.vz(), Vec3.vy());
        System.out.println("Meshnr:\t"+k);
        Vec3 tempvert[]=new Vec3[obj.moba.entrys[k].getEndVertex()-obj.moba.entrys[k].getStartVertex()];	
        int pos=0;
        for(int n=obj.moba.entrys[k].getStartVertex();n<obj.moba.entrys[k].getEndVertex();n++){
        	tempvert[pos]=vert[n];
        	pos++;
        }
        pos=0;
        int tempfc[][]=new int[obj.mobn.entrys[k].getnTriangles()][3];
        for(int n=obj.mobn.entrys[k].getFirstFace();n<obj.mobn.entrys[k].getFirstFace()+obj.mobn.entrys[k].getnTriangles();n++){
        	tempfc[pos]=fc[n];
        	pos++;
        }
        submeshes[k]=new ObjectInfo(new TriangleMesh(vert,tempfc), coords, wmoName+"_"+k);
        TriangleMesh.Edge edge[] = ((TriangleMesh) submeshes[k].getObject()).getEdges();
        for (int i = 0; i < edge.length; i++)
          {
                edge[i].smoothness = 0.0f;
                continue;

          }
        theScene.addObject(submeshes[k],k, null);
        }





        ArtOfIllusion.newWindow(theScene);
        return;
            
}}
  
