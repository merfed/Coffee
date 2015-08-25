package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MCINChunk extends Chunk {
    
    static final String MCINNAME="MCIN";
    
    public MCINChunk(int o, int s, byte[] c) {
        super(MCINNAME, o, s, c);
    }
    
}
