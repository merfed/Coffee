package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MWIDChunk extends Chunk {
    
    static final String MWIDNAME="MWID";
    
    public MWIDChunk(int o, int s, byte[] c) {
        super(MWIDNAME, o, s, c);
    }
    
}
