package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MMIDChunk extends Chunk {
    
    static final String MMIDNAME="MMID";
    
    public MMIDChunk(int o, int s, byte[] c) {
        super(MMIDNAME, o, s, c);
    }
    
}
