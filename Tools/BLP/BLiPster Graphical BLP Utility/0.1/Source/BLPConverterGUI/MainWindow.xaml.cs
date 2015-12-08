#region Usings
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Xml;
using Microsoft.Windows.Controls.Ribbon;
using System.IO;
#endregion

namespace BLPConverterGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : RibbonWindow
    {
        #region Class-level Variables

        // Dictionary for storing custom rules
        //public static Dictionary<string, string> dictCustomRules = new Dictionary<string, string>();

        // Binding List for storing file formats
        public static BindingList<FileFormatClass> ocFormats;

        // Binding List for storing conversion rules
        public static BindingList<ConversionClass> ocConversions;

        // Storage for chosen files to convert.  At present, only the string list is used
        public FileList flFiles = new FileList();
        public List<string> lstFiles = new List<string>();
        public List<FileInfo> fiFiles = new List<FileInfo>();
        public Dictionary<FileInfo, string> dictFiles = new Dictionary<FileInfo, string>();

        // Storage for forced conversion
        private static string strForcedRule = "";

        // Keep track of threads as they finish
        private int doneThreads;

        #endregion

        #region Constructor

        /// <summary>
        /// Constructor
        /// </summary>
        public MainWindow()
        {
            //this.Resources.MergedDictionaries.Add(Microsoft.Windows.Controls.Ribbon.PopularApplicationSkins.Office2007Silver);

            InitializeComponent();
            ocConversions = RuleReader.ConversionRules();
            rddbListFormats.ItemsSource = ocConversions;
            rddbForceFormat.ItemsSource = RuleReader.FileFormats();
            lstInput.ItemsSource = flFiles;

            ClearOutputScreen();
        }

        #endregion

        #region Generic Ribbon Command Functions

        /// <summary>
        /// Automatically enables the linked control to allow execution
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        protected void RibbonCommand_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        /// <summary>
        /// Generic Executed function
        /// 
        /// Throws a NotImplementedException if a linked Ribbon item is executed
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        protected void RibbonCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            RibbonCommand rc = (RibbonCommand)e.Command;
            throw new NotImplementedException();
        }

        /// <summary>
        /// Used for ribbon commands that should do nothing but be clickable
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnIgnore(object sender, ExecutedRoutedEventArgs e)
        {
            // Ignore, do nothing
        }

        #endregion

        #region Ribbon CanExecute functions

        /// <summary>
        /// Determines whether the conversion command can execute
        /// 
        /// If there are no files queues for conversion, then the user cannot click on the Convert button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        protected void cmdConvert_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = (flFiles.Count > 0);
        }

        /// <summary>
        /// Enables or disables the Clear Output window button depending on if there is data in the output window
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        protected void cmdClearOutput_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            if (richTextBox1 == null)
                e.CanExecute = false;
            else if (richTextBox1.Document == null)
                e.CanExecute = false;
            else e.CanExecute = (richTextBox1.Document.Blocks.Count > 0);
        }

        /// <summary>
        /// Enables or disables the Clear Everything button depending on whether there are files listed to convert or data in the output window
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        protected void cmdClearEverything_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            if (richTextBox1 == null || lstInput == null)
                e.CanExecute = false;
            else if (richTextBox1.Document == null)
                e.CanExecute = false;
            else e.CanExecute = (richTextBox1.Document.Blocks.Count > 0) || (flFiles.Count > 0);
        }

        #endregion

        #region Mutually Exclusive Toggleboxs

        /// <summary>
        /// Runs when user toggles "create mips" setting
        /// 
        /// Turns "no mips" setting off if "create mips" is turned on
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdMipsOn(object sender, ExecutedRoutedEventArgs e)
        {
            if ((bool)rtbMipsOn.IsChecked && (bool)rtbMipsOff.IsChecked)
                rtbMipsOff.IsChecked = false;
        }

        /// <summary>
        /// Runs when user toggles "no mips" setting
        /// 
        /// Turns "create mips" setting off if "no mips" is turned on
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdMipsOff(object sender, ExecutedRoutedEventArgs e)
        {
            if ((bool)rtbMipsOn.IsChecked && (bool)rtbMipsOff.IsChecked)
                rtbMipsOn.IsChecked = false;
        }

        /// <summary>
        /// Runs when user toggles "character texture" setting
        /// 
        /// Turns "clothing texture" setting off if "character texture" is turned on
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdCharTex(object sender, ExecutedRoutedEventArgs e)
        {
            if ((bool)rtbCharTex.IsChecked && (bool)rtbClothTex.IsChecked)
                rtbClothTex.IsChecked = false;
        }

        /// <summary>
        /// Runs when user toggles "clothing texture" setting
        /// 
        /// Turns "character texture" setting off if "clothing texture" is turned on
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdClothTex(object sender, ExecutedRoutedEventArgs e)
        {
            if ((bool)rtbCharTex.IsChecked && (bool)rtbClothTex.IsChecked)
                rtbCharTex.IsChecked = false;
        }

        #endregion

        #region RibbonTab Executed functions

        /// <summary>
        /// Runs when the user toggles the "info only" setting
        /// 
        /// Disables the "verbose" mode togglebutton
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdInfo(object sender, ExecutedRoutedEventArgs e)
        {
            rtbVerbose.IsEnabled = !(bool)rtbInfo.IsChecked;
        }

        /// <summary>
        /// Runs when the user clicks the "custom conversion" button
        /// 
        /// Opens a dialog window to set up custom conversions
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdCustomConvert(object sender, ExecutedRoutedEventArgs e)
        {
            CustomConversion ccWin = new CustomConversion();
            ccWin.Owner = this;

            Nullable<bool> dialogResult = ccWin.ShowDialog();

            if (dialogResult == true)
            {
                rddbListFormats.ItemsSource = ocConversions;
            }
        }

        /// <summary>
        /// Supposed to run when the user clicks on an item in the Force Format dropdown.
        /// Unfortunately, I can't get it to register here if the user clicks along the borders of
        /// the menu (which, despite showing a highlight across the menu, doesn't actually click the menu item)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdForceFormat(object sender, ExecutedRoutedEventArgs e)
        {
            // Can't get it to be called?
        }

        /// <summary>
        /// Runs when user clicks conversion rule dropdown
        /// 
        /// Does nothing
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdListFormats(object sender, ExecutedRoutedEventArgs e)
        {
        }

        /// <summary>
        /// Runs when user toggles verbose mode
        /// 
        /// Does nothing
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdVerbose(object sender, ExecutedRoutedEventArgs e)
        {
        }

        /// <summary>
        /// Runs when the user sets a radiobutton in the Force Format dropdown button
        /// 
        /// The format specified in the chosen node is stored in a global string variable
        /// to be used when calling the converter program.  If the user chooses the 
        /// UNSPECIFIED option, the global variable is cleared (indicating no forced conversion)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void dtradForceFormat_Click(object sender, RoutedEventArgs e)
        {
            XmlNode pickedNode = (XmlNode)((ContentControl)e.OriginalSource).Content;
            if (pickedNode.FirstChild.InnerText == "[UNSPECIFIED]")
                strForcedRule = "";
            else
                strForcedRule = pickedNode.FirstChild.InnerText;
        }

        #endregion

        #region Ribbon Commands not yet implemented
        private void cmdOpenConvert(object sender, ExecutedRoutedEventArgs e)
        {
        }
        private void cmdOpenConversion(object sender, ExecutedRoutedEventArgs e)
        {
        }
        private void cmdSaveStuff(object sender, ExecutedRoutedEventArgs e)
        {
        }
        private void cmdSaveResults(object sender, ExecutedRoutedEventArgs e)
        {
        }
        private void cmdSaveConversion(object sender, ExecutedRoutedEventArgs e)
        {
        }
        private void cmdAddFiles(object sender, ExecutedRoutedEventArgs e)
        {
        }
        private void cmdConvert(object sender, ExecutedRoutedEventArgs e)
        {
        }
        private void cmdManage(object sender, ExecutedRoutedEventArgs e)
        {
        }
        #endregion

        #region RibbonApplicationMenu Executed functions

        /// <summary>
        /// Runs when the user clicks the New button
        /// 
        /// Clears out the current list of files and the output box
        /// (essentially returns the program to a startup state but maintains settings)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdNew(object sender, ExecutedRoutedEventArgs e)
        {
            flFiles.Clear();
            ClearOutputScreen();
        }

        /// <summary>
        /// Runs when the user presses the "clear output" button
        /// 
        /// Calls the ClearOutputScreen function
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdClearOutput(object sender, ExecutedRoutedEventArgs e)
        {
            ClearOutputScreen();
        }

        /// <summary>
        /// Runs when the user clicks the Add New Files button
        /// (yes, this is a misnomer)
        /// 
        /// Opens files for conversion
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdOpenFiles(object sender, ExecutedRoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            // Set filter for file extension and default file extension 
            dlg.DefaultExt = ".blp";
            dlg.Filter = "Valid Files|*.blp;*.png|WoW Image Files (*.blp)|*.blp|Image Files (*.png)|*.png|All files|*.*";
            dlg.Multiselect = true;

            // Display OpenFileDialog by calling ShowDialog method 
            Nullable<bool> result = dlg.ShowDialog();

            // Get the selected file names and store them in a list
            if (result == true)
            {
                foreach (string filename in dlg.FileNames)
                {
                    try
                    {
                        FileInfo fi = new FileInfo(filename);
                        flFiles.Add(fi);
                    }
                    catch
                    {
                    }
                }
            }
        }

        /// <summary>
        /// Runs when the user clicks the Convert Files button
        /// 
        /// This program executes the converter program using the settings specified by the user
        /// for the files desired.
        /// 
        /// Currently, it runs in multimode, but I may adapt is to run each file singly so I can
        /// better track the progress of the conversion process.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdConvertFiles(object sender, ExecutedRoutedEventArgs e)
        {
            if (flFiles.Count > 0)
            {
                string ArgumentSetup =
                    ((bool)rtbAlpha.IsChecked ? "/A" + rtxtAlpha.Text + " " : "") +
                    ((bool)rtbMipsOn.IsChecked ? "/C " : "") +
                    ((strForcedRule != "") ? "/F" + strForcedRule + " " : "") +
                    ((bool)rtbCharTex.IsChecked ? "/H " : "") +
                    ((bool)rtbInfo.IsChecked ? "/I " : "") +
                    ((bool)rtbMipsOff.IsChecked ? "/N " : "") +
                    ((bool)rtbClothTex.IsChecked ? "/R " : "") +
                    GetConversionRules() +
                    ((bool)rtbVerbose.IsChecked ? "/V " : "");

                // Disable most controls on the main window
                ToggleWindowEnabled(false);

                doneThreads = 0;

                // Run each file in its own backgroundworker
                foreach (FileInfo fi in flFiles)
                {
                    BackgroundWorker bgWorker = new BackgroundWorker();
                    bgWorker.DoWork += workerDoWork;
                    bgWorker.RunWorkerCompleted += workerRunCompleted;
                    bgWorker.RunWorkerAsync(ArgumentSetup + "\"" + fi.FullName + "\"");
                }
            }
            else
            {
                // If I set up the CanExecute commands properly, the user should never see this
                MessageBox.Show("No files were selected for conversion!");
            }
        }

        /// <summary>
        /// Exits the application
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdExitApp(object sender, ExecutedRoutedEventArgs e)
        {
            Close();
        }

        #endregion

        #region Alpha-Related Functions

        /// <summary>
        /// Runs when the user toggles the "alpha threshold" setting
        /// 
        /// Enables or disables a textbox to enter the alpha threshold
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdAlpha(object sender, ExecutedRoutedEventArgs e)
        {
            rtxtAlpha.IsEnabled = (bool)rtbAlpha.IsChecked;
        }

        /// <summary>
        /// Gets a preview of the text being entered into the alpha threshold textbox
        /// 
        /// Allows the user to enter only numbers
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void rtxtAlpha_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            if (e.Text.IndexOfAny(("0123456789").ToCharArray()) < 0)
                e.Handled = true;
        }

        /// <summary>
        /// Runs when the alpha threshold textbox loses focus
        /// 
        /// Converts the value in the textbox to a value between 0 and 255.  Any value above 255 is
        /// automatically set to 255.  An empty field is set to 0 by default.  This behavior could
        /// easily be modified to reset the value to the default of 128.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void rtxtAlpha_LostFocus(object sender, RoutedEventArgs e)
        {
            if (rtxtAlpha.Text == "")
                rtxtAlpha.Text = "0";
            else
            {
                int newValue = Int32.Parse(rtxtAlpha.Text);
                if (newValue > 255) newValue = 255;
                rtxtAlpha.Text = newValue.ToString();
            }
        }

        #endregion

        #region Helper functions
        /// <summary>
        /// Iterates through the list of conversion rules, finds the non-default rules, and creates
        /// a switch for the custom rule to pass to the converter program
        /// </summary>
        /// <returns></returns>
        private string GetConversionRules()
        {
            string tempString = "";
            foreach (ConversionClass cc in ocConversions)
                if (!cc.IsDefault)
                    tempString += "/U" + cc.SourceFormat + "=" + cc.TargetFormat + " ";

            return tempString;
        }

        /// <summary>
        /// Enables or disables controls on the main window depending on whether a batch conversion is taking place or not
        /// </summary>
        /// <param name="MakeEnabled"></param>
        private void ToggleWindowEnabled(bool MakeEnabled)
        {
            if (!MakeEnabled)
            {
                TaskbarItemInfo.ProgressValue = 0;
                TaskbarItemInfo.ProgressState = System.Windows.Shell.TaskbarItemProgressState.Normal;
                pbConversion.Value = 0;
                pbConversion.Visibility = Visibility.Visible;
                ribbon1.IsEnabled = false;
                spListBoxButtons.IsEnabled = false;
            }
            else
            {
                TaskbarItemInfo.ProgressValue = 1;
                TaskbarItemInfo.ProgressState = System.Windows.Shell.TaskbarItemProgressState.None;
                pbConversion.Value = 100;
                pbConversion.Visibility = Visibility.Collapsed;
                ribbon1.IsEnabled = true;
                spListBoxButtons.IsEnabled = true;
            }
        }

        /// <summary>
        /// Clears the RichTextBox used for outputting messages from the converter program.
        /// It does this by creating a new, blank FlowDocument and setting the paragraph spacing to 0,
        /// then assigning the new FlowDocument to the RichTextBox's Document property.
        /// </summary>
        private void ClearOutputScreen()
        {
            Style st = new Style(typeof(Paragraph));
            st.Setters.Add(new Setter(Block.MarginProperty, new Thickness(0)));
            richTextBox1.Document = new FlowDocument();
            richTextBox1.Document.Resources.Add(typeof(Paragraph), st);
        }

        #endregion

        #region BackgroundWorker functions

        /// <summary>
        /// Designates the work for the background worker
        /// 
        /// Basically, this is the meat and potatoes of the conversion program.  In a background thread, it executes the 
        /// BLPConverter program with the given arguments against a single file.  
        /// 
        /// The conversion is done asyncrhonously to allow multithreading the conversion.  
        /// Conversions can then be done simultaneously (or near enough)
        /// </summary>
        /// <param name="s"></param>
        /// <param name="args"></param>
        public void workerDoWork(object s, DoWorkEventArgs args)
        {   
            System.Diagnostics.Process executable = new System.Diagnostics.Process();
            string executablePath = "BLPConverter_8_1/BLPConverter.exe";
            executable.StartInfo.FileName = executablePath;
            executable.StartInfo.CreateNoWindow = true;

            executable.StartInfo.ErrorDialog = true;
            executable.StartInfo.UseShellExecute = false;
            executable.StartInfo.WorkingDirectory = AppDomain.CurrentDomain.BaseDirectory;
            executable.StartInfo.RedirectStandardOutput = true;

            // Run the conversion program on the file
            executable.StartInfo.Arguments = (string)args.Argument;
            try
            {
                executable.Start();

                // Get the output from the conversion program
                System.IO.StreamReader srRes = executable.StandardOutput;
                args.Result = srRes.ReadToEnd();
            }
            catch
            {
                args.Result = "BLPConverter is missing\n";
            }
        }

        /// <summary>
        /// Designates what needs to happen when the conversion is complete
        /// 
        /// If all created workers have completed, the last worker will re-enable the main controls of the window
        /// </summary>
        /// <param name="s"></param>
        /// <param name="args"></param>
        public void workerRunCompleted(object s, RunWorkerCompletedEventArgs args)
        {
            pbConversion.Value += 100 / (double)flFiles.Count;
            TaskbarItemInfo.ProgressValue += 1 / (double)flFiles.Count;
            richTextBox1.AppendText((string)args.Result);

            // Re-enable the window
            doneThreads++;
            if (doneThreads == flFiles.Count)
                ToggleWindowEnabled(true);
        }

        #endregion

        #region File Manipulation Functions
        public string Filename(string LongFileName) { return LongFileName.Substring(LongFileName.LastIndexOf('\\') + 1, LongFileName.Substring(LongFileName.LastIndexOf('\\')).Length - LongFileName.Substring(LongFileName.LastIndexOf('.')).Length - 1); }
        public string Path(string LongFileName) { return LongFileName.Substring(0, LongFileName.LastIndexOf('\\') + 1); }
        public string Extension(string LongFileName) { return LongFileName.Substring(LongFileName.LastIndexOf('.') + 1); }
        #endregion

        #region File ListBox functions

        /// <summary>
        /// Runs when the user presses the Clear Selected files button
        /// 
        /// Removes those selected files from the listbox
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnClearSelected_Click(object sender, RoutedEventArgs e)
        {
            while (lstInput.SelectedItems.Count > 0)
                flFiles.Remove((FileInfo)lstInput.SelectedItem);
        }

        /// <summary>
        /// Runs when the user presses the Clear All files button
        /// 
        /// Removes all files from the listbox
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnClearAll_Click(object sender, RoutedEventArgs e)
        {
            flFiles.Clear();
        }

        #endregion
    }
}
