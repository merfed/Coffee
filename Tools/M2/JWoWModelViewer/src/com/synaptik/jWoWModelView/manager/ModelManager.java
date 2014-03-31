package com.synaptik.jWoWModelView.manager;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import com.synaptik.jWoWModelView.models.Model;
import com.synaptik.jWoWModelView.models.ModelException;
import com.synaptik.jWoWModelView.models.m2.M2Model;

/**
 * TODO:
 *	- Make it actually manage the models.
 *		+ Example, a request is made for a specific model, it should then load it.
 *		+ When a specific model is no longer used after an amount of time, free the
 *			associated resources.
 * 
 * @author Dan Watling (dwatling@synaptik.com)
 */
public class ModelManager {
	public Map<String,Model> models = null;
	public String basePath = null;
	
	public ModelManager(String basePath) {
		models = new HashMap<String,Model>();
		this.basePath = basePath;
	}
	
	public void loadModels() throws ModelException, IOException {
		File f = new File(basePath);
		loadModels(f);
	}
	
	protected void loadModels(File f) throws ModelException, IOException {
		if (f.isFile()) {
			if (f.getAbsolutePath().endsWith("m2")) {
				addModel(f.getAbsolutePath());
			}
		} else if (f.isDirectory()) {
			File[] files = f.listFiles();
			for (int index = 0; index < files.length; index ++) {
				loadModels(files[index]);
			}
		}
	}
	
	public void addModel(String filename) throws ModelException, IOException {
		FileInputStream fis = null;
		ByteBuffer bb = null;
		try {
			File f = new File(filename);
			fis = new FileInputStream(f);
			
			bb = ByteBuffer.allocate((int)f.length());
			bb.order(ByteOrder.LITTLE_ENDIAN);
			
			byte[] bytes = new byte[(int)f.length()];
			fis.read(bytes);
			bb.put(bytes);
			bb.rewind();
			
			M2Model m = new M2Model();
			m.load(bb);
			models.put(m.getName(), m);
			
			System.out.println("Added model " + filename + " (Name: " + m.getName() + "; Animation count: " + m.animations.size() + ")");
		} finally {
			if (fis != null) {
				fis.close();
			}
		}
	}
	
	public Model getModel(String filename) {
		return models.get(filename);
	}
	
	public Collection<Model> getModels() {
		return models.values();
	}
	
	public void removeModel(String filename) {
		// TODO -- Is this all we really have to do? Won't jME need to know to toss the TriMesh?
		models.remove(filename);
	}
}
