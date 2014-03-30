/*
 * Copyright (c) 2006 Simon Brown                          si@sjbrown.co.uk
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
using System.Runtime.InteropServices;

namespace MDS.cBlp2 {

    /// <summary>
    /// A wrapper for libsquish by Simon Brown.
    /// </summary>
    /// <seealso cref="http://code.google.com/p/libsquish/"/>
    public static class LibSquish {

        /// <summary>
        /// Compresses raw image data using S3 texture compression (DXT).
        /// </summary>
        /// <param name="rgba">RGB pixel array.</param>
        /// <param name="width">Width of the input image.</param>
        /// <param name="height">Height of the input image.</param>
        /// <param name="blocks">Reference to the output array containing compressed DXT blocks.</param>
        /// <param name="flags">LibSquish conversion flags. See <see cref="DxtFormat"/>, <see cref="ColorCompression"/>, <see cref="ColorErrorMetric"/> for flag options.</param>
        [DllImport("squish.dll", EntryPoint = "CompressImage")]
        internal static extern void CompressImage([MarshalAs(UnmanagedType.LPArray)] byte[] rgba, uint width, uint height, [MarshalAs(UnmanagedType.LPArray)] byte[] blocks, int flags);

        /// <summary>
        /// Decompresses the SC3T texture data.
        /// </summary>
        /// <param name="rgba">Reference to the output array containing uncompressed RGB pixel.</param>
        /// <param name="width">Width of the input image.</param>
        /// <param name="height">Height of the input image.</param>
        /// <param name="blocks">Compressed DXT block array.</param>
        /// <param name="flags">LibSquish conversion flags. See <see cref="DxtFormat"/>, <see cref="ColorCompression"/>, <see cref="ColorErrorMetric"/> for flag options.</param>
        [DllImport("squish.dll", EntryPoint = "DecompressImage")]
        internal static extern void DecompressImage([MarshalAs(UnmanagedType.LPArray)] byte[] rgba, uint width, uint height, [MarshalAs(UnmanagedType.LPArray)] byte[] blocks, int flags);

        /// <summary>
        /// Calculates the storage requirements of DXT compressed image data.
        /// </summary>
        /// <param name="width">Width of the input image.</param>
        /// <param name="height">Height of the input image.</param>
        /// <param name="flags">LibSquish conversion flags. See <see cref="DxtFormat"/>, <see cref="ColorCompression"/>, <see cref="ColorErrorMetric"/> for flag options.</param>
        /// <returns>Calculated size of the compressed image data in bytes.</returns>
        [DllImport("squish.dll", EntryPoint = "GetStorageRequirements")]
        internal static extern uint GetStorageRequirements(uint width, uint height, int flags);

        [Flags]
        public enum DxtFormat : int {
            Dxt1 = 0x1, // Default
            Dxt3 = 0x2,
            Dxt5 = 0x4,
        }

        [Flags]
        public enum ColorCompression : int {
            ClusterFit           = 0x008, // Slow, high quality color compressor (the default).
            RangeFit             = 0x010, // Fast, low quality color compressor.
            IterativeClusterFit  = 0x100, // Very slow, very high quality color compressor.
            WeightByAlpha        = 0x080  // Weight the color by alpha during cluster fit (disabled by default).
        }

        [Flags]
        public enum ColorErrorMetric : int {
            Perceptual = 0x20, // Perceptual metric (the default).
            Uniform    = 0x40  // Uniform metric.
        }

    }

}
