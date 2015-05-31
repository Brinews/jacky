/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ajp.middleware;

/**
 *
 * @author yangliang
 */
public class PortalTest {
     /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        //test code
        Portal p = new Portal("MainPortal");
        p.advisePort(10034);
        
        Agent from = new Agent("remote1");
        Agent to = new Agent("remote2");
        
        to.addMessageListener(new MessageListener() {
            public void messageReceived(String from, String to, String message, MsgId id) {
                System.out.println("Agent ["+to+"]: received message<" 
                        + message + "> from agent ["+from+"] through middleware's portal");
            }
        });
        
        p.addAgent(from);
        p.addAgent(to);
        
        p.portalConnect("127.0.0.1", 10033); /* connect other portal */
        
        from.sendMessage("remote2", "local message.");
        from.sendMessage("to", "message to remote portal");
    }
}
