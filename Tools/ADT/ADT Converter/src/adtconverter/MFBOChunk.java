package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MFBOChunk extends Chunk {
    
    static final String MFBONAME="MFBO";
    
    public MFBOChunk(int o, int s, byte[] c) {
        super(MFBONAME, o, s, c);
    }
    
}
