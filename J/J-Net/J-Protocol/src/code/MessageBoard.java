
/**
 * concurrency messageboard
 */

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class MessageBoard {
    public ConcurrentHashMap<MessageHeader, String> myboard = null;

    public MessageBoard() {
        myboard = new ConcurrentHashMap<MessageHeader, String>();
    }

    public String listMessages() {
        String ret = "";
        for (MessageHeader key : myboard.keySet()) {
            ret += key.toString();
            ret += "##";
        }

        if (ret == "") ret = "NULL";

        return ret;
    }

    public void addMessage(MessageHeader key, String value) {
        if (!myboard.containsKey(key)) {
            myboard.put(key, value);
        }
    }

    /**
     * @param key MessageHeader key
     * @return String null means no found
     */
    public String getMessage(MessageHeader key) {

        if (myboard.containsKey(key))
            return myboard.get(key);

        return null;
    }
}
