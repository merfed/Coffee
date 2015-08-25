import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.RandomAccessFile;
import java.util.Arrays;



/**
 * Uses a well-sized bitmap (272², 24 bits BMP) to draw a WoW ADT in Cataclysm.
 * It displays a frame to wait during drawing, but it's really useless and for entertainment :)
 * 
 * version 2
 * latest versions at gamhea.net/dev/paintwow.php
 * 
 * @author Gamhea
 */
public class paintWoWCata {

	final static int MCCVsize = 588;
	final static int MCNKheaderSize = 128;
	static int FirstMCNKoffset = 84; // well it's always 84... ? NOPE could have MH20 and maybe more before
	// will avoid searching many times if is there already a MCCV
	static boolean[] isThereAlreadyMCCV = new boolean[256];
	// explicit name. will be built later
	static byte[] defaultMCCV = new byte[MCCVsize];
	
	
	
	public static void main(String[] args) throws IOException, InterruptedException, FileNotFoundException {

		System.out.println("paintWoWCata, by Gamhea (v2)\n" +
				"Creative Commons BY-NC-SA\n" +
				"http://gamhea.net/\n");
		
		
		
		// preparing useful arrays and files
		for (int i = 0; i < 256; i++)
			isThereAlreadyMCCV[i] = true;
		for (int i = 0; i < 588; i++) {
			if (i < 8)
				defaultMCCV[i] = new byte[] {0x56, 0x43, 0x43, 0x4D, 0x44, 0x02, 0x00, 0x00}[i];
			else
				defaultMCCV[i] = new byte[] {0x7F, 0x7F, 0x7F, (byte) 0xFF}[i % 4];
		}
		
		File bmp = new File("template.bmp");
		FileInputStream fis;
		FileOutputStream fos;
		RandomAccessFile raf;
		byte[] ADTbytes;
		
		String[] ADTinRep = getADTlist(new File("."));
		if(ADTinRep.length != 1) {
			System.out.println("You need to have ONLY 1 ADT in the folder D:" +
					"\npaintWoW will exit.");
			System.exit(1);
		}
		File inputADT = new File(ADTinRep[0]);
		File outputADT = new File("p_" + ADTinRep[0]);
		
		
		fis = new FileInputStream(inputADT);
		fos = new FileOutputStream(outputADT);
		
		PrintWriter debug = new PrintWriter(new FileWriter("debug.txt"));
		
		
			ADTbytes = fixMCNKflags(fileToByte((int)inputADT.length(), fis), debug);
			
			while ( !(ADTbytes[FirstMCNKoffset] == 0x4B && ADTbytes[FirstMCNKoffset + 1] == 0x4E && ADTbytes[FirstMCNKoffset + 2] == 0x43 && ADTbytes[FirstMCNKoffset + 3] == 0x4D) )
				FirstMCNKoffset += 1;
			System.out.println("First MCNK is at : " + FirstMCNKoffset);
			
			addMCCVs(ADTbytes, fos, debug);
		fis.close();
		fos.close();
		
		raf = new RandomAccessFile(outputADT, "rw");
		fis = new FileInputStream(outputADT);
			ADTbytes = fileToByte((int)outputADT.length(), fis);
			fixMCNKsizes(ADTbytes, raf, debug);
		fis.close();
		
		fis = new FileInputStream(outputADT);
			ADTbytes = fileToByte((int)outputADT.length(), fis);
		fis.close();
		
		fis = new FileInputStream(bmp);
		byte[] BMPbytes = fileToByte((int)bmp.length(), fis);
		Bitmap templateBMP = new Bitmap(Arrays.copyOfRange(BMPbytes, 54, BMPbytes.length));
		
		paintTheFuckingBitmap(templateBMP, ADTbytes, raf);
		fis.close();
		raf.close();
		
		System.out.println("paintWoWCata has finished ! It will explode in 2 seconds !");
		Thread.sleep(2000);
		System.out.println("BOOM");
		System.exit(1);
	}
	
	
	
