package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MMDXChunk extends Chunk{
    
    static final String MMDXNAME="MMDX";
    
    public MMDXChunk(int o, int s, byte[] c) {
        super(MMDXNAME, o, s, c);
    }
    
}
