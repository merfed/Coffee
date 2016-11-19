using Autodesk.Maya.OpenMaya;
using M2Export;

[assembly: ExtensionPlugin(typeof(M2Editor), "M2Editor")]

namespace M2Export
{

    // This class is instantiated by Maya once and kept alive for the 
    // duration of the session. If you don't do any one time initialization 
    // then you should remove this class.
    // Its presence still improve load performance whilst you don't do any
    // initialization in it.
    public class M2Editor : IExtensionPlugin
    {
        public bool InitializePlugin()
        {
            // Initialize your plug-in application here
            MGlobal.executeCommand("m2EditorCreateMenu");
            return true;
        }

        public bool UninitializePlugin()
        {
            // Do plug-in application clean up here
            MGlobal.executeCommand("m2EditorDeleteMenu");
            return true;
        }

        // Return the Maya API version number
        // The actual plug-in can return an empty string by default
        public string GetMayaDotNetSdkBuildVersion() => "";
    }
}
