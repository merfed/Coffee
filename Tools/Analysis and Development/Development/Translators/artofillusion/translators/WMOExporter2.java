package artofillusion.translators;

import artofillusion.*;
import artofillusion.math.*;
import artofillusion.object.*;
import artofillusion.texture.*;
import artofillusion.ui.*;
import buoy.widget.*;

import java.io.*;
import java.text.*;
import java.util.*;


import starlight.taliis.core.chunks.ChunkNotFoundException;
import starlight.taliis.core.chunks.wmo.group.MOBA_Entry;
import starlight.taliis.core.chunks.wmo.group.MOBN_Entry;
import starlight.taliis.core.chunks.wmo.group.MOBR;
import starlight.taliis.core.files.wmo_group;
import starlight.taliis.helpers.fileLoader;


/** WMOExporter contains the actual routines for exporting OBJ files. */
public class WMOExporter2
{
	static File f;
	/** Display a dialog which allows the user to export a scene to an OBJ file. */
	  
	  public static void exportFile(BFrame parent, Scene theScene)
	  {
		// Display a dialog box with options on how to export the scene.
		    
		    ValueField errorField = new ValueField(0.05, ValueField.POSITIVE);
		    //final ValueField widthField = new ValueField(200.0, ValueField.INTEGER+ValueField.POSITIVE);
		    //final ValueField heightField = new ValueField(200.0, ValueField.INTEGER+ValueField.POSITIVE);
		    //final ValueSlider qualitySlider = new ValueSlider(0.0, 1.0, 100, 0.5);
		    final BCheckBox smoothBox = new BCheckBox(Translate.text("subdivideSmoothMeshes"), false);
		    final BCheckBox injectChoice = new BCheckBox(Translate.text("Inject"), false);
		    final BCheckBox UVChoice = new BCheckBox(Translate.text("Export UV"), false);
		    
		    
		    BComboBox exportChoice = new BComboBox(new String [] {
		      Translate.text("exportWholeScene"),
		      Translate.text("selectedObjectsOnly")
		    });
		    
		    
		     
		      ComponentsDialog dlg;
		      if (theScene.getSelection().length > 0)
		        dlg = new ComponentsDialog(parent, Translate.text("InjectToWMO"), 
		  	  new Widget [] {exportChoice, errorField, smoothBox, injectChoice,UVChoice },
		  	  new String [] {null, Translate.text("maxSurfaceError"), null, null, null});
		      else
		        dlg = new ComponentsDialog(parent, Translate.text("exportToWMO"), 
		  	  new Widget [] {errorField, smoothBox, injectChoice,UVChoice },
		  	  new String [] {Translate.text("maxSurfaceError"), null, null, null});
		      if (!dlg.clickedOk())
		        return;
		      
		   // Ask the user to select the output file.

		      BFileChooser fc = new BFileChooser(BFileChooser.SAVE_FILE, Translate.text("exportToWMO"));
		      fc.setSelectedFile(new File("Untitled_000.wmo"));
		      if (ArtOfIllusion.getCurrentDirectory() != null)
		        fc.setDirectory(new File(ArtOfIllusion.getCurrentDirectory()));
		      if (!fc.showDialog(parent))
		        return;
		      File dir = fc.getDirectory();
		      f = fc.getSelectedFile();
		      String name = f.getName();
		      String baseName = (name.endsWith(".wmo") ? name.substring(0, name.length()-4) : name);
		      ArtOfIllusion.setCurrentDirectory(dir.getAbsolutePath());

		      wmo_group obj = null;
		      if(injectChoice.getState()==true)
		      try {
				obj= new wmo_group( fileLoader.openBuffer(f.getAbsolutePath()));
		      } catch (InvalidClassException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
		      } catch (ChunkNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
		      }
		      else
		    	  obj = new wmo_group();
		      writeScene(theScene, obj, exportChoice.getSelectedIndex() == 0, errorField.getValue(), smoothBox.getState(), injectChoice.getState(), UVChoice.getState()); 
	  }
	  public static void writeScene(Scene theScene, wmo_group out, boolean wholeScene, double tol, boolean smooth, boolean inject, boolean useuv)
	  {
		  int nsubfiles=0;
		  int numVert = 0, numNorm = 0, numTriangles = 0;
		   Hashtable<String, String> groupNames = new Hashtable<String, String>();
		   NumberFormat nf = NumberFormat.getNumberInstance(Locale.US);
		   nf.setMaximumFractionDigits(5);
		   nf.setGroupingUsed(false); 
		    for (int i = 0; i < theScene.getNumObjects(); i++)
		      {
		        // Get a rendering mesh for the object.

		        ObjectInfo info = theScene.getObject(i);
		        if (!wholeScene && !info.selected)
		          continue;
		        if (info.getObject().getTexture() == null)
		          continue;
		        FacetedMesh mesh;

		        if (!smooth && info.getObject() instanceof FacetedMesh)
		            mesh = (FacetedMesh) info.getObject();
		          else
		            mesh = info.getObject().convertToTriangleMesh(tol);
		          if (mesh == null)
		            continue;

		          nsubfiles++;
		     }
		    short faceCount[][][]=new short[nsubfiles][][];
		    float[][][] uvcoord=new float[nsubfiles][][];
	        MeshVertex vert[][] = new MeshVertex[nsubfiles][];
	        
	        Mat4 trans[]=new Mat4[nsubfiles];
	        out.moba.setnewSize(nsubfiles, false);
	        out.mogp.setnMeshes(nsubfiles);
	        out.mobn.setnewSize(nsubfiles, false);


			int cur=0;
			BoundingBox boundingb=new BoundingBox(0, 0, 0, 0, 0, 0);
		    for (int i = 0; i < theScene.getNumObjects(); i++)
		      {
		        // Get a rendering mesh for the object.

		        ObjectInfo info = theScene.getObject(i);
		        if (!wholeScene && !info.selected)
		          continue;
		        if (info.getObject().getTexture() == null)
		          continue;
		        FacetedMesh mesh;

		        if (!smooth && info.getObject() instanceof FacetedMesh)
		            mesh = (FacetedMesh) info.getObject();
		          else
		            mesh = info.getObject().convertToTriangleMesh(tol);
		          if (mesh == null)
		            continue;

		          vert[cur]=mesh.getVertices();
		          System.out.println("Current Block:\t"+vert[cur].length);
		          trans[cur] = info.getCoords().fromLocal();

		          if(useuv==true){
		        	  
			          UVMapping uvmap=(UVMapping) info.getObject().getTextureMapping();      
			          Vec2 uvs[]=uvmap.findTextureCoordinates(mesh);
			          uvcoord[cur]=new float[uvs.length][2];
			          for(int n=0;n<uvs.length;n++){
			        	  float[] txcoord={Math.abs((float) uvs[n].x),Math.abs((float) uvs[n].y)};
			        	  uvcoord[cur][n]=txcoord;
			         }
			      }

		          boundingb.extend(info.getBounds());
		          
		          
		          faceCount[cur]=new short[mesh.getFaceCount()][3];
		          numTriangles+=mesh.getFaceCount();
		          for(int f=0;f<mesh.getFaceCount();f++){
		        	  faceCount[cur][f][0]=(short)(numVert+mesh.getFaceVertexIndex(f, 0));
		        	  faceCount[cur][f][1]=(short)(numVert+ mesh.getFaceVertexIndex(f, 1));
		        	  faceCount[cur][f][2]=(short) (numVert+mesh.getFaceVertexIndex(f, 2));
		          }
		          
		          
		          		          
		          numVert += vert[cur].length;
		          cur++;
		        }
		    
	        float[] boundmax={(float) boundingb.getCorners()[0].x,(float) boundingb.getCorners()[0].y,(float) boundingb.getCorners()[0].z};
	        float[] boundmin={(float) boundingb.getCorners()[1].x,(float) boundingb.getCorners()[1].y,(float) boundingb.getCorners()[1].z};
	        

		    
		    out.movt.setnewSize(numVert, false);
		    out.monr.setnewSize(numVert, false);
		    out.motv.setnewSize(numVert, false);
		    out.movi.setnewSize(numTriangles, false);
		    out.mobr.setnewSize(numTriangles);
	        int vertexposcount=0;
	        int faceposcount=0;
		    for(int i=0;i<nsubfiles;i++){
		    	out.mobn.entrys[i]=new MOBN_Entry((short) (3*faceCount[i].length),(short) faceposcount);
		    	out.moba.entrys[i]=new MOBA_Entry((short)vertexposcount,(short) vert[i].length,(short) faceposcount,(short) (3*faceCount[i].length));
		    for (int j = 0; j < faceCount[i].length; j++)
	         {
	             out.movi.setnewIndex((faceposcount/3)+j, faceCount[i][j]);
	             out.mobr.setTriangle((faceposcount/3)+j, (short) ((faceposcount/3)+j));
	         }  
		     faceposcount+=3*faceCount[i].length;
		     if(useuv)
			     for(int n=0;n<uvcoord[i].length;n++){
			    	 out.motv.setnewTexVertex(vertexposcount+n,uvcoord[i][n]);
			     }
		     for (int j = 0; j < vert[i].length; j++)
	            {
	              Vec3 v = trans[i].times(vert[i][j].r);
	              float[] tf= new float[3];
	              tf[0]=(float) v.x;
	              tf[1]=(float) v.y;
	              tf[2]=(float) v.z;
	              out.movt.setnewVertexPos(vertexposcount+j, tf);
	              float[] fn= new float[3];
	              v.normalize();
	              fn[0]=(float) v.x;
	              fn[1]=(float) v.y;
	              fn[2]=(float) v.z;
	              out.monr.setnewNormalPos(vertexposcount+j, fn);
	            }
		     vertexposcount+=vert[i].length;
		    }
		  
		    out.mogp.setBoundingBox(boundmax, boundmin);
		  
		    save(out);
	  }
	  
	  public static int save(wmo_group out) {
			//TODO: catch failures and so on
			
				
				out.render();
				fileLoader.saveBuffer(out.buff , f.getAbsolutePath());
				return 1;
			
		}
}