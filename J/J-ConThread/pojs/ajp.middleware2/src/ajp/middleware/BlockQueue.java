/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ajp.middleware;

import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

/**
 *
 * @author coder
 */
public class BlockQueue {

    /**
     * the queue: one for reader and writer
     */
    private Queue<Message> writeQueue = new LinkedBlockingQueue<Message>();

    public BlockQueue() {
    }

    /**
     * productor
     *
     * @param msg yield a message
     * @return int 0
     */
    public synchronized int enqueue(Message msg) {

        writeQueue.add(msg);
        this.notify();

        return 0;
    }

    /**
     * consumer
     *
     * @return msg get a message
     */
    public synchronized Message dequeue() {

        Message msg;
        
        try {
            while (writeQueue.isEmpty()) {
                this.wait();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        if (!writeQueue.isEmpty()) {
            msg = writeQueue.peek();
            writeQueue.remove();

            return msg;
        }

        return null;
    }
}
