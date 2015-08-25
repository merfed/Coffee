package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MCSHChunk extends Chunk {
    
    static final String MCSHNAME="MCSH";
    
    public MCSHChunk(int o, int s, byte[] c) {
        super(MCSHNAME, o, s, c);
    }
    
}
