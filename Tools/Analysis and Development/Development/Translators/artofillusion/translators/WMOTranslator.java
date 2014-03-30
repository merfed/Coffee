package artofillusion.translators;

import artofillusion.*;
import buoy.widget.*;

/** OBJTranslator is a Translator which imports and exports OBJ files. */

public class WMOTranslator implements Translator
{
  public String getName()
  {
    return "World Model Object (.wmo)";
  }

  public boolean canImport()
  {
    return true;
  }
  
  public boolean canExport()
  {
    return true;
  }
  
  public void importFile(BFrame parent)
  {
    WMOImporter.importFile(parent);
  }
  
  public void exportFile(BFrame parent, Scene theScene)
  {
    WMOExporter2.exportFile(parent, theScene);
  }
}