/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ajp.middleware;

public class Def {	// please discuss the use of this type of class with S
    // this approach fits a design pattern & overcomes some
    // messy problems but some people don't like the approach

    /**
     * a brief time delay mechanism delay is in millisecs
     */
    public static void snooze(int delay) {
        if (delay == 0) {
            return;		// catch no delay case
        }
        try {
            Thread.sleep(delay);
        } catch (InterruptedException e) {
        }
    }
}