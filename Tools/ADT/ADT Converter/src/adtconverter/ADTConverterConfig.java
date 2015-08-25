/**
 * ADTConverterConfig.java
 * 
 * The project-specific configuration class that inherits the Config class.
 * 
 * @author pagdzin & Mjollnà
 */

package adtconverter;

import java.io.FileWriter;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

public final class ADTConverterConfig extends config.Config {

    
    static void generateConfFile(String confFileName) {
        try {
            String s=System.getProperty("line.separator");
            FileWriter fw=new FileWriter(confFileName);
            fw.write("# Auto-generated configuration file."+s+s);
            fw.write("# debug : \"yes\" is very verbose, preferably use on single files, or the parsing will take forever."+s);
            fw.write("debug=\"no\""+s+s);
            fw.write("# directory : the directory the converted files will be written to"+s);
            fw.write("directory=\""+System.getProperty("user.home")+"\""+s+s);
            fw.write("# source_directory : the directory in which you will be when you open the file chooser. Set to the user home if blank. "+s);
            fw.write("# source_directory=\"the directory you like the most\""+s);
            fw.close();
        } catch (IOException ex) {
            Logger.getLogger(ADTConverterConfig.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    @Deprecated
    public ADTConverterConfig() {
        this(ADTConverterConfig.class.getPackage().getName()+fileExtension);
    }
    
    public ADTConverterConfig(String s) {
        super(s);
        handleProperties();
    }
    
    @Override
    protected void handleProperties() {
        
        // debug
        if ((!properties.get("debug").equals("yes")) && ((!properties.get("debug").equals("no")))) {
            System.out.println("Erreur dans le fichier de configuration : la valeur de la proprieté debug doit être soit \"yes\", soit \"false\"");
        }
        
        // directory
        if (!(properties.get("directory").substring(properties.get("directory").length()-1).equals(System.getProperty("file.separator")))) { 
            // if the last character is not a "\" or a "/" (accordingly to the O.S.) ...
            properties.put("directory", properties.get("directory")+System.getProperty("file.separator")); // ... we just add it
        }
        
        // source_directory
        if ((properties.get("source_directory") != null) &&!(properties.get("source_directory").substring(properties.get("source_directory").length()-1).equals(System.getProperty("file.separator")))) { 
            // if the last character is not a "\" or a "/" (accordingly to the O.S.) ...
            properties.put("source_directory", properties.get("source_directory")+System.getProperty("file.separator")); // ... we just add it
        }
    }
}
