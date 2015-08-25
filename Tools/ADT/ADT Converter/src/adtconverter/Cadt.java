package adtconverter;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.Map;

/**
 * @author Mjollnà
 */

public class Cadt extends CataFile {

	// Attributes
	
	MHDRChunk mhdr;
	MH2OChunk mh2o;
	ArrayList<ArrayList<Chunk>> mcnk;
	Map<String, Integer> mcnkChunkNames;        
	MCVTChunk mcvt;
	MCVTChunk mccv;        
	MCNRChunk mcnr;
	MCLQChunk mclq;
	MCSEChunk mcse;
        MFBOChunk mfbo;
	
	// Constructor
	
	public Cadt(MVERChunk mver, String fileName, MHDRChunk mhdr, MH2OChunk mh2o, ArrayList<ArrayList<Chunk>> mcnk, Map<String, Integer> mcnkChunkNames, MFBOChunk mfbo) {
		super(mver, fileName);

		this.mhdr = mhdr;
		this.mh2o = mh2o;
		this.mcnk = mcnk;
		this.mcnkChunkNames = mcnkChunkNames;
                this.mfbo = mfbo;		
	}
	
	// Methods
	
	@Override
	public void buildByteFile() {

           byte[] emptyChunk = {0x0,0x0,0x0,0x0};            
            
           try {
                
                RandomAccessFile raf = new RandomAccessFile(ADTConverterApp.config.getProperty("directory")+ADTConverterApp.relativeDir+this.fileName+this.fileType+".adt", "rw");                
                
		// MVER
		raf.write(ADTConverterApp.reverseArray(mver.getChunkCode()));
		raf.write(mver.getChunkData());
		
		// MHDR (we'll have to return to this later to set MFBO offset)
		raf.write(ADTConverterApp.reverseArray(mhdr.getChunkCode()));		
		mhdr.setCataValues(mhdr.getChunkData()[0], mhdr.hasMH2O());
		raf.write(mhdr.getChunkData());

		// MH2O 	
                if (mh2o == null) {
                        // do nothing
                }
                else {
                    raf.write(ADTConverterApp.reverseArray(mh2o.getChunkCode()));
                    raf.write(toLittleEndianBytes(mh2o.getSize()));
                    raf.write(mh2o.getChunkData());                    
                }
                
		// 256(MCNK + MCVT + MCCV + MCNR + MCLQ + MCSE)
		
		// Create empty MCNK, MCCV, MCLQ, MCSE to paste every time its necessary.
		
		MCNKChunk emptyMcnk = new MCNKChunk(0, 132, emptyChunk);
                MCVTChunk emptyMcvt = new MCVTChunk(0, 8, emptyChunk);
		MCCVChunk emptyMccv = new MCCVChunk(0, 8, emptyChunk);		                
                MCNRChunk emptyMcnr = new MCNRChunk(0, 8, emptyChunk);                
                MCLQChunk emptyMclq = new MCLQChunk(0, 8, emptyChunk);	                
                MCSEChunk emptyMcse = new MCSEChunk(0, 8, emptyChunk); 
                
		for (int i = 0 ; i <= 255 ; i++) {                
                
                    // ---------------------------------------------------			
                    // See what we have in the file and find the size of MCNK
                    // ---------------------------------------------------		

                    int mcnksize = 0;		

                    mcnksize = mcnksize + 128;
                    
                    // MCVT
                    mcnksize = mcnksize + 8 + mcnk.get(i).get(mcnkChunkNames.get("MCVT")).getSize();
                    
                    // MCCV
                    if (mcnk.get(i).get(mcnkChunkNames.get("MCCV")) == null) {
                            // do nothing
                    }
                    else if (mcnk.get(i).get(mcnkChunkNames.get("MCCV")).getSize() == 0) {
                            // do nothing
                    }
                    else {
                            mcnksize = mcnksize + 8 + mcnk.get(i).get(mcnkChunkNames.get("MCCV")).getSize();
                    }
                    
                    // MCNR (different treatment whether the size includes the 13 trailing bytes or not)
                    if (mcnk.get(i).get(mcnkChunkNames.get("MCNR")).getSize() == 435) {
                        mcnksize = mcnksize + 8 + mcnk.get(i).get(mcnkChunkNames.get("MCNR")).getSize()+13;
                    }
                    else {
                        mcnksize = mcnksize + 8 + mcnk.get(i).get(mcnkChunkNames.get("MCNR")).getSize();
                    }                    

                    // MCLQ
                    if (mcnk.get(i).get(mcnkChunkNames.get("MCLQ")) == null) {
                            // do nothing
                    }
                    else if (mcnk.get(i).get(mcnkChunkNames.get("MCLQ")).getSize() == 0) {
                            // do nothing, empty MCLQ is not kept in 4.x.
                    }
                    else {
                            mcnksize = mcnksize + 8 + mcnk.get(i).get(mcnkChunkNames.get("MCLQ")).getSize();
                    }

                    // MCSE
                    if (mcnk.get(i).get(mcnkChunkNames.get("MCSE")) == null) {
                            // do nothing
                    }
                    else { // Empty MCSE kept
                            mcnksize = mcnksize + 8 + mcnk.get(i).get(mcnkChunkNames.get("MCSE")).getSize();
                    }
                    
                    // ---------------------------------------------------
                    // Add remaining chunks to file
                    // ---------------------------------------------------

                    // MCNK
                    raf.write(ADTConverterApp.reverseArray(emptyMcnk.getChunkCode()));			
                    raf.write(toLittleEndianBytes(mcnksize));                    
                    
                    // MCNK contains all its subchunks in the parser, and we only need to keep only the header to create the new MCNK header
                    byte[] MCNKHeader = new byte[128];
                    
                    for (int j = 0 ; j < 128 ; j++) { 
                        MCNKHeader[j] = mcnk.get(i).get(mcnkChunkNames.get("MCNK")).getChunkData()[j];
                    }
                    
                    boolean hasMccv = (mcnk.get(i).get(mcnkChunkNames.get("MCCV")) != null); // created to determine right MCNR offset in MCNK Cata Chunk
                    raf.write(createCataMCNKChunk(MCNKHeader, hasMccv, mcnksize));
                    
                    // MCVT
                    raf.write(ADTConverterApp.reverseArray(emptyMcvt.getChunkCode()));	
                    raf.write(toLittleEndianBytes(mcnk.get(i).get(mcnkChunkNames.get("MCVT")).getSize()));	
                    raf.write(mcnk.get(i).get(mcnkChunkNames.get("MCVT")).getChunkData());
                    
                    // MCCV
                    if (mcnk.get(i).get(mcnkChunkNames.get("MCCV")) == null) {
                            // do nothing
                    }
                    else if (mcnk.get(i).get(mcnkChunkNames.get("MCCV")).getSize() == 0) {
                            // do nothing
                    }
                    else {
                            raf.write(ADTConverterApp.reverseArray(emptyMccv.getChunkCode()));	
                            raf.write(toLittleEndianBytes(mcnk.get(i).get(mcnkChunkNames.get("MCCV")).getSize()));	
                            raf.write(mcnk.get(i).get(mcnkChunkNames.get("MCCV")).getChunkData());	
                    }                    
                    
                    // MCNR
                    raf.write(ADTConverterApp.reverseArray(emptyMcnr.getChunkCode()));
                    if (mcnk.get(i).get(mcnkChunkNames.get("MCNR")).getSize() == 435) { // Update the size if necessary
                        raf.write(toLittleEndianBytes(mcnk.get(i).get(mcnkChunkNames.get("MCNR")).getSize()+13));	
                    }
                    else {
                        raf.write(toLittleEndianBytes(mcnk.get(i).get(mcnkChunkNames.get("MCNR")).getSize()));
                    }
                    raf.write(mcnk.get(i).get(mcnkChunkNames.get("MCNR")).getChunkData());                    

                    // MCLQ
                    if (mcnk.get(i).get(mcnkChunkNames.get("MCLQ")) == null) {
                            // do nothing
                    }
                    else if (mcnk.get(i).get(mcnkChunkNames.get("MCLQ")).getSize() == 0) {
                            // do nothing
                    }
                    else {
                            raf.write(ADTConverterApp.reverseArray(emptyMclq.getChunkCode()));	
                            raf.write(toLittleEndianBytes(mcnk.get(i).get(mcnkChunkNames.get("MCLQ")).getSize()));	
                            raf.write(mcnk.get(i).get(mcnkChunkNames.get("MCLQ")).getChunkData());	
                    } 

                    // MCSE                
                    if (mcnk.get(i).get(mcnkChunkNames.get("MCSE")) == null) {
                            // do nothing
                    }
                    else if (mcnk.get(i).get(mcnkChunkNames.get("MCSE")).getSize() == 0) {
                            raf.write(ADTConverterApp.reverseArray(emptyMcse.getChunkCode()));
                            raf.write(emptyChunk);                             
                    }
                    else {
                            raf.write(ADTConverterApp.reverseArray(emptyMcse.getChunkCode()));	
                            raf.write(toLittleEndianBytes(mcnk.get(i).get(mcnkChunkNames.get("MCSE")).getSize()));	
                            raf.write(mcnk.get(i).get(mcnkChunkNames.get("MCSE")).getChunkData());	
                    } 
		
                }

                // Set the new offset for MFBO in MHDR                
                byte[] mfboOffset = toLittleEndianBytes((int)raf.getFilePointer()-20); //-20 --> offset is calculated from the start of MHDR (after size) and not the beginning of the file
                
                mhdr.setMFBOOffset(mfboOffset);                
                
		// MFBO	
                if (mfbo == null) {
                        // do nothing
                }
                else {
                    raf.write(ADTConverterApp.reverseArray(mfbo.getChunkCode()));
                    raf.write(toLittleEndianBytes(mfbo.getSize()));
                    raf.write(mfbo.getChunkData());
                    
                    // Copy MHDR again with MFBO ok (could probably made a little better... writing twice MHDR is not quite good)
                    raf.seek(16);
                    raf.write(mhdr.getChunkData());
                    
                }                
                
                raf.close();                
                
            }
            catch(IOException e) {
                e.printStackTrace();
            }                
	}	
	
