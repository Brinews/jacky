/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ajp.middleware;

/**
 * the agent class, each agent is a thread
 * @author coder
 */
public class Agent extends BlockQueue {
    private Thread myThread;
    private MessageListener listener;
    private String name;
    private Portal portal;
    
    public Agent(String f) {
        super();
        name = f;
        //name = this.getClass().getName();
        
        /* register the message handler, and start a thread */
        myThread = new Thread(new Runnable(){
            /**
             * override the run method
             */
            public void run() {
                while (true) {
                    /* get message and dispose it */
                    Message msg = dequeue();
                    if (msg != null) {
                        handleMsg(msg);
                    }
                }
            }
        });
        
        myThread.start();
        
        listener = null;
    }
    
    /**
     * actual message handle method
     * @param msg message body
     */
    public void handleMsg(Message msg) {
        listener.messageReceived(msg.getFrom(), msg.getTo(), msg.getMsg(), msg.getMsgId());
    }
    
    /**
     * message handler register
     * @param msgListener interface method
     */
    public void addMessageListener(MessageListener msgListener) {
        listener = msgListener;
    }
    
    /**
     * send message method
     * @param to destination agent
     * @param msg message content
     */
    public void sendMessage(String to, String msg) {
        MsgId msgId = new MsgId();
        msgId.getId();
        Message message = new Message(name, to, msg, msgId);
        
        Agent toAg = portal.getAgent(to);
        
        if (toAg != null) toAg.enqueue(message);
        else {
            /*todo: when msg contains "#", need escape it */
            String conMesg = "message#" + name + "#" + to + "#" + msg + "#" + msgId.toString();
            portal.sendMessage(to, conMesg);
        }
    }
    
    /**
     * get function
     * @return the agent's name
     */
    public String getName() {
        return name;
    }
    
    public void setName(String f) {
        name = f;
    }
    
    /**
     * set the portal
     * @param p portal class
     */
    public void setPortal(Portal p) {
        portal = p;
    }
}
