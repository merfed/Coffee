using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Xml;

namespace BLPConverterGUI
{
    /// <summary>
    /// Interaction logic for CustomConversion.xaml
    /// </summary>
    public partial class CustomConversion : Window
    {
        // Dictionary for storing custom rules
        public BindingList<ConversionClass> ocConversions = new BindingList<ConversionClass>();
        private static BindingList<FileFormatClass> ffcFormats = RuleReader.FileFormats();
        private bool IsDirty = false;

        public CustomConversion()
        {
            InitializeComponent();
            GetRules();
            lstConversions.ItemsSource = ocConversions;

            cbSource.ItemsSource = ffcFormats;
            cbSource.ItemTemplate = null;
            cbSource.DisplayMemberPath = "Format"; 
            cbTarget.ItemsSource = ffcFormats;
            cbTarget.ItemTemplate = null;
            cbTarget.DisplayMemberPath = "Format";
        }

        protected void GetRules()
        {
            ocConversions.Clear();

            foreach (ConversionClass cc in MainWindow.ocConversions)
                if (!cc.IsDefault)
                    ocConversions.Add(cc);

            IsDirty = false;
        }

        private bool SaveRules()
        {
            BindingList<ConversionClass> tempOC = new BindingList<ConversionClass>();
            
            // Did the user leave a rule unfinished before trying to save?
            if ((cbSource.SelectedIndex > 0) && (cbTarget.SelectedIndex > 0))
            {
                MessageBoxResult dialogResult = MessageBox.Show("You have not saved your last rule.  Save it now?",
                        CustomConversionWindow.Title, MessageBoxButton.YesNo, MessageBoxImage.Warning, MessageBoxResult.No);

                switch (dialogResult)
                {
                    case MessageBoxResult.Yes:      { AddRule(); break; }
                    case MessageBoxResult.No:       { break; }
                    case MessageBoxResult.Cancel:   { return false; }
                }
            }

            // First, get back of all the old rules and reset back to defaults
            foreach (ConversionClass leRule in MainWindow.ocConversions)
                if (leRule.IsDefault)
                {
                    leRule.IsOverridden = false;
                    tempOC.Add(leRule);
                }

            // Now, sort through the update list
            // Known issue: multiple overrides will be allowed!
            foreach (ConversionClass cc in ocConversions)
            {
                // Skip over any "unspecified" or null elements
                if (!((cc.SourceFormat == "[UNSPECIFIED]") || (cc.TargetFormat == "[UNSPECIFIED]") ||
                    (cc.SourceFormat == "") || (cc.TargetFormat == "")))
                {
                    // Find the default that was overridden and set it's override flag to true
                    foreach (ConversionClass dd in tempOC)
                    {
                        if ((dd.SourceFormat == cc.SourceFormat) && (dd.IsDefault))
                            dd.IsOverridden = true;
                    }

                    // Then add the new item to the list
                    tempOC.Add(cc);
                }
            }

            // Now put the new list back to the main list
            MainWindow.ocConversions.Clear();
            foreach (ConversionClass cc in tempOC)
                MainWindow.ocConversions.Add(cc);

            IsDirty = false;
            return true;
        }

        protected void CopyCollection(BindingList<ConversionClass> dictSource, BindingList<ConversionClass> dictDest)
        {
            BindingList<ConversionClass> tempOC = new BindingList<ConversionClass>();

            // First, get back of all the old rules and reset back to defaults
            foreach (ConversionClass leRule in dictDest)
                if (leRule.IsDefault)
                {
                    leRule.IsOverridden = false;
                    tempOC.Add(leRule);
                }

            // Now, sort through the update list
            // Known issue: multiple overrides will be allowed!
            foreach (ConversionClass cc in dictSource)
            {
                // Skip over any "unspecified" or null elements
                if (!((cc.SourceFormat == "[UNSPECIFIED]") || (cc.TargetFormat == "[UNSPECIFIED]") || 
                    (cc.SourceFormat=="") || (cc.TargetFormat=="")))
                {
                    // Find the default that was overridden and set it's override flag to true
                    foreach (ConversionClass dd in tempOC)
                    {
                        if ((dd.SourceFormat == cc.SourceFormat) && (dd.IsDefault))
                            dd.IsOverridden = true;
                    }

                    // Then add the new item to the list
                    tempOC.Add(cc);
                }
            }

            // Now put the new list back to the main list
            dictDest.Clear();
            foreach (ConversionClass cc in tempOC)
                dictDest.Add(cc);
        }

        private void btnOk_Click(object sender, RoutedEventArgs e)
        {
            SaveRules();
            DialogResult = true;
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (IsDirty)
            {
                // User made some changes...let's confirm those changes before just dropping the user back to the main screen
                MessageBoxResult saveDialogResult = MessageBox.Show("Do you want to save changes to the custom rules?",
                        CustomConversionWindow.Title, MessageBoxButton.YesNoCancel, MessageBoxImage.Question, MessageBoxResult.No);

                switch (saveDialogResult)
                {
                    case MessageBoxResult.Yes:      { e.Cancel = !SaveRules(); break; }
                    case MessageBoxResult.No:       { break; }
                    case MessageBoxResult.Cancel:   { e.Cancel = true; break; }
                }
            }

            if ((cbSource.SelectedIndex > 0) && (cbTarget.SelectedIndex > 0) && !e.Cancel)
            {
                MessageBoxResult dialogResult = MessageBox.Show("You have not saved your last rule.  Save it now?",
                        CustomConversionWindow.Title, MessageBoxButton.YesNo, MessageBoxImage.Warning, MessageBoxResult.No);

                switch (dialogResult)
                {
                    case MessageBoxResult.Yes: { AddRule(); SaveRules(); break; }
                    case MessageBoxResult.No: { break; }
                    case MessageBoxResult.Cancel: { e.Cancel = false; break; }
                }
            }
        }

        private void AddRule()
        {
            if ((cbSource.SelectedIndex > 0) && (cbTarget.SelectedIndex > 0))
            {
                IsDirty = true;
                ConversionClass cc = new ConversionClass();
                cc.SourceFormat = ((FileFormatClass)cbSource.SelectedItem).Format;
                cc.TargetFormat = ((FileFormatClass)cbTarget.SelectedItem).Format;
                cbSource.SelectedIndex = -1;
                cbTarget.SelectedIndex = -1;
                ocConversions.Add(cc);
            }
        }

        private void btnAdd_Click(object sender, RoutedEventArgs e)
        {
            AddRule();
        }

        private void btnClear_Click(object sender, RoutedEventArgs e)
        {
            if (ocConversions.Count > 0)
            {
                ocConversions.Clear();
                IsDirty = true;
            }
            cbSource.SelectedIndex = -1;
            cbTarget.SelectedIndex = -1;
        }

        private void btnClearSelected_Click(object sender, RoutedEventArgs e)
        {
            List<ConversionClass> lstCC = new List<ConversionClass>();

            foreach (ConversionClass cc in lstConversions.SelectedItems)
                lstCC.Add(cc);

            if (lstCC != null)
                IsDirty = true;

            foreach (ConversionClass cc in lstCC)
                ocConversions.Remove(cc);

            lstCC = null;
        }
    }
}
