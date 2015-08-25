package adtconverter;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.Map;

/**
 * @author Mjollnà
 */

public class Ctex0 extends CataFile {

	// Attributes

	MAMPChunk mamp;
	MTEXChunk mtex;	
	ArrayList<ArrayList<Chunk>> mcnk;
	Map<String, Integer> mcnkChunkNames;
	MCLYChunk mcly;
	MCALChunk mcal;
	MCSHChunk mcsh;	
	MTXFChunk mtxf;
	
	// Constructor
	
	public Ctex0(MVERChunk mver, String fileName, MTEXChunk mtex, ArrayList<ArrayList<Chunk>> mcnk, Map<String, Integer> mcnkChunkNames, MTXFChunk mtxf) {
		super(mver, fileName);

		// Creating MAMP out of nothing
		this.mamp = new MAMPChunk();
		
		this.mtex = mtex;
		this.mcnk = mcnk;
		this.mcnkChunkNames = mcnkChunkNames;
                this.mtxf = mtxf;
		
		this.fileType = "_tex0";	
		
	}
	
	// Methods

	@Override
	public void buildByteFile() {
		
            try {
                
                RandomAccessFile raf = new RandomAccessFile(ADTConverterApp.config.getProperty("directory")+ADTConverterApp.relativeDir+this.fileName+this.fileType+".adt", "rw");                
                
		//MVER
		raf.write(ADTConverterApp.reverseArray(mver.getChunkCode()));
		raf.write(mver.getChunkData());
		
		//MAMP
		raf.write(ADTConverterApp.reverseArray(mamp.getChunkCode()));						
		raf.write(mamp.getChunkData());
		
		// MTEX
		raf.write(ADTConverterApp.reverseArray(mtex.getChunkCode()));	
		raf.write(toLittleEndianBytes(mtex.getSize()));	
		raf.write(mtex.getChunkData());	
		
		// 256(MCNK + MCLY + MCAL + MCSH)
		
		// Create empty MCNK, MCLY, MCAL and MCSH to paste every time its necessary.
		byte[] emptyChunk = {0x0,0x0,0x0,0x0};
		
		MCNKChunk emptyMcnk = new MCNKChunk(0, 8, emptyChunk);
		MCLYChunk emptyMcly = new MCLYChunk(0, 8, emptyChunk);
		MCALChunk emptyMcal = new MCALChunk(0, 8, emptyChunk);		
		MCSHChunk emptyMcsh = new MCSHChunk(0, 8, emptyChunk);	
		
		for (int i = 0 ; i <= 255 ; i++) {

			// ---------------------------------------------------			
			// See what we have in the file and find the size of MCNK
			// ---------------------------------------------------			
			
			int mcnksize = 0;
			
				// MCLY
			if (mcnk.get(i).get(mcnkChunkNames.get("MCLY")).getSize() == 0) {
				mcnksize = mcnksize + 8;
			}
			else {
				mcnksize = mcnksize + 8 + mcnk.get(i).get(mcnkChunkNames.get("MCLY")).getSize();
			}
			
				// MCSH
			if (mcnk.get(i).get(mcnkChunkNames.get("MCSH")) == null) {
				// do nothing
			}
			else if (mcnk.get(i).get(mcnkChunkNames.get("MCSH")).getSize() == 0) {
				// do nothing
			}
			else if(mcnk.get(i).get(mcnkChunkNames.get("MCSH")).isEmpty()) {
				// do nothing, empty MCSH == no MCSH
			}			
			else {
				mcnksize = mcnksize + 8 + mcnk.get(i).get(mcnkChunkNames.get("MCSH")).getSize();
			}
			
				// MCAL
			if (mcnk.get(i).get(mcnkChunkNames.get("MCAL")) == null) {
				// do nothing
			}
			else if (mcnk.get(i).get(mcnkChunkNames.get("MCAL")).getSize() == 0) {
				// do nothing
			}
			else {
				mcnksize = mcnksize + 8 + mcnk.get(i).get(mcnkChunkNames.get("MCAL")).getSize();
			}
			
			// ---------------------------------------------------
			// Add chunks to file
			// ---------------------------------------------------			
			
			// Add MCNK
			raf.write(ADTConverterApp.reverseArray(emptyMcnk.getChunkCode()));			
			raf.write(toLittleEndianBytes(mcnksize));
			
			// Add MCLY
			if (mcnk.get(i).get(mcnkChunkNames.get("MCLY")).getSize() == 0) {
                                raf.write(ADTConverterApp.reverseArray(emptyMcly.getChunkCode()));
                                raf.write(emptyChunk);                                
			}
			else {
				raf.write(ADTConverterApp.reverseArray(emptyMcly.getChunkCode()));	
				raf.write(toLittleEndianBytes(mcnk.get(i).get(mcnkChunkNames.get("MCLY")).getSize()));	
				raf.write(mcnk.get(i).get(mcnkChunkNames.get("MCLY")).getChunkData());	
			}
			
			// Add MSCH
			if (mcnk.get(i).get(mcnkChunkNames.get("MCSH")) == null) {
				// do nothing
			}
			else if (mcnk.get(i).get(mcnkChunkNames.get("MCSH")).getSize() == 0) {
                                // do nothing
			}
			else if(mcnk.get(i).get(mcnkChunkNames.get("MCSH")).isEmpty()) {
				// do nothing, empty MCSH == no MCSH
			}
			else {
				raf.write(ADTConverterApp.reverseArray(emptyMcsh.getChunkCode()));	
				raf.write(toLittleEndianBytes(mcnk.get(i).get(mcnkChunkNames.get("MCSH")).getSize()));	
				raf.write(mcnk.get(i).get(mcnkChunkNames.get("MCSH")).getChunkData());	
			}
			
			// Add MCAL
			if (mcnk.get(i).get(mcnkChunkNames.get("MCAL")) == null) {
				// do nothing
			}
			else if (mcnk.get(i).get(mcnkChunkNames.get("MCAL")).getSize() == 0) {
				// do nothing
			}                               
			else {
				raf.write(ADTConverterApp.reverseArray(emptyMcal.getChunkCode()));	
				raf.write(toLittleEndianBytes(mcnk.get(i).get(mcnkChunkNames.get("MCAL")).getSize()));	
				raf.write(mcnk.get(i).get(mcnkChunkNames.get("MCAL")).getChunkData());	
			}
		}
                
                // MTXF
                if (mtxf == null) {
                   // do nothing 
                }
                else {
                    raf.write(ADTConverterApp.reverseArray(mtxf.getChunkCode()));	
                    raf.write(toLittleEndianBytes(mtxf.getSize()));	
                    raf.write(mtxf.getChunkData());	 
                }
                               
                
                raf.close();
                
             }
                    
             catch(IOException e) {
                e.printStackTrace();
            }               
                
	}
	

	
}