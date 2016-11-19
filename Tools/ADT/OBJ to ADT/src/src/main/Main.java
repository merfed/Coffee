package src.main;

import java.io.BufferedReader;
import java.io.FileReader;

import com.mindprod.ledatastream.LERandomAccessFile;

/**
 * Put the vertices of an OBJ into an ADT
 * 
 * @author Koward, Skarn
 *
 */
public class Main {
	private final static int ROW_NUMBER = 16;
	private final static int COL_NUMBER = 16;
	private final static int VERTICES_NUMBER = 145;
	static long[][] offsets = new long[ROW_NUMBER][COL_NUMBER];
	static float[][][] heights = new float[ROW_NUMBER][COL_NUMBER][VERTICES_NUMBER];

	public static void main(String[] args) throws Exception {
		System.out.println("[[ Java OBJ to ADT injector by Koward and Skarn ]]");
		final String name = removeExtension(args[0]);

		final String objName = name + ".obj";
		final String adtName = name + ".adt";

		// Get values from OBJ
		BufferedReader bufferRead = new BufferedReader(new FileReader(objName));
		String line = bufferRead.readLine();
		while (line != null) {
			String token = line.split(" ")[0];
			switch (token) {
			case "o":
				int mcnk = Integer.parseInt(line.split(" ")[1].split("_")[0]);
				int row = Integer.parseInt(line.split(" ")[1].split("_")[1]);
				int index = 0;
				line = bufferRead.readLine();
				while (line.split(" ")[0].equals("v")) {
					heights[row][mcnk][index] = Float.parseFloat(line.split(" ")[3]);
					index++;
					line = bufferRead.readLine();
				}
				break;
			default:
			}
			line = bufferRead.readLine();
		}
		bufferRead.close();
		System.out.println("\t[" + objName + "] Successfully read.");

		// Get offsets for (i,j) chunks heights
		LERandomAccessFile raf = new LERandomAccessFile(adtName, "rw");
		int row = 0;
		int mcnk = 0;
		byte[] data = new byte[(int) raf.length()];
		raf.readFully(data);
		raf.seek(KMPMatch.indexOf(data, new byte[] { 'K', 'N', 'C', 'M' }));
		while (raf.getFilePointer() < raf.length()) {
			String magic = new String(new byte[] { raf.readByte(), raf.readByte(), raf.readByte(), raf.readByte() });
			switch (magic) {
			case "KNCM":
				long beginRef = raf.getFilePointer() - 4;// The offset refer
															// from this point,
															// before the magic
				long end = raf.readInt() + raf.getFilePointer();
				raf.seek(raf.getFilePointer() + 0x14);//skip what we do not use
				offsets[row][mcnk] = beginRef + raf.readInt();//get the offset of the beginning of heights
				mcnk++;
				if (mcnk == COL_NUMBER) {
					row++;
					mcnk = 0;
				}
				raf.seek(end);
				break;
			default:
				System.out.println("\t\tSkipping Chunk [" + magic + "] ! ");
				raf.seek(raf.readInt() + raf.getFilePointer());
			}
		}
		System.out.println("\t[" + adtName + "] Offsets to Heights saved.");

		// Write heights
		for (int i = 0; i < heights.length; i++) {
			for (int j = 0; j < heights[i].length; j++) {
				raf.seek(offsets[i][j]);
				raf.readInt();// Magic MCVT
				raf.readInt();// Chunk size
				for (int k = 0; k < heights[i][j].length; k++) {
					raf.writeFloat(heights[i][j][k]);
				}
			}
		}
		System.out.println("\t[" + adtName + "] Heights writing done.");
		raf.close();
		System.out.println("-- " + name + " done.");
	}

	/**
	 * Remove the extension in a file path. Works even with multiple '.' in
	 * path.
	 * 
	 * @author coobird
	 * @param s
	 * @return
	 */
	public static String removeExtension(String s) {
		String filename;
		filename = s;
		int extensionIndex = filename.lastIndexOf(".");
		if (extensionIndex == -1)
			return filename;
		return filename.substring(0, extensionIndex);
	}
}