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
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using System.Windows.Media.Effects;

using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security;
using System.Security.Permissions;
using System.Windows.Data;
using System.ComponentModel;
using System.Threading;
using MDS.cBlp2;

namespace MDS.cBlp2.test {
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged {

        private Blp2 blp = new Blp2();
        private SaveSettings settings;

        public static RoutedCommand About = new RoutedCommand();
        public static RoutedCommand AssociateFile = new RoutedCommand();
        public static RoutedCommand BatchConversion = new RoutedCommand();

        // Notify windows that file associations have changed
        [DllImport("shell32.dll", EntryPoint = "SHChangeNotify")]
        internal static extern void ChangeNotify(int eventId = 0x8000000, int flags = 0, int dwItem1 = 0, int dwItem2 = 0);

        [DllImport("gdi32.dll")]
        public static extern bool DeleteObject(IntPtr hObject);

        // Notify UI on data bound property changes
        public event PropertyChangedEventHandler PropertyChanged;

        private void Update(string property) {
            if (this.PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(property));
        }

        // Mouse action state
        private Cursor curHand, curHandGrab, curZoomIn, curZoomOut;
        public enum MouseTool : byte { Hand = 1, ZoomIn = 2, ZoomOut = 3 }
        private MouseTool mouseAction = MouseTool.Hand;

        public MouseTool MouseAction {
            get {
                return mouseAction;
            }
            set {
                // Set cursor
                switch (value) {
                    case MouseTool.Hand:
                        scrollViewer.Cursor = curHand;
                        break;
                    case MouseTool.ZoomIn:
                        scrollViewer.Cursor = curZoomIn;
                        break;
                    case MouseTool.ZoomOut:
                        scrollViewer.Cursor = curZoomOut;
                        break;
                }
                mouseAction = value;
                // Notify DataBindings
                Update("MouseAction");
            }
        }

        // Zoom levels
        private double[] zoomLevels = new double[14] { 1f / 8, 1f / 6, 1f / 4, 1f / 3, 1f / 2, 2f / 3, 1, 2, 3, 4, 5, 6, 8, 10 };
        private int zoomFactor = 6;

        // Zoom and drag points
        private System.Windows.Point mouseDragStartPoint;
        private System.Windows.Point scrollStartOffset;
        private System.Windows.Point zoomCenterPoint;

        /// <summary>
        /// Gets the current zoom factor
        /// </summary>
        public int ZoomFactor {
            get { return zoomFactor; }
            set {
                if (0 <= value && value <= 13) {
                    zoomFactor = value;

                    Update("ZoomFactor");
                    Update("TransformedSize");
                }
            }
        }

        /// <summary>
        /// Gets zoomed image size inside the ScrollViewer control
        /// </summary>
        public System.Windows.Size TransformedSize {
            get {
                return new System.Windows.Size(img1.ActualWidth * zoomLevels[zoomFactor], img1.ActualHeight * zoomLevels[zoomFactor]);
            }
        }

        // Feedback window for conversion thread
        WorkingWindow winW;

        public MainWindow() {

            InitializeComponent();

            string[] cmdParams = System.Environment.GetCommandLineArgs();

            // Check if application is already running
            string instance = System.Diagnostics.Process.GetCurrentProcess().ProcessName;
            System.Diagnostics.Process[] instances = System.Diagnostics.Process.GetProcessesByName(instance);
            if (instances.Length > 1) {
                // This is a subsequent instance of the application. Send command line
                // parameters to the main process.
                if (cmdParams.Length >= 2)
                    OpenFilePipe.PassArgument(cmdParams[1].Replace("\"", ""));
                // Exit
                App.Current.Shutdown();
            } else {
                // This is the first instance of the application. Open a pipe.
                OpenFilePipe.CreatePipe();
                OpenFilePipe.ArgumentPassedEvent += new EventHandler<ArgumentPassedArgs>(OpenFilePipe_ArgumentPassed);
            }

            // Deserialize from a previous session
            FileInfo info = new FileInfo(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData));
            settings = SaveSettings.Deserialize(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData) + @"\Blpc.conf");

            // Some custom CommandBindings for menus
            this.CommandBindings.Add(new CommandBinding(About, CommandBinding_Executed));
            this.CommandBindings.Add(new CommandBinding(AssociateFile, CommandBinding_Executed));
            this.CommandBindings.Add(new CommandBinding(BatchConversion, CommandBinding_Executed));
            menuAbout.Command = About;
            menuExtension.Command = AssociateFile;
            menuBatch.Command = BatchConversion;

