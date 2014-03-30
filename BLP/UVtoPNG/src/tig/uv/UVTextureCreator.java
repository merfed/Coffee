package tig.uv;


import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.io.InvalidClassException;

import javax.imageio.ImageIO;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;
import javax.swing.filechooser.FileFilter;

import de.taliis.plugins.wowimage.blp;

import starlight.taliis.core.files.m2;
import starlight.taliis.core.files.skin;
import starlight.taliis.helpers.fileLoader;




public class UVTextureCreator extends JFrame implements ActionListener {
	/**
	 * 
	 */
	private static final long serialVersionUID = 223693395981270554L;
	JPanel mainPanel;
	JButton modelButton, texButton;
	JButton saveButton;
	JScrollPane scr;
	Icon icon;
	JLabel label;
	JComboBox colorChoose;
	GridBagConstraints c;
	
	BufferedImage img;
	int ColorData[][];
	
	Color TRed	=	new Color(0xFFFF0000);
	Color TGreen=	new Color(0xFF00FF00);
	Color TBlue	=	new Color(0xFF0000FF);
	
	Color usedColor = TRed;
	int xres=512;
	int yres=512;
	
	public UVTextureCreator(){
		super("UV to PNG");
		mainPanel = new JPanel();
		mainPanel.setLayout(new GridBagLayout());
		c = new GridBagConstraints();
		
		String[] cNames={"Red","Green","Blue"};
		colorChoose=new JComboBox(cNames);
		colorChoose.addActionListener(this);
		mainPanel.add(colorChoose,c);

		c.gridx=5;
		texButton = new JButton("Load Texture");
		texButton.addActionListener(this);
		mainPanel.add(texButton,c);
		c.gridx+=5;
		modelButton = new JButton("Load Model");
		modelButton.addActionListener(this);
		mainPanel.add(modelButton,c);
		c.gridx+=5;
		saveButton = new JButton("Save UVTexture");
		saveButton.addActionListener(this);
		mainPanel.add(saveButton,c);
		
		ColorData = new int[xres][yres];
		for(int i=0;i<xres;i++){
			for(int j=0;j<yres;j++){
				ColorData[i][j]=0xFFFFFF;
			}
		}
		
		img = new BufferedImage(xres,yres, BufferedImage.TYPE_INT_ARGB);
		for(int i=0;i<xres;i++){
			for(int j=0;j<yres;j++){
				img.setRGB(i, j, ColorData[i][j]);
			}
		}

		
		// create panel, display
		icon = new ImageIcon(img);
		label = new JLabel(icon);
		label.setMinimumSize(new Dimension(xres, yres));


		scr = new JScrollPane(label, ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS, ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
		scr.setPreferredSize(new Dimension(xres, yres));
		c.gridx=0;
		c.gridy=10;
		mainPanel.add(scr,c);
		getContentPane().add(mainPanel, BorderLayout.NORTH);
	}

	@Override
	public void actionPerformed(ActionEvent arg0) {
		if(arg0.getSource()==modelButton){
			openM2();
		}
		else if(arg0.getSource()==texButton){
			openTex();
		}
		else if(arg0.getSource()==saveButton){
			saveUVTex();
		}
		else if(arg0.getSource()==colorChoose){
			switch(colorChoose.getSelectedIndex()){
			case 0:
				usedColor=TRed;break;
			case 1:
				usedColor=TGreen;break;
			case 2:
				usedColor=TBlue;break;
			default:
				break;
			}
		}

	}
	
	void updateImage(){
		mainPanel.remove(scr);
		getContentPane().remove(mainPanel);
		
		// create panel, display
		icon = new ImageIcon(img);
		label = new JLabel(icon);
		label.setMinimumSize(new Dimension(xres, yres));


		scr = new JScrollPane(label, ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS, ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
		scr.setPreferredSize(new Dimension(xres, yres));
		mainPanel.add(scr,c);
		getContentPane().add(mainPanel, BorderLayout.NORTH);
		//--HACK
		resize(new Dimension(this.getWidth()+1,this.getHeight()));

	}
	
	
	int openM2(){
		JFileChooser fc = new JFileChooser();

		FileFilter f=new FileFilter() {
			public boolean accept(File f) {
				if (f.isDirectory()) {
		            return true;
		        }
				
				String name = f.getName();
				
				if(name.endsWith(".m2")||name.endsWith(".M2")) return true;
				else return false;
          }

			
			public String getDescription() {
				return "WoW Model Files";
			}
		};
		fc.addChoosableFileFilter(f);
		fc.setFileFilter( fc.getAcceptAllFileFilter() );
		fc.showOpenDialog(this);
		if(fc.getSelectedFile()==null){
			System.out.println("Died!");
			return -1;
		}
		System.out.println(fc.getSelectedFile().getAbsolutePath());
		m2 m=null;
		try {
			m=new m2(fileLoader.openBuffer(fc.getSelectedFile().getAbsolutePath()));
		} catch (InvalidClassException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		skin s = null;
		String st=fc.getSelectedFile().getAbsolutePath().substring(0, fc.getSelectedFile().getAbsolutePath().length()-3)+"00.skin";
		try {
			s=new skin(fileLoader.openBuffer(st));
		} catch (InvalidClassException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Graphics2D g=img.createGraphics();
		for(int i=0;i<s.triangles.length;i++){
		
		int px1=(int)(m.vertex[s.indices[s.triangles[i][0]]].getTexCoord()[0]*xres);
		int px2=(int)(m.vertex[s.indices[s.triangles[i][1]]].getTexCoord()[0]*xres);
		int px3=(int)(m.vertex[s.indices[s.triangles[i][2]]].getTexCoord()[0]*xres);
		int py1=(int)(m.vertex[s.indices[s.triangles[i][0]]].getTexCoord()[1]*yres);
		int py2=(int)(m.vertex[s.indices[s.triangles[i][1]]].getTexCoord()[1]*yres);
		int py3=(int)(m.vertex[s.indices[s.triangles[i][2]]].getTexCoord()[1]*yres);
		g.setColor(usedColor);
		g.drawLine(px1, py1, px2, py2);
		g.drawLine(px3, py3, px2, py2);
		g.drawLine(px1, py1, px3, py3);
		}
		//g.create();
		//updateImage();

	
		return 1;
		
	}
	
	void saveUVTex(){
		JFileChooser fc = new JFileChooser();
		fc.setFileFilter( fc.getAcceptAllFileFilter() );
		fc.showSaveDialog(this);
		if(fc.getSelectedFile()==null){
			System.out.println("Died!");
			return;
		}
		System.out.println(fc.getSelectedFile().getAbsolutePath());

		File file = new File(fc.getSelectedFile().getAbsolutePath());

        try {
			ImageIO.write(img, "png", file);
		} catch (IOException e) {
			e.printStackTrace();
		}
			
	}
	
	void openTex(){
		JFileChooser fc = new JFileChooser();

		FileFilter f=new FileFilter() {
			public boolean accept(File f) {
				if (f.isDirectory()) {
		            return true;
		        }
				
				String name = f.getName();
				
				if(name.endsWith(".blp")||name.endsWith(".BLP")) return true;
				else return false;
          }

			
			public String getDescription() {
				return "WoW Texture Files";
			}
		};
		fc.addChoosableFileFilter(f);
		fc.setFileFilter( fc.getAcceptAllFileFilter() );
		fc.showOpenDialog(this);
		if(fc.getSelectedFile()==null){
			System.out.println("Died!");
			return ;
		}
		System.out.println(fc.getSelectedFile().getAbsolutePath());

		blp bim;
		bim=new blp(fc.getSelectedFile());
		img=bim.getImg();
		xres=img.getWidth();
		yres=img.getHeight();
		updateImage();
	}
		
	
}
