package adtconverter;

/**
 *
 * @author Mjollnà
 */
public class MCRWChunk extends Chunk {
    
    static final String MCRWNAME="MCRW";
    
    public MCRWChunk(int o, int s, byte[] c) {
        super(MCRWNAME, o, s, c);
    }
    
}