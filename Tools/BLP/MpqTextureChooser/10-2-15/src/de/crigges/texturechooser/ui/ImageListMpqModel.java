package de.crigges.texturechooser.ui;

import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

import javax.swing.AbstractListModel;
import javax.swing.Icon;
import javax.swing.ImageIcon;

import com.google.common.cache.CacheBuilder;
import com.google.common.cache.CacheLoader;
import com.google.common.cache.LoadingCache;

import de.ogerlord.wcdatalib.image.BlpFile;
import de.ogerlord.wcdatalib.image.TgaFile;
import de.peeeq.jmpq3.JMpqEditor;
import de.peeeq.jmpq3.JMpqException;
import de.peeeq.jmpq3.MpqFile;

public class ImageListMpqModel extends AbstractListModel<Icon> {
	private static final long serialVersionUID = 1L;
	private ArrayList<JMpqEditor> mpqs = new ArrayList<>();
	private ArrayList<String> mergedListFile = new ArrayList<>();
	private ArrayList<String> filteredListFile = new ArrayList<>();
	private LoadingCache<String, ImageIcon> imageCacher;
	private ImageIcon defaultImage = createDefaultImage();
	private static final int imageScaleX = 64;
	private static final int imageScaleY = imageScaleX;
	private ExecutorService fileLoader = Executors.newSingleThreadExecutor();
	private ExecutorService imageLoader = Executors.newFixedThreadPool(2);


	public ImageListMpqModel(File... files) throws JMpqException {
		for (File f : files) {
			mpqs.add(new JMpqEditor(f));
		}
		mergeListFiles();
		CacheBuilder<Object, Object> builder = CacheBuilder.newBuilder();
		builder.maximumSize(1000).expireAfterWrite(10, TimeUnit.MINUTES);
		imageCacher = builder.build(new CacheLoader<String, ImageIcon>() {
					public ImageIcon load(String key){
						return loadImage(key);
					}
				});
	}

	private void mergeListFiles() {
		for (JMpqEditor e : mpqs) {
			for (String s : e.getFileNames()) {
				s = s.toLowerCase();
				if ((s.endsWith(".tga") || s.endsWith(".blp")) && !mergedListFile.contains(s)) {
					mergedListFile.add(s);
				}
			}
		}
	}

	private ImageIcon createDefaultImage() {
		return new MpqImageIcon(new BufferedImage(imageScaleX, imageScaleY, BufferedImage.TYPE_INT_RGB), "Invaild");
	}

	public void filterByString(String filter) {
		filter = filter.toLowerCase();
		filteredListFile.clear();
		for (String s : mergedListFile) {
			if (s.contains(filter)) {
				filteredListFile.add(s);
			}
		}
	}

	public ImageIcon getDefaultImage() {
		return defaultImage;
	}

	private BufferedImage resize(BufferedImage original, int newWidth,
			int newHeight) {
		BufferedImage resized = new BufferedImage(newWidth, newHeight,
				original.getType());
		Graphics2D g = resized.createGraphics();
		g.setRenderingHint(RenderingHints.KEY_INTERPOLATION,
				RenderingHints.VALUE_INTERPOLATION_BILINEAR);
		g.drawImage(original, 0, 0, newWidth, newHeight, 0, 0,
				original.getWidth(), original.getHeight(), null);
		g.dispose();
		return resized;
	}
	
	private ImageIcon loadImage(String path){
		final String fixedPath = path;
		final ImageIcon icon = new MpqImageIcon(defaultImage.getImage(), path);
		fileLoader.execute(new Runnable() {
			
			@Override
			public void run() {
				MpqFile file = null;
				ByteArrayInputStream inStream = null;
				for (JMpqEditor editor : mpqs) {
					if (editor.hasFile(fixedPath)) {
						try {
							file = editor.getMpqFile(fixedPath);
							ByteArrayOutputStream outStream = new ByteArrayOutputStream(file.getNormalSize());
							file.extractToOutputStream(outStream);
							inStream = new ByteArrayInputStream(outStream.toByteArray());
							break;
						} catch (IOException e) {}
					}
				}
				if(file != null){
					imageLoader.execute(new LoadTask(file, inStream, icon));
				}
			}
		});
		return icon;
	}

	@Override
	public int getSize() {
		return filteredListFile.size();
	}

	@Override
	public Icon getElementAt(int index) {
		String s = filteredListFile.get(index);
		try {
			return imageCacher.get(s);
		} catch (ExecutionException e) {
			return defaultImage;
		}
	}

	private class LoadTask implements Runnable{
		private ByteArrayInputStream stream;
		private ImageIcon icon;
		private MpqFile file;
		
		private LoadTask(MpqFile file, ByteArrayInputStream stream, ImageIcon icon) {
			this.file = file;
			this.stream = stream;
			this.icon = icon;
		}

		@Override
		public void run() {
			try {
				BufferedImage img;
				if(file.getName().toLowerCase().endsWith(".blp")){
					img = BlpFile.read("Picture", stream);
				}else{
					try{
						img = TgaFile.readTGA("Picture", stream);
					}catch(IllegalStateException e){
						return;
					}
				}
				img = resize(img, imageScaleX, imageScaleY);
				icon.setImage(img);
				fireContentsChanged(icon, 0, mergedListFile.size() - 1);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
	}
	
}
