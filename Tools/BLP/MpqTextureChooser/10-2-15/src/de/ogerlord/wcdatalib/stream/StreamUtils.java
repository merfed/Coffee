package de.ogerlord.wcdatalib.stream;

import java.io.IOException;
public class StreamUtils {

	/*
	 * private static Set<MdxComponent> optionalPool = new HashSet();
	 * 
	 * public void addOptionalPool(MdxComponent component){
	 * optionalPool.add(component); }
	 * 
	 * 
	 * public void executeOptionalPool(){
	 * 
	 * }
	 */

        
        public static boolean checkOptionalId(BlizzardDataInputStream in,
			String name) throws IOException {

		in.mark(8);

		if (name.equals(in.readCharsAsString(4))) {
			in.reset();
			return true;
		}

		in.reset();
		return false;
	}

	public static String readOptionalId(BlizzardDataInputStream in) throws IOException {
		in.mark(8);
                char[] c = in.readChars(4);
                in.reset();
                
                int allEmpty = 0;
                for(int i = 0;i<4; i++){
                    if((short)c[0] == -1){
                        allEmpty++;
                    }
                }
                
                if(allEmpty == 4){
                    return null;
                }
                
                return ""+c[0] + c[1]+ c[2]+ c[3];
	}

	public static void checkId(BlizzardDataInputStream in, String name)
			throws IOException {
		String found = in.readCharsAsString(4);
		if (!found.equals(name)) {
			throw new IOException(
					"Error loading model: CheckID failed, required " + name
							+ " found " + found);
		}
	}

	/*
	 * public static boolean checkOptionalId(BlizzardDataInputStream in, String
	 * name) throws IOException { if(lastCheck == null){ lastCheck =
	 * in.readCharsAsString(4); }
	 * 
	 * return lastCheck.equals(name); }
	 * 
	 * public static void checkId(BlizzardDataInputStream in, String name)
	 * throws IOException {
	 * 
	 * if(lastCheck != null){
	 * 
	 * if(!name.equals(lastCheck)){ throw new IOException(
	 * "Error loading model: CheckID failed after optinal check, required " +
	 * name + " found " + lastCheck); }
	 * 
	 * lastCheck=null;
	 * 
	 * }else{
	 * 
	 * String found = in.readCharsAsString(4); if (!found.equals(name)) { throw
	 * new IOException("Error loading model: CheckID failed, required " + name +
	 * " found " + found); } } }
	 */

	public static float[] loadFloatArray(BlizzardDataInputStream in, int size)
			throws IOException {
		float array[] = new float[size];

		for (int i = 0; i < size; i++) {
			array[i] = in.readFloat();
		}
		return array;
	}

	public static int[] loadIntArray(BlizzardDataInputStream in, int size)
			throws IOException {
		int array[] = new int[size];

		for (int i = 0; i < size; i++) {
			array[i] = in.readInt();
		}

		return array;
	}

	public static short[] loadShortArray(BlizzardDataInputStream in, int size)
			throws IOException {
		short array[] = new short[size];

		for (int i = 0; i < size; i++) {
			array[i] = (short) (in.readShort() & 0xFFFF);
		}

		return array;
	}

	public static byte[] loadByteArray(BlizzardDataInputStream in, int size)
			throws IOException {
		byte array[] = new byte[size];

		for (int i = 0; i < size; i++) {
			array[i] = in.readByte();
		}
		return array;
	}

	public static char[] loadCharArray(BlizzardDataInputStream in, int size)
			throws IOException {
		char array[] = new char[size];

		for (int i = 0; i < size; i++) {
			array[i] = in.readChar();
		}
		return array;
	}

	public static void saveFloatArray(BlizzardDataOutputStream out,
			float[] array) throws IOException {
		for (int i = 0; i < array.length; i++) {
			out.writeFloat(array[i]);
		}
	}

	public static void saveIntArray(BlizzardDataOutputStream out, int[] array)
			throws IOException {
		for (int i = 0; i < array.length; i++) {
			out.writeInt(array[i]);
		}
	}

	public static void saveShortArray(BlizzardDataOutputStream out,
			short[] array) throws IOException {
		for (int i = 0; i < array.length; i++) {
			out.writeNByteInt(array[i], 2);
		}
	}

	public static void saveByteArray(BlizzardDataOutputStream out, byte[] array)
			throws IOException {
		for (int i = 0; i < array.length; i++) {
			out.writeByte(array[i]);
		}
	}

	public static void saveCharArray(BlizzardDataOutputStream out, char[] array)
			throws IOException {
		out.writeChars(array);
	}
}
