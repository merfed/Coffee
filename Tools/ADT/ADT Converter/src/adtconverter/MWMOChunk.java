package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MWMOChunk extends Chunk {
    
    static final String MWMONAME="MWMO";
    
    public MWMOChunk(int o, int s, byte[] c) {
        super(MWMONAME, o, s, c);
    }
    
}
