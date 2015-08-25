package adtconverter;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Map;

/**
 * ADTConverter.java
 * 
 * This class is mostly used for the parsing of the 3.3.5 file.
 * 
 * @author pagdzin (mostly) & Mjollnà (a little)
 */
public abstract class ADTConverter {
    
    // Attributes

    MVERChunk mverChunk=null;
    MHDRChunk mhdrChunk=null;
    MCINChunk mcinChunk=null;
    MTEXChunk mtexChunk=null;
    MMDXChunk mmdxChunk=null;
    MMIDChunk mmidChunk=null;
    MWMOChunk mwmoChunk=null;
    MWIDChunk mwidChunk=null;
    MDDFChunk mddfChunk=null;
    MODFChunk modfChunk=null;
    MH2OChunk mh2oChunk=null;
    
    /* 
     * In this implementation, there are no "sub-chunks" on their own, simply because
     * this is no trivial need for that concept for the sake of format conversion.
     * In order to maximise granularity, all the sub-chunks are stored separated.
     * The mnckChunk is merely an aggregation of all the sub-chunks PLUS the header.
     */
    
    /*
     * WARNING : the element in the index 0 of the second dimension contains the entire
     * MCNK chunk. After it come all the sub-chunks.
     */
    
    ArrayList<ArrayList<Chunk>> mcnkChunks=new ArrayList<ArrayList<Chunk>>();
    
    Map<String, Integer> mcnkChunkNames=null;
    
    /* size of the data located right after the MCNR chunk (but not technically in it).
     * It is to note that, in cataclysm, this data is used, and even included in the chunk
     * (so the size must be updated as well).
     */
    static final int POSTMCNRDATASIZE=13;
    
    MFBOChunk mfboChunk=null;
    MTXFChunk mtxfChunk=null;
    
    boolean mfbo=true;
    boolean mh2o=true;
    boolean mtxf=true;
    
    boolean mclv=true;
    boolean mccv=true;
    boolean mcsh=true;
    boolean mclq=true;
    boolean mcse=true;
    
    StringBuilder parsing = new StringBuilder(); // Added by Mjo
    String inputFileName; // Added by Mjo
    
    int offset,local_mcnk_offset=0;
    
    // Methods

    /**
     * A method that cleans any static var between two parsings (for example when 
     * we parse a directory). The goal is to avoid any unexpected interaction.
     */
    private void cleanVars() {
        mverChunk=null;
        mhdrChunk=null;
        mcinChunk=null;
        mtexChunk=null;
        mmdxChunk=null;
        mmidChunk=null;
        mwmoChunk=null;
        mwidChunk=null;
        mddfChunk=null;
        modfChunk=null;
        mh2oChunk=null;
        mcnkChunks=new ArrayList<ArrayList<Chunk>>();
        mcnkChunkNames=null;
        mfboChunk=null;
        mtxfChunk=null;
        mfbo=true;
        mh2o=true;
        mtxf=true;
        mclv=true;
        mccv=true;
        mcsh=true;
        mclq=true;
        mcse=true;
    }
    
    public static ADTConverter initConverter(String fileType) {
        if ("Retail".equals(fileType)) {
            return new ReleaseConverter();
        } else {
            if ("World building".equals(fileType)) {
                return new WBConverter();
            }
        }
        return null;
    }
    
    /**
     * parses a file, in order to store its chunks in separate variables, which
     * can be esaily used later, for any purpose.
     * By the way, it also checks the validity of the data given : a malformed 
     * file will imply an error. Finally, it launches the file conversion process.
     * 
     * @param bytes the byte array to be parsed
     * @return true if the operation is a success.
     */
    public void parseADT(byte[] bytes) {  
    	
        this.cleanVars();
        this.mcnkChunkNames=MCNKChunk.initMCNKChunkNames();
        
        this.parseMVER(bytes);
        this.parseMHDR(bytes);
        this.parseMCIN(bytes);
        this.parseMTEX(bytes);
        this.parseMMDX(bytes);
        this.parseMMID(bytes);
        this.parseMWMO(bytes);
        this.parseMWID(bytes);
        this.parseMDDF(bytes);
        this.parseMODF(bytes);
        this.parseMH2O(bytes);
        this.parseMCNK(bytes);
        this.parseMFBO(bytes);
        this.parseMTXF(bytes);
        
        if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
        	//ADTConverterApp.generateFile(ADTConverterApp."ADTConverter", parsing.toString());
        }
    }
    
    public void generateCataFiles() {
        // Mjo's code
        // Generate the 3 Cataclysm files : adt, obj0, tex0 (no tex1 for the moment, TODO later :p)
		        
        MVERChunk blip = new MVERChunk();    
        
        Cadt cadt = new Cadt(blip, inputFileName, mhdrChunk, mh2oChunk, mcnkChunks, mcnkChunkNames, mfboChunk);
        cadt.buildByteFile();        
        
        Cobj0 cobj0 = new Cobj0(blip, inputFileName, mmdxChunk, mmidChunk, mwmoChunk, mwidChunk, mddfChunk, modfChunk, mcnkChunks, mcnkChunkNames);          
        cobj0.buildByteFile();
        
        Ctex0 ctex0 = new Ctex0(blip, inputFileName, mtexChunk, mcnkChunks, mcnkChunkNames, mtxfChunk);        
        ctex0.buildByteFile();
    }
    
    public abstract void convertADT(File f);

    void parseMVER(byte[] bytes) {
        
        byte[] mver_verif= {Byte.decode("0x52"),    // R
                            Byte.decode("0x45"),    // E
                            Byte.decode("0x56"),    // V
                            Byte.decode("0x4d")};   // M
               
        if (Arrays.equals(mver_verif, Arrays.copyOfRange(bytes, offset, offset+mver_verif.length))) {
            mverChunk=new MVERChunk();
            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                
            	System.out.println(mverChunk);

            }
        } else {
            
        	System.out.println("Invalid file : problem at chunk MVER"); 
        	
            System.exit(1);
        }
        
        this.offset+=mver_verif.length+mverChunk.getSize();
    }

    void parseMHDR(byte[] bytes) {
        
        /* There is a flag which allow us to know whether the adt has a MFBO chunk or not :
         * MHDRChunk.MFBOFLAGOFFSET.
         * The MH2O and MTXF chunks are optionnal, hence the check of MHDRChunk.MH2OOFFSET and
         * MHDRChunk.MTXFOFFSET.
         */
        
        byte[] mhdr_verif= {Byte.decode("0x52"),    // R
                            Byte.decode("0x44"),    // D
                            Byte.decode("0x48"),    // H
                            Byte.decode("0x4d")};   // M
        
        if (Arrays.equals(mhdr_verif, Arrays.copyOfRange(bytes, offset, offset+mhdr_verif.length))) {
            offset+=mhdr_verif.length;
            int mhdr_offset=offset;
            
            byte[] mhdr_size_flag=Arrays.copyOfRange(bytes, offset, offset+Chunk.SIZEFLAG_SIZE);
            mhdr_size_flag=ADTConverterApp.reverseArray(mhdr_size_flag);
            offset+=Chunk.SIZEFLAG_SIZE;
            
            byte mhdr_mfbo_flag[]=Arrays.copyOfRange(bytes, offset+MHDRChunk.MFBOFLAGOFFSET, offset+MHDRChunk.MFBOFLAGOFFSET+4); // TODO : normalize the size of the flags?
            mhdr_mfbo_flag=ADTConverterApp.reverseArray(mhdr_mfbo_flag);
            if (ADTConverterApp.byteArray2int(mhdr_mfbo_flag) != 1 && ADTConverterApp.byteArray2int(mhdr_mfbo_flag) != 3) { // Mjo's cooking
                mfbo=false;
                if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                	
                	System.out.println("No chunk MFBO");
                	
                }
            }
            byte mhdr_mh2o[]=Arrays.copyOfRange(bytes, offset+MHDRChunk.MH2OOFFSET, offset+MHDRChunk.MH2OOFFSET+Chunk.SIZEFLAG_SIZE);
            if (ADTConverterApp.byteArray2int(mhdr_mh2o)==0) {
                mh2o=false;
                if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                    
                	System.out.println("No chunk MH2O");                	
                	
                }
            }
            
            byte mhdr_mtxf[]=Arrays.copyOfRange(bytes, offset+MHDRChunk.MTXFOFFSET, offset+MHDRChunk.MTXFOFFSET+Chunk.SIZEFLAG_SIZE);
            
            if (ADTConverterApp.byteArray2int(mhdr_mtxf)==0) {
                mtxf=false;
                if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                	
                	System.out.println("No chunk MTXF"); 
                	
                }
            }
            
            int mhdr_size=ADTConverterApp.byteArray2int(mhdr_size_flag);
            byte[] mhdr_data=Arrays.copyOfRange(bytes, offset, offset+mhdr_size);
            
            mhdrChunk=new MHDRChunk(mhdr_offset, mhdr_size, mhdr_data);
            offset+=mhdr_size;
            
            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
            	
            	System.out.println(mhdrChunk); 
            	
            }
        } else {
            
        	System.out.println("Invalid file : problem at chunk MHDR"); 
            System.exit(1);
        }
    }

    void parseMCIN(byte[] bytes) {
        
        /* The .data seems to have little to no interest, granted there are flags and size indications...
         * But experience learnt us NOT to trust the size. EVER.
         * So, don't underevaluate it :p
         */
        
        byte[] mcin_verif= {Byte.decode("0x4e"),    // N
                            Byte.decode("0x49"),    // I
                            Byte.decode("0x43"),    // C
                            Byte.decode("0x4d")};   // M
        
        if (Arrays.equals(mcin_verif, Arrays.copyOfRange(bytes, offset, offset+mcin_verif.length))) {
            offset+=mcin_verif.length;
            int mcin_offset=offset;
            
            byte[] mcin_size_flag=Arrays.copyOfRange(bytes, offset, offset+Chunk.SIZEFLAG_SIZE);
            mcin_size_flag=ADTConverterApp.reverseArray(mcin_size_flag);
            offset+=Chunk.SIZEFLAG_SIZE;
            
            int mcin_size=ADTConverterApp.byteArray2int(mcin_size_flag);
            byte[] mcin_data=Arrays.copyOfRange(bytes, offset, offset+mcin_size);
            
            mcinChunk=new MCINChunk(mcin_offset, mcin_size, mcin_data);
            offset+=mcin_size;
            
            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                
            	System.out.println(mcinChunk); 
            	
            }
        } else {
        	
        	System.out.println("Invalid file : problem at chunk MCIN"); 
            System.exit(1);
        }
    }

    void parseMTEX(byte[] bytes) {
        
        byte[] mtex_verif= {Byte.decode("0x58"),    // X
                            Byte.decode("0x45"),    // E
                            Byte.decode("0x54"),    // T
                            Byte.decode("0x4d")};   // M
        
        if (Arrays.equals(mtex_verif, Arrays.copyOfRange(bytes, offset, offset+mtex_verif.length))) {
            offset+=mtex_verif.length;
            int mtex_offset=offset;
            
            byte[] mtex_size_flag=Arrays.copyOfRange(bytes, offset, offset+Chunk.SIZEFLAG_SIZE);
            mtex_size_flag=ADTConverterApp.reverseArray(mtex_size_flag);
            offset+=Chunk.SIZEFLAG_SIZE;
            
            int mtex_size=ADTConverterApp.byteArray2int(mtex_size_flag);
            byte[] mtex_data=Arrays.copyOfRange(bytes, offset, offset+mtex_size);
            
            mtexChunk=new MTEXChunk(mtex_offset, mtex_size, mtex_data);
            offset+=mtex_size;
            
            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                
            	System.out.println(mtexChunk); 

            }
        } else {
            
        	System.out.println("Invalid file : problem at chunk MTEX"); 
            System.exit(1);
        }
    }

    void parseMMDX(byte[] bytes) {
        
        byte[] mmdx_verif= {Byte.decode("0x58"),    // X
                            Byte.decode("0x44"),    // D
                            Byte.decode("0x4d"),    // M
                            Byte.decode("0x4d")};   // M
        
        if (Arrays.equals(mmdx_verif, Arrays.copyOfRange(bytes, offset, offset+mmdx_verif.length))) {
            offset+=mmdx_verif.length;
            int mmdx_offset=offset;
            
            byte[] mmdx_size_flag=Arrays.copyOfRange(bytes, offset, offset+Chunk.SIZEFLAG_SIZE);
            mmdx_size_flag=ADTConverterApp.reverseArray(mmdx_size_flag);
            offset+=Chunk.SIZEFLAG_SIZE;
            
            int mmdx_size=ADTConverterApp.byteArray2int(mmdx_size_flag);
            byte[] mmdx_data=Arrays.copyOfRange(bytes, offset, offset+mmdx_size);
            
            mmdxChunk=new MMDXChunk(mmdx_offset, mmdx_size, mmdx_data);
            offset+=mmdx_size;
            
            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                
            	System.out.println(mmdxChunk); 
            	
            }
        } else {
            
        	System.out.println("Invalid file : problem at chunk MMDX"); 
            System.exit(1);
        }
    }

    void parseMMID(byte[] bytes) {
        
        byte[] mmid_verif= {Byte.decode("0x44"),    // D
                            Byte.decode("0x49"),    // I
                            Byte.decode("0x4d"),    // M
                            Byte.decode("0x4d")};   // M
        
        if (Arrays.equals(mmid_verif, Arrays.copyOfRange(bytes, offset, offset+mmid_verif.length))) {
            offset+=mmid_verif.length;
            int mmid_offset=offset;
            
            byte[] mmid_size_flag=Arrays.copyOfRange(bytes, offset, offset+Chunk.SIZEFLAG_SIZE);
            mmid_size_flag=ADTConverterApp.reverseArray(mmid_size_flag);
            offset+=Chunk.SIZEFLAG_SIZE;
            
            int mmid_size=ADTConverterApp.byteArray2int(mmid_size_flag);
            byte[] mmid_data=Arrays.copyOfRange(bytes, offset, offset+mmid_size);
            
            mmidChunk=new MMIDChunk(mmid_offset, mmid_size, mmid_data);
            offset+=mmid_size;
            
            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                
            	System.out.println(mmidChunk); 
            	
            }
        } else {
        	
        	System.out.println("Invalid file : problem at chunk MMID"); 
            System.exit(1);
        }
    }

    void parseMWMO(byte[] bytes) {
        
        byte[] mwmo_verif= {Byte.decode("0x4f"),    // O
                            Byte.decode("0x4d"),    // M
                            Byte.decode("0x57"),    // W
                            Byte.decode("0x4d")};   // M
        
        if (Arrays.equals(mwmo_verif, Arrays.copyOfRange(bytes, offset, offset+mwmo_verif.length))) {
            offset+=mwmo_verif.length;
            int mwmo_offset=offset;
            
            byte[] mwmo_size_flag=Arrays.copyOfRange(bytes, offset, offset+Chunk.SIZEFLAG_SIZE);
            mwmo_size_flag=ADTConverterApp.reverseArray(mwmo_size_flag);
            offset+=Chunk.SIZEFLAG_SIZE;
            
            int mwmo_size=ADTConverterApp.byteArray2int(mwmo_size_flag);
            byte[] mwmo_data=Arrays.copyOfRange(bytes, offset, offset+mwmo_size);
            
            mwmoChunk=new MWMOChunk(mwmo_offset, mwmo_size, mwmo_data);
            offset+=mwmo_size;
            
            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
            	
            	System.out.println(mwmoChunk); 
            
            }
        } else {
        	
        	System.out.println("Invalid file : problem at chunk MWMO");
            System.exit(1);
        }
    }

    void parseMWID(byte[] bytes) {
        
        byte[] mwid_verif= {Byte.decode("0x44"),    // D
                            Byte.decode("0x49"),    // I
                            Byte.decode("0x57"),    // W
                            Byte.decode("0x4d")};   // M
        
        if (Arrays.equals(mwid_verif, Arrays.copyOfRange(bytes, offset, offset+mwid_verif.length))) {
            offset+=mwid_verif.length;
            int mwid_offset=offset;
            
            byte[] mwid_size_flag=Arrays.copyOfRange(bytes, offset, offset+Chunk.SIZEFLAG_SIZE);
            mwid_size_flag=ADTConverterApp.reverseArray(mwid_size_flag);
            offset+=Chunk.SIZEFLAG_SIZE;
            
            int mwid_size=ADTConverterApp.byteArray2int(mwid_size_flag);
            byte[] mwid_data=Arrays.copyOfRange(bytes, offset, offset+mwid_size);
            
            mwidChunk=new MWIDChunk(mwid_offset, mwid_size, mwid_data);
            offset+=mwid_size;
            
            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
            	
            	System.out.println(mwidChunk); 
            	
            }
        } else {
        	
        	System.out.println("Invalid file : problem at chunk MWID"); 
            System.exit(1);
        }
    }

    void parseMDDF(byte[] bytes) {
        
        byte[] mddf_verif= {Byte.decode("0x46"),    // F
                            Byte.decode("0x44"),    // D
                            Byte.decode("0x44"),    // D
                            Byte.decode("0x4d")};   // M
        
        if (Arrays.equals(mddf_verif, Arrays.copyOfRange(bytes, offset, offset+mddf_verif.length))) {
            offset+=mddf_verif.length;
            int mddf_offset=offset;
            
            byte[] mddf_size_flag=Arrays.copyOfRange(bytes, offset, offset+Chunk.SIZEFLAG_SIZE);
            mddf_size_flag=ADTConverterApp.reverseArray(mddf_size_flag);
            offset+=Chunk.SIZEFLAG_SIZE;
            
            int mddf_size=ADTConverterApp.byteArray2int(mddf_size_flag);
            byte[] mddf_data=Arrays.copyOfRange(bytes, offset, offset+mddf_size);
            
            mddfChunk=new MDDFChunk(mddf_offset, mddf_size, mddf_data);
            offset+=mddf_size;
            
            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
            	
            	System.out.println(mddfChunk); 
            	
            }
        } else {
            
            System.out.println("Invalid file : problem at chunk MDDF"); 
            System.exit(1);
        }
    }

    void parseMODF(byte[] bytes) {
        
        byte[] modf_verif= {Byte.decode("0x46"),    // F
                            Byte.decode("0x44"),    // D
                            Byte.decode("0x4f"),    // O
                            Byte.decode("0x4d")};   // M
        
        if (Arrays.equals(modf_verif, Arrays.copyOfRange(bytes, offset, offset+modf_verif.length))) {
            offset+=modf_verif.length;
            int modf_offset=offset;
            
            byte[] modf_size_flag=Arrays.copyOfRange(bytes, offset, offset+Chunk.SIZEFLAG_SIZE);
            modf_size_flag=ADTConverterApp.reverseArray(modf_size_flag);
            offset+=Chunk.SIZEFLAG_SIZE;
            
            int modf_size=ADTConverterApp.byteArray2int(modf_size_flag);
            byte[] modf_data=Arrays.copyOfRange(bytes, offset, offset+modf_size);
            
            modfChunk=new MODFChunk(modf_offset, modf_size, modf_data);
            offset+=modf_size;
            
            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
            	
            	System.out.println(modfChunk); 

            }
        } else {
            
            System.out.println("Invalid file : problem at chunk MODF"); 
            System.exit(1);
        }
    }

    void parseMH2O(byte[] bytes) {
        
        if (mh2o) {
            byte[] mh2o_verif= {Byte.decode("0x4f"),    // O
                                Byte.decode("0x32"),    // 2
                                Byte.decode("0x48"),    // H
                                Byte.decode("0x4d")};   // M

            int mh2o_absolute_offset=ADTConverterApp.byteArray2int(ADTConverterApp.reverseArray(Arrays.copyOfRange(mhdrChunk.getChunkData(),MHDRChunk.MH2OOFFSET,MHDRChunk.MH2OOFFSET+Chunk.SIZEFLAG_SIZE)))+20; // MVER + MHDR name and size
            
            
            if (Arrays.equals(mh2o_verif, Arrays.copyOfRange(bytes, mh2o_absolute_offset, mh2o_absolute_offset+mh2o_verif.length))) {
                
                int mh2o_offset=mh2o_absolute_offset+mh2o_verif.length+Chunk.SIZEFLAG_SIZE;

                byte[] mh2o_size_flag=Arrays.copyOfRange(bytes, mh2o_absolute_offset+mh2o_verif.length, mh2o_absolute_offset+mh2o_verif.length+Chunk.SIZEFLAG_SIZE);
                mh2o_size_flag=ADTConverterApp.reverseArray(mh2o_size_flag);
                
                int mh2o_size=ADTConverterApp.byteArray2int(mh2o_size_flag);
                byte[] mh2o_data=null;
                if (mh2o_size==1413829191) { // size is set to GRET : it means Gretchin's "AllWater Wotlk" was used. Because he places his chunk at the end of the file, we must read until EOF.
                    mh2o_data=Arrays.copyOfRange(bytes, mh2o_absolute_offset+mh2o_verif.length+Chunk.SIZEFLAG_SIZE, bytes.length);
                    mh2o_size=mh2o_data.length;
                } else { // normal case
                    mh2o_data=Arrays.copyOfRange(bytes, offset+mh2o_verif.length+Chunk.SIZEFLAG_SIZE, offset+mh2o_verif.length+Chunk.SIZEFLAG_SIZE+mh2o_size);
                    offset+=mh2o_verif.length;
                    offset+=Chunk.SIZEFLAG_SIZE;
                    offset+=mh2o_size;
                }

                mh2oChunk=new MH2OChunk(mh2o_offset, mh2o_size, mh2o_data);

                if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                    
                	System.out.println(mh2oChunk); 
                	
                }
            } else {
                
            	System.out.println("Invalid file : problem at chunk MH20");    
                System.exit(1);
            } 
        } 
    }
    
    abstract void parseMCNK(byte[] bytes);

    void parseMCVT(byte[] bytes,int i) {
        
        byte[] mcvt_verif= {Byte.decode("0x54"),    // T
                            Byte.decode("0x56"),    // V
                            Byte.decode("0x43"),    // C
                            Byte.decode("0x4d")};   // M

        if (Arrays.equals(mcvt_verif, Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcvt_verif.length))) {
            local_mcnk_offset+=mcvt_verif.length;
            int mcvt_offset=local_mcnk_offset;

            byte[] mcvt_size_flag=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+Chunk.SIZEFLAG_SIZE);
            mcvt_size_flag=ADTConverterApp.reverseArray(mcvt_size_flag);
            local_mcnk_offset+=Chunk.SIZEFLAG_SIZE;

            int mcvt_size=ADTConverterApp.byteArray2int(mcvt_size_flag);
            byte[] mcvt_data=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcvt_size);

            mcnkChunks.get(i).add(mcnkChunkNames.get("MCVT"),new MCVTChunk(mcvt_offset, mcvt_size, mcvt_data));
            local_mcnk_offset+=mcvt_size;

            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {

                System.out.println(mcnkChunks.get(i).get(mcnkChunkNames.get("MCVT")));

            }
        } else {

            System.out.println("Invalid file : problem at chunk MCVT");
            System.exit(1);
        }
    }
    
    void parseMCLV(byte[] bytes, int i) {
        
        if (mclv) {
                    
            byte[] mclv_verif= {Byte.decode("0x56"),    // V
                                Byte.decode("0x4c"),    // L
                                Byte.decode("0x43"),    // C
                                Byte.decode("0x4d")};   // M

            if (Arrays.equals(mclv_verif, Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mclv_verif.length))) {
                local_mcnk_offset+=mclv_verif.length;
                int mclv_offset=local_mcnk_offset;

                byte[] mclv_size_flag=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+Chunk.SIZEFLAG_SIZE);
                mclv_size_flag=ADTConverterApp.reverseArray(mclv_size_flag);
                local_mcnk_offset+=Chunk.SIZEFLAG_SIZE;

                int mclv_size=ADTConverterApp.byteArray2int(mclv_size_flag);
                byte[] mclv_data=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mclv_size);

                mcnkChunks.get(i).add(new MCLVChunk(mclv_offset, mclv_size, mclv_data));
                local_mcnk_offset+=mclv_size;

                if (ADTConverterApp.config.getProperty("debug").equals("yes")) {

                        System.out.println(mcnkChunks.get(i).get(mcnkChunkNames.get("MCLV")));

                }
            } else {

                System.out.println("Invalid file : problem at chunk MCLV");
                System.exit(1);
            }

        } else {
            mcnkChunks.get(i).add(null);
        }
    }
    
    void parseMCCV(byte[] bytes, int i) {
        
        if (mccv) {
                    
            byte[] mccv_verif= {Byte.decode("0x56"),    // V
                                Byte.decode("0x43"),    // C
                                Byte.decode("0x43"),    // C
                                Byte.decode("0x4d")};   // M

            if (Arrays.equals(mccv_verif, Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mccv_verif.length))) {
                local_mcnk_offset+=mccv_verif.length;
                int mccv_offset=local_mcnk_offset;

                byte[] mccv_size_flag=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+Chunk.SIZEFLAG_SIZE);
                mccv_size_flag=ADTConverterApp.reverseArray(mccv_size_flag);
                local_mcnk_offset+=Chunk.SIZEFLAG_SIZE;

                int mccv_size=ADTConverterApp.byteArray2int(mccv_size_flag);
                byte[] mccv_data=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mccv_size);

                mcnkChunks.get(i).add(new MCCVChunk(mccv_offset, mccv_size, mccv_data));
                local_mcnk_offset+=mccv_size;

                if (ADTConverterApp.config.getProperty("debug").equals("yes")) {

                        System.out.println(mcnkChunks.get(i).get(mcnkChunkNames.get("MCCV")));

                }
            } else {

                System.out.println("Invalid file : problem at chunk MCCV");
                System.exit(1);
            }
        } else {
            mcnkChunks.get(i).add(null);
        }
    }
    
    void parseMCNR(byte[] bytes, int i) {
        
        byte[] mcnr_verif= {Byte.decode("0x52"),    // R
                            Byte.decode("0x4e"),    // N
                            Byte.decode("0x43"),    // C
                            Byte.decode("0x4d")};   // M

        if (Arrays.equals(mcnr_verif, Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcnr_verif.length))) {
            local_mcnk_offset+=mcnr_verif.length;
            int mcnr_offset=local_mcnk_offset;

            byte[] mcnr_size_flag=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+Chunk.SIZEFLAG_SIZE);
            mcnr_size_flag=ADTConverterApp.reverseArray(mcnr_size_flag);
            local_mcnk_offset+=Chunk.SIZEFLAG_SIZE;

            int mcnr_size=ADTConverterApp.byteArray2int(mcnr_size_flag);

            if (mcnr_size==MCNRChunk.MCNRREGULARSIZE) {
                byte[] mcnr_data=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcnr_size+POSTMCNRDATASIZE); // +POSTMCNRDATASIZE Added by Mjo : we need this !
                mcnkChunks.get(i).add(new MCNRChunk(mcnr_offset, mcnr_size, mcnr_data));
                local_mcnk_offset+=mcnr_size;

                /* We skip the bytes after the chunk, because they aren't technically part of the chunk ; 
                 * but remember they are used in the cataclysm ADT file (see Cadt.java, at "MCNR").
                 */
                local_mcnk_offset+=POSTMCNRDATASIZE;
            } else {
                if (mcnr_size==MCNRChunk.MCNRCATAALPHASIZE) {
                    byte[] mcnr_data=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcnr_size); // +POSTMCNRDATASIZE Added by Mjo : we need this !
                    mcnkChunks.get(i).add(new MCNRChunk(mcnr_offset, mcnr_size, mcnr_data));
                    local_mcnk_offset+=mcnr_size;
                }
            }

            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {

                System.out.println(mcnkChunks.get(i).get(mcnkChunkNames.get("MCNR")));

            }
        } else {

            System.out.println("Invalid file : problem at chunk MCNR");
            System.exit(1);
        }
    }
    
    void parseMCLY(byte[] bytes, int i) {
        
         byte[] mcly_verif= {Byte.decode("0x59"),   // Y
                            Byte.decode("0x4c"),    // L
                            Byte.decode("0x43"),    // C
                            Byte.decode("0x4d")};   // M

        if (Arrays.equals(mcly_verif, Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcly_verif.length))) {
            local_mcnk_offset+=mcly_verif.length;
            int mcly_offset=local_mcnk_offset;

            byte[] mcly_size_flag=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+Chunk.SIZEFLAG_SIZE);
            mcly_size_flag=ADTConverterApp.reverseArray(mcly_size_flag);
            local_mcnk_offset+=Chunk.SIZEFLAG_SIZE;

            int mcly_size=ADTConverterApp.byteArray2int(mcly_size_flag);
            byte[] mcly_data=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcly_size);

            mcnkChunks.get(i).add(mcnkChunkNames.get("MCLY"),new MCLYChunk(mcly_offset, mcly_size, mcly_data));
            local_mcnk_offset+=mcly_size;

            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {

                System.out.println(mcnkChunks.get(i).get(mcnkChunkNames.get("MCLY")));

            }
        } else {

            System.out.println("Invalid file : problem at chunk MCLY");
            System.exit(1);
        }
    }
    
    void parseMCRF(byte[] bytes, int i) {
        
        byte[] mcrf_verif= {Byte.decode("0x46"),    // F
                            Byte.decode("0x52"),    // R
                            Byte.decode("0x43"),    // C
                            Byte.decode("0x4d")};   // M

        if (Arrays.equals(mcrf_verif, Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcrf_verif.length))) {
            local_mcnk_offset+=mcrf_verif.length;
            int mcrf_offset=local_mcnk_offset;

            byte[] mcrf_size_flag=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+Chunk.SIZEFLAG_SIZE);
            mcrf_size_flag=ADTConverterApp.reverseArray(mcrf_size_flag);
            local_mcnk_offset+=Chunk.SIZEFLAG_SIZE;

            int mcrf_size=ADTConverterApp.byteArray2int(mcrf_size_flag);
            byte[] mcrf_data=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcrf_size);

            mcnkChunks.get(i).add(mcnkChunkNames.get("MCRF"),new MCRFChunk(mcrf_offset, mcrf_size, mcrf_data));
            local_mcnk_offset+=mcrf_size;

            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {

                System.out.println(mcnkChunks.get(i).get(mcnkChunkNames.get("MCRF")));

            }
        } else {

                System.out.println("Invalid file : problem at chunk MCRF");
            System.exit(1);
        }
    }
    
    void parseMCSH(byte[] bytes, int i) {
        
        if (mcsh) {
            byte[] mcsh_verif= {Byte.decode("0x48"),    // H
                                Byte.decode("0x53"),    // S
                                Byte.decode("0x43"),    // C
                                Byte.decode("0x4d")};   // M

            if (Arrays.equals(mcsh_verif, Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcsh_verif.length))) {
                local_mcnk_offset+=mcsh_verif.length;
                int mcsh_offset=local_mcnk_offset;

                byte[] mcsh_size_flag=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+Chunk.SIZEFLAG_SIZE);
                mcsh_size_flag=ADTConverterApp.reverseArray(mcsh_size_flag);
                local_mcnk_offset+=Chunk.SIZEFLAG_SIZE;

                int mcsh_size=ADTConverterApp.byteArray2int(mcsh_size_flag);
                byte[] mcsh_data=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcsh_size);

                mcnkChunks.get(i).add(mcnkChunkNames.get("MCSH"),new MCSHChunk(mcsh_offset, mcsh_size, mcsh_data));
                local_mcnk_offset+=mcsh_size;

                if (ADTConverterApp.config.getProperty("debug").equals("yes")) {

                        System.out.println(mcnkChunks.get(i).get(mcnkChunkNames.get("MCSH")));

                }
            } else {

                System.out.println("Invalid file : problem at chunk MCSH");
                System.exit(1);
            }
        } else {
            mcnkChunks.get(i).add(null);
        }
    }
    
    void parseMCAL(byte[] bytes, int i, int mclq_offset, int mcse_offset) {
        
        /* DON'T TRUST THE SIZE!
         * This one is an great example of the above advice : sometimes he announces
         * a zero size, while he does hava data. To counter this, we have to calculate it
         * using the MCLQ offset (thanks, MCNK header). If there is a MCLQ, obviously...
         */

        byte[] mcal_verif= {Byte.decode("0x4c"),    // L
                            Byte.decode("0x41"),    // A
                            Byte.decode("0x43"),    // C
                            Byte.decode("0x4d")};   // M

        if (Arrays.equals(mcal_verif, Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcal_verif.length))) {
            local_mcnk_offset+=mcal_verif.length;
            int mcal_offset=local_mcnk_offset;
            local_mcnk_offset+=Chunk.SIZEFLAG_SIZE;

            int mcal_size=0;
            if (mclq) {
                mcal_size=mclq_offset+mcnkChunks.get(i).get(mcnkChunkNames.get("MCNK")).getOffset()-local_mcnk_offset;
            } else {
                mcal_size=mcse_offset+mcnkChunks.get(i).get(mcnkChunkNames.get("MCNK")).getOffset()-local_mcnk_offset;
            }
            byte[] mcal_data=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mcal_size); 

            mcnkChunks.get(i).add(mcnkChunkNames.get("MCAL"),new MCALChunk(mcal_offset, mcal_size, mcal_data));
            local_mcnk_offset+=mcal_size;

            if (ADTConverterApp.config.getProperty("debug").equals("yes")) {

                System.out.println(mcnkChunks.get(i).get(mcnkChunkNames.get("MCAL")));

            }
        } else {

                System.out.println("Invalid file : problem at chunk MCAL");
            System.exit(1);
        }
    }
    
    void parseMCLQ(byte[] bytes, int i, int mclq_offset, int mcse_offset) {
        
        if (mclq) {
            byte[] mclq_verif= {Byte.decode("0x51"),    // Q
                                Byte.decode("0x4c"),    // L
                                Byte.decode("0x43"),    // C
                                Byte.decode("0x4d")};   // M

            if (Arrays.equals(mclq_verif, Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mclq_verif.length))) {
                local_mcnk_offset+=mclq_verif.length;
                mclq_offset=local_mcnk_offset;

                //byte[] mclq_size_flag=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+Chunk.SIZEFLAG_SIZE);
                //mclq_size_flag=ADTConverterApp.reverseArray(mclq_size_flag);
                local_mcnk_offset+=Chunk.SIZEFLAG_SIZE;


                int mclq_size=0;
                if (mcse) {
                    mclq_size=mcse_offset+mcnkChunks.get(i).get(mcnkChunkNames.get("MCNK")).getOffset()-local_mcnk_offset;
                    // DON'T TRUST THE SIZE!!!
                    // int mclq_size=ADTConverterApp.byteArray2int(mclq_size_flag);
                } else {
                    if (offset>=bytes.length) { // EOF
                        mclq_size=bytes.length-local_mcnk_offset;
                    } else {
                        if (Byte.decode("0x4b")==bytes[offset]) { // there is a MCNK chunk directly after this MCLQ
                            mclq_size=offset-local_mcnk_offset;
                        } else {
                            System.out.println("Unexpected chunk following the MCLQ.");
                            System.exit(1);
                        }
                    }

                }
                byte[] mclq_data=Arrays.copyOfRange(bytes, local_mcnk_offset, local_mcnk_offset+mclq_size);

                mcnkChunks.get(i).add(mcnkChunkNames.get("MCLQ"),new MCLQChunk(mclq_offset, mclq_size, mclq_data));
                local_mcnk_offset+=mclq_size;

                if (ADTConverterApp.config.getProperty("debug").equals("yes")) {

                    System.out.println(mcnkChunks.get(i).get(mcnkChunkNames.get("MCLQ")));

                }
            } else {

                System.out.println("Invalid file : problem at chunk MCLQ");
                System.exit(1);
                }
            } else {
            mcnkChunks.get(i).add(null);
        }
    }
    
    abstract void parseMCSE(byte[] bytes, int i, int mcse_offset);
    
    void parseMFBO(byte[] bytes) {
        
        if (mfbo) {
            byte[] mfbo_verif= {Byte.decode("0x4f"),    // O
                                Byte.decode("0x42"),    // B
                                Byte.decode("0x46"),    // F
                                Byte.decode("0x4d")};   // M

            if (Arrays.equals(mfbo_verif, Arrays.copyOfRange(bytes, offset, offset+mfbo_verif.length))) {
                offset+=mfbo_verif.length;
                int mfbo_offset=offset;

                byte[] mfbo_size_flag=Arrays.copyOfRange(bytes, offset, offset+Chunk.SIZEFLAG_SIZE);
                mfbo_size_flag=ADTConverterApp.reverseArray(mfbo_size_flag);
                offset+=Chunk.SIZEFLAG_SIZE;

                int mfbo_size=ADTConverterApp.byteArray2int(mfbo_size_flag);
                byte[] mfbo_data=Arrays.copyOfRange(bytes, offset, offset+mfbo_size);

                mfboChunk=new MFBOChunk(mfbo_offset, mfbo_size, mfbo_data);
                offset+=mfbo_size;

                if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                	System.out.println(mfboChunk);
                }
            } else {
                
            	System.out.println("Invalid file : problem at chunk MFBO");  
                System.exit(1);
            }
        }
    }
    
    void parseMTXF(byte[] bytes) {
        
        if (mtxf) {
            byte[] mtxf_verif= {Byte.decode("0x46"),    // F
                                Byte.decode("0x58"),    // X
                                Byte.decode("0x54"),    // T
                                Byte.decode("0x4d")};   // M            
            
            if (Arrays.equals(mtxf_verif, Arrays.copyOfRange(bytes, offset, offset+mtxf_verif.length))) {
                
                offset+=mtxf_verif.length;
                int mtxf_offset=offset;
                
                byte[] mtxf_size_flag=Arrays.copyOfRange(bytes, offset, offset+Chunk.SIZEFLAG_SIZE);
                mtxf_size_flag=ADTConverterApp.reverseArray(mtxf_size_flag);
                offset+=Chunk.SIZEFLAG_SIZE;

                int mtxf_size=ADTConverterApp.byteArray2int(mtxf_size_flag);
                byte[] mtxf_data=Arrays.copyOfRange(bytes, offset, offset+mtxf_size);

                mtxfChunk=new MTXFChunk(mtxf_offset, mtxf_size, mtxf_data);
                offset+=mtxf_size;

                if (ADTConverterApp.config.getProperty("debug").equals("yes")) {
                    
                	System.out.println(mtxfChunk); 
                	
                }
            } else {
                
            	System.out.println("Invalid file : problem at chunk MTXF"); 
                System.exit(1);
            }
        }
    }
}
