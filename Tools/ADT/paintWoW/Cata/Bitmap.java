/**
 * About 272x272 24 bits BMPs
 * @author Gamhea
 */
public class Bitmap {
	
	private static byte[] bytes;
	
	Bitmap(byte[] b) {
		bytes = b;
	}
	
	public byte[] getBytes() {
		return bytes;
	}
	
	/**
	 * @param i
	 * @param j
	 * @return byte[blue, green, red]
	 */
	public static byte[] getPixelAt(int i, int j) {
		int coords = (271 - i)*272*3 + j*3;
		return new byte[] { bytes[coords], bytes[coords + 1], bytes[coords + 2] };
	}
	
	public static byte getRedAt(int i, int j) {
		return getPixelAt(i, j)[2];
	}
	public static byte getGreenAt(int i, int j) {
		return getPixelAt(i, j)[1];
	}
	public static byte getBlueAt(int i, int j) {
		return getPixelAt(i, j)[0];
	}
	
	/**
	 * I KNOW, that really sucks...
	 * @param v (0 <= v <= 144)
	 * @return the pixel that fits to v position
	 */
    public static byte[] getPixelFromVertex(int v) {
    	int nChunk = v / 145; // number of the mcnk position
    	int chunkVertex = v % 145; // vertex - 1 in the chunk
    	
    	int i = 0, j = 0;
    	
    	if (chunkVertex < 9) {
    		i = 0; j = chunkVertex*2;
    	} else if (chunkVertex < 17) {
    		i = 1; j = (chunkVertex - 9)*2 + 1;
    	} else if (chunkVertex < 26) {
    		i = 2; j = (chunkVertex - 17)*2;
    	} else if (chunkVertex < 34) {
    		i = 3; j = (chunkVertex - 26)*2 + 1;
    	} else if (chunkVertex < 43) {
    		i = 4; j = (chunkVertex - 34)*2;
    	} else if (chunkVertex < 51) {
    		i = 5; j = (chunkVertex - 43)*2 + 1;
    	} else if (chunkVertex < 60) {
    		i = 6; j = (chunkVertex - 51)*2;
    	} else if (chunkVertex < 68) {
    		i = 7; j = (chunkVertex - 60)*2 + 1;
    	} else if (chunkVertex < 77) {
    		i = 8; j = (chunkVertex - 68)*2;
    	} else if (chunkVertex < 85) {
    		i = 9; j = (chunkVertex - 77)*2 + 1;
    	} else if (chunkVertex < 94) {
    		i = 10; j = (chunkVertex - 85)*2;
    	} else if (chunkVertex < 102) {
    		i = 11; j = (chunkVertex - 94)*2 + 1;
    	} else if (chunkVertex < 111) {
    		i = 12; j = (chunkVertex - 102)*2;
    	} else if (chunkVertex < 119) {
    		i = 13; j = (chunkVertex - 111)*2 + 1;
    	} else if (chunkVertex < 128) {
    		i = 14; j = (chunkVertex - 119)*2;
    	} else if (chunkVertex < 136) {
    		i = 15; j = (chunkVertex - 128)*2 + 1;
    	} else if (chunkVertex < 145) {
    		i = 16; j = (chunkVertex - 136)*2;
    	}
    	
    	int rowMCNKs = nChunk / 16;
    	int colMCNKs = nChunk % 16;
    	
    	return getPixelAt((rowMCNKs*17) + i, (colMCNKs*17) + j);
    }
	
}