	/**
	 * on a full file in byte[], it will fix MCNK 0x40 flag (MCCV)
	 * @param bytes fixed
	 */
	private static byte[] fixMCNKflags(byte[] bytes, PrintWriter debug) throws IOException {
		int offset = 0, searchOffset = 0;
		boolean isThereMCCV = false;
		byte[] fixedFlags;
		
		System.out.print("Fixing flags... ");
		//debug.println("fixMCNKflags\n");
		
		for (int i = 0; i < 256; i++) {
			// while offset isn't on next MCNK
			while ( !(bytes[offset] == 0x4B && bytes[offset + 1] == 0x4E &&	bytes[offset + 2] == 0x43 && bytes[offset + 3] == 0x4D) )
				offset += 1;
			
				//debug.println("MCNK " + i + " : " + offset);
			
			searchOffset = offset + 8;
			
			// now that we have MCNK offset, we search if MCCV already exists until next MCNK
			while ( !(bytes[searchOffset] == 0x4B && bytes[searchOffset + 1] == 0x4E &&	bytes[searchOffset + 2] == 0x43 && bytes[searchOffset + 3] == 0x4D) 
					&& isThereMCCV == false 
					&& searchOffset < bytes.length - 3) {
				if (bytes[searchOffset] == 0x56 && bytes[searchOffset + 1] == 0x43 && bytes[searchOffset + 2] == 0x43 && bytes[searchOffset + 3] == 0x4D) {
						isThereMCCV = true;
						
						//debug.println("MCNK " + i + " got mccv at " + searchOffset + " (" + (searchOffset - offset) + ")");
						
						break;
					}
					searchOffset += 1;
			}
			
			// if none was found, we add the flag, fix many offsets in MCNK header and record the MCNK position
			if (!isThereMCCV) {
				isThereAlreadyMCCV[i] = false;
				fixedFlags = intToLIbyte(LIbyteToInt(Arrays.copyOfRange(bytes, offset + 8, offset + 12)) + 64);
				for (int val = 0; val < 4; val++) {
					//debug.print(bytes[offset + 8 + val] + " -> " + fixedFlags[val] + " || ");
					bytes[offset + 8 + val] = fixedFlags[val];
				}
				//debug.println();
			}
			
			// and finally we reset isThereMCCV, and put offset after last read MCNK
			isThereMCCV = false;
			offset += 8;
		}
		
		System.out.println("done.");
		
		return bytes;
	}
	
	
	
	/**
	 * add MCCV where there are not already
	 * @param bytes
	 * @param fos
	 * @throws IOException
	 */
	private static void addMCCVs(byte[] bytes, FileOutputStream fos, PrintWriter debug) throws IOException {
		int offset = FirstMCNKoffset, nextMCCVoffset = 0, MCNKgivenSize = 0;
		
		System.out.print("Adding MCCVs... ");
		
		// write bytes before MCNKs
		fos.write(Arrays.copyOfRange(bytes, 0, offset));
		
		for (int i = 0; i < 256; i++) {
			MCNKgivenSize = LIbyteToInt(Arrays.copyOfRange(bytes, offset + 4, offset + 8));
			
			// if there is already a MCCV, we just copy-paste
			if (isThereAlreadyMCCV[i]) {
				fos.write(Arrays.copyOfRange(bytes, offset, offset + 8 + MCNKgivenSize));
				//debug.println("Chunk " + i + " : existing mccv pasted from " + offset);
			}
			// else, we have to insert a new one, full of 7F (default value)
			else {
				nextMCCVoffset = offset + 8 + MCNKheaderSize + MCCVsize;
				//debug.println("Chunk " + i + " : existing mccv pasted from " + nextMCCVoffset);
				fos.write(Arrays.copyOfRange(bytes, offset, nextMCCVoffset)); // before the MCCV
				fos.write(defaultMCCV);
				fos.write(Arrays.copyOfRange(bytes, nextMCCVoffset, offset + 8 + MCNKgivenSize)); // after the MCCV
			}
			
			offset += 8 + MCNKgivenSize;
		}
		
		// write if there is something after MCNKs
		fos.write(Arrays.copyOfRange(bytes, offset, bytes.length));
		
		System.out.println("done.");
	}
	
	
	
	/**
	 * add MCCVsize (588) to MCNK size when necessary
	 * @param bytes
	 * @param raf
	 * @throws IOException
	 */
	private static void fixMCNKsizes(byte[] bytes, RandomAccessFile raf, PrintWriter debug) throws IOException {
		int offset = FirstMCNKoffset, MCNKgivenSize = 0;
		
		System.out.print("Fixing MCNK sizes... ");
					
		for (int i = 0; i < 256; i++) {
			MCNKgivenSize = LIbyteToInt(Arrays.copyOfRange(bytes, offset + 4, offset + 8));
			
			if (!isThereAlreadyMCCV[i]) {
				raf.seek(offset + 4);
				raf.write(intToLIbyte(LIbyteToInt(Arrays.copyOfRange(bytes, offset + 4, offset + 8)) + MCCVsize));
				MCNKgivenSize += MCCVsize;
				
			}
			offset += 8 + MCNKgivenSize;
		}
		
		System.out.println("done.");
	}
	
	
	
