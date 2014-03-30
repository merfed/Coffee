package artofillusion.translators;

import artofillusion.*;
import artofillusion.animation.*;
import artofillusion.image.ImageMap;
import artofillusion.image.ImageOrColor;
import artofillusion.image.ImageOrValue;
import artofillusion.math.*;
import artofillusion.object.*;
import artofillusion.texture.ImageMapTexture;
import artofillusion.texture.Texture;
import artofillusion.texture.Texture2D;
import artofillusion.texture.UVMapping;
import artofillusion.texture.UniformTexture;
import artofillusion.ui.*;
import buoy.widget.*;
import starlight.taliis.core.files.*;
import starlight.taliis.helpers.fileLoader;

import java.io.*;
import java.util.*;

/**
 * A simple importplugin for World of Warcrafts *.wmo-files
 * 
 * @author Tigurius
 */

/** WMOImporter implements the importing of WMO files. */

public class M2Importer
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
    //info = new ObjectInfo(new DirectionalLight(new RGBColor(1.0f, 1.0f, 1.0f), 0.8f), coords.duplicate(), "Light 1");
    //info.addTrack(new PositionTrack(info), 0);
    //info.addTrack(new RotationTrack(info), 1);
    //theScene.addObject(info, null);

    // Open the file and read the contents.
    
    Hashtable groupTable = new Hashtable(), textureTable = new Hashtable();
    Vector vertex = new Vector(), normal = new Vector(), texture = new Vector(), face = new Vector();
    Joint bones[];
    groupTable.put("default", face);
    int lineno = 0, smoothingGroup = -1;

    double val[] = new double [3];
    double min[] = new double [] {Double.MAX_VALUE, Double.MAX_VALUE, Double.MAX_VALUE};
    double max[] = new double [] {-Double.MAX_VALUE, -Double.MAX_VALUE, -Double.MAX_VALUE};
    String s;
    m2 obj = null;
    try {
		obj= new m2( fileLoader.openBuffer(f.getAbsolutePath()));
	} catch (InvalidClassException e) {
		// TODO Auto-generated catch block
		System.out.println("Couldn't load File");
	}
	skin infos[]= new skin[obj.header.getnViews()];
	for(int i=0;i<obj.header.getnViews();i++){
		String str= f.getAbsolutePath();
		str=str.substring(0, str.length()-3);
		str=str+"0"+i+".skin";
		try {
			infos[i]= new skin( fileLoader.openBuffer(str));
		} catch (InvalidClassException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	System.out.println("nVertices:\t"+obj.header.getnVertices());
	for(int i=0;i<obj.header.getnVertices();i++){
		vertex.addElement(new Vec3(obj.vertex[i].getVertexPos()[0], obj.vertex[i].getVertexPos()[1], obj.vertex[i].getVertexPos()[2]));
	}
	/*System.out.println("nNormals:\t"+obj.monr.nNormals);
	for(int i=0;i<obj.monr.nNormals;i++){
			normal.addElement(new Vec3(obj.monr.getNormalPos(i)[0], obj.monr.getNormalPos(i)[1], obj.monr.getNormalPos(i)[2]));
	}*/
	for(int j=0;j<infos.length;j++){
	System.out.println("nTriangles:\t"+infos[j].header.getnTriangles()/3);
	for(int i=0;i<infos[j].header.getnTriangles()/3;i++){
		face.addElement(new Vec3(infos[j].triangles[i][0],infos[j].triangles[i][1],infos[j].triangles[i][2]));
	}}
	
	
	
	// Create a triangle mesh for each group.
   
    Enumeration keys = groupTable.keys();
    Hashtable realizedTextures = new Hashtable();
    Hashtable imageMaps = new Hashtable();
    while (keys.hasMoreElements())
      {
        String group = (String) keys.nextElement();
        Vector groupFaces = (Vector) groupTable.get(group);
        if (groupFaces.size() == 0)
          continue;
        
        // Find which vertices are used by faces in this group.
        
        int realIndex[] = new int[obj.header.getnVertices()];
        for (int i = 0; i < realIndex.length; i++)
          realIndex[i] = i;
        int numVert = obj.header.getnVertices();
        int fc[][] = new int [infos[0].header.getnTriangles()/3][];
        for (int i = 0; i < fc.length; i++)
          {
            fc[i] = new int [] {realIndex[infos[0].indices[infos[0].triangles[i][0]]], realIndex[infos[0].indices[infos[0].triangles[i][1]]], realIndex[infos[0].indices[infos[0].triangles[i][2]]]};
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
        coords = new CoordinateSystem(center, Vec3.vz(), Vec3.vy());
        

        //bone==joint
         Skeleton skel = new Skeleton();
        bones=new Joint[obj.header.getnBones()];
        for(int i=0;i<obj.header.getnBones();i++){
        	if(obj.bone[i].getParent()==-1){
    		bones[i]=new Joint(coords,null,"Bone_"+String.valueOf(i));
    		System.out.println("Created Bone:\t"+i);
    		skel.addJoint(bones[i], -1);
        	}
    		else
    		{
    		bones[i]=new Joint(coords,bones[obj.bone[i].getParent()],"Bone_"+String.valueOf(i));
    		System.out.println("Created Bone:\t"+i);
    		skel.addJoint(bones[i], obj.bone[i].getParent());
    	}}
        TriangleMesh model = new TriangleMesh(vert,fc);
        model.setSkeleton(skel);
        MeshVertex[] vertices = model.getVertices();
        //atm I attach the vertex just to the first bone <.<
        //but wow attaches them sometimes to four
        /*for(int i=0;i<obj.header.getnVertices();i++){
        	vertices[i].attachBone(bones[obj.bonelookup[obj.vertex[i].getBoneIndex()]].id);
        }
        model.setVertices(vertices);*/
        info = new ObjectInfo(model, coords, ("default".equals(group) ? wmoName : group));
        info.addTrack(new PositionTrack(info), 0);
        info.addTrack(new RotationTrack(info), 1);
        
        
        // Find the smoothness values for the edges.
        
        TriangleMesh.Edge edge[] = ((TriangleMesh) info.getObject()).getEdges();
        for (int i = 0; i < edge.length; i++)
          {
                edge[i].smoothness = 0.0f;
                continue;

          }
        
        // Set the texture.  For the moment, assume a single texture per group.  In the future, this could possibly
        // be improved to deal correctly with per-face textures.
        
        String texName = groupFaces.elementAt(0).toString();
        if (texName != null && textureTable.get(texName) != null)
          {
            Texture tex = (Texture) realizedTextures.get(texName);
            if (tex == null)
              {
                try {
					tex = createTexture((TextureInfo) textureTable.get(texName), theScene, bfc.getDirectory(), imageMaps, parent);
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
                realizedTextures.put(texName, tex);
              }
            if (tex instanceof Texture2D)
              {
                // Set the UV coordinates.
                
                UVMapping map = new UVMapping(info.getObject(), tex);
                info.setTexture(tex, map);
                Vec2 uv[] = new Vec2 [obj.header.getnVertices()];

                for (int j = 0; j < obj.header.getnVertices(); j++)
                  {
                        Vec2 tc = new Vec2(obj.vertex[j].getTexCoord()[0],obj.vertex[j].getTexCoord()[1]);
                         uv[realIndex[j]] = tc;
                      
                  }
                
                  map.setTextureCoordinates(info.getObject(), uv);
              }
            else
              info.setTexture(tex, tex.getDefaultMapping(info.getObject()));
          }
        theScene.addObject(info, null);
        ArtOfIllusion.newWindow(theScene);
        return;
            
}}
  
  
/** Create a texture from a TextureInfo and add it to the scene. */
  
  private static Texture createTexture(TextureInfo info, Scene scene, File baseDir, Hashtable imageMaps, BFrame parent) throws Exception
  {
    info.resolveColors();
    ImageMap diffuseMap = loadMap(info.diffuseMap, scene, baseDir, imageMaps, parent);
    ImageMap specularMap = loadMap(info.specularMap, scene, baseDir, imageMaps, parent);
    ImageMap transparentMap = loadMap(info.transparentMap, scene, baseDir, imageMaps, parent);
    ImageMap bumpMap = loadMap(info.bumpMap, scene, baseDir, imageMaps, parent);
    RGBColor transparentColor =  new RGBColor(info.transparency, info.transparency, info.transparency);
    if (diffuseMap == null && specularMap == null && transparentMap == null && bumpMap == null)
      {
        // Create a uniform texture.
        
        UniformTexture tex = new UniformTexture();
        tex.diffuseColor = info.diffuse.duplicate();
        tex.specularColor = info.specular.duplicate();
        tex.transparentColor = transparentColor;
        tex.shininess = (float) info.specularity;
        tex.specularity = 0.0f;
        tex.roughness = info.roughness;
        tex.setName(info.name);
        scene.addTexture(tex);
        return tex;
      }
    else
      {
        // Create an image mapped texture.

        ImageMapTexture tex = new ImageMapTexture();
        tex.diffuseColor = (diffuseMap == null ? new ImageOrColor(info.diffuse) : new ImageOrColor(info.diffuse, diffuseMap));
        tex.specularColor = (specularMap == null ? new ImageOrColor(info.specular) : new ImageOrColor(info.specular, specularMap));
        tex.transparentColor = (transparentMap == null ? new ImageOrColor(transparentColor) : new ImageOrColor(transparentColor, transparentMap));
        if (bumpMap != null)
          tex.bump = new ImageOrValue(1.0f, bumpMap, 0);
        tex.shininess = new ImageOrValue((float) info.specularity);
        tex.specularity = new ImageOrValue(0.0f);
        tex.roughness = new ImageOrValue((float) info.roughness);
        tex.tileX = tex.tileY = true;
        tex.mirrorX = tex.mirrorY = false;
        tex.setName(info.name);
        scene.addTexture(tex);
        return tex;
      }
  }  
/** Return the image map corresponding to the specified filename, and add it to the scene. */
  
  private static ImageMap loadMap(String name, Scene scene, File baseDir, Hashtable imageMaps, BFrame parent) throws Exception
  {
    if (name == null)
      return null;
    ImageMap map = (ImageMap) imageMaps.get(name);
    if (map != null)
      return map;
    File f = new File(baseDir, name);
    if (!f.isFile())
      f = new File(name);
    if (!f.isFile())
      throw new Exception("Cannot locate image map file '"+name+"'.");
    try
      {
        map = ImageMap.loadImage(f);
      }
    catch (InterruptedException ex)
      {
        throw new Exception("Unable to load image map file '"+f.getAbsolutePath()+"'.");
      }
    scene.addImage(map);
    imageMaps.put(name, map);
    return map;
  }

/** Inner class for storing information about a texture in a .mtl file. */

private static class TextureInfo
{
  public String name;
  public RGBColor ambient, diffuse, specular;
  public double shininess, transparency, specularity, roughness;
  public String ambientMap, diffuseMap, specularMap, transparentMap, bumpMap;
  
  /** This should be called once, after the TextureInfo is created but before it is actually used.  It converts from the
      representation used by .obj files to the one used by Art of Illusion. */
      
 public void resolveColors()
  {
    if (diffuse == null)
      diffuse = new RGBColor(0.0, 0.0, 0.0);
    if (ambient == null)
      ambient = new RGBColor(0.0, 0.0, 0.0);
    if (specular == null)
      specular = new RGBColor(0.0, 0.0, 0.0);
    else
      specularity = 1.0;
    diffuse.scale(1.0-transparency);
    specular.scale(1.0-transparency);
    roughness = 1.0-(shininess-1.0)/128.0;
    if (roughness > 1.0)
      roughness = 1.0;
    checkColorRange(ambient);
    checkColorRange(diffuse);
    checkColorRange(specular);
  }
  
  /** Make sure that the components of a color are all between 0 and 1. */
  
 private void checkColorRange(RGBColor c)
  {
    float r = c.getRed(), g = c.getGreen(), b = c.getBlue();
    if (r < 0.0f) r = 0.0f;
    if (r > 1.0f) r = 1.0f;
    if (g < 0.0f) g = 0.0f;
    if (g > 1.0f) g = 1.0f;
    if (b < 0.0f) b = 0.0f;
    if (b > 1.0f) b = 1.0f;
    c.setRGB(r, g, b);
  }
}
}  
  

