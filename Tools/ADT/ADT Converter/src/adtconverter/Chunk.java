/**
 * The class representing any kind of chunk. No further explaination needed, I think.
 * 
 * @author pagdzin & Mjollnà
 */

package adtconverter;



public abstract class Chunk {
    
    // Attributes
	
    private String name;
    private int offset; // just after the letters (contains the first int with the size)
    private int size;
    private byte[] chunkData;
    private byte[] code; // added by Mjo
    
    static final int SIZEFLAG_SIZE=4;
    
    /*
     * number of chunks in one file, when they are repeatable (af of now, only the MCNK sub-chunks).
     */
    static final int NBMCNKCHUNKS=256;
    
    // Constructors
    
    public Chunk(String name, int offset, int size, byte[] c) {
        this.name = name;
        this.offset = offset;
        this.size = size;
        this.chunkData = c;
        this.code = name.getBytes(); // Added by Mjo
    }
    
    public Chunk(String name, int offset, int size) { // Mostly added for MVER and its constant data
        this.name = name;
        this.offset = offset;
        this.size = size;
        this.chunkData = null;
        this.code = name.getBytes(); // Added by Mjo
    }
    
    // getters & setters
    
    public String getName() {
    	return this.name;
    }
    
    public int getOffset() {
    	return this.offset;
    }
    
    public int getSize() {
    	return this.size;
    }
    
    public byte[] getChunkData() {
    	return this.chunkData;
    }
    
    public byte[] getChunkCode() { // Added by Mjo
    	return this.code;
    }
    
    public void setChunkData(byte[] b) { // Added by Mjo
    	this.chunkData = b;
    }
    
    // Methods
    
    @Override
    public String toString() {
        StringBuilder sb=new StringBuilder();
        
        sb.append("\n--------------------------\n");
        sb.append(name).append(" chunk");
        sb.append("\ntaille : ").append(size);
        sb.append("\ncommence � l'offset : ").append(offset);
        sb.append(" ou en hex 0x").append(Integer.toHexString(offset)); // Offset in Hex added by Mjo
        sb.append("\ncontenu : \n\n");
        for (int i=0;i<chunkData.length;i++) {
            sb.append( String.format("%02X", chunkData[i])).append("\t"); // Leading zero added by Mjo (%X -> %02X)
            if ((i+1) % 8 == 0){
                sb.append("\n");
            }
        }
        sb.append("\n\n");
        return sb.toString();
    }
    
    /* This method checks if a Chunk is only filled with 00. 
     * Mainly added for MCSH. If true, then the chunk shouldn't be added to the tex0 file. 
     * 
     * @return true if the cunk contains only 0's, false if at least one byte doesn't respect this condition.
     */
    public boolean isEmpty() {
    	int j = 0;
    	for (int i = 0 ; i < this.size ; i++) {
    		j = j + (int)this.chunkData[i];
    		if (j != 0) return false;
    	}
    	return true;
    }
    
}
