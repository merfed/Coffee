package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MCNRChunk extends Chunk {
    
    static final String MCNRNAME="MCNR";
    
    /*
     * Up to cataclysm alpha build (11927 at least), the 13 bytes are forgotten into the chunk size.
     * In this particular build (and maybe a few others), the size is corrected, hence the special condition.
     */
    static final int MCNRREGULARSIZE=435;
    static final int MCNRCATAALPHASIZE=MCNRREGULARSIZE+ADTConverter.POSTMCNRDATASIZE;
    
    public MCNRChunk(int o, int s, byte[] c) {
        super(MCNRNAME, o, s, c);
    }
    
}