        // Create a Cataclysm like MCNK for Cadt file.
        // Should probably go in MCNKChunk class, actually
        
        public static byte[] createCataMCNKChunk(byte[] oldMCNK, boolean hasMccv, int mcnksize) {
            
            byte[] size = toLittleEndianBytes(mcnksize);
            byte[] newMCNK = new byte[128];
            
            /*
             *    byte   |   int    | content |      value
             *  ---------|---------------------------------------------
             *  0        |  1       |  flags  | copy from oldMCNK
             *  5 -> 19  |  2 -> 5  |         | 0x0  0x0 0x0 0x0
             * 20        |  6       |  mcvt   | 0x88 0x0 0x0 0x0
             * 24 &  25  |  7       |  mcnr   | if mccv != null : 0x20 0x05 0x0 0x0 else 0xd4 0x02 0x0 0x0
             * 26 -> 51  |  7 -> 13 |         | 0x0  0x0 0x0 0x0
             * 52 -> 87  | 14 -> 23 |         | copy from oldMCNK
             * 88 -> 91  | 24       | mcnk size
             * 92 -> 95  | 25       |         | 0x0  0x0 0x0 0x0
             * 96 -> 99  | 26       | mcnk size (pb with non empty MCSE ?)
             * 100 -> 127| rest     |         | copy from oldMCNK
             * 
             */
            
            for (int i = 0 ; i <= 3 ; i++) {
                newMCNK[i] = oldMCNK[i];
                //newMCNK[i] = 0x0; // testing purposes
            }
            
            for (int i = 4 ; i <= 19 ; i++) {
                newMCNK[i] = 0x0;
            }
            
            newMCNK[20] = (byte)0x88;
            
            if (hasMccv) {
                newMCNK[24] = (byte)0x20;
                newMCNK[25] = (byte)0x05;
            }
            else {
                newMCNK[24] = (byte)0xd4;
                newMCNK[25] = (byte)0x02;
            }
            
            for (int i = 26 ; i <= 51 ; i++) {
                newMCNK[i] = 0x0;
            }
            
            for (int i = 52 ; i <= 87 ; i++) {
                newMCNK[i] = oldMCNK[i];
            }
            
            for (int i = 88 ; i <= 91 ; i++) {
                newMCNK[i] = size[i-88];
            }
            
            for (int i = 92 ; i <= 95 ; i++) {
                newMCNK[i] = 0x0;
            }
            
             for (int i = 96 ; i <= 99 ; i++) {
                newMCNK[i] = size[i-96];
            }           

             for (int i = 100 ; i <= 127 ; i++) {
                newMCNK[i] = oldMCNK[i];
            }            
             
            if (hasMccv) { // + offset MCCV
                newMCNK[116] = (byte)0xd4;
                newMCNK[117] = (byte)0x2;
            }
            else {
                newMCNK[116] = (byte)0x0;
                newMCNK[117] = (byte)0x0;
            }
             
            return newMCNK;
        }
        
}