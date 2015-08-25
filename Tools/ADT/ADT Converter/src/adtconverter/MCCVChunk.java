package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MCCVChunk extends Chunk {
    
    static final String MCCVNAME="MCCV";
    
    public MCCVChunk(int o, int s, byte[] c) {
        super(MCCVNAME, o, s, c);
    }
    
}
