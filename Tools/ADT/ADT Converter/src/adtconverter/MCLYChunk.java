package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MCLYChunk extends Chunk {
    
    static final String MCLYNAME="MCLY";
    
    public MCLYChunk(int o, int s, byte[] c) {
        super(MCLYNAME, o, s, c);
    }
    
}
