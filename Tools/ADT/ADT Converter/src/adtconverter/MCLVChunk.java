package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MCLVChunk extends Chunk {
    
    static final String MCLVNAME="MCLV";
    
    public MCLVChunk(int o, int s, byte[] c) {
        super(MCLVNAME, o, s, c);
    }
    
}
