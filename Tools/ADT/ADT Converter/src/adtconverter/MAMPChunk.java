package adtconverter;

/**
 *
 * @author Mjollnà
 */
public class MAMPChunk extends Chunk {
    
    static final String MAMPNAME="MAMP";
    
    public MAMPChunk() {
        super(MAMPNAME, 16, 8);
        
		byte[] mampData = {
				0x4,0x0,0x0,0x0,
				0x0,0x0,0x0,0x0
			};
		
		this.setChunkData(mampData);
        
    }
    
}