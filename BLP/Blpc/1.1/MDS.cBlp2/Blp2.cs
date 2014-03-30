/*
 * Copyright (c) 2010 Marcus Schweda <marcus.schweda@rwth-aachen.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;

namespace MDS.cBlp2 {

    /* 2DO
     * 
     *      Preserve existing 256 color palettes
     *      JPEG format
     *      Optimize visibility
     *      Scale down data types
     *      Unsafe code
     *      Reset on BLPFormatException?
     * 
     */

    /// <summary>
    /// Provides functionality to view and save Blp2 files.
    /// </summary>
    public sealed class Blp2 : IDisposable {

        private Bitmap bmp = null;
        private Nullable<Blp2Header> header;
        private FileFormat format;

        public enum FileFormat : int {
            Unknown = 0x0,
            Raw = 0x1,
            Dxt = 0x2,
            Jpeg = 0x4,
            Image = 0x8
        }

        public enum RawFormat : int {
            Raw1 = 0x1,
            Raw3 = 0x2,
        }

        public Blp2() { }

        ~Blp2() {
            this.Dispose();
        }

        /// <summary>
        /// Dispose object
        /// </summary>
        public void Dispose() {
            if (this.bmp != null) this.bmp.Dispose();
            header = null;
        }

        /// <summary>
        /// Implicit conversion to Bitmap (for displaying or altering the content).
        /// </summary>
        /// <param name="blp">BLP object.</param>
        /// <returns>Bitmap object.</returns>
        public static implicit operator Bitmap(Blp2 blp) {
            return blp.bmp;
        }

        /// <summary>
        /// Saves the currently loaded image to a common image file.
        /// </summary>
        /// <param name="format">The target Mime type of the image.</param>
        public void Save(string filename, ImageFormat format) {
            if (bmp != null) {
                // Create a copy from the image to avoid a GDI+ bug where InternalException is thrown
                Bitmap temp = new Bitmap(bmp);
                temp.Save(filename, format);
                temp.Dispose();
            } else {
                throw new BlpConversionException("No image data to convert.");
            }
        }

        /// <summary>
        /// Saves the currently loaded image to a uncompressed or paletted Blp file.
        /// </summary>
        /// <param name="filename">The path to the output file.</param>
        /// <param name="format">Raw format. See <see cref="RawFormat"/></param>
        /// <param name="resizeMethod">Method for possibly required resizing. See <see cref="FixSize"/></param>
        /// <param name="hasMipmaps">Specifies whether mipmaps are generated or not.</param>
        /// <param name="samplingFactor">(Optional) Sampling factor 1-30. Lower values stand for higher quality to the disadvantage of speed. Does not apply to Raw3 encoding.</param>
        /// <param name="samplingFactor">(Optional) Specifies whether or not to use dithering. Does not apply to Raw3 encoding.</param>
        public void Save(string filename, RawFormat format, ResizeMethod resizeMethod, bool hasMipmaps, byte samplingFactor = 10, bool dither = false) {

            if (bmp != null) {

                FileStream fs = new FileStream(filename, FileMode.Create, FileAccess.Write, FileShare.Write);

                using (BinaryWriter bw = new BinaryWriter(fs)) {

                    Blp2Header header = new Blp2Header(DataType.Uncompressed_DirectX, 0, AlphaDepth.Alpha8Bit, AlphaEncoding.RAW8BIT, (byte)(hasMipmaps ? 1 : 0), 0, 0);

                    List<Bitmap> bmps = new List<Bitmap>();
                    // Add resized image to the output
                    bmps.Add(FixSize(resizeMethod));

                    header.width = (uint)bmps[0].Width;
                    header.height = (uint)bmps[0].Height;

                    // Add mipmaps
                    if (hasMipmaps) bmps.AddRange(GenerateMipmaps(bmps[0]));

                    byte[] compImgData = { };
                    byte[] colorPalette = new byte[256 * 4];

                    if (format == RawFormat.Raw1) {
                        // Uncompressed paletted image
                        if (samplingFactor < 1 || samplingFactor > 30) throw new ArgumentException("Sampling factor must be between 1 and 30.");
                        header.encoding = Encoding.RAW1;

                        // Quantize colors to a 256 color palette
                        NeuQuant quant = new NeuQuant(bmps[0]);
                        quant.Quantize();

                        // Set color palette
                        int j = 0;
                        foreach (Color c in quant.Palette) {
                            colorPalette[j] = c.R;
                            colorPalette[j + 1] = c.G;
                            colorPalette[j + 2] = c.B;
                            colorPalette[j + 3] = 0;
                            j += 4;
                        }

                        for (int i = 0; i < bmps.Count; i++) {
                            // Copy raw image data to byte array
                            BitmapData bmpData = bmps[i].LockBits(new Rectangle(0, 0, bmps[i].Width, bmps[i].Height), ImageLockMode.ReadOnly, bmps[i].PixelFormat);
                            byte[] imgData = new byte[bmpData.Stride * bmps[i].Height];
                            Marshal.Copy(bmpData.Scan0, imgData, 0, bmpData.Stride * bmps[i].Height);
                            bmps[i].UnlockBits(bmpData);

                            byte[] alphaData = new byte[imgData.Length / 4];

                            // Index image colors
                            int curOffset = compImgData.Length;
                            Array.Resize(ref compImgData, compImgData.Length + imgData.Length / 4);

                            for (j = 0; j < imgData.Length; j += 4) {
                                Color c = Color.FromArgb(imgData[j], imgData[j + 1], imgData[j + 2]);
                                compImgData[curOffset + j / 4] = (byte)quant.GetPaletteIndex(c);

                                // Floyd-Steinberg dithering
                                if (dither) {
                                    // Quantization error
                                    int diffErrR = c.R - quant.Palette[compImgData[curOffset + j / 4]].R;
                                    int diffErrG = c.G - quant.Palette[compImgData[curOffset + j / 4]].G;
                                    int diffErrB = c.B - quant.Palette[compImgData[curOffset + j / 4]].B;

                                    // Diffuse error, distribute  to neighbour bytes as follows:
                                    //  X      P     7/16
                                    // 3/16   5/16   1/16
                                    if (bmpData.Stride - (j % bmpData.Stride) >= 8) {
                                        imgData[j + 4] = truncateByte(imgData[j + 4], (diffErrR * 7) >> 4);
                                        imgData[j + 5] = truncateByte(imgData[j + 5], (diffErrG * 7) >> 4);
                                        imgData[j + 6] = truncateByte(imgData[j + 6], (diffErrB * 7) >> 4);
                                    } else if (j + bmpData.Stride < imgData.Length) {
                                        imgData[j + bmpData.Stride] = truncateByte(imgData[j + bmpData.Stride], (diffErrR * 5) >> 4);
                                        imgData[j + bmpData.Stride + 1] = truncateByte(imgData[j + bmpData.Stride + 1], (diffErrG * 5) >> 4);
                                        imgData[j + bmpData.Stride + 2] = truncateByte(imgData[j + bmpData.Stride + 2], (diffErrB * 5) >> 4);

                                        if (bmpData.Stride - (j % bmpData.Stride) >= 8) {
                                            imgData[j + bmpData.Stride + 4] = truncateByte(imgData[j + bmpData.Stride + 4], (diffErrR * 1) >> 4);
                                            imgData[j + bmpData.Stride + 5] = truncateByte(imgData[j + bmpData.Stride + 5], (diffErrG * 1) >> 4);
                                            imgData[j + bmpData.Stride + 6] = truncateByte(imgData[j + bmpData.Stride + 6], (diffErrB * 1) >> 4);
                                        } else if (j % bmpData.Stride >= 4) {
                                            imgData[j + bmpData.Stride - 4] = truncateByte(imgData[j + bmpData.Stride - 4], (diffErrR * 3) >> 4);
                                            imgData[j + bmpData.Stride - 3] = truncateByte(imgData[j + bmpData.Stride - 3], (diffErrG * 3) >> 4);
                                            imgData[j + bmpData.Stride - 2] = truncateByte(imgData[j + bmpData.Stride - 2], (diffErrB * 3) >> 4);
                                        }
                                    }
                                }

                                // Fill alpha array (8 bit)
                                alphaData[j / 4] = imgData[j + 3];
                            }

                            // Append alpha channels
                            Array.Resize(ref compImgData, compImgData.Length + alphaData.Length);
                            alphaData.CopyTo(compImgData, compImgData.Length - alphaData.Length);

                            // Image data locations
                            header.offsets[i] = (uint)(curOffset + Marshal.SizeOf(typeof(Blp2Header)) + colorPalette.Length);
                            header.lengths[i] = (uint)(compImgData.Length - ((i > 0) ? header.offsets[i - 1] : 0));
                        }

                    } else if (format == RawFormat.Raw3) {

                        // Uncompressed 32bpp image
                        header.encoding = Encoding.RAW3;

                        for (int i = 0; i < bmps.Count; i++) {
                            // Copy raw image data to byte array
                            BitmapData bmpData = bmps[i].LockBits(new Rectangle(0, 0, bmps[i].Width, bmps[i].Height), ImageLockMode.ReadOnly, bmps[i].PixelFormat);
                            byte[] imgData = new byte[bmpData.Stride * bmps[i].Height];
                            Marshal.Copy(bmpData.Scan0, imgData, 0, bmpData.Stride * bmps[i].Height);
                            bmps[i].UnlockBits(bmpData);

                            Array.Resize(ref compImgData, compImgData.Length + imgData.Length);
                            imgData.CopyTo(compImgData, compImgData.Length - imgData.Length);

                            // Image data locations
                            header.offsets[i] = (i > 0) ? header.offsets[i - 1] + header.lengths[i - 1] : (uint)(Marshal.SizeOf(typeof(Blp2Header)) + colorPalette.Length);
                            header.lengths[i] = (uint)imgData.Length;
                        }

                    }

                    bw.Write(header.GetBytes());
                    bw.Write(colorPalette);
                    bw.Write(compImgData);

                }

            } else {
                throw new BlpConversionException("No image data to convert.");
            }

        }

        /// <summary>
        /// Saves the currently loaded image to a Dxt encoded Blp file.
        /// </summary>
        /// <param name="filename">The path to the output file.</param>
        /// <param name="format">Dxt format. See <see cref="LibSquish.DxtFormat"/></param>
        /// <param name="resizeMethod">Method for possibly required resizing. See <see cref="FixSize"/></param>
        /// <param name="hasMipmaps">Specifies whether mipmaps are generated or not.</param>
        /// <param name="compression">Compression method used by LibSquish. Affects quality / speed. See <see cref="LibSquish.ColorCompression"/></param>
        public void Save(string filename, LibSquish.DxtFormat format, ResizeMethod resizeMethod, bool hasMipmaps, LibSquish.ColorCompression compression = LibSquish.ColorCompression.ClusterFit) {
            if (bmp != null) {
                FileStream fs = new FileStream(filename, FileMode.Create, FileAccess.Write, FileShare.Write);

                using (BinaryWriter bw = new BinaryWriter(fs)) {

                    Blp2Header header = new Blp2Header(DataType.Uncompressed_DirectX, Encoding.DXT, 0, 0, (byte)(hasMipmaps ? 1 : 0), 0, 0);

                    List<Bitmap> bmps = new List<Bitmap>();
                    // Add resized image to the output
                    bmps.Add(FixSize(resizeMethod));

                    header.width = (uint)bmps[0].Width;
                    header.height = (uint)bmps[0].Height;

                    // Add mipmaps
                    if (hasMipmaps) bmps.AddRange(GenerateMipmaps(bmps[0]));

                    byte[] compImgData = { };
                    byte[] colorPalette = new byte[256 * 4];

                    switch (format) {
                        case LibSquish.DxtFormat.Dxt1:
                            header.alphaEncoding = AlphaEncoding.DXT1;
                            header.alphaDepth = AlphaDepth.Alpha1Bit;
                            break;
                        case LibSquish.DxtFormat.Dxt3:
                            header.alphaEncoding = AlphaEncoding.DXT3;
                            header.alphaDepth = AlphaDepth.Alpha8Bit;
                            break;
                        case LibSquish.DxtFormat.Dxt5:
                            header.alphaEncoding = AlphaEncoding.DXT5;
                            header.alphaDepth = AlphaDepth.Alpha8Bit;
                            break;
                    }

                    for (int i = 0; i < bmps.Count; i++) {
                        // Copy raw image data to byte array
                        BitmapData bmpData = bmps[i].LockBits(new Rectangle(0, 0, bmps[i].Width, bmps[i].Height), ImageLockMode.ReadOnly, bmps[i].PixelFormat);
                        byte[] imgData = new byte[bmpData.Stride * bmps[i].Height];
                        Marshal.Copy(bmpData.Scan0, imgData, 0, bmpData.Stride * bmps[i].Height);
                        bmps[i].UnlockBits(bmpData);

                        // Set up conversion flags
                        int flags = (int)format | (int)compression | (int)LibSquish.ColorErrorMetric.Perceptual;

                        // Compress to DXT format
                        RemapRGBA(ref imgData);
                        byte[] buff = new byte[LibSquish.GetStorageRequirements((uint)bmps[i].Width, (uint)bmps[i].Height, flags)];
                        LibSquish.CompressImage(imgData, (uint)bmps[i].Width, (uint)bmps[i].Height, buff, flags);

                        Array.Resize(ref compImgData, compImgData.Length + buff.Length);
                        buff.CopyTo(compImgData, compImgData.Length - buff.Length);

                        // Image data locations
                        header.offsets[i] = (i > 0) ? header.offsets[i - 1] + header.lengths[i - 1] : (uint)(Marshal.SizeOf(typeof(Blp2Header)) + colorPalette.Length);
                        header.lengths[i] = (uint)buff.Length;
                    }

                    bw.Write(header.GetBytes());
                    bw.Write(colorPalette);
                    bw.Write(compImgData);

                }

            } else {
                throw new BlpConversionException("No image data to convert.");
            }

        }

        /// <summary>
        /// Creates a Blp2 object from the specified file path.
        /// </summary>
        /// <param name="filename">A file path that contains the data of a Blp2 or System.Drawing.Image picture.</param>
        /// <returns>Blp2 object containing the image of the file.</returns>
        /// <exception cref=""></exception>
        public static Blp2 FromFile(string filename) {
            using (FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.Read)) {
                Blp2 ret = FromStream(fs);
                return ret;
            }
        }

        /// <summary>
        /// Creates a Blp2 object from the specified data stream.
        /// </summary>
        /// <param name="stream">A stream that contains the data of a Blp2 or System.Drawing.Image picture.</param>
        /// <returns>Blp2 object containing the image of the stream.</returns>
        /// <exception cref=""></exception>
        public static Blp2 FromStream(Stream stream) {
            using (BinaryReader br = new BinaryReader(stream)) {
                Blp2Header header = Blp2Header.FromBinaryReader(br);

                Blp2 ret = new Blp2();

                if (new string(header.fourCC) == "BLP2") {

                    ret.bmp = new Bitmap((int)header.width, (int)header.height, PixelFormat.Format32bppArgb);

                    BitmapData bmpData = ret.bmp.LockBits(new Rectangle(0, 0, ret.bmp.Width, ret.bmp.Height), ImageLockMode.ReadWrite, ret.bmp.PixelFormat);

                    byte[] imgData = new byte[header.lengths[0]];
                    br.BaseStream.Seek(header.offsets[0], SeekOrigin.Begin);
                    imgData = br.ReadBytes(imgData.Length);

                    // BLP2 conversion
                    if (header.type == DataType.Uncompressed_DirectX && header.encoding == Encoding.RAW1 &&
                        (header.alphaDepth == AlphaDepth.NoAlpha || header.alphaDepth == AlphaDepth.Alpha1Bit || header.alphaDepth == AlphaDepth.Alpha8Bit)) {
                        // Uncompressed paletted image
                        ret.format = FileFormat.Raw;

                        // Load palette (4-byte BGRA color values)
                        br.BaseStream.Seek(Marshal.SizeOf(typeof(Blp2Header)), SeekOrigin.Begin);
                        byte[] colorData = br.ReadBytes(256 * 4);
                        Color[] c = new Color[256];
                        for (int i = 0; i < 1024; i += 4) {
                            c[i / 4] = Color.FromArgb(255, colorData[i], colorData[i + 1], colorData[i + 2]);
                        }

                        // Read indexed bitmap
                        byte[] decompImgData = new byte[header.width * header.height * 4];
                        for (int i = 0; i < (header.width * header.height); i++) {
                            decompImgData[i * 4] = c[imgData[i]].R;
                            decompImgData[i * 4 + 1] = c[imgData[i]].G;
                            decompImgData[i * 4 + 2] = c[imgData[i]].B;
                            decompImgData[i * 4 + 3] = 255;
                        }

                        // Apply alpha channels
                        if (header.alphaDepth == AlphaDepth.Alpha8Bit) {
                            // 8-bits alpha
                            br.BaseStream.Seek(header.offsets[0] + header.width * header.height, SeekOrigin.Begin);
                            byte[] alphaData = br.ReadBytes((int)(header.width * header.height));
                            for (int i = 0; i < (header.width * header.height); i++) {
                                decompImgData[i * 4 + 3] = alphaData[i];
                            }
                        } else if (header.alphaDepth == AlphaDepth.Alpha1Bit) {
                            // 1-bits alpha
                            br.BaseStream.Seek(header.offsets[0] + (header.width * header.height), SeekOrigin.Begin);
                            byte[] alphaData = br.ReadBytes((int)(header.width * header.height / 8));
                            for (int i = 0; i < (header.width * header.height / 8); i++)
                                for (int j = 0; j < 8; j++)
                                    decompImgData[i * 32 + ((j + 1) * 4) - 1] = (byte)((alphaData[i] & 1 << j) == 1 << j ? 255 : 0);
                        }

                        Marshal.Copy(decompImgData, 0, bmpData.Scan0, decompImgData.Length);

                    } else if (header.type == DataType.Uncompressed_DirectX && header.encoding == Encoding.RAW3 &&

                        (header.alphaDepth == AlphaDepth.NoAlpha || header.alphaDepth == AlphaDepth.Alpha1Bit || header.alphaDepth == AlphaDepth.Alpha8Bit)) {
                        // Uncompressed BGRA image
                        ret.format = FileFormat.Raw;
                        Marshal.Copy(imgData, 0, bmpData.Scan0, imgData.Length);

                    } else if (header.type == DataType.Uncompressed_DirectX && header.encoding == Encoding.DXT &&
                        (header.alphaEncoding == AlphaEncoding.DXT1 || header.alphaEncoding == AlphaEncoding.DXT3 || header.alphaEncoding == AlphaEncoding.DXT5)) {

                        // DXT1/3/5
                        ret.format = FileFormat.Dxt;

                        byte[] decompImgData = new byte[header.width * header.height * 4];

                        LibSquish.DxtFormat flags = 0;
                        switch (header.alphaEncoding) {
                            case AlphaEncoding.DXT1:
                                flags = LibSquish.DxtFormat.Dxt1;
                                break;
                            case AlphaEncoding.DXT3:
                                flags = LibSquish.DxtFormat.Dxt3;
                                break;
                            case AlphaEncoding.DXT5:
                                flags = LibSquish.DxtFormat.Dxt5;
                                break;
                        }

                        LibSquish.DecompressImage(decompImgData, header.width, header.height, imgData, (int)flags);
                        RemapRGBA(ref decompImgData);
                        Marshal.Copy(decompImgData, 0, bmpData.Scan0, decompImgData.Length);

                    } else if (header.type == DataType.JPEG) {
                        ret.format = FileFormat.Jpeg;
                        ret.bmp.UnlockBits(bmpData);
                        throw new NotImplementedException();
                    } else {
                        ret.bmp.UnlockBits(bmpData);
                        throw new BlpFormatException("The given BLP2 format is not supported.");
                    }

                    ret.bmp.UnlockBits(bmpData);

                } else {

                    try {
                        // Let System.Drawing.Image handle loading
                        ret.bmp = (Bitmap)Image.FromStream(stream);
                        ret.format = FileFormat.Image;
                    } catch (ArgumentException e) {
                        throw new BlpFormatException("The stream did not contain valid image data.", e.InnerException);
                    }

                }

                ret.header = header;
                return ret;
            }
        }

        #region Header members

        /// <summary>
        /// Gets a value indicating whether the image contains mip maps.
        /// </summary>
        public bool HasMipMaps {
            get {
                if (format != FileFormat.Image)
                    return header.Value.hasMips == 1 ? true : false;
                else
                    return false;
            }
        }

        /// <summary>
        /// Gets the width of the image in pixels.
        /// </summary>
        public int Width {
            get {
                if (bmp != null)
                    return bmp.Width;
                else
                    return 0;
            }
        }

        /// <summary>
        /// Gets the height of the image in pixels.
        /// </summary>
        public int Height {
            get {
                if (bmp != null)
                    return bmp.Height;
                else
                    return 0;
            }
        }

        /// <summary>
        /// Gets the Blp format or Mime type of the image.
        /// </summary>
        public string Format {
            get {
                string ret = "Unknown";
                if (format == FileFormat.Image) {
                    foreach (ImageCodecInfo codec in ImageCodecInfo.GetImageDecoders()) {
                        if (codec.FormatID == bmp.RawFormat.Guid)
                            ret = codec.MimeType;
                    }
                } else if (format == FileFormat.Dxt) {
                    switch (header.Value.alphaEncoding) {
                        case AlphaEncoding.DXT1:
                            ret = "DXT1";
                            break;
                        case AlphaEncoding.DXT3:
                            ret = "DXT3";
                            break;
                        case AlphaEncoding.DXT5:
                            ret = "DXT5";
                            break;
                    }
                } else if (format == FileFormat.Raw) {
                    if (header.Value.encoding == Encoding.RAW1)
                        ret = "RAW1";
                    else if (header.Value.encoding == Encoding.RAW3)
                        ret = "RAW3";
                }
                return ret;
            }
        }

        /// <summary>
        /// Gets the the alpha depth of the image.
        /// </summary>
        public AlphaDepth Alpha {
            get {
                if (format == FileFormat.Image)
                    // Image is System.Drawing.Image
                    return AlphaDepth.Alpha8Bit;
                else
                    return header.Value.alphaDepth;
            }
        }

        #endregion

        #region Bitmap resizing

        public enum ResizeMethod : int {
            Crop = 0,
            Extend = 1,
            Shrink = 2,
            Stretch = 3
        }

        /// <summary>
        /// Resizes an image so that its width and height are a power of 2.
        /// </summary>
        /// <param name="method">The method used when resizing. See <see cref="ResizeMethod"/>.</param>
        /// <returns>Bitmap with fixed dimensions.</returns>
        /// <remarks>World of Warcraft requires this to be able to process the Blp image.</remarks>
        private Bitmap FixSize(ResizeMethod method) {
            if (bmp == null) throw new ArgumentException("No valid bitmap provided.");

            int widthLower = (bmp.Width > 1) ? 2 << (int)Math.Floor(Math.Log(bmp.Width, 2)) - 1 : 1;
            int heightLower = (bmp.Height > 1) ? 2 << (int)Math.Floor(Math.Log(bmp.Height, 2)) - 1 : 1;
            int widthUpper = (bmp.Width > 1) ? 2 << (int)Math.Ceiling(Math.Log(bmp.Width, 2)) - 1 : 1;
            int heightUpper = (bmp.Height > 1) ? 2 << (int)Math.Ceiling(Math.Log(bmp.Height, 2)) - 1 : 1;

            Bitmap ret = null;
            Graphics g;

            if (widthLower != bmp.Width || heightLower != bmp.Height) {
                switch (method) {
                    case ResizeMethod.Crop:
                        ret = new Bitmap(widthLower, heightLower, bmp.PixelFormat);
                        ret.MakeTransparent();
                        g = Graphics.FromImage((Image)ret);
                        g.DrawImage((Image)bmp, 0, 0, new Rectangle(0, 0, widthLower, Height), GraphicsUnit.Pixel);
                        g.Dispose();
                        break;
                    case ResizeMethod.Extend:
                        ret = new Bitmap(widthUpper, heightUpper, bmp.PixelFormat);
                        ret.MakeTransparent();
                        g = Graphics.FromImage((Image)ret);
                        g.DrawImage((Image)bmp, 0, 0, bmp.Width, bmp.Height);
                        g.Dispose();
                        break;
                    case ResizeMethod.Shrink:
                        ret = new Bitmap(widthLower, heightLower, bmp.PixelFormat);
                        ret.MakeTransparent();
                        g = Graphics.FromImage((Image)ret);
                        g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
                        g.DrawImage((Image)bmp, 0, 0, widthLower, heightLower);
                        g.Dispose();
                        break;
                    case ResizeMethod.Stretch:
                        ret = new Bitmap(widthUpper, heightUpper, bmp.PixelFormat);
                        ret.MakeTransparent();
                        g = Graphics.FromImage((Image)ret);
                        g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
                        g.DrawImage((Image)bmp, 0, 0, widthUpper, heightUpper);
                        g.Dispose();
                        break;
                    default:
                        throw new ArgumentException("Resize method not supported.");
                }
            }

            return (ret == null) ? bmp : ret;
        }

        /// <summary>
        /// Generates all mipmaps down to the 1px version.
        /// </summary>
        /// <param name="src">Source bitmap.</param>
        /// <returns>List of images containing the mipmaps.</returns>
        private IEnumerable<Bitmap> GenerateMipmaps(Bitmap src) {

            for (int i = 0; i < (int)Math.Log((src.Width > src.Height) ? src.Height : src.Width, 2); i++) {
                Graphics g;

                int mmW = src.Width / 2 >> i;
                int mmH = src.Height / 2 >> i;

                Bitmap ret = new Bitmap(mmW, mmH, src.PixelFormat);

                g = Graphics.FromImage((Image)ret);
                g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
                g.DrawImage((Image)src, 0, 0, mmW, mmH);
                g.Dispose();

                yield return ret;
            }

        }

        #endregion

        #region Helper functions

        /// <summary>
        /// Convert BGRA to RGBA color space.
        /// </summary>
        /// <param name="decompImgData">Reference to a BGRA byte array.</param>
        /// <remarks>Libsquish returns BGRA values, whereas System.Drawing.Image expects RGBA values.</remarks>
        private static void RemapRGBA(ref byte[] decompImgData) {
            for (int i = 0; i < decompImgData.Length; i += 4) {
                decompImgData[i] ^= decompImgData[i + 2];
                decompImgData[i + 2] ^= decompImgData[i];
                decompImgData[i] ^= decompImgData[i + 2];
            }
        }

        /// <summary>
        /// Add int and byte without over- or underflow
        /// </summary>
        private static byte truncateByte(byte a, int b) {
            if (a + b < Byte.MinValue)
                return Byte.MinValue;
            else if (a + b > Byte.MaxValue)
                return Byte.MaxValue;
            else
                return (byte)(a + b);
        }


        //private static uint toRGB8888(ushort rgb565, byte alpha) {
        //    return (uint)((rgb565 & 0xF800) << 16) + (uint)((rgb565 & 0x7E0) << 13) + (uint)((rgb565 & 0x1F) << 11) + (uint)alpha;
        //}

        #endregion

    }

}
