package artofillusion.translators;

import artofillusion.*;
import buoy.widget.*;

/** OBJTranslator is a Translator which imports and exports OBJ files. */

public class M2Translator implements Translator
{
  public String getName()
  {
    return "Model File (.m2)";
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
    M2Importer.importFile(parent);
  }
  
  public void exportFile(BFrame parent, Scene theScene)
  {
   M2Exporter.exportFile(parent, theScene);
  }
}