/*
 * ADTConverterApp.java
 * 
 * Contains utility methods, and some auto-generated code (for the GUI).
 * 
 * @author pagdzin & Mjollnà
 */

package adtconverter;


import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.jdesktop.application.Application;
import org.jdesktop.application.SingleFrameApplication;

import config.Config;
import java.nio.ByteBuffer;
import javax.swing.JFileChooser;

/**
 * The main class of the application.
 */

public class ADTConverterApp extends SingleFrameApplication {
	
    static Config config=null;
    static String confFileName="adtconverter.conf";
    static String relativeDir="";
    
    static String fileType=null;
    
    static ADTConverter converter=null;
    
    // Mjo's utilities

    // Quick methods for time & creating file (yay I know it's ugly to have that here, but it's convenient) - Mjo

    /*
     * Gets the current time, in order to have a unique log file name.
     * 
     * @return a String containing the exact date and time.
     */
    
    public static String getDateTime() {
        DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd-HH-mm-ss");
        Date date = new Date();
        return dateFormat.format(date);
    }    
    
    // --------------------------------------------------------------------------------------------------------

    // Pag's utilities
    
    /**
     * Selects a file or a directory via a JFileChooser.
     * 
     * @return the file descriptor which will be used  the program
     */
    public static File chooseFile() {
     
        final JFileChooser fc = new JFileChooser();
        if (config.getProperty("source_directory") !=null) {
            fc.setCurrentDirectory(new File(config.getProperty("source_directory")));
        }
        fc.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES );
        int returnVal = fc.showOpenDialog(ADTConverterApp.getApplication().getMainFrame());
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            return fc.getSelectedFile();
            
        } else {
            return null; // I think we might need to put something here...
        }
    }
	
    /**
     * prints a byte array in a readable shape. Mainly for debug.
     *
     * @param bytes the array to print
     * @return a String containing the hexadecimal representation of every byte (with leading zeros).
     */
    public static String getHexString(byte[] bytes) {
		StringBuilder sb = new StringBuilder(bytes.length*2);
		for (byte b : bytes) {
			sb.append( String.format("%02X ", b)); // %X -> %02X changed by Mjo
		}
		return sb.toString();
	}

    /**
     * reverses a byte array (screw you, litte endian), with no call to the Collection(s) method, which are painful
     * to use on arrays.
     * WARNING : no check is done on the data returned. Anyway, this method is normally useful only to read chunk sizes.
     * @param bytes the array to reverse
     * @return the reversed array (!)
     */
    public static byte[] reverseArray(byte[] bytes) {
        byte[] bytes2=new byte[bytes.length];
        for (int i=0;i<bytes.length;i++) {
            bytes2[i]=bytes[bytes.length-i-1];
        }
        return bytes2;
    }

    public static int byteArray2int(byte[] bytes) {
        int res=0;
        for (int i=0;i<bytes.length;i++) {
            res*=256;
            res+=bytes[i] & 0xff;
        }
        return res;
    }
    
    /*
     * transforms a file into a byte array.
     * 
     * @param file the file descriptor
     * @return an array of bytes containing the file's data
     */
    public static byte[] file2Bytes(File file) {
        
        InputStream is=null;
            
            try {
                is = new FileInputStream(file);
            } catch (FileNotFoundException ex) {
                Logger.getLogger(ADTConverterApp.class.getName()).log(Level.SEVERE, null, ex);
            }
            
            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
            	
            	System.out.println("Taille du fichier "+file.toString()+" : "+file.length() + " octets");
            	
            }
            
            byte[] bytes = new byte[(int)file.length()];    
            int offset = 0;
            int numRead = 0;
            try {
                while (offset < bytes.length && (numRead=is.read(bytes, offset, bytes.length-offset)) >= 0) {
                    offset += numRead;
                }
            } catch (IOException ex) {
                Logger.getLogger(ADTConverterApp.class.getName()).log(Level.SEVERE, null, ex);
            }
            
            converter.inputFileName = file.getName().substring(0, file.getName().length()-4); // Added by Mjo to generate Cata files with a nice name 
            return bytes;
        
    }

    // --------------------------------------------------------------------------------------------------------
    
    // Generated UI code
    
    /**
     * At startup create and show the main frame of the application.
     */
    @Override protected void startup() {
        show(new ADTConverterView(this));
    }

    /**
     * This method is to initialize the specified window by injecting resources.
     * Windows shown in our application come fully initialized from the GUI
     * builder, so this additional configuration is not needed.
     */
    @Override protected void configureWindow(java.awt.Window root) {
    }

    /**
     * A convenient static getter for the application instance.
     * @return the instance of ADTConverterApp
     */
    public static ADTConverterApp getApplication() {
        return Application.getInstance(ADTConverterApp.class);
    }

    /**
     * Main method launching the application.
     */
    public static void main(String[] args) {
        
        if (!(Config.isConfigFile(confFileName))) {
            ADTConverterConfig.generateConfFile(confFileName);
        }
        config=new ADTConverterConfig(confFileName);
        launch(ADTConverterApp.class, args);     
        
    }
}
