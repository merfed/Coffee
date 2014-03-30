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
import starlight.taliis.core.chunks.wmo.group.MOBR;
import starlight.taliis.core.files.wmo_group;
import starlight.taliis.helpers.fileLoader;


/** WMOExporter contains the actual routines for exporting OBJ files. */
public class WMOExporter
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
		// Write the objects in the scene.
		  	short startv=0,endv=0,startindex=0,endindex=0;
		    int numVert = 0, numNorm = 0, numTexVert = 0;
		    Hashtable<String, String> groupNames = new Hashtable<String, String>();
		    NumberFormat nf = NumberFormat.getNumberInstance(Locale.US);
		    nf.setMaximumFractionDigits(5);
		    nf.setGroupingUsed(false);
		    int groups=0,posgroups=0;
		    int nVertices=0; short posVert=0;
		    int nFaces=0,nTriangles=0; short posFaces=0;
		    int nNormals=0,posNormals=0;
		    int nUVs=0,posUVs=0;
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
		          nVertices+=mesh.getVertices().length;
		          nFaces+=mesh.getFaceCount();
		          nNormals+=mesh.getNormals().length;
		          groups++;
		      }
		    nTriangles=nFaces;
		    if(inject==false){
		          out.moba.setnewSize(groups, false);
		          out.movt.setnewSize(nVertices, false);
		          out.movt.render();
		          out.movi.setnewSize(nFaces, false);
		          out.movi.render();
		          out.mogp.setnMeshes(groups);
		          out.mobr=new MOBR(nTriangles);
		          for(int n=0;n<nTriangles;n++){
		        	  out.mobr.setTriangle(n, (short)n);
		          }
		          out.mobn.entrys[0].setnTriangles(nTriangles);
		    }
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

		          startv=posVert;
		          startindex=(short) (3*posFaces);
		       // Find the normals.
		          
		          Vec3 norm[];
		          int normIndex[][] = new int[mesh.getFaceCount()][];
		          if (mesh instanceof TriangleMesh)
		          {
		            RenderingMesh rm = ((TriangleMesh) mesh).getRenderingMesh(Double.MAX_VALUE, false, info);
		            norm = rm.norm;
		            for (int j = 0; j < normIndex.length; j++)
		              normIndex[j] = new int[] {rm.triangle[j].n1, rm.triangle[j].n2, rm.triangle[j].n3};
		          }
		          else
		          {
		            norm = mesh.getNormals();
		            for (int j = 0; j < normIndex.length; j++)
		            {
		              normIndex[j] = new int[mesh.getFaceVertexCount(j)];
		              for (int k = 0; k < normIndex[j].length; k++)
		                normIndex[j][k] = mesh.getFaceVertexIndex(j, k);
		            }
		          }
		       // Write out the object.
		          
		          
		          Mat4 trans = info.getCoords().fromLocal();
		          MeshVertex vert[] = mesh.getVertices();

		          for (int j = 0; j < vert.length; j++)
		            {
		              Vec3 v = trans.times(vert[j].r);
		              float[] tf= new float[3];
		              tf[0]=(float) v.x;
		              tf[1]=(float) v.y;
		              tf[2]=(float) v.z;
		              out.movt.setnewVertexPos(posVert, tf);
		              posVert++;
		            }
		          short faceCount[][]=new short[mesh.getFaceCount()][3];
		          for(int f=0;f<mesh.getFaceCount();f++){
		        	  faceCount[f][0]=(short) mesh.getFaceVertexIndex(f, 0);
		        	  faceCount[f][1]=(short) mesh.getFaceVertexIndex(f, 1);
		        	  faceCount[f][2]=(short) mesh.getFaceVertexIndex(f, 2);
		          }

		          for (int j = 0; j < faceCount.length; j++)
		            {
		              out.movi.setnewIndex(posFaces, faceCount[j]);
		              posFaces++;
		            }
		          
		          if(useuv==true){
		          UVMapping uvmap=(UVMapping) info.getObject().getTextureMapping();      
		          Vec2 uv[]=uvmap.findTextureCoordinates(mesh);
		          out.motv.setnewSize(out.movt.nVertices, inject);
		          for(int n=0;n<out.movt.nVertices;n++){
		        	  if(n<=uv.length){
		        	  float[] txcoord={(float) uv[n].x,(float) uv[n].y};
		        	  out.motv.setnewTexVertex(n, txcoord);}
		        	  else{
		        		  float[] txcoord={0.0f,0.0f};
		        		  out.motv.setnewTexVertex(n, txcoord);
		        	  }
		          }}
		          
		          /* out.monr.setnewSize(norm.length);
		          out.monr.render();
		          for (int j = 0; j < norm.length; j++)
		            {
		        	  float[] tf = new float[3];
		              if (norm[j] == null){
		            	  tf[0]=1f;tf[1]=0f;tf[2]=0f;
		            	  out.monr.setNormalPos(j, tf);}
		              else
		                {
		                  Vec3 v = trans.timesDirection(norm[j]);
		                  tf[0]=(float) v.x;tf[1]=(float) v.y;tf[2]=(float)v.z;
		                  out.monr.setnewNormalPos(j, tf);
		                }
		            }*/
		          endv=posVert;
		          endindex=(short) (3*posFaces);
		          if(inject==false){
		        	  out.moba.entrys[posgroups]=new MOBA_Entry(startv,endv,startindex,endindex);
		          }
		          posgroups++;
		          numVert += vert.length;
		          numNorm += norm.length;
		        }
		    	/*if(inject==false){
		          out.moba.setnewSize(groups, false);
		          out.movt.render();
		          out.movi.render();
		          out.moba.entrys[0]=new MOBA_Entry((short)0,(short)out.movt.nVertices,0,out.movi.nTriangles);
		          out.mobn.setnewSize(1, false);
		          out.mobn.entrys[0]=new MOBN_Entry(out.movi.nTriangles, 1);
		          }*/
		    save(out);
	  }
	  
	  public static int save(wmo_group out) {
			//TODO: catch failures and so on
			
				
				out.render();
				fileLoader.saveBuffer(out.buff , f.getAbsolutePath());
				return 1;
			
		}
}