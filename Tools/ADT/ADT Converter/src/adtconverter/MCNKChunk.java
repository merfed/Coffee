package adtconverter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/**
 * @author pagdzin
 */

public class MCNKChunk extends Chunk {
    
    static final String MCNKNAME="MCNK";
    
    /*
     * Since it's the full MCNK size which is indicated at the beginning, 
     * we've got to know the header size to reach the first subchunk.
     */
    static final int MCNKHHEADERSIZE=128;

    
    /**
     * Offsets of useful chunk information, relative to the beginning of
     * MHDR.data (after size indication).
     */
    
    static final int MCLVOFFSET=120;
    static final int MCCVOFFSET=116;
    static final int MCSHFLAGOFFSET=0; // unused
    static final int MCSHOFFSET=44;
    static final int MCLQOFFSET=96;
    static final int MCSEOFFSET=88;
    
    public MCNKChunk(int o, int s, byte[] c) {
        super(MCNKNAME, o, s, c);
    }
    
    /*
     * In this method we define MCK subchunks and order.
     * The HashMap makes it easy to reach a particular element with no "for" or indices.
     */
    
    public static Map<String, Integer> initMCNKChunkNames() {
        Map<String, Integer> m=new HashMap<String, Integer>();
        
        m.put("MCNK",new Integer(0)); 
        m.put("MCVT",new Integer(1)); 
        m.put("MCLV",new Integer(2)); 
        m.put("MCCV",new Integer(3)); 
        m.put("MCNR",new Integer(4)); 
        m.put("MCLY",new Integer(5)); 
        m.put("MCRF",new Integer(6)); 
        m.put("MCSH",new Integer(7)); 
        m.put("MCAL",new Integer(8)); 
        m.put("MCLQ",new Integer(9)); 
        m.put("MCSE",new Integer(10)); 
        
        return m;
    }
    
    public static ArrayList<Chunk> getMCNKChunks(String chunkName) { 
    	
    	ArrayList<Chunk> chunkList = new ArrayList<Chunk>(); 
    	
    	
        return chunkList;
    }
   
    
    
}
