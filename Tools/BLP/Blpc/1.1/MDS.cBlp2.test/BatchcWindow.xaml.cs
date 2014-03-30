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
using System.Windows;
using System.Windows.Input;
using System.IO;
using System.Linq;
using System.Drawing.Imaging;
using System.Threading;

namespace MDS.cBlp2.test {
    public partial class BatchcWindow : Window {

        internal SaveSettings settings;
        System.Windows.Controls.ListBoxItem listItem;

        // UI thread & lock
        Thread conversionWorker;
        bool locked = false;

        // Output information
        List<FileInfo> filesFiltered = new List<FileInfo>();
        ImageFormat imgFormat;
        string outputExt;

        public BatchcWindow() {
            InitializeComponent();
            cmbIn.Items.Add("BLP");
            cmbOut.Items.Add("BLP");
            ImageCodecInfo.GetImageDecoders().ToList().ForEach(codec => cmbIn.Items.Add(codec.FormatDescription));
            ImageCodecInfo.GetImageEncoders().ToList().ForEach(codec => cmbOut.Items.Add(codec.FormatDescription));
            cmbIn.SelectedIndex = 0;
            cmbOut.SelectedIndex = 1;
        }

        /// <summary>
        /// Choose input directory
        /// </summary>
        private void OpenInputDir(object sender, RoutedEventArgs e) {
            using (System.Windows.Forms.FolderBrowserDialog dlgFolder = new System.Windows.Forms.FolderBrowserDialog()) {
                dlgFolder.RootFolder = Environment.SpecialFolder.Desktop;
                dlgFolder.Description = "Please specify the input directory.";
                if (dlgFolder.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
                    txtFolderIn.Text = dlgFolder.SelectedPath;
                    if (String.IsNullOrEmpty(txtFolderOut.Text))
                        txtFolderOut.Text = dlgFolder.SelectedPath;
                }
            }
        }

        /// <summary>
        /// Choose output directory
        /// </summary>
        private void OpenOutputDir(object sender, RoutedEventArgs e) {
            using (System.Windows.Forms.FolderBrowserDialog dlgFolder = new System.Windows.Forms.FolderBrowserDialog()) {
                dlgFolder.RootFolder = Environment.SpecialFolder.Desktop;
                dlgFolder.Description = "Please specify the output directory.";
                if (dlgFolder.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
                    txtFolderOut.Text = dlgFolder.SelectedPath;
                }
            }
        }