	/**
	 * may look weird because i've been stucked on an offset bug here so i messed things around...
	 * but no it works \o/
	 * -> it draws the bitmap in the MCCV chunks
	 * @param thatBMP Bitmap object of the template.bmp
	 * @param bytes
	 * @param raf
	 * @throws IOException
	 */
	private static void paintTheFuckingBitmap(Bitmap thatBMP, byte[] bytes, RandomAccessFile raf) throws IOException {
		int MCNKoffset = FirstMCNKoffset, 
				MCCVoffset = MCNKoffset + 8 + MCNKheaderSize + 588,
				MCNKgivenSize = LIbyteToInt(Arrays.copyOfRange(bytes, MCNKoffset + 4, MCNKoffset + 8));
		
		// this to avoid MCBB, MCBI and MCBV
		while ( !(bytes[MCCVoffset] == 0x56 && bytes[MCCVoffset + 1] == 0x43 &&	bytes[MCCVoffset + 2] == 0x43 && bytes[MCCVoffset + 3] == 0x4D) )
			MCCVoffset += 1;
		
		
		System.out.print("Painting... ");
		
		// for every vertex in every chunk (145 * 256 = 37120)
		for (int c = 0; c < 256; c++) {
			
			raf.seek(MCCVoffset + 8);
			
			for (int v = 0; v < 145; v++) {
			
			raf.write(Bitmap.getPixelFromVertex(v + c*145));
			raf.seek(raf.getFilePointer() + 1);
		
			}
			
			MCNKoffset += 8 + MCNKgivenSize;
			MCNKgivenSize = LIbyteToInt(Arrays.copyOfRange(bytes, MCNKoffset + 4, MCNKoffset + 8));
			MCCVoffset = MCNKoffset + 8 + MCNKheaderSize + 588;
			
			if (c != 255)
				while ( !(bytes[MCCVoffset] == 0x56 && bytes[MCCVoffset + 1] == 0x43 &&	bytes[MCCVoffset + 2] == 0x43 && bytes[MCCVoffset + 3] == 0x4D) )
					MCCVoffset += 1;
		}
		
		System.out.println("done.");
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	/*
	 * 
	 * SOME METHODS ABOUT INT AND BYTE[], ETC
	 * 
	 */
	
	
	
	/**
	 * @param l length of the file
	 * @param fis FileInputStream to avoid multiple input streams
	 * @return byte[] with the currentFile bytes
	 */
	private static byte[] fileToByte(int l, FileInputStream fis) {
		byte[] b = new byte[l];
		try {
			fis.read(b);
		} catch (IOException e) {
			e.printStackTrace();
		}
		return b;
	}
	
	
	
    /**
     * @author Mjollna / Gamhea
     * @param b bytes[] containing bytes in little indian
     * @return integer (uint_32 if length = 4)
     */
    private static int LIbyteToInt(byte[] b) {
    	byte[] b2 = new byte[b.length];
    	for (int i = 0; i < b.length; i++)
    		b2[i] = b[b.length - i - 1];

    	int n = 0;
    	for (int i = 0 ; i < b2.length; i++) {
    		n *= 256;
    		n += b2[i] & 0xff;
    	}
    	
    	return n;
    }
    
    
    
    /**
     * @author MichaelTague, on a random website
     * @param value
     * @return value in little-indian byte[]
     */
    private static final byte[] intToLIbyte(int value) {
        return new byte[] {
                (byte) value,
                (byte) (value >>> 8),
                (byte) (value >>> 16),
                (byte) (value >>> 24) };
    }
    
    
    
	/**
	 * get the String[] of ADTs found in the current directory
	 * @return String[], ex "test.adt"
	 */
	private static String[] getADTlist(File rep) {
		FilenameFilter adtFilter = new FilenameFilter() {
			public boolean accept(File arg0, String arg1) {
				return arg1.endsWith(".adt");
			}
		};
					
		return rep.list(adtFilter);
	}

}
