package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MCALChunk extends Chunk {
    
    static final String MCALNAME="MCAL";
    
    public MCALChunk(int o, int s, byte[] c) {
        super(MCALNAME, o, s, c);
    }
    
}
