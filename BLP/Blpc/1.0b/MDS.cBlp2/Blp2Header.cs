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
using System.IO;
using System.Runtime.InteropServices;

namespace MDS.cBlp2 {

    [Flags]
    internal enum DataType : uint {
        JPEG,                 // JPEG compressed
        Uncompressed_DirectX  // Uncompressed or DXT compressed
    }

    [Flags]
    internal enum Encoding : byte {
        RAW1    = 0x1, // RAW1 (BGR + alpha field)
        DXT     = 0x2, // DXT compressed
        RAW3    = 0x3  // RAW3 (BGRA + flags)
    }

    [Flags]
    public enum AlphaDepth : byte {
        NoAlpha     = 0x0, // Uncompressed, DXT1
        Alpha1Bit   = 0x1, // Uncompressed, DXT1
        Alpha4Bit   = 0x4, // DXT3
        Alpha8Bit   = 0x8  // Uncompressed, DXT3, DXT5
    }

    [Flags]
    internal enum AlphaEncoding : byte {
        DXT1    = 0x0,
        DXT3    = 0x1,
        DXT5    = 0x7,
        RAW8BIT = 0x8
    }

    /// <summary>
    /// This structure holds the header information of the BLP file.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    internal struct Blp2Header {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public char[] fourCC;
        public DataType type;
        public Encoding encoding;
        public AlphaDepth alphaDepth;
        public AlphaEncoding alphaEncoding;
        public byte hasMips;
        public uint width;
        public uint height;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public uint[] offsets;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public uint[] lengths;

        public Blp2Header(DataType type, Encoding encoding, AlphaDepth alphaDepth, AlphaEncoding alphaEncoding, byte hasMips, uint width, uint height) {
            this.fourCC = ("BLP2").ToCharArray();
            this.type = type;
            this.encoding = encoding;
            this.alphaDepth = alphaDepth;
            this.alphaEncoding = alphaEncoding;
            this.hasMips = hasMips;
            this.width = width;
            this.height = height;
            this.offsets = new uint[16];
            this.lengths = new uint[16];
        }

        internal byte[] GetBytes() {
            byte[] ret = new byte[Marshal.SizeOf(typeof(Blp2Header))];
            GCHandle handle = GCHandle.Alloc(ret, GCHandleType.Pinned);
            Marshal.StructureToPtr(this, handle.AddrOfPinnedObject(), false);
            handle.Free();
            return ret;
        }

        internal static Blp2Header FromBinaryReader(BinaryReader br) {
            byte[] buff = br.ReadBytes(Marshal.SizeOf(typeof(Blp2Header)));
            GCHandle handle = GCHandle.Alloc(buff, GCHandleType.Pinned);
            Blp2Header ret = (Blp2Header)Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(Blp2Header));
            handle.Free();
            return ret;
        }
    }

}
