package adtconverter;

/**
 * @author pagdzin & Mjollnà
 */

public class MVERChunk extends Chunk {
    
	// Constructor : we automatically add MVER Data, since it's always the same
	
    public MVERChunk() {
    	
        super("MVER", 4, 8);
        
        byte[] mverData = {  
            	0x04,0x0,0x0,0x0,
            	0x12,0x0,0x0,0x0
            };        
        
        this.setChunkData(mverData);
        
    }
    
}