/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package adtconverter;

/**
 *
 * @author pagdzin
 */
public class MTEXChunk extends Chunk{
    
    static final String MTEXNAME="MTEX";
    
    public MTEXChunk(int o, int s, byte[] c) {
        super(MTEXNAME, o, s, c);
    }
}
