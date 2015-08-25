package adtconverter;

/**
 * @author Mjollnà
 */

abstract class CataFile {

	// Attributes
	
	String fileType;
	String fileName;
	
	MVERChunk mver; // MVER is common to all
	
	int size;
	
	// Constructor
	
	public CataFile(MVERChunk mver, String fileName) {
		
		this.fileType = ""; // Empty in abstract class and Cadt
		this.fileName = fileName;
		
		this.mver = mver;
		
		size = mver.getSize()+4; // +1 == ugly way to add the size of "MVER" ---- TODO better
		
	}
	
	// Methods
	
	public void buildByteFile() {	
	}
	
	// Thx to my housemate :)
	public static byte[] toLittleEndianBytes(int i) {
		return new byte[] {
			(byte)i,
			(byte)(i >> 8),
			(byte)(i >> 16),
			(byte)(i >> 24)
		};
	}
	
}