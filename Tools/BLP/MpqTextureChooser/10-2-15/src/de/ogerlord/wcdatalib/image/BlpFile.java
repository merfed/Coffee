package de.ogerlord.wcdatalib.image;

import com.sun.image.codec.jpeg.JPEGCodec;
import com.sun.image.codec.jpeg.JPEGEncodeParam;
import com.sun.image.codec.jpeg.JPEGImageDecoder;
import com.sun.image.codec.jpeg.JPEGImageEncoder;

import de.ogerlord.wcdatalib.stream.BlizzardDataInputStream;
import de.ogerlord.wcdatalib.stream.BlizzardDataOutputStream;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.awt.image.IndexColorModel;
import java.awt.image.Raster;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.HashMap;

/**
 * This class offers the utilities to convert blp files into BufferedImages and
 * BufferedImages into blps.
 *
 * @author Gexxo, Oger-Lord
 */
public class BlpFile {

    /**
     * Loads a blp file into a BufferedImage
     * @param f
     * @return
     * @throws IOException 
     */
    public static BufferedImage read(File f) throws IOException {
        return read(f.getName(), new FileInputStream(f));
    }

    /**
     * Reads a blp file from a given input stream.
     * 
     * @param name
     * @param stream
     * @return
     * @throws IOException 
     */
    public static BufferedImage read(String name, InputStream stream)
            throws IOException {
        @SuppressWarnings("resource")
		BlizzardDataInputStream in = new BlizzardDataInputStream(stream);
        if (!in.readCharsAsString(4).equals("BLP1")) {
            throw new IOException(name + " is no valid blp file");
        }
        int type = in.readInt();
        @SuppressWarnings("unused")
		int alpha = in.readInt();
        int width = in.readInt();
        int height = in.readInt();
        int teamColorFlag = in.readInt();
        @SuppressWarnings("unused")
		int always1 = in.readInt();
        int[] mipmapOffsets = new int[16];

        for (int i = 0; i < 16; i++) {
            mipmapOffsets[i] = in.readInt();
        }
        int[] mipmapSizes = new int[16];
        for (int i = 0; i < 16; i++) {
            mipmapSizes[i] = in.readInt();
        }

        if (type == 0) {  //JPEG BLP

            int jpgHeaderSize = in.readInt();
            byte[] jpgHeader = new byte[jpgHeaderSize];
            in.read(jpgHeader);


            byte[] zeros = new byte[(int) (mipmapOffsets[0] - in.getOffset())];
            in.read(zeros);


            byte[][] mipmaps = new byte[16][];

            //Only load first mipmap
            mipmaps[0] = new byte[mipmapSizes[0]];
            in.read(mipmaps[0]);

            byte[] result = Arrays.copyOf(jpgHeader, jpgHeader.length + mipmaps[0].length);
            System.arraycopy(mipmaps[0], 0, result, jpgHeader.length, mipmaps[0].length);


            ByteArrayInputStream read = new ByteArrayInputStream(result);//byteOut.toByteArray());
            JPEGImageDecoder jpg = JPEGCodec.createJPEGDecoder(read);
            Raster r = jpg.decodeAsRaster();
            read.close();

            int picwidth = r.getWidth();
            int picheight = r.getHeight();
            BufferedImage b = new BufferedImage(picwidth, picheight, BufferedImage.TYPE_4BYTE_ABGR);
            for (int x = 0; x < picwidth; x++) {
                for (int y = 0; y < picheight; y++) {
                    int[] pix = new int[4];
                    pix = r.getPixel(x, y, pix);
                    b.setRGB(x, y, new Color(pix[2], pix[1], pix[0], pix[3]).getRGB());
                }
            }
            return b;
        }


        if (type == 1) { //Paletted BLP

            byte[][] palette = new byte[256][4];
            for (int i = 0; i < palette.length; i++) {
                in.read(palette[i]);
            }
            byte[][] colors = new byte[height][width];
            for (int i = 0; i < height; i++) {
                in.read(colors[i]);
            }
            byte[][] alphaChannel = (byte[][]) null;
            if (teamColorFlag != 5) {
                alphaChannel = new byte[height][width];
                for (int i = 0; i < height; i++) {
                    in.read(alphaChannel[i]);
                }
            }

            BufferedImage b = new BufferedImage(width, height, 6);
            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {

                    int position = colors[y][x] & 0xff;

                    if (alphaChannel != null) {
                        int alphaPos = alphaChannel[y][x] & 0xff;
                        b.setRGB(x, y, new Color(palette[position][2] & 0xff, palette[position][1] & 0xff, palette[position][0] & 0xff, alphaPos).getRGB());
                    } else {
                        b.setRGB(x, y, new Color(palette[position][2] & 0xff, palette[position][1] & 0xff, palette[position][0] & 0xff, 255).getRGB());
                    }

                }
            }
            return b;
        } else {
            throw new IOException(name + " has an unknown BLP type: " + type);
        }
    }


    /**
     *
     * @param b
     * @param f
     * @param useAlpha
     * @param generateMipMaps
     * @param antiDither if the image is not already indexed, a palette needs to
     * be generated for this image. false is normal, true tries to avoid dither
     * @throws IOException
     */
    public static void writePalettedBLP(BufferedImage b, File f, boolean useAlpha, boolean generateMipMaps, boolean antiDither) //, boolean increasePalette
            throws IOException {
        BlizzardDataOutputStream out = new BlizzardDataOutputStream(f);
        out.writeNByteString("BLP1", 4);
        out.writeInt(1); //Palettet
        out.writeInt(useAlpha ? 8 : 0);
        out.writeInt(b.getWidth());
        out.writeInt(b.getHeight());
        out.writeInt(useAlpha ? 4 : 5);
        out.writeInt(1);
        out.close();
        if (useAlpha && b.getColorModel().hasAlpha() == false) {
            throw new IllegalArgumentException("The image has no alpha channel");
        }


        int offset = 7 * 4 + 256 * 4 + 32 * 4;
        int size = b.getHeight() * b.getWidth();

        if (useAlpha) {
            size *= 2;
        }

        int[] sizes = new int[16];

        //Prepare mipmap sizes
        if(generateMipMaps){
            for (int i = 0; i < 16; i++) {
                sizes[i] = size;
                size /= 4;
            }
        }else{
            int startsize=size;
            for (int i = 0; i < 16; i++) {
                if(size != 0){
                    sizes[i] = startsize;
                }
                size /= 4;
            }
        }
    

        //Write MipMap offsets
        if(generateMipMaps){
            for (int i = 0; i < 16; i++) {
                out.writeInt(offset);
                offset += sizes[i];
            }
        }else{
            for (int i = 0; i < 16; i++) {
                if(sizes[i] != 0){
                    out.writeInt(offset);
                }else{
                    out.writeInt(0);
                }
            }
        }

        //Write MipMap sizes
        for (int i = 0; i < 16; i++) {
            out.writeInt(sizes[i]);
        }

        //Create Mipmaps
        BufferedImage[] mips;
        BufferedImage[] mips_indexed;
        
        if(generateMipMaps){
            mips = ImageUtils.generateMipMaps(b);
            
        }else{
            mips = new BufferedImage[1];
            mips[0] = b;
        }
        
        mips_indexed = new BufferedImage[mips.length];

        for (int i = 0; i < mips.length; i++) {
            if (mips[i].getType() != BufferedImage.TYPE_BYTE_INDEXED) {

                if (antiDither) {
                    mips_indexed[i] = ImageUtils.antiDitherConvert(mips[i]);
                } else {
                    mips_indexed[i] = ImageUtils.changeImageType(mips[i], BufferedImage.TYPE_BYTE_INDEXED);
                }
            }
        }

        //Write Paletted colors
        IndexColorModel colorModel = ((IndexColorModel) mips_indexed[0].getColorModel());
        int rgb[] = new int[colorModel.getMapSize()];
        colorModel.getRGBs(rgb);

        HashMap<Integer, Integer> colorMap = new HashMap<Integer, Integer>();


        for (int i = 0; i < 256; i++) {
            Color c = new Color(rgb[i]);//rgb[0]

            colorMap.put(rgb[i], i);

            out.write(c.getBlue() & 0xff);
            out.write(c.getGreen() & 0xff);
            out.write(c.getRed() & 0xff);

            out.write(0xff);//c.getAlpha() & 0xff
        }

        
        //Write MipMaps
        for (int i = 0; i < mips.length; i++) {

            int width = mips_indexed[i].getWidth();
            int height = mips_indexed[i].getHeight();

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    out.write(colorMap.get(mips_indexed[i].getRGB(x, y)) & 0xff);
                }
            }

            if (useAlpha) {
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        int[] pix = new int[4];
                        pix = mips[i].getRaster().getPixel(x, y, pix);

                        out.writeByte(pix[3]);
                    }
                }
            }
        }

        out.close();
    }

    

    public static void writeJpgBLP(BufferedImage b, File f, boolean useAlpha, float quality)
            throws IOException {

        BlizzardDataOutputStream out = new BlizzardDataOutputStream(f);
        out.writeNByteString("BLP1", 4);
        out.writeInt(0); //JPG
        out.writeInt(useAlpha ? 8 : 0); // No Alpha
        out.writeInt(b.getWidth());
        out.writeInt(b.getHeight());
        out.writeInt(useAlpha ? 4 : 5);
        out.writeInt(1);


        //The header size of the java generated jpgs
        int originalHeaderSize = 308;
        //Size of the header information we will add to every mipmap
        int mipMapHeaderSize = 20;//useAlpha ? 20 : 17;

        if (useAlpha == false) {
            originalHeaderSize -= 3;
        }

        //Manipulate Image, Swap Colors
        BufferedImage newImage = ImageUtils.convertStandardImageType(b,useAlpha);
        /*if(b.getType() != BufferedImage.TYPE_INT_ARGB && b.getType() != BufferedImage.TYPE_INT_RGB){
            newImage = ImageUtils.changeImageType(b, useAlpha? BufferedImage.TYPE_INT_ARGB : BufferedImage.TYPE_INT_RGB);
        }else{
            newImage = b;
        }*/

        for (int x = 0; x < newImage.getWidth(); x++) {
            for (int y = 0; y < newImage.getHeight(); y++) {
                int[] pix = new int[4];
                pix = newImage.getRaster().getPixel(x, y, pix);

                newImage.setRGB(x, y, new Color(pix[2], pix[1], pix[0], pix[3]).getRGB());

            }
        }

        //Generate MipMaps
        BufferedImage[] mips = ImageUtils.generateMipMaps(newImage);
        byte[][] imagesBytes = new byte[16][];

        int i = 0;
        for (BufferedImage image : mips) {

            ByteArrayOutputStream byteOut = new ByteArrayOutputStream();

            JPEGImageEncoder jpg = JPEGCodec.createJPEGEncoder(byteOut);

            JPEGEncodeParam param = JPEGCodec.getDefaultJPEGEncodeParam(image.getData(), JPEGEncodeParam.COLOR_ID_UNKNOWN);
            param.setQuality(quality, true);
            jpg.setJPEGEncodeParam(param);

            jpg.encode(image.getData());
            imagesBytes[i] = byteOut.toByteArray();

            i++;
        }
        
        //Create the jpg header we want to write
        byte[] newHeader = removeJpgHeaderImageSize(imagesBytes[0], originalHeaderSize);
        int headerSize = newHeader.length;
        
        //offset of the first mipmap
        int offset = 160 + headerSize;

        //Write Mipmap Offset
        for (byte[] imagesByte : imagesBytes) {

            if (imagesByte == null) {
                out.writeInt(0);
            } else {
                out.writeInt(offset);
                    offset += (imagesByte.length - originalHeaderSize - 1 + mipMapHeaderSize);
                }
                
            }

        //Write Mipmap Size
        for (byte[] imagesByte : imagesBytes) {
            if (imagesByte == null) {
                out.writeInt(0);
            } else {
                out.writeInt((imagesByte.length - originalHeaderSize - 1 + mipMapHeaderSize));
            }
        }


        //HeaderSize
        out.writeInt(headerSize);

        //Header
        out.write(newHeader);


        i = 0;
        //Write Mipmaps
        for (byte[] imagesByte : imagesBytes) {
            if (imagesByte != null) {

                createMipMapJpgHeader(out, mips[i].getWidth(), mips[i].getHeight(), useAlpha);

                out.write(imagesByte, originalHeaderSize + 1, imagesByte.length - originalHeaderSize - 1);
                //writeByteArray(out, imagesByte, originalHeaderSize + 1, imagesByte.length);

            }
            i++;
        }

        out.close();
    }

    /**
     * This function generates the jpg header of mipmaps.
     * The mipmaps uses the overall header and complet the information.
     * 
     * @param out
     * @param width
     * @param height
     * @param hasAlpha
     * @throws IOException 
     */
    private static void createMipMapJpgHeader(BlizzardDataOutputStream out, int width, int height, boolean hasAlpha) throws IOException {

        //if (hasAlpha) {
        out.writeByte(0x00);
        out.writeByte(0x14);
        out.writeByte(0x08);
        // }

        out.writeByte(height / 256);
        out.writeByte(height % 256);
        out.writeByte(width / 256);
        out.writeByte(width % 256);

        out.writeByte(0x04);
        out.writeByte(0x00);
        out.writeByte(0x11);
        out.writeByte(0x00);
        out.writeByte(0x01);
        out.writeByte(0x11);
        out.writeByte(0x00);
        out.writeByte(0x02);
        out.writeByte(0x11);
        out.writeByte(0x00);
        out.writeByte(0x03);
        out.writeByte(0x11);
        out.writeByte(0x00);
    }

    /**
     * Remove the image size out of a jpg header.
     * This is important because wc3 adds this information at the beginning of the mipmaps.
     * As a result the header should not be closed but instead end with FAFFC0
     * 
     * @param jpg The complere jpg file
     * @param headerSize The size of the header
     * @return 
     */
    private static byte[] removeJpgHeaderImageSize(byte[] jpg, int headerSize) {

        int skip = -1;
        ByteArrayOutputStream byteOut = new ByteArrayOutputStream();

        for (int i = 0; i < headerSize - 1; i++) {

            if (skip == 1) {

                if (jpg[i] == -1) { //Stop at next FF
                    skip = -2;
                    byteOut.write(jpg[i]);
                }

            } else {

                if (skip == -1 && jpg[i] == -1 && jpg[i + 1] == -64) { //FFC0

                    skip = 1;
                } else {
                    byteOut.write(jpg[i]);
                }
            }
        }

        byteOut.write(jpg[headerSize - 1]);
        byteOut.write(0xFA);
        byteOut.write(0xFF);
        byteOut.write(0xC0);

        return byteOut.toByteArray();

    }
}
