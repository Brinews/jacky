/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ajp.middleware;

/**
 *
 * @author coder
 */
public interface MessageListener {
    public void messageReceived(String from, String to, String msg, MsgId msgid);
}
