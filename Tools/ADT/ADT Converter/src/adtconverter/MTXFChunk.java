package adtconverter;

/**
 *
 * @author pagdzin (& Mjo for the letters inverted)
 */
public class MTXFChunk extends Chunk {
    
    static final String MTXFNAME="MTXF";
    
    public MTXFChunk(int o, int s, byte[] c) {
        super(MTXFNAME, o, s, c);
    }
    
}
