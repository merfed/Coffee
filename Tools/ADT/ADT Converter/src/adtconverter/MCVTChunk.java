package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MCVTChunk extends Chunk {
    
    static final String MCVTNAME="MCVT";
    
    public MCVTChunk(int o, int s, byte[] c) {
        super(MCVTNAME, o, s, c);
    }
    
}
