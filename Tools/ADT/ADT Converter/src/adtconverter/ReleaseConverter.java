package adtconverter;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;

/**
 *
 * @author pagdzin & Mjollnà
 */
public class ReleaseConverter extends ADTConverter{

    
    
    public ReleaseConverter() {
        super();
    }
    
    @Override
    public void convertADT(File f) {
        this.parseADT(ADTConverterApp.file2Bytes(f));
        this.generateCataFiles();
        System.out.println(f.getName().substring(0,f.getName().length()-4)+" : ok!");
    }

    @Override
    void parseMCNK(byte[] bytes) {
        
        for (int i=0;i<Chunk.NBMCNKCHUNKS;i++) {
     
            byte[] mcnk_verif= {Byte.decode("0x4b"),    // K
                                Byte.decode("0x4e"),    // N
                                Byte.decode("0x43"),    // C
                                Byte.decode("0x4d")};   // M

            if (Arrays.equals(mcnk_verif, Arrays.copyOfRange(bytes, offset, offset+mcnk_verif.length))) {
                
                mcnkChunks.add(new ArrayList<Chunk>());

                offset+=mcnk_verif.length;
                int mcnk_offset=offset;

                byte[] mcnk_size_flag=Arrays.copyOfRange(bytes, offset, offset+Chunk.SIZEFLAG_SIZE);
                mcnk_size_flag=ADTConverterApp.reverseArray(mcnk_size_flag);
                offset+=Chunk.SIZEFLAG_SIZE;
                
                byte mcnk_mclv_flag[]=Arrays.copyOfRange(bytes, offset+MCNKChunk.MCLVOFFSET, offset+MCNKChunk.MCLVOFFSET+Chunk.SIZEFLAG_SIZE);
                if (ADTConverterApp.byteArray2int(mcnk_mclv_flag)==0) {
                    mclv=false;
                    if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                        
                    	System.out.println("No chunk MCLV"); 
                    	
                    }
                } else {
                    mclv=true;
                }
                byte mcnk_mccv_flag[]=Arrays.copyOfRange(bytes, offset+MCNKChunk.MCCVOFFSET, offset+MCNKChunk.MCCVOFFSET+Chunk.SIZEFLAG_SIZE);
                if (ADTConverterApp.byteArray2int(mcnk_mccv_flag)==0) {
                    mccv=false;
                    if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                        
                    	System.out.println("No chunk MCCV"); 
                    	
                    }
                } else {
                    mccv=true;
                }
                
                byte mcnk_mclq_flag[]=Arrays.copyOfRange(bytes, offset+MCNKChunk.MCLQOFFSET, offset+MCNKChunk.MCLQOFFSET+Chunk.SIZEFLAG_SIZE);
                if (ADTConverterApp.byteArray2int(mcnk_mclq_flag)==0) {
                    mclq=false;
                    if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                        
                    	System.out.println("No chunk MCLQ"); 
                    	
                    }
                } else {
                    mclq=true;
                }
                
                byte mcnk_mcse_offset[]=Arrays.copyOfRange(bytes, offset+MCNKChunk.MCSEOFFSET, offset+MCNKChunk.MCSEOFFSET+Chunk.SIZEFLAG_SIZE);
                int mcse_offset=ADTConverterApp.byteArray2int(ADTConverterApp.reverseArray(mcnk_mcse_offset))-Chunk.SIZEFLAG_SIZE;
                
                byte mcnk_mcsh_offset[]=Arrays.copyOfRange(bytes, offset+MCNKChunk.MCSHOFFSET, offset+MCNKChunk.MCSHOFFSET+Chunk.SIZEFLAG_SIZE);
                int mcsh_offset;
                byte mcsh_test;
                
