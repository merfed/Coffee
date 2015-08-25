package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MCLQChunk extends Chunk {
    
    static final String MCLQNAME="MCLQ";
    
    public MCLQChunk(int o, int s, byte[] c) {
        super(MCLQNAME, o, s, c);
    }
    
}
