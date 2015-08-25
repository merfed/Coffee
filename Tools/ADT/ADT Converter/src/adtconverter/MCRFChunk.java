package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MCRFChunk extends Chunk {
    
    static final String MCRFNAME="MCRF";
    
    public MCRFChunk(int o, int s, byte[] c) {
        super(MCRFNAME, o, s, c);
    }
    
}
