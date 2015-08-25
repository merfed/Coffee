package adtconverter;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Map;

/**
 * @author Mjollnà
 */

public class Cobj0 extends CataFile {
	
	// Attributes
	
	MMDXChunk mmdx;
	MMIDChunk mmid;
	MWMOChunk mwmo;
	MWIDChunk mwid;
	MDDFChunk mddf;
	MODFChunk modf;
	ArrayList<ArrayList<Chunk>> mcnk;
	Map<String, Integer> mcnkChunkNames;
	
	// Constructor
	
	public Cobj0(MVERChunk mver, String fileName, MMDXChunk mmdx, MMIDChunk mmid, MWMOChunk mwmo, MWIDChunk mwid, MDDFChunk mddf, MODFChunk modf, ArrayList<ArrayList<Chunk>> mcnk, Map<String, Integer> mcnkChunkNames) {
		super(mver, fileName);	
		
		this.mmdx = mmdx;
		this.mmid = mmid;
		this.mwmo = mwmo;
		this.mwid = mwid;
		this.mddf = mddf;
		this.modf = modf;
		this.mcnk = mcnk;
		this.mcnkChunkNames = mcnkChunkNames;	
		
		this.fileType = "_obj0";
		
	}
	
	// Methods

	@Override
	public void buildByteFile() {
            
            try {
                
                RandomAccessFile raf = new RandomAccessFile(ADTConverterApp.config.getProperty("directory")+ADTConverterApp.relativeDir+this.fileName+this.fileType+".adt", "rw");
																											
            // MVER
            raf.write(ADTConverterApp.reverseArray(mver.getChunkCode()));
            raf.write(mver.getChunkData());

            // Add simple chunks (i.e. apart from MCNK / MCRD / MCRW) (note : the size is not included in the data)
            
            raf.write(ADTConverterApp.reverseArray(mmdx.getChunkCode()));
            raf.write(toLittleEndianBytes(mmdx.getSize()));
            raf.write(mmdx.getChunkData());            

            raf.write(ADTConverterApp.reverseArray(mmid.getChunkCode()));
            raf.write(toLittleEndianBytes(mmid.getSize()));
            raf.write(mmid.getChunkData());

            raf.write(ADTConverterApp.reverseArray(mwmo.getChunkCode()));
            raf.write(toLittleEndianBytes(mwmo.getSize()));
            raf.write(mwmo.getChunkData());            
            
            raf.write(ADTConverterApp.reverseArray(mwid.getChunkCode())); 
            raf.write(toLittleEndianBytes(mwid.getSize()));
            raf.write(mwid.getChunkData());            
            
            raf.write(ADTConverterApp.reverseArray(mddf.getChunkCode())); 
            raf.write(toLittleEndianBytes(mddf.getSize()));
            raf.write(mddf.getChunkData());               
            
            raf.write(ADTConverterApp.reverseArray(modf.getChunkCode())); 
            raf.write(toLittleEndianBytes(modf.getSize()));
            raf.write(modf.getChunkData());          
            
            // 256(MCNK + MCRD + MCRW)

            // Create an empty MCNK to paste every time its necessary
            byte[] emptyChunk = {0x0,0x0,0x0,0x0};

            MCNKChunk emptyMcnk = new MCNKChunk(0, 8, emptyChunk);
            MCRDChunk emptyMcrd = new MCRDChunk(0, 8, emptyChunk);  
            MCRWChunk emptyMcrw = new MCRWChunk(0, 8, emptyChunk);                

            for (int i = 0 ; i <= 255 ; i++) {

                // ---------------------------------------------------			
                // See what we have in the file and find the size of MCNK
                // ---------------------------------------------------                    
                
                int mcnksize = 0;
                
                // Mjo's note : We have to take byte[4] to convert to int, or we have errors casting byte -> int if more than 127 M2/WMO per chunk.
                
                byte[] nbM2Array = ADTConverterApp.reverseArray(Arrays.copyOfRange(mcnk.get(i).get(mcnkChunkNames.get("MCNK")).getChunkData(), 16, 20));
                int nbM2 = ADTConverterApp.byteArray2int(nbM2Array);
                
                byte[] nbWMOArray = ADTConverterApp.reverseArray(Arrays.copyOfRange(mcnk.get(i).get(mcnkChunkNames.get("MCNK")).getChunkData(), 56, 60));
                int nbWMO = ADTConverterApp.byteArray2int(nbWMOArray);
                
                //System.out.println("nbWMO / MCNK number/ MCNK offset "+nbWMO+" / "+i+" / "+mcnk.get(i).get(mcnkChunkNames.get("MCNK")).getOffset());
                
                
                    if (nbM2 > 0) {
                        mcnksize = mcnksize+nbM2*4+8;

                    }
                    if (nbWMO > 0) {
                        mcnksize = mcnksize+nbWMO*4+8;
                    }                        

                // ---------------------------------------------------
                // Add remaining chunks to file if necessary (MCRD/MCRW)
                // ---------------------------------------------------                    

                // MCNK
                raf.write(ADTConverterApp.reverseArray(emptyMcnk.getChunkCode()));			
                raf.write(toLittleEndianBytes(mcnksize));                     

                // MCRD (find its size + copy data)
                if (nbM2 > 0) {
                    raf.write(ADTConverterApp.reverseArray(emptyMcrd.getChunkCode()));  
                    raf.write(toLittleEndianBytes(nbM2*4));
                    
                    byte[] MCRDData = new byte[nbM2*4];

                    for (int j = 0 ; j < nbM2*4 ; j++) {    
                        MCRDData[j] = mcnk.get(i).get(mcnkChunkNames.get("MCRF")).getChunkData()[j];
                    }

                    raf.write(MCRDData);

                }

                // MCRW (find its size + copy data)                    
                if (nbWMO > 0) {
                    raf.write(ADTConverterApp.reverseArray(emptyMcrw.getChunkCode()));                    
                    raf.write(toLittleEndianBytes(nbWMO*4)); 
                    
                    byte[] MCRWData = new byte[nbWMO*4];

                    //System.out.println("MCRWData length : "+MCRWData.length);
                    
                    for (int k = nbM2*4 ; k < MCRWData.length+nbM2*4 ; k++) {
                        MCRWData[k-nbM2*4] = mcnk.get(i).get(mcnkChunkNames.get("MCRF")).getChunkData()[k];
                    }

                    //System.out.print("MCRWData : "+ADTConverterApp.getHexString(MCRWData));
        
                    raf.write(MCRWData);                        

                }


            }
               
                raf.close();
                
            }
            catch(IOException e) {
                e.printStackTrace();
            }

	}
       
	
}
