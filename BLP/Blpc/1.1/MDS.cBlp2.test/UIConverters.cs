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
using System.Windows.Data;

namespace MDS.cBlp2.test.UIConverters {

    // Value converters for UI DataBinding

    /// <summary>
    /// Format menu converter (LibSquish.DxtFormat<=>bool)
    /// </summary>
    /// <remarks>MenuItem[parameter].IsChecked</remarks>
    internal class FormatConverter : IValueConverter {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            // LibSquish.DXTFormat.DXT3 == LibSquish.DXTFormat.DXT3?
            //  True => Check DXT3 MenuItem 
            //  Otherwise => Uncheck DXT3 MenuItem
            return ((int)value).Equals(Int32.Parse((string)parameter));
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            // Menu state was changed to checked => Use corresponding conversion format
            return (SaveSettings.SaveFormat)Int32.Parse((string)parameter);
        }
    }

    /// <summary>
    /// Resize menu converter (Blp2.ResizeMethod<=>bool)
    /// </summary>
    /// <remarks>MenuItem[parameter].IsChecked</remarks>
    internal class ResizeConverter : IValueConverter {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            return ((int)value).Equals(Int32.Parse((string)parameter));
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            return (Blp2.ResizeMethod)Int32.Parse((string)parameter);
        }
    }

    /// <summary>
    /// Dxt quality converter (LibSquish.ColorCompression<=>bool)
    /// </summary>
    /// <remarks>MenuItem[parameter].IsChecked</remarks>
    internal class DxtQualConverter : IValueConverter {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            return ((int)value).Equals(Int32.Parse((string)parameter));
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            return (LibSquish.ColorCompression)Int32.Parse((string)parameter);
        }
    }

    /// <summary>
    /// Raw1 quality converter (int<=>bool)
    /// </summary>
    /// <remarks>MenuItem[parameter].IsChecked</remarks>
    internal class Raw1QualConverter : IValueConverter {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            return (value).Equals(Byte.Parse((string)parameter));
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            return Byte.Parse((string)parameter);
        }
    }

    /// <summary>
    /// Selected mouse action converter (MainWindow.MouseTool<=>bool)
    /// </summary>
    /// <remarks>ToggleButton[parameter].IsChecked</remarks>
    internal class MouseToolConverter : IValueConverter {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            return ((byte)value).Equals(Byte.Parse((string)parameter));
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            return (MainWindow.MouseTool)Byte.Parse((string)parameter);
        }
    }

    /// <summary>
    /// ZoomFactor conveter
    /// </summary>
    /// <remarks>ScaleTransform.Scale</remarks>
    internal class ZoomConverter : IValueConverter {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            double[] zoomLevels = new double[14] { 1f / 8, 1f / 6, 1f / 4, 1f / 3, 1f / 2, 2f / 3, 1, 2, 3, 4, 5, 6, 8, 10 };
            if ((string)parameter == "PercentText")
                return " - " + (zoomLevels[(int)value] * 100).ToString("0.#", System.Globalization.CultureInfo.InvariantCulture) + "%";
            else
                return zoomLevels[(int)value];
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            throw new NotImplementedException();
        }
    }

    /// <summary>
    /// ZoomQuality converter
    /// </summary>
    /// <remarks>HighQuality for <= 100%, NearestNeighbor for > 100%</remarks>
    [ValueConversion(typeof(double), typeof(System.Windows.Media.BitmapScalingMode))]
    public class ZoomQualityConverter : IValueConverter {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            return ((int)value > 6) ? System.Windows.Media.BitmapScalingMode.NearestNeighbor : System.Windows.Media.BitmapScalingMode.HighQuality;
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
            throw new NotImplementedException();
        }
    }

}
