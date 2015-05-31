/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ajp.middleware;

/**
 *
 * @author coder
 */
public class AjpMiddleware {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        //test code
        Portal p = new Portal("MainPortal");
        p.advisePort(10033);
        
        Agent from = new Agent("from");
        Agent to = new Agent("to");
        
        to.addMessageListener(new MessageListener() {
            public void messageReceived(String from, String to, String message, MsgId id) {
                System.out.println("Agent ["+to+"]: received message<" 
                        + message + "> from agent ["+from+"] through middleware");
            }
        });
        
        p.addAgent(from);
        p.addAgent(to);
        
        from.sendMessage("to", "hello message.");
        from.sendMessage("to", "bye message.");
    }
}
