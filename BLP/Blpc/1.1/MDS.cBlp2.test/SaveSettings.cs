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
using System.ComponentModel;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;

namespace MDS.cBlp2.test {

    /// <summary>
    /// Just a small class to hold conversion parameters
    /// </summary>
    /// <remarks>We use this as a DataBinding for WPF controls</remarks>
    [Serializable]
    internal sealed class SaveSettings : INotifyPropertyChanged {
        
        public enum SaveFormat : int {
            Raw1 = 0,
            Raw3 = 1,
            Dxt1 = 2,
            Dxt3 = 3,
            Dxt5 = 4,
            Jpeg = 5
        }

        private SaveFormat format = SaveFormat.Dxt3;
        private bool mipmaps = false;
        private Blp2.ResizeMethod resize = Blp2.ResizeMethod.Extend;
        private LibSquish.ColorCompression dxtQual = LibSquish.ColorCompression.ClusterFit;
        private byte quantQual = 10;
        private bool dither = false;

        #region Members

        /// <summary>
        /// Save as ...
        /// </summary>
        public SaveFormat Format {
            get {
                return format;
            }

            set {
                format = value;
                Update("Format");
            }
        }

        /// <summary>
        /// Save with mipmaps?
        /// </summary>
        public bool Mipmaps {
            get {
                return mipmaps;
            }

            set {
                mipmaps = value;
                Update("Mipmaps");
            }
        }

        /// <summary>
        /// Resize how?
        /// </summary>
        public Blp2.ResizeMethod ResizeMethod {
            get {
                return resize;
            }

            set {
                resize = value;
                Update("ResizeMethod");
            }
        }

        /// <summary>
        /// Use which Libsquish conversion for Dxt?
        /// </summary>
        public LibSquish.ColorCompression DxtQuality {
            get {
                return dxtQual;
            }

            set {
                dxtQual = value;
                Update("DxtQuality");
            }
        }

        /// <summary>
        /// NeuQuant sampling 1-30 (Raw1)
        /// </summary>
        public byte Raw1Quality {
            get {
                return quantQual;
            }

            set {
                quantQual = value;
                Update("Raw1Quality");
            }
        }

        /// <summary>
        /// Dithering (Raw1)
        /// </summary>
        public bool Dither {
            get {
                return dither;
            }

            set {
                dither = value;
                Update("Dither");
            }
        }

        #endregion

        // Dont serialize the backing field of the delegate
        [field: NonSerialized]
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Notify UI that a class member's value was changed for DataBinding purpose
        /// </summary>
        /// <param name="property">Member name</param>
        private void Update(string property) {
            if (this.PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(property));
        }

        /// <summary>
        /// Serialize this object into a binary file.
        /// </summary>
        /// <param name="file">Path to the file to save object state into.</param>
        /// <remarks>That is to say, saves settings</remarks>
        internal void Serialize(string file) {
            try {
                FileStream fs = new FileStream(file, FileMode.Create);
                BinaryFormatter bf = new BinaryFormatter();
                bf.Serialize(fs, this);
                fs.Close();
            } catch (Exception e) {
                // Access, security and whatnot exceptions
            }
        }

        /// <summary>
        /// Deserialize from a binary file into a SaveSettings object.
        /// </summary>
        /// <param name="file">Path to the file that holds the settings.</param>
        /// <param name="obj">SaveSettings object to load the object state into.</param>
        /// <remarks>That is to say, loads settings</remarks>
        internal static SaveSettings Deserialize(string file) {
            SaveSettings ret;

            if (File.Exists(file)) {
                try {
                    FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.Read);
                    BinaryFormatter bf = new BinaryFormatter();
                    ret = (SaveSettings)bf.Deserialize(fs);
                    fs.Close();
                } catch (Exception e) {
                    // Access, security and whatnot exceptions
                    ret = new SaveSettings();
                }
            } else {
                // File doesn't exist (first application start etc.)
                ret = new SaveSettings();
            }

            return ret;
        }
        
    }

}
