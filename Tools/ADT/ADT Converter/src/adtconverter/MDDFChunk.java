package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MDDFChunk extends Chunk {
    
    static final String MDDFNAME="MDDF";
    
    public MDDFChunk(int o, int s, byte[] c) {
        super(MDDFNAME, o, s, c);
    }
    
}
