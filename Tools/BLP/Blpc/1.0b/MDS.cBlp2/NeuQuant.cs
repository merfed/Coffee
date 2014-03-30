/*
 * NeuQuant Neural-Net Quantization Algorithm
 * ------------------------------------------
 *
 * Copyright (c) 1994 Anthony Dekker
 *
 * NEUQUANT Neural-Net quantization algorithm by Anthony Dekker, 1994.
 * See "Kohonen neural networks for optimal colour quantization"
 * in "Network: Computation in Neural Systems" Vol. 5 (1994) pp 351-367.
 * for a discussion of the algorithm.
 * See also  http://members.ozemail.com.au/~dekker/NEUQUANT.HTML
 *
 * Any party obtaining a copy of these files from the author, directly or
 * indirectly, is granted, free of charge, a full and unrestricted irrevocable,
 * world-wide, paid up, royalty-free, nonexclusive right and license to deal
 * in this software and documentation files (the "Software"), including without
 * limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so, with the only requirement being
 * that this copyright notice remain intact.
 * 
 */

using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace MDS.cBlp2 {

    internal class NeuQuant : IDisposable {

        private static readonly int NCYCLES = 100; // Number of learning cycles
        private static readonly int NETSIZE = 256; // Number of colors used
        private static readonly int SPECIALCOLORS = 3; // Number of reserved colors used
        private static readonly int BGCOLOR = SPECIALCOLORS - 1; // Reserved background color
        private static readonly int CUTNETSIZE = NETSIZE - SPECIALCOLORS;
        private static readonly int MAXNETPOS = NETSIZE - 1;
        private static readonly int INITRAD = NETSIZE / 8; // For 256 cols, radius starts at 32
        private static readonly int RADIUSBIASSHIFT = 6;
        private static readonly int RADIUSBIAS = 1 << RADIUSBIASSHIFT;
        private static readonly int INITBIASRADIUS = INITRAD * RADIUSBIAS;
        private static readonly int RADIUSDEC = 30; // Factor of 1/30 each cycle
        private static readonly int ALPHABIASSHIFT = 10; // Alpha starts at 1
        private static readonly int INITALPHA = 1 << ALPHABIASSHIFT; // Biased by 10 bits
        private static readonly double GAMMA = 1024.0;
        private static readonly double BETA = 1.0 / 1024.0;
        private static readonly double BETAGAMMA = BETA * GAMMA;

        // Four primes near 500 - assume no image has a length so large
        // that it is divisible by all four primes
        private static readonly int prime1 = 499;
        private static readonly int prime2 = 491;
        private static readonly int prime3 = 487;
        private static readonly int prime4 = 503;
        private static readonly int maxprime = prime4;

        private double[][] network = new double[NETSIZE][]; // The network itself
        private int[][] colormap = new int[NETSIZE][];
        private int[] netIndex = new int[256]; // For network lookup - really 256
        private double[] bias = new double[NETSIZE];  // Bias and freq arrays for learning
        private double[] freq = new double[NETSIZE];

        private int[] pixels = null; // Pixel array of RGB colors
        private int sampleFac = 0;

        private NeuQuant(int sample) {
            sampleFac = sample;
        }

        public NeuQuant(Bitmap bmp) : this(1) {
            SetPixels(bmp);
            SetUpArrays();
        }

        public NeuQuant(Bitmap bmp, int sample) : this(sample) {
            SetPixels(bmp);
            SetUpArrays();
        }

        ~NeuQuant() {
            Dispose();
        }

        public void Dispose() {
            network = null;
            colormap = null;
            netIndex = pixels = null;
            bias = freq = null;
        }

        public int ColorCount {
            get {
                return NETSIZE;
            }
        }

        public List<Color> Palette {
            get {
                List<Color> ret = new List<Color>();
                for (int i = 0; i < NETSIZE; i++)
                    ret.Add(Color.FromArgb((byte)colormap[i][0], (byte)colormap[i][1], (byte)colormap[i][2]));
                return ret;
            }
        }

        private void SetUpArrays() {
            for (int i = 0; i < NETSIZE; i++) {
                network[i] = new double[3];
                colormap[i] = new int[4];
            }

            // Black
            network[0] = new double[] { 0.0, 0.0, 0.0 };
            // White
            network[0] = new double[] { 255.0, 255.0, 255.0 };

            for (int i = 0; i < SPECIALCOLORS; i++) {
                freq[i] = 1.0 / NETSIZE;
                bias[i] = 0.0;
            }

            for (int i = SPECIALCOLORS; i < NETSIZE; i++) {
                double[] p = network[i];
                p[0] = (255.0 * (i - SPECIALCOLORS)) / CUTNETSIZE;
                p[1] = (255.0 * (i - SPECIALCOLORS)) / CUTNETSIZE;
                p[2] = (255.0 * (i - SPECIALCOLORS)) / CUTNETSIZE;
                freq[i] = 1.0 / NETSIZE;
                bias[i] = 0.0;
            }
        }

        private void SetPixels(Bitmap bmp) {
            if (bmp.Width * bmp.Height < maxprime) throw new Exception("Image is too small");

            BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);
            pixels = new int[bmpData.Stride * bmp.Height / 4];
            Marshal.Copy(bmpData.Scan0, pixels, 0, bmpData.Stride * bmp.Height / 4);
            bmp.UnlockBits(bmpData);
        }

        public void Quantize() {
            Learn();
            Fix();
            InxBuild();
        }

        // Move neuron i towards biased (b,g,r) by factor alpha
        private void AlterSingle(double alpha, int i, double b, double g, double r) {
            // Alter hit neuron
            double[] n = network[i];
            n[0] -= (alpha * (n[0] - b));
            n[1] -= (alpha * (n[1] - g));
            n[2] -= (alpha * (n[2] - r));
        }

        private void AlterNeigh(double alpha, int rad, int i, double b, double g, double r) {
            int lo = i - rad; if (lo < SPECIALCOLORS - 1) lo = SPECIALCOLORS - 1;
            int hi = i + rad; if (hi > NETSIZE) hi = NETSIZE;

            int j = i + 1;
            int k = i - 1;
            int q = 0;
            while ((j < hi) || (k > lo)) {
                double a = (alpha * (rad * rad - q * q)) / (rad * rad);
                q++;
                if (j < hi) {
                    double[] p = network[j];
                    p[0] -= (a * (p[0] - b));
                    p[1] -= (a * (p[1] - g));
                    p[2] -= (a * (p[2] - r));
                    j++;
                }
                if (k > lo) {
                    double[] p = network[k];
                    p[0] -= (a * (p[0] - b));
                    p[1] -= (a * (p[1] - g));
                    p[2] -= (a * (p[2] - r));
                    k--;
                }
            }
        }

        // Finds closest neuron (min dist) and updates freq 
        // Finds best neuron (min dist-bias) and returns position 
        // For frequently chosen neurons, freq[i] is high and bias[i] is negative 
        // bias[i] = gamma*((1/netsize)-freq[i]) 
        private int Contest(double b, double g, double r) {
            double bestd = float.MaxValue;
            double bestbiasd = bestd;
            int bestpos = -1;
            int bestbiaspos = bestpos;

            for (int i = SPECIALCOLORS; i < NETSIZE; i++) {
                double[] n = network[i];
                double dist = n[0] - b; if (dist < 0) dist = -dist;
                double a = n[1] - g; if (a < 0) a = -a;
                dist += a;
                a = n[2] - r; if (a < 0) a = -a;
                dist += a;
                if (dist < bestd) { bestd = dist; bestpos = i; }
                double biasdist = dist - bias[i];
                if (biasdist < bestbiasd) { bestbiasd = biasdist; bestbiaspos = i; }
                freq[i] -= BETA * freq[i];
                bias[i] += BETAGAMMA * freq[i];
            }
            freq[bestpos] += BETA;
            bias[bestpos] -= BETAGAMMA;
            return bestbiaspos;
        }

        private int SpecialFind(double b, double g, double r) {
            for (int i = 0; i < SPECIALCOLORS; i++) {
                double[] n = network[i];
                if (n[0] == b && n[1] == g && n[2] == r) return i;
            }
            return -1;
        }

        private void Learn() {
            int biasRadius = INITBIASRADIUS;
            int alphadec = 30 + ((sampleFac - 1) / 3);
            int lengthcount = pixels.Length;
            int samplepixels = lengthcount / sampleFac;
            int delta = samplepixels / NCYCLES;
            int alpha = INITALPHA;

            int i = 0;
            int rad = biasRadius >> RADIUSBIASSHIFT;
            if (rad <= 1) rad = 0;

            int step = 0;
            int pos = 0;

            if ((lengthcount % prime1) != 0) step = prime1;
            else {
                if ((lengthcount % prime2) != 0) step = prime2;
                else {
                    if ((lengthcount % prime3) != 0) step = prime3;
                    else step = prime4;
                }
            }

            i = 0;
            while (i < samplepixels) {
                int p = pixels[pos];
                int red = (p >> 16) & 0xff;
                int green = (p >> 8) & 0xff;
                int blue = (p) & 0xff;

                double r = red;
                double g = green;
                double b = blue;

                // Remember background color
                if (i == 0) {
                    network[BGCOLOR][0] = b;
                    network[BGCOLOR][1] = g;
                    network[BGCOLOR][2] = r;
                }

                int j = SpecialFind(b, g, r);
                j = j < 0 ? Contest(b, g, r) : j;

                // Don't learn for specials
                if (j >= SPECIALCOLORS) {
                    double a = (1.0 * alpha) / INITALPHA;
                    AlterSingle(a, j, b, g, r);
                    // Alter neighbours
                    if (rad > 0) AlterNeigh(a, rad, j, b, g, r);
                }

                pos += step;
                while (pos >= lengthcount) pos -= lengthcount;

                i++;
                if (i % delta == 0) {
                    alpha -= alpha / alphadec;
                    biasRadius -= biasRadius / RADIUSDEC;
                    rad = biasRadius >> RADIUSBIASSHIFT;
                    if (rad <= 1) rad = 0;
                }
            }
        }

        private void Fix() {
            for (int i = 0; i < NETSIZE; i++) {
                for (int j = 0; j < 3; j++) {
                    int x = (int)(0.5 + network[i][j]);
                    if (x < 0) x = 0;
                    if (x > 255) x = 255;
                    colormap[i][j] = x;
                }
                colormap[i][3] = i;
            }
        }

        private void InxBuild() {
            // Insertion sort of network and building of netindex[0..255]
            int previouscol = 0;
            int startpos = 0;

            for (int i = 0; i < NETSIZE; i++) {
                int[] p = colormap[i];
                int[] q = null;
                int smallpos = i;
                // Index on g
                int smallval = p[1];
                // find smallest in i..netsize-1
                for (int j = i + 1; j < NETSIZE; j++) {
                    q = colormap[j];
                    // Index on g
                    if (q[1] < smallval) {
                        smallpos = j;
                        // Index on g
                        smallval = q[1];
                    }
                }
                q = colormap[smallpos];
                // Swap p (i) and q (smallpos) entries
                if (i != smallpos) {
                    int j = q[0]; q[0] = p[0]; p[0] = j;
                    j = q[1]; q[1] = p[1]; p[1] = j;
                    j = q[2]; q[2] = p[2]; p[2] = j;
                    j = q[3]; q[3] = p[3]; p[3] = j;
                }
                // Smallval entry is now in position i
                if (smallval != previouscol) {
                    netIndex[previouscol] = (startpos + i) >> 1;
                    for (int j = previouscol + 1; j < smallval; j++) netIndex[j] = i;
                    previouscol = smallval;
                    startpos = i;
                }
            }
            netIndex[previouscol] = (startpos + MAXNETPOS) >> 1;
            // Really 256
            for (int j = previouscol + 1; j < 256; j++) netIndex[j] = MAXNETPOS;
        }

        public int GetPaletteIndex(Color c) {
            return InxSearch(c.R, c.G, c.B);
        }

        // Search for BGR values 0..255 and return color index
        protected int InxSearch(int r, int g, int b) {
            // Biggest possible dist is 256 * 3
            int bestd = 1000;
            int best = -1;
            // Index on g
            int i = netIndex[g];
            // Start at netindex[g] and work outwards
            int j = i - 1;

            while ((i < NETSIZE) || (j >= 0)) {
                if (i < NETSIZE) {
                    int[] p = colormap[i];
                    // Inx key
                    int dist = p[1] - g;
                    // Stop iter
                    if (dist >= bestd)
                        i = NETSIZE;
                    else {
                        if (dist < 0) dist = -dist;
                        int a = p[0] - r; if (a < 0) a = -a;
                        dist += a;
                        if (dist < bestd) {
                            a = p[2] - b; if (a < 0) a = -a;
                            dist += a;
                            if (dist < bestd) { bestd = dist; best = i; }
                        }
                        i++;
                    }
                }
                if (j >= 0) {
                    int[] p = colormap[j];
                    // Inx key - reverse dif
                    int dist = g - p[1];
                    // Stop iter
                    if (dist >= bestd)
                        j = -1;
                    else {
                        if (dist < 0) dist = -dist;
                        int a = p[0] - r; if (a < 0) a = -a;
                        dist += a;
                        if (dist < bestd) {
                            a = p[2] - b; if (a < 0) a = -a;
                            dist += a;
                            if (dist < bestd) { bestd = dist; best = j; }
                        }
                        j--;
                    }
                }
            }

            return best;
        }

    }

}