        private void btnConvert_Click(object sender, RoutedEventArgs e) {
            if (cmbIn.SelectedValue.ToString() == cmbOut.SelectedValue.ToString()) {
                System.Windows.Forms.MessageBox.Show("Input and output types must be different!", "Check your parameters", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Exclamation);
                return;
            }

            if (String.IsNullOrEmpty(txtFolderIn.Text) || String.IsNullOrEmpty(txtFolderOut.Text)) {
                System.Windows.Forms.MessageBox.Show("Please specify a input and output directory!", "Check your parameters", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Exclamation);
                return;
            }

            // Cancel conversion
            if (locked) {
                if (conversionWorker.ThreadState == ThreadState.Running) {
                    System.Diagnostics.Debug.WriteLine("abort...");
                    conversionWorker.Suspend();
                    ToggleUI();
                    return;
                }
            }

            lstStatus.Items.Clear();
            listItem = new System.Windows.Controls.ListBoxItem();
            listItem.Content = "Generating file list -- " + DateTime.Now;
            listItem.Foreground = (System.Windows.Media.SolidColorBrush)App.Current.Resources["TextColor2"];
            lstStatus.Items.Add(listItem);
            progress.Value = 0;
            ToggleUI();

            string folderIn = txtFolderIn.Text;
            string folderOut = txtFolderOut.Text;
            bool subfolder = (chkSubfolders.IsChecked == true) ? true : false;
            string cmbInTxt = cmbIn.SelectedValue.ToString();
            string cmbOutTxt = cmbOut.SelectedValue.ToString();

            if (File.Exists(folderOut + @"\conversion.log")) File.Delete(folderOut + @"\conversion.log");
            
            conversionWorker = new Thread(delegate() {

                DirectoryInfo di = new DirectoryInfo(folderIn);
                IEnumerable<FileInfo> filesUnfiltered = new DirectoryInfo(folderIn).EnumerateFiles("*", (subfolder) ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly);

                // Filter complete file listfiles
                if (cmbInTxt == "BLP") {
                    filesFiltered.AddRange(filesUnfiltered.Where(file => file.Extension.ToUpper() == ".BLP"));
                } else {
                    ImageCodecInfo.GetImageDecoders().First(codec => codec.FormatDescription == cmbInTxt).FilenameExtension.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries).ToList().ForEach(
                        ext => filesFiltered.AddRange(filesUnfiltered.Where(file => file.Extension.ToUpper() == ext.Replace("*", "").ToUpper()))
                    );
                }

                // Collect unfiltered file list
                filesUnfiltered = null;
                GC.Collect();

                if (cmbOutTxt == "BLP") {
                    outputExt = ".BLP";
                } else {
                    outputExt = ImageCodecInfo.GetImageEncoders().First(
                        codec => codec.FormatDescription.ToUpper() == cmbOutTxt
                    ).FilenameExtension.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries)[0].Replace("*", "");

                    imgFormat = typeof(ImageFormat).GetProperties().First(
                        format => format.PropertyType.Equals(typeof(ImageFormat)) && ((ImageFormat)format.GetValue(null, null)).Guid == ImageCodecInfo.GetImageEncoders().First(
                            decoder => decoder.FilenameExtension.ToUpper().Contains(outputExt.ToUpper())
                        ).FormatID
                    ).GetValue(null, null) as ImageFormat;
                }

                for (int i = 0; i < filesFiltered.Count; i++) {
                    string dirName = filesFiltered[i].FullName.Replace(di.FullName, "").Replace(filesFiltered[i].Name, "");

                    // Add status list item
                    this.Dispatcher.Invoke((Action)delegate() {
                        // Add listbox item
                        listItem = new System.Windows.Controls.ListBoxItem();
                        listItem.Content = filesFiltered[i].Name;
                        lstStatus.Items.Add(listItem);

                        // Limit list box item count to 50
                        if (lstStatus.Items.Count > 50) lstStatus.Items.RemoveAt(lstStatus.Items.Count - 51);
                        lstStatus.ScrollIntoView(listItem);

                        // Update progress bar
                        progress.Value = (float)(i + 1) / filesFiltered.Count() * 100;
                    });

                    try {

                        // Create subdirectory
                        if (!Directory.Exists(folderOut + dirName))
                            Directory.CreateDirectory(folderOut + dirName);

                        using (Blp2 blp = Blp2.FromFile(filesFiltered[i].FullName)) {
                            if (cmbOutTxt == "BLP") {
                                if (settings.Format == SaveSettings.SaveFormat.Dxt1) {
                                    blp.Save(folderOut + dirName + filesFiltered[i].Name.Substring(0, filesFiltered[i].Name.LastIndexOf('.')) + outputExt, LibSquish.DxtFormat.Dxt1, settings.ResizeMethod, settings.Mipmaps, settings.DxtQuality);
                                } else if (settings.Format == SaveSettings.SaveFormat.Dxt3) {
                                    blp.Save(folderOut + dirName + filesFiltered[i].Name.Substring(0, filesFiltered[i].Name.LastIndexOf('.')) + outputExt, LibSquish.DxtFormat.Dxt3, settings.ResizeMethod, settings.Mipmaps, settings.DxtQuality);
                                } else if (settings.Format == SaveSettings.SaveFormat.Dxt5) {
                                    blp.Save(folderOut + dirName + filesFiltered[i].Name.Substring(0, filesFiltered[i].Name.LastIndexOf('.')) + outputExt, LibSquish.DxtFormat.Dxt5, settings.ResizeMethod, settings.Mipmaps, settings.DxtQuality);
                                } else if (settings.Format == SaveSettings.SaveFormat.Raw1) {
                                    blp.Save(folderOut + dirName + filesFiltered[i].Name.Substring(0, filesFiltered[i].Name.LastIndexOf('.')) + outputExt, Blp2.RawFormat.Raw1, settings.ResizeMethod, settings.Mipmaps, settings.Raw1Quality, settings.Dither);
                                } else if (settings.Format == SaveSettings.SaveFormat.Raw3) {
                                    blp.Save(folderOut + dirName + filesFiltered[i].Name.Substring(0, filesFiltered[i].Name.LastIndexOf('.')) + outputExt, Blp2.RawFormat.Raw3, settings.ResizeMethod, settings.Mipmaps);
                                }
                            } else {
                                blp.Save(folderOut + dirName + filesFiltered[i].Name.Substring(0, filesFiltered[i].Name.LastIndexOf('.')) + outputExt, imgFormat);
                            }
                        }

                    } catch (Exception exc) {
                        try {
                            using (FileStream fs = new FileStream(folderOut + @"\conversion.log", FileMode.Append, FileAccess.Write, FileShare.Write)) {
                                StreamWriter sw = new StreamWriter(fs);
                                sw.WriteLine("err @" + filesFiltered[i].Name + ": " + exc.Message);
                                sw.Flush();
                                sw.Close();
                            }
                        } catch (Exception) { }
                    }

                }

                this.Dispatcher.Invoke((Action)delegate() {
                    listItem = new System.Windows.Controls.ListBoxItem();
                    listItem.Content = "Conversion completed -- " + DateTime.Now;
                    listItem.Foreground = (System.Windows.Media.SolidColorBrush)App.Current.Resources["TextColor2"];
                    lstStatus.Items.Add(listItem);
                    lstStatus.ScrollIntoView(listItem);
                    ToggleUI();
                });

            });

            // Start conversion thread
            conversionWorker.Priority = ThreadPriority.Normal;
            conversionWorker.Start();

        }

        private void ToggleUI() {
            if (locked) {
                btnClose.IsEnabled = true;
                btnConvert.Content = "Start";
                lblBrowseIn.IsEnabled = true;
                lblBrowseOut.IsEnabled = true;
                locked = false;
            } else {
                btnClose.IsEnabled = false;
                btnConvert.Content = "Stop";
                lblBrowseIn.IsEnabled = false;
                lblBrowseOut.IsEnabled = false;
                locked = true;
            }
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.ChangedButton == MouseButton.Left) this.DragMove();
        }

        private void btnClose_Click(object sender, RoutedEventArgs e) {
            this.Close();
        }
    }

}
