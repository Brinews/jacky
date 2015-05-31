/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ajp.middleware;

/*
 * a simple map from name to class name
 * by default names map to themselves (ie: the name is the class name)
 */
import java.util.*;
import java.lang.reflect.*;

public class ClassMap extends LinkedHashMap<String, String> {
    /*
     * use this when descriptive name is the same as the class name
     */

    public void put(String name) {	// assume name is the same as the class name
        put(name, name);
    }
    /*
     * lookup a name & make an instance of the class name found
     */

    public Object getAsInstance(String key) {
        try {
            /*
            Class argType = Class.forName("String");
            Class cl = Class.forName(get(key));
            
            Constructor cons = cl.getConstructor(String);
            
            return cons.newInstance(key);
            */
            return Class.forName(get(key)).newInstance();
        } catch (Throwable e) {
            e.printStackTrace();
            return null;
        }
    }
}
