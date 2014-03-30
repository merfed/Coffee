package artofillusion.translators;

import artofillusion.*;
import artofillusion.animation.Joint;
import artofillusion.animation.Skeleton;
import artofillusion.math.*;
import artofillusion.object.*;
import artofillusion.texture.UVMapping;
import artofillusion.ui.*;
import buoy.widget.*;

import java.io.*;
import java.text.*;
import java.util.*;

import starlight.taliis.core.binary.m2.geometry.Bones;
import starlight.taliis.core.binary.skin.Submesh;
import starlight.taliis.core.binary.skin.TexUnits;
import starlight.taliis.core.files.m2;
import starlight.taliis.core.files.skin;
import starlight.taliis.helpers.fileLoader;


/** WMOExporter contains the actual routines for exporting OBJ files. */
public class M2Exporter
{
	static File f;
	/** Display a dialog which allows the user to export a scene to an OBJ file. */
	  
	  public static void exportFile(BFrame parent, Scene theScene)
	  {
		// Display a dialog box with options on how to export the scene.
		    
		    //ValueField errorField = new ValueField(0.05, ValueField.POSITIVE);
		    //final ValueField widthField = new ValueField(200.0, ValueField.INTEGER+ValueField.POSITIVE);
		    //final ValueField heightField = new ValueField(200.0, ValueField.INTEGER+ValueField.POSITIVE);
		    //final ValueSlider qualitySlider = new ValueSlider(0.0, 1.0, 100, 0.5);
		    //final BCheckBox smoothBox = new BCheckBox(Translate.text("subdivideSmoothMeshes"), true);
		    final BCheckBox injectChoice = new BCheckBox(Translate.text("Inject"), false);
		    final BTextField modelname=new BTextField(Translate.text("Modelname"));
		    final BCheckBox UVChoice = new BCheckBox(Translate.text("Export UV"), false);
		    final BCheckBox BoneChoice = new BCheckBox(Translate.text("Export Bones"), false);
		    final BCheckBox BoundChoice = new BCheckBox(Translate.text("Add BoundingVolume"), false);
		    
		    
		    /*BComboBox exportChoice = new BComboBox(new String [] {
		      Translate.text("exportWholeScene"),
		      Translate.text("selectedObjectsOnly")
		    });*/
		    
		    
		     
		      ComponentsDialog dlg;
		      if (theScene.getSelection().length > 0)
		        dlg = new ComponentsDialog(parent, Translate.text("InjectToM2"), 
		  	  new Widget [] {modelname,   injectChoice, UVChoice,BoneChoice,BoundChoice},
		  	  new String [] {null, null, null, null, null});
		      else
		        dlg = new ComponentsDialog(parent, Translate.text("exportToM2"), 
		  	  new Widget [] { modelname, injectChoice, UVChoice,BoneChoice,BoundChoice},
		  	  new String [] {null, null, null, null});
		      if (!dlg.clickedOk())
		        return;
		      
		   // Ask the user to select the output file.

		      BFileChooser fc = new BFileChooser(BFileChooser.SAVE_FILE, Translate.text("exportToM2"));
		      fc.setSelectedFile(new File(modelname.getText()+".m2"));
		      if (ArtOfIllusion.getCurrentDirectory() != null)
		        fc.setDirectory(new File(ArtOfIllusion.getCurrentDirectory()));
		      if (!fc.showDialog(parent))
		        return;
		      File dir = fc.getDirectory();
		      f = fc.getSelectedFile();
		      String name = f.getName();
		      String baseName = (name.endsWith(".m2") ? name.substring(0, name.length()-3) : name);
		      ArtOfIllusion.setCurrentDirectory(dir.getAbsolutePath());

		      m2 obj = null;
		      skin info = null;
		      if(injectChoice.getState()==true){
		      try {
				obj= new m2( fileLoader.openBuffer(f.getAbsolutePath()));
		      } catch (InvalidClassException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
		      }
		      writeScene(theScene, obj, false, 0.05, false, injectChoice.getState(),null,UVChoice.getState(),BoneChoice.getState(),BoundChoice.getState()); 
		      }
		      else{
		    	  obj = new m2();
		    	  obj.setModelName(modelname.getText());
		    	  info = new skin();
		    	  writeScene(theScene, obj, false, 0.05, false, injectChoice.getState(),info, UVChoice.getState(),BoneChoice.getState(),BoundChoice.getState()); 
		      }
	  }
	  public static void writeScene(Scene theScene, m2 out, boolean wholeScene, double tol, boolean smooth, boolean inject,skin data, boolean uv,boolean skel,boolean bounding)
	  {

		// Write the objects in the scene.
		  if(inject==true){
		    int numVert = 0, numNorm = 0, numTexVert = 0;
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
		          
		          System.out.println("nVertixes:\t"+out.vertex.length);
		          for (int j = 0; j < vert.length; j++)
		            {
		              Vec3 v = trans.times(vert[j].r);
		              float[] tf= new float[3];
		              tf[0]=(float) v.x;
		              tf[1]=(float) v.y;
		              tf[2]=(float) v.z;
		              out.vertex[j].setVertexPos(tf);
		              
		            }
		          		          
		          numVert += vert.length;
		          numNorm += norm.length;
		        }
		    save(out,null);
		  }
		  else{
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
			    int bonecount=0;
			    Joint joints[][]=new Joint[nsubfiles][];
		        MeshVertex vert[][] = new MeshVertex[nsubfiles][];
		        Vec3 norm[][]=new Vec3[nsubfiles][];
		        Mat4 trans[]=new Mat4[nsubfiles];
		        data=new skin(nsubfiles,nsubfiles);
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
			          data.submeshes[cur].setnVertices((short) vert[cur].length);
			          data.submeshes[cur].setStartVertex((short) numVert);
			          data.texunits[cur].setSubmeshIndex((short)cur);
			          
			          
			          
			          if(skel==true){
			        	  Skeleton skelett=mesh.getSkeleton();
			        	  bonecount+=skelett.getNumJoints();
			        	  joints[cur]=new Joint[skelett.getNumJoints()];
			        	  for(int n=0;n<skelett.getNumJoints();n++){
			        		  joints[cur][n]=skelett.getJoint(n);
			        	  }
			          }
			          
			          if(uv==true){
			        	  
				          UVMapping uvmap=(UVMapping) info.getObject().getTextureMapping();      
				          Vec2 uvs[]=uvmap.findTextureCoordinates(mesh);
				          uvcoord[cur]=new float[uvs.length][2];
				          for(int n=0;n<uvs.length;n++){
				        	  float[] txcoord={Math.abs((float) uvs[n].x),Math.abs((float) uvs[n].y)};
				        	  uvcoord[cur][n]=txcoord;
				         }
				      }
			          
			          faceCount[cur]=new short[mesh.getFaceCount()][3];
			          numTriangles+=mesh.getFaceCount();
			          for(int f=0;f<mesh.getFaceCount();f++){
			        	  faceCount[cur][f][0]=(short)(numVert+mesh.getFaceVertexIndex(f, 0));
			        	  faceCount[cur][f][1]=(short)(numVert+ mesh.getFaceVertexIndex(f, 1));
			        	  faceCount[cur][f][2]=(short) (numVert+mesh.getFaceVertexIndex(f, 2));
			          }
			          numNorm+=mesh.getNormals().length;
			          norm[cur]=mesh.getNormals();
			          
			          boundingb.extend(info.getBounds());
			          		          
			          numVert += vert[cur].length;
			          cur++;
			        }
			    out.setnVertices(numVert);
		        data.setnVertex(numVert);
		        data.setnProperties(numVert); 
		        data.setnTriangles(numTriangles);
		        if(bounding){
		        out.setnBoundingTriangles(numTriangles);
		        out.setnBoundingVertices(numVert);
		        out.setnBoundingNormals(numNorm);
		        }
		        
