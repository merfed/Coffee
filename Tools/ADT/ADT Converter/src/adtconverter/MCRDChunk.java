package adtconverter;

/**
 *
 * @author Mjollnà
 */
public class MCRDChunk extends Chunk {
    
    static final String MCRDNAME="MCRD";
    
    public MCRDChunk(int o, int s, byte[] c) {
        super(MCRDNAME, o, s, c);
    }
    
}
