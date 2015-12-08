namespace BLPConverterGUI
{
    using Microsoft.Windows.Controls.Ribbon;
    using System.Windows;

    /// <summary>
    /// This class holds the global commands used by the application
    /// </summary>
    public static class AppCommands
    {
        public static RibbonCommand Options { get { return (RibbonCommand)Application.Current.Resources["cmdOptions"]; } }
        public static RibbonCommand OpenFiles { get { return (RibbonCommand)Application.Current.Resources["cmdOpenFiles"]; } }
        public static RibbonCommand ConvertFiles { get { return (RibbonCommand)Application.Current.Resources["cmdConvertFiles"]; } }
        public static RibbonCommand ExitApp { get { return (RibbonCommand)Application.Current.Resources["cmdExitApp"]; } }
        public static RibbonCommand HomeExitApp { get { return (RibbonCommand)Application.Current.Resources["cmdHomeExitApp"]; } }
        public static RibbonCommand MipsOn { get { return (RibbonCommand)Application.Current.Resources["cmdMipsOn"]; } }
        public static RibbonCommand MipsOff { get { return (RibbonCommand)Application.Current.Resources["cmdMipsOff"]; } }
        public static RibbonCommand CharTex { get { return (RibbonCommand)Application.Current.Resources["cmdCharTex"]; } }
        public static RibbonCommand ClothTex { get { return (RibbonCommand)Application.Current.Resources["cmdClothTex"]; } }
        public static RibbonCommand Verbose { get { return (RibbonCommand)Application.Current.Resources["cmdVerbose"]; } }
        public static RibbonCommand Info { get { return (RibbonCommand)Application.Current.Resources["cmdInfo"]; } }
        public static RibbonCommand ForceFormat { get { return (RibbonCommand)Application.Current.Resources["cmdForceFormat"]; } }
        public static RibbonCommand Alpha { get { return (RibbonCommand)Application.Current.Resources["cmdAlpha"]; } }
        public static RibbonCommand ListFormats { get { return (RibbonCommand)Application.Current.Resources["cmdListFormats"]; } }
        public static RibbonCommand CustomConvert { get { return (RibbonCommand)Application.Current.Resources["cmdCustomConvert"]; } }
        public static RibbonCommand OpenConversion { get { return (RibbonCommand)Application.Current.Resources["cmdOpenConversion"]; } }
        public static RibbonCommand SaveOutput { get { return (RibbonCommand)Application.Current.Resources["cmdSaveOutput"]; } }
        public static RibbonCommand SaveConversion { get { return (RibbonCommand)Application.Current.Resources["cmdSaveConversion"]; } }
        public static RibbonCommand SaveStuff { get { return (RibbonCommand)Application.Current.Resources["cmdSaveStuff"]; } }
        public static RibbonCommand New { get { return (RibbonCommand)Application.Current.Resources["cmdNew"]; } }
        public static RibbonCommand Manage { get { return (RibbonCommand)Application.Current.Resources["cmdManage"]; } }
        public static RibbonCommand ClearOutput { get { return (RibbonCommand)Application.Current.Resources["cmdClearOutput"]; } }
        
    }
}