            // Menu DataBindings
            menuMips.DataContext = settings;
            foreach (MenuItem m in menuFormat.Items)
                m.DataContext = settings;
            foreach (MenuItem m in menuResize.Items)
                m.DataContext = settings;
            foreach (MenuItem m in menuDxtQuality.Items)
                m.DataContext = settings;
            foreach (var m in menuRaw1Quality.Items)
                if (m.GetType() == typeof(MenuItem)) ((MenuItem)m).DataContext = settings;

            // Set file association menu (un)checked
            try {
                var keyBlp = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(@"Software\Classes\.blp");
                menuExtension.IsChecked = ((string)keyBlp.GetValue("")) == "Blpc";
            } catch (Exception) { };

            // Open file from command line (file association, drag & drop on .exe)
            if (cmdParams.Length >= 2) {
                if ((new FileInfo(cmdParams[1].Replace("\"", "")).Exists))
                    Open(cmdParams[1].Replace("\"", ""));
            }

            // Cursor resources
            curHand = ((TextBlock)App.Current.Resources["curHand"]).Cursor;
            curHandGrab = ((TextBlock)App.Current.Resources["curHandGrab"]).Cursor;
            curZoomIn = ((TextBlock)App.Current.Resources["curZoomIn"]).Cursor;
            curZoomOut = ((TextBlock)App.Current.Resources["curZoomOut"]).Cursor;
            scrollViewer.Cursor = curHand;
            
        }

        /// <summary>
        /// Save an image.
        /// </summary>
        private void Save() {
            System.Windows.Forms.SaveFileDialog dlgSave = new System.Windows.Forms.SaveFileDialog();
            dlgSave.Filter = ImageCodecInfo.GetImageDecoders().Aggregate("Blp texture|*.blp", (str, codec) =>
                str += "|" + codec.FormatDescription + "|" + codec.FilenameExtension.ToLower());
            dlgSave.ShowDialog();

            if (dlgSave.FileName != "") {
                FileInfo info = new FileInfo(dlgSave.FileName);
                Thread conversionWorker = null;

                if (info.Extension.ToUpper() == ".BLP") {
                    // Save as Blp
                    if (settings.Format == SaveSettings.SaveFormat.Dxt1) {
                        conversionWorker = new Thread(delegate() {
                            blp.Save(info.FullName, LibSquish.DxtFormat.Dxt1, settings.ResizeMethod, settings.Mipmaps, settings.DxtQuality);
                            this.Dispatcher.Invoke((Action)delegate() { ToggleWorking(false); });
                        });
                    } else if (settings.Format == SaveSettings.SaveFormat.Dxt3) {
                        conversionWorker = new Thread(delegate() {
                            blp.Save(info.FullName, LibSquish.DxtFormat.Dxt3, settings.ResizeMethod, settings.Mipmaps, settings.DxtQuality);
                            this.Dispatcher.Invoke((Action)delegate() { ToggleWorking(false); });
                        });
                    } else if (settings.Format == SaveSettings.SaveFormat.Dxt5) {
                        conversionWorker = new Thread(delegate() {
                            blp.Save(info.FullName, LibSquish.DxtFormat.Dxt5, settings.ResizeMethod, settings.Mipmaps, settings.DxtQuality);
                            this.Dispatcher.Invoke((Action)delegate() { ToggleWorking(false); });
                        });
                    } else if (settings.Format == SaveSettings.SaveFormat.Raw1) {
                        conversionWorker = new Thread(delegate() {
                            blp.Save(info.FullName, Blp2.RawFormat.Raw1, settings.ResizeMethod, settings.Mipmaps, settings.Raw1Quality, settings.Dither);
                            this.Dispatcher.Invoke((Action)delegate() { ToggleWorking(false); });
                        });
                    } else if (settings.Format == SaveSettings.SaveFormat.Raw3) {
                        conversionWorker = new Thread(delegate() {
                            blp.Save(info.FullName, Blp2.RawFormat.Raw3, settings.ResizeMethod, settings.Mipmaps);
                            this.Dispatcher.Invoke((Action)delegate() { ToggleWorking(false); });
                        });
                    } else if (settings.Format == SaveSettings.SaveFormat.Jpeg) {
                        throw new NotImplementedException();
                    }

                    // Start conversion thread
                    conversionWorker.Priority = ThreadPriority.Highest;
                    conversionWorker.Start();
                    if (conversionWorker.ThreadState == ThreadState.Running) ToggleWorking(true);
                } else {
                    // Save as regular image
                    ImageFormat imgFormat;
                    try {
                        // Choose the ImageFormats whose Guids equal the FormatID of the corresponding ImageCodecInfos
                        imgFormat = typeof(ImageFormat).GetProperties().First(
                            format => format.PropertyType.Equals(typeof(ImageFormat)) && ((ImageFormat)format.GetValue(null, null)).Guid == ImageCodecInfo.GetImageEncoders().First(
                                decoder => decoder.FilenameExtension.ToLower().Contains(info.Extension.ToLower())
                            ).FormatID
                        ).GetValue(null, null) as ImageFormat;
                    } catch (InvalidOperationException) {
                        // Fall back to bmp on error
                        imgFormat = ImageFormat.Bmp;
                    }
                    blp.Save(info.FullName, imgFormat);
                }
            }
        }

        /// <summary>
        /// Opens a dialog informing the user that the program is currently converting.
        /// </summary>
        /// <param name="show">Show or hide window.</param>
        /// <remarks>Big images can take some time to convert.</remarks>
        private void ToggleWorking(bool show) {
            if (show) {
                this.Effect = new BlurEffect();
                ((BlurEffect)this.Effect).Radius = 5.0;
                winW = new WorkingWindow();
                winW.Owner = this;
                winW.ShowDialog();
            } else if (!show) {
                if (winW != null && winW.IsVisible == true) {
                    this.Effect = null;
                    winW.Close();
                }
            }
        }

        /// <summary>
        /// Open an image.
        /// </summary>
        private void Open(string filename) {
            try {
                blp.Dispose();
                blp = Blp2.FromFile(filename);
                img1.Source = GetBitmapSource((Bitmap)blp);
                scrollViewer.Background = (System.Windows.Media.VisualBrush)this.FindResource("Checkerboard");
                lblFilename.Text = System.IO.Path.GetFileName(filename);
                lblZoom.Visibility = System.Windows.Visibility.Visible;
                lblInfo1.Text = blp.Width.ToString() + "x" + blp.Height.ToString();
                lblInfo2.Text = blp.HasMipMaps ? "Yes" : "No";
                lblInfo3.Text = blp.Format;
                lblInfo4.Text = blp.Alpha.ToString();
            } catch {
                scrollViewer.Background = null;
                img1.Source = new BitmapImage(new Uri("Resources/broken.png", UriKind.Relative));
                lblFilename.Text = "Invalid image";
                lblZoom.Visibility = System.Windows.Visibility.Hidden;
                lblInfo1.Text = lblInfo2.Text = lblInfo3.Text = lblInfo4.Text = "--";
            }
        }

        /// <summary>
        /// Another instance of the application passed a filename to the main process.
        /// </summary>
        private void OpenFilePipe_ArgumentPassed(object sender, ArgumentPassedArgs arg) {
            this.Dispatcher.Invoke((Action)delegate() {
                Open(arg.Argument);
                this.Activate();
            });
        }

        /// <summary>
        /// Drag & .....
        /// </summary>
        private void previewDragEnter(object sender, DragEventArgs e) {
            FileInfo info;

            if (e.Data.GetDataPresent(DataFormats.FileDrop, true) == true) {
                string[] files = (string[])e.Data.GetData(DataFormats.FileDrop, true);

                if (files.Length == 1 && (info = new FileInfo(files[0])).Exists && info.Extension != "") {
                    if (info.Extension.ToLower() == ".blp" || ImageCodecInfo.GetImageDecoders().Any(c => c.FilenameExtension.ToLower().Split(new char[] { '*', ';' }).Any(ext => ext.Equals(info.Extension.ToLower())))) {
                        e.Effects = DragDropEffects.All;
                        e.Handled = true;
                        return;
                    }
                }
            }

            e.Effects = DragDropEffects.None;
            e.Handled = true;
        }

        /// <summary>
        /// ..... Drop
        /// </summary>
        private void DropFile(object sender, DragEventArgs e) {
            string[] file = (string[])e.Data.GetData(DataFormats.FileDrop, true);
            Open(file[0]);
            e.Handled = true;
        }

        /// <summary>
        /// Sets or deletes .blp file association inside the Windows registry
        /// </summary>
        private void ToggleFileAssociation() {
            if (menuExtension.IsChecked) {
                try {
                    var keyExt = Microsoft.Win32.Registry.CurrentUser.CreateSubKey(@"Software\Classes\.blp");
                    keyExt.SetValue("", "Blpc");
                    var keyBlpc = Microsoft.Win32.Registry.CurrentUser.CreateSubKey(@"Software\Classes\Blpc");
                    keyBlpc.SetValue("", "Blp Texture");
                    var keyBlpcIco = keyBlpc.CreateSubKey("DefaultIcon");
                    keyBlpcIco.SetValue("", "\"" + new FileInfo(App.ResourceAssembly.Location).DirectoryName + "\\cBlp2.dll\",0");
                    var keyBlpcCmd = keyBlpc.CreateSubKey(@"shell\open\command");
                    keyBlpcCmd.SetValue("", "\"" + new FileInfo(App.ResourceAssembly.Location).DirectoryName + "\\Blpc.exe\" \"%1%\"");
                    (new[] { keyExt, keyBlpc, keyBlpcIco, keyBlpcCmd }).ToList().ForEach((key) => key.Close());
                    ChangeNotify();
                } catch (Exception) {
                    menuExtension.IsChecked = false;
                }
            } else {
                try {
                    Microsoft.Win32.Registry.CurrentUser.DeleteSubKeyTree(@"Software\Classes\.blp", false);
                    Microsoft.Win32.Registry.CurrentUser.DeleteSubKeyTree(@"Software\Classes\Blpc", false);
                    ChangeNotify();
                } catch {
                    menuExtension.IsChecked = true;
                }
            }
        }

        /// <summary>
        /// Save settings before exiting
        /// </summary>
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e) {
            settings.Serialize(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData) + @"\Blpc.conf");
        }

        /// <summary>
        /// Menu actions
        /// </summary>
        private void CommandBinding_Executed(object sender, ExecutedRoutedEventArgs e) {
            if (e.Command.Equals(ApplicationCommands.Close)) {
                // Exit
                App.Current.Shutdown();
            } else if (e.Command.Equals(ApplicationCommands.Open)) {
                // Open file
                System.Windows.Forms.OpenFileDialog dlgOpen = new System.Windows.Forms.OpenFileDialog();
                dlgOpen.Filter = ImageCodecInfo.GetImageDecoders().Aggregate("All files|*.*|Blp texture|*.blp", (str, codec) =>
                    str += "|" + codec.FormatDescription + "|" + codec.FilenameExtension.ToLower()); ;
                dlgOpen.FilterIndex = 2;
                dlgOpen.ShowDialog();
                if (dlgOpen.FileName != "") Open(dlgOpen.FileName);
            } else if (e.Command.Equals(ApplicationCommands.Save)) {
                // Save file
                Save();
            } else if (e.Command.Equals(BatchConversion)) {
                // Batch conversion
                this.Effect = new BlurEffect();
                ((BlurEffect)this.Effect).Radius = 5.0;
                BatchcWindow winB = new BatchcWindow();
                winB.Owner = this;
                winB.settings = settings;
                winB.ShowDialog();
                this.Effect = null;
            } else if (e.Command.Equals(ApplicationCommands.Help)) {
                // Help
                this.Effect = new BlurEffect();
                ((BlurEffect)this.Effect).Radius = 5.0;
                HelpWindow winH = new HelpWindow();
                winH.Owner = this;
                winH.ShowDialog();
                this.Effect = null;
            } else if (e.Command.Equals(About)) {
                // About
                this.Effect = new BlurEffect();
                ((BlurEffect)this.Effect).Radius = 5.0;
                InfoWindow winI = new InfoWindow();
                winI.Owner = this;
                winI.ShowDialog();
                this.Effect = null;
            } else if (e.Command.Equals(AssociateFile)) {
                ToggleFileAssociation();
            }
        }

        /// <summary>
        /// Bitmap => BitmapSource (Image.Source = ...)
        /// </summary>
        private System.Windows.Media.Imaging.BitmapSource GetBitmapSource(System.Drawing.Bitmap bitmap) {
            IntPtr hBitmap = bitmap.GetHbitmap();

            System.Windows.Media.Imaging.BitmapSource bitmapSource = System.Windows.Interop.Imaging.CreateBitmapSourceFromHBitmap(
                hBitmap,
                IntPtr.Zero,
                Int32Rect.Empty,
                System.Windows.Media.Imaging.BitmapSizeOptions.FromEmptyOptions()
            );

            // Free the memory used by the GDI bitmap object (GDI does not release this).
            DeleteObject(hBitmap);
            return bitmapSource;
        }

        #region Drag-to-scroll & zooming

        /// <summary>
        /// Centers the viewpoint about a given point.
        /// </summary>
        /// <param name="pt">Point that to be the middle of the viewport.</param>
        private void CenterViewport(System.Windows.Point pt) {
            // Reset zoomCenter
            zoomCenterPoint = new System.Windows.Point();
            scrollViewer.ScrollToHorizontalOffset(pt.X - scrollViewer.ViewportWidth / 2);
            scrollViewer.ScrollToVerticalOffset(pt.Y - scrollViewer.ViewportHeight / 2);
        }

        /// <summary>
        /// Start dragging the viewport
        /// </summary>
        private void scrollViewer_PreviewMouseDown(object sender, MouseButtonEventArgs e) {
            if (e.LeftButton == MouseButtonState.Pressed && mouseAction == MouseTool.Hand) {
                mouseDragStartPoint = e.GetPosition(scrollViewer);
                scrollStartOffset.X = scrollViewer.HorizontalOffset;
                scrollStartOffset.Y = scrollViewer.VerticalOffset;

                // Update the cursor if scrolling is possible 
                scrollViewer.Cursor = (scrollViewer.ExtentWidth > scrollViewer.ViewportWidth) || (scrollViewer.ExtentHeight > scrollViewer.ViewportHeight) ? curHandGrab : curHand;

                scrollViewer.CaptureMouse();
            } else if (e.LeftButton == MouseButtonState.Pressed && mouseAction == MouseTool.ZoomIn) {
                zoomCenterPoint = new System.Windows.Point(e.GetPosition(scrollViewer).X / zoomLevels[zoomFactor] + scrollViewer.HorizontalOffset / zoomLevels[zoomFactor],
                    e.GetPosition(scrollViewer).Y / zoomLevels[zoomFactor] + scrollViewer.VerticalOffset / zoomLevels[zoomFactor]);
                this.ZoomFactor++;
            } else if (e.LeftButton == MouseButtonState.Pressed && mouseAction == MouseTool.ZoomOut) {
                zoomCenterPoint = new System.Windows.Point(e.GetPosition(scrollViewer).X / zoomLevels[zoomFactor] + scrollViewer.HorizontalOffset / zoomLevels[zoomFactor],
                    e.GetPosition(scrollViewer).Y / zoomLevels[zoomFactor] + scrollViewer.VerticalOffset / zoomLevels[zoomFactor]);
                this.ZoomFactor--;
            }
        }

        /// <summary>
        /// Drag the viewport
        /// </summary>
        private void scrollViewer_PreviewMouseMove(object sender, MouseEventArgs e) {
            if (scrollViewer.IsMouseCaptured) {
                // Get the new mouse position. 
                System.Windows.Point mouseDragCurrentPoint = e.GetPosition(scrollViewer);

                // Determine the new amount to scroll. 
                System.Windows.Point delta = new System.Windows.Point(
                    (mouseDragCurrentPoint.X > this.mouseDragStartPoint.X) ?
                    -(mouseDragCurrentPoint.X - this.mouseDragStartPoint.X) :
                    (this.mouseDragStartPoint.X - mouseDragCurrentPoint.X),
                    (mouseDragCurrentPoint.Y > this.mouseDragStartPoint.Y) ?
                    -(mouseDragCurrentPoint.Y - this.mouseDragStartPoint.Y) :
                    (this.mouseDragStartPoint.Y - mouseDragCurrentPoint.Y));

                // Scroll to the new position. 
                scrollViewer.ScrollToHorizontalOffset(this.scrollStartOffset.X + delta.X);
                scrollViewer.ScrollToVerticalOffset(this.scrollStartOffset.Y + delta.Y);
            }
        }

        /// <summary>
        /// Release the viewport
        /// </summary>
        private void scrollViewer_PreviewMouseUp(object sender, MouseButtonEventArgs e) {
            if (scrollViewer.IsMouseCaptured && e.LeftButton == MouseButtonState.Released) {
                scrollViewer.Cursor = curHand;
                scrollViewer.ReleaseMouseCapture();
            }
        }

        /// <summary>
        /// Center viewport about the clicked coordinate after zoom
        /// </summary>
        private void scrollViewer_ScrollChanged(object sender, ScrollChangedEventArgs e) {
            // Scroll was updated due to zooming
            if (zoomCenterPoint != new System.Windows.Point())
                CenterViewport(new System.Windows.Point(zoomCenterPoint.X * zoomLevels[zoomFactor], zoomCenterPoint.Y * zoomLevels[zoomFactor]));
        }

        #endregion

        /// <summary>
        /// Local hotkeys for mouse actions. See <see cref="MouseAction"/>.
        /// </summary>
        private void Main_PreviewKeyDown(object sender, KeyEventArgs e) {
            if (e.Key == Key.OemPlus)
                this.MouseAction = MouseTool.ZoomIn;
            else if (e.Key == Key.OemMinus)
                this.MouseAction = MouseTool.ZoomOut;
            else if (e.Key == Key.H)
                this.MouseAction = MouseTool.Hand; ;
        }

    }

}