		        float[] boundmax={-9999,-9999,-9999};
		        float[] boundmin={9999,9999,9999};

		        
		        out.header.setBoundingBox(boundmax, boundmin);
		        out.header.setCamInfo(boundmax, boundmin);
		        int vertexposcount=0;
		        int faceposcount=0;
		        int normposcount=0;
		        int bonepos=0;
		        if(skel)out.setnBones(bonecount);
			    for(int i=0;i<nsubfiles;i++){
			     data.submeshes[i].setnTriangles((short) (3*faceCount[i].length)); 
			     data.submeshes[i].setStartTriangle((short) faceposcount);
			     for (int j = 0; j < faceCount[i].length; j++)
		         {
		             data.setTriangle((faceposcount/3)+j, faceCount[i][j]);
		             if(bounding)out.setBoundingTriangle((faceposcount/3)+j,faceCount[i][j]);
		         }
			     for(int j=0;j<norm[i].length;j++){
			    	 float enorm[]={(float) norm[i][j].x,(float) norm[i][j].y,(float) norm[i][j].z};
			    	 if(bounding)out.setBoundingNormal(normposcount+j, enorm);
			     }
			     normposcount+=norm[i].length;
			     faceposcount+=3*faceCount[i].length;
			     if(uv)
				     for(int n=0;n<uvcoord[i].length;n++){
				    	 out.vertex[vertexposcount+n].setTexCoord(uvcoord[i][n]);
				     }
			     if(skel)
			    	 for(int n=0;n<joints[i].length;i++){
			    		 out.bone[bonepos+i]=new Bones();
			    	 }
			     for (int j = 0; j < vert[i].length; j++)
		            {
		              Vec3 v = trans[i].times(vert[i][j].r);
		              float[] tf= new float[3];
		              tf[0]=(float) v.x;
		              tf[1]=(float) v.y;
		              tf[2]=(float) v.z;
		              out.vertex[vertexposcount+j].setVertexPos(tf);
		              if(bounding)out.setBoundingVertice((vertexposcount+j),tf);
		              data.setVertex(vertexposcount+j, (short) (vertexposcount+j));
		              
		            }
			     vertexposcount+=vert[i].length;
			    }
			    data.render();
			    save(out,data);
		  }
	  }
	  
	  public static int save(m2 out,skin data) {
			//TODO: catch failures and so on
			
				
				out.render();
				fileLoader.saveBuffer(out.buff , f.getAbsolutePath());
				if(data!=null){
					data.render();
					fileLoader.saveBuffer(data.buff , f.getAbsolutePath().substring(0, f.getAbsolutePath().length()-3)+"00.skin");
				}
				return 1;
			
		}
}