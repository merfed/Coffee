/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package adtconverter;

/**
 *
 * @author pagdzin and Mjollnà
 */
public class MHDRChunk extends Chunk {
    
    static final String MHDRNAME="MHDR";
    
    /**
     * Chunk useful information offsets.
     * Relative to the start of MHDR.data (after size info).
     */
    static final int MFBOFLAGOFFSET=0;
    static final int MH2OOFFSET=40;
    static final int MTXFOFFSET=44;
    
    public MHDRChunk(int o, int s, byte[] c) {
        super(MHDRNAME, o, s, c);
    }
    
    // Following methods added by Mjollnà
    
    
    // Creates a new chunk with 00 everywhere but :
    // - flags (copied from the 3.3.5 MHDR)
    // - and MH2O offset if there's one (which also has a fixed value : 0x40) ... or I think it's MH2O offset there -_-
    
    // Notes : Size also has a fixed value : 0x40. Added here since the initial adt 3.3.5 slicing for MHDR doesn't contain size.
    // MFBO is set afterwards according to the final size of the file. See method below.
    
    public void setCataValues(byte flags, boolean mh2o) {
    	
    	byte[] cataMHDR = new byte[68];
    	
        cataMHDR[0] = 0x40; // yes, I know, not quite elegant... no comments :)
        cataMHDR[1] = 0x0;
        cataMHDR[2] = 0x0;
        cataMHDR[3] = 0x0;
        
    	for (int i = 4 ; i < 64 ; i++) {
    		if (i == 4) {
    			cataMHDR[i] = flags;
    		}
                else if (mh2o && (i == 44)) { // 44 --> 11th int (mh2o ?)
    			cataMHDR[i] = 0x40;
    		}
    		else {
                    cataMHDR[i] = 0x0;
                } 
    	}
    	this.setChunkData(cataMHDR);
    }
    
    public boolean hasMH2O() {
        
    	if (this.getChunkData()[MH2OOFFSET] != 0) {
            return true;
    	}
    	return false;
    }
    
    // Set right offsets for MFBO in the new file. 
    // Goes to chunkFlag in chunkData and pastes newOffset
    
    public void setMFBOOffset(byte[] newOffset) {

    	for (int i = 40 ; i < 44 ; i++) {
    		this.getChunkData()[i] = newOffset[i-40];
        }
    }
}
