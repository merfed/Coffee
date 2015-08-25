package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MODFChunk extends Chunk {
    
    static final String MODFNAME="MODF";
    
    public MODFChunk(int o, int s, byte[] c) {
        super(MODFNAME, o, s, c);
    }  
    
}
