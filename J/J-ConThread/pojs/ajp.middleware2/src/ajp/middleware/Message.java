/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ajp.middleware;

/**
 *
 * @author coder
 */
/**
 * the Message sequence number, increase auto
 * @author coder
 */
class MsgId {
    static Integer seqNo = 0;
    public MsgId() {}
    
    /**
     * constructor with parameters int
     * @param seq int 
     */
    public MsgId(int seq) {
        seqNo = seq;
    }
    
    /**
     * construstor with parameters String
     * @param s String
     */
    public MsgId(String s) {
        seqNo = Integer.parseInt(s);
    }
    
    /**
     * when get id, it increase by 1
     * @return sequence number
     */
    public int getId() {
        synchronized(seqNo) {
            seqNo++;
        }
        return seqNo;
    }
    
    /**
     * override toString
     * @return seqNo
     */
    public String toString() {
        return new Integer(seqNo).toString();
    }
}

/**
 * message class, contains (from, to, content, id)
 * @author coder
 */

public class Message {
    private String from;
    private String to;
    private String msg;
    private MsgId id;
    
    /**
     * message structure
     * @param f from name
     * @param t to name
     * @param m message body
     * @param i message id
     */
    public Message(String f, String t, String m, MsgId i) {
        from = f;
        to = t;
        msg = m;
        id = i;
    }
    
    /**
     * constructor
     */
    public Message() {
        from = ""; to = ""; msg = ""; id = null;
    }
   
    /**
     * @return from name
     */
    public String getFrom() {
        return from;
    }
    
    /**
     * destination agent
     * @return to name
     */
    public String getTo() {
        return to;
    }
    
    /**
     * get message content
     * @return message body
     */
    public String getMsg() {
        return msg;
    }
    
    /**
     * get message id
     * @return id
     */
    public MsgId getMsgId() {
        return id;
    }
}