                mcsh_offset=offset+ADTConverterApp.byteArray2int(ADTConverterApp.reverseArray(mcnk_mcsh_offset))-Chunk.SIZEFLAG_SIZE-mcnk_verif.length;
                mcsh_test=bytes[mcsh_offset];
                if (Byte.decode("0x4c")==mcsh_test) { // a "L" : no MCSH chunk
                    mcsh=false;

                    if (ADTConverterApp.config.getProperty("debug").equals("yes")) {

                        System.out.println("No chunk MCSH"); 

                    }

                } else {
                    if (Byte.decode("0x48")==mcsh_test) { // we do have a MCSH chunk
                        mcsh=true;
                    } else { // Now, we have a big problem

                        System.out.println("MCSH pointer doesn't point to a chunk start."); 
                        System.exit(1);
                    }
                }
                
                byte mcnk_mclq_offset[]=Arrays.copyOfRange(bytes, offset+MCNKChunk.MCLQOFFSET, offset+MCNKChunk.MCLQOFFSET+Chunk.SIZEFLAG_SIZE);
                int mclq_offset=ADTConverterApp.byteArray2int(ADTConverterApp.reverseArray(mcnk_mclq_offset))-Chunk.SIZEFLAG_SIZE;
                
                
                /*
                 * we create a new "local" MCNK offset, running from after the header.
                 */
                local_mcnk_offset=offset+MCNKChunk.MCNKHHEADERSIZE;
                
                int mcnk_size=ADTConverterApp.byteArray2int(mcnk_size_flag);
                byte[] mcnk_data=Arrays.copyOfRange(bytes, offset, offset+mcnk_size);

                mcnkChunks.get(i).add(new MCNKChunk(mcnk_offset, mcnk_size, mcnk_data));
                
                this.offset+=mcnk_size;
                
                parseMCVT(bytes,i);
                parseMCLV(bytes,i);
                parseMCCV(bytes,i);
                parseMCNR(bytes,i);
                parseMCLY(bytes,i);
                parseMCRF(bytes,i);
                parseMCSH(bytes,i);
                parseMCAL(bytes,i,mclq_offset,mcse_offset);
                parseMCLQ(bytes,i,mclq_offset,mcse_offset);
                parseMCSE(bytes,i,mcse_offset);
                
                if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                    
                    System.out.println("Chunk "+(i)+"/255 correctly detected ("+mcnk_size+" bytes)\n"); // @Pag : I removed the "+1" to see from 0 to 255 properly.               
                }
                
            } else {
                
            	System.out.println("Invalid file : problem at chunk MCNK number " +(i+1));        
                System.exit(1);
            }
        } // end for
    }
    
    @Override
    void parseMCSE(byte[] bytes, int i, int mcse_offset) {
        
        byte[] mcse_verif= {Byte.decode("0x45"),    // E
                            Byte.decode("0x53"),    // S
                            Byte.decode("0x43"),    // C
                            Byte.decode("0x4d")};   // M

        byte[] mcvs_verif= {Byte.decode("0x53"),    // S // Alpha LK ADT quick fix : empty unknown MCVS subchunk in place of empty known MCSE. Haven't seen any non-empty one, but haven't had the occasion to test many adt either (Mjo)
                            Byte.decode("0x56"),    // V
                            Byte.decode("0x43"),    // C
                            Byte.decode("0x4d")};   // M        
        
        if (Arrays.equals(mcse_verif, Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcse_verif.length)) || Arrays.equals(mcvs_verif, Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcse_verif.length))) {
            local_mcnk_offset+=mcse_verif.length;
            mcse_offset=local_mcnk_offset;

            byte[] mcse_size_flag=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+Chunk.SIZEFLAG_SIZE);
            mcse_size_flag=ADTConverterApp.reverseArray(mcse_size_flag);
            local_mcnk_offset+=Chunk.SIZEFLAG_SIZE;

            int mcse_size=ADTConverterApp.byteArray2int(mcse_size_flag);
            byte[] mcse_data=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcse_size);

            mcnkChunks.get(i).add(mcnkChunkNames.get("MCSE"),new MCSEChunk(mcse_offset, mcse_size, mcse_data));
            local_mcnk_offset+=mcse_size;

            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {

                System.out.println(mcnkChunks.get(i).get(mcnkChunkNames.get("MCSE")));                    	

            }
        } else {
            System.out.println("Invalid file : problem at chunk MCSE"); 
            System.exit(1); 
        }
    }
    
    
}
