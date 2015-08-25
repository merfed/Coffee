/**
 * Config.java
 * 
 * This class (supposed to be reusable) is useful to set properties easily
 * from a configuration file (no need to recompile the whole thing).
 * 
 * NOTHING is checked here, because all this stuff is supposed to be located in a specific 
 * class extending this one. This way, Config is never modified.
 * 
 * @author pagdzin & Mjollnà
 */

package config;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.logging.Level;
import java.util.logging.Logger;


public abstract class Config {
    
    @Deprecated
    protected static final String fileExtension=".conf";
    
    protected HashMap<String, String> properties=null;
    private String fileName="";
    
    public static boolean isConfigFile(String fileName) {
        File file=new File(fileName);
        return (file.exists());
    }
     
    /*
     * extracts a property in the HashMap.
     * 
     * @param the key String used in the HashMap
     * @return the value associated with that key.
     */
    public String getProperty(String p) {
        return this.properties.get(p);
    }
    
    /*
     * Noone should ever need this, because the reading of the data is done inside the constructor itself.
     * Feel free to de-deprecate if you do think it is useful in any way.
     * 
     * @ return the configuration file name, without the absolute path.
     */
    @Deprecated
    public String getFileName() {
        return this.fileName;
    }
    
    protected abstract void handleProperties();
    
    
    public Config(String s) {
        properties=new HashMap<String, String>();
        this.fileName=s;
        File file=new File(fileName);
        BufferedReader br=null;
            
        try {    
            br = new BufferedReader(new FileReader(file));
            String line=null;
            String key=null;
            String value=null;
            while ((line=br.readLine()) != null) {
                if (!line.startsWith("#") && !line.startsWith("//") && !line.startsWith(";") && !line.isEmpty()) {
                   
                    key=line.substring(0, line.indexOf("="));
                    //key=key.replaceAll("[\t ]", ""); // TODO filter tabs and spaces where they must be filtered (certainly not inside the word itself!)

                    value=line.substring(line.indexOf("=")+1);
                    //value=value.replaceAll("[\t \"]", ""); // see prvious comment
                    value=value.replaceAll("\"", "");

                    properties.put(key, value);
                }
                
            }
        } catch (FileNotFoundException ex) { // stream creation
            Logger.getLogger(Config.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) { // the whole while
            Logger.getLogger(Config.class.getName()).log(Level.SEVERE, null, ex);
        }
        
    }
   
}
