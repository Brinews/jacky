package polarman.threadpool;

import java.util.Collection;
import java.util.Iterator;
import java.util.Vector;

/**
 * 线程池
 * @author ryang
 * 2006-8-8
 */
public class ThreadPool {

    protected int maxPoolSize;
    protected int initPoolSize;
    protected Vector threads = new Vector();
    protected boolean initialized = false;
    protected boolean hasIdleThread = false;

    public ThreadPool(int maxPoolSize, int initPoolSize){
        this.maxPoolSize = maxPoolSize;
        this.initPoolSize = initPoolSize;
    }

    public void init(){
        initialized = true;
        for(int i=0; i<initPoolSize; i++){
            PooledThread thread = new PooledThread(this);
            thread.start();
            threads.add(thread);
        }

        //System.out.println("线程池初始化结束，线程数=" + threads.size() + " 最大线程数=" + maxPoolSize);
    }

    public void setMaxPoolSize(int maxPoolSize){
        //System.out.println("重设最大线程数，最大线程数=" + maxPoolSize);
        this.maxPoolSize = maxPoolSize;
        if(maxPoolSize < getPoolSize())
            setPoolSize(maxPoolSize);
    }

    /**
     * 重设当前线程数
     * 若需杀掉某线程，线程不会立刻杀掉，而会等到线程中的事务处理完成
     * 但此方法会立刻从线程池中移除该线程，不会等待事务处理结束
     * @param size
     */
    public void setPoolSize(int size){
        if(!initialized){
            initPoolSize = size;
            return;
        }else if(size > getPoolSize()){
            for(int i=getPoolSize(); i<size && i<maxPoolSize; i++){
                PooledThread thread = new PooledThread(this);
                thread.start();
                threads.add(thread);
            }
        }else if(size < getPoolSize()){
            while(getPoolSize() > size){
                PooledThread th = (PooledThread)threads.remove(0);
                th.kill();
            }
        }

        //System.out.println("重设线程数，线程数=" + threads.size());
    }

    public int getPoolSize(){
        return threads.size();
    }

    protected void notifyForIdleThread(){
        hasIdleThread = true;
    }

    protected boolean waitForIdleThread(){
        hasIdleThread = false;
        while(!hasIdleThread && getPoolSize() >= maxPoolSize){
            try {
                Thread.sleep(5);
            } catch (InterruptedException e) {
                return false;
            }
        }

        return true;
    }

    public synchronized PooledThread getIdleThread(){
        while(true){
            for(Iterator itr=threads.iterator(); itr.hasNext();){
                PooledThread th = (PooledThread)itr.next();
                if(!th.isRunning())
                    return th;
            }

            if(getPoolSize() < maxPoolSize){
                PooledThread thread = new PooledThread(this);
                thread.start();
                threads.add(thread);
                return thread;
            }

            //System.out.println("线程池已满，等待...");
            if(waitForIdleThread() == false)
                return null;
        }
    }

    public void processTask(ThreadTask task){
        PooledThread th = getIdleThread();
        if(th != null){
            th.putTask(task);
            th.startTasks();
        }
    }

    public void processTasksInSingleThread(ThreadTask[] tasks){
        PooledThread th = getIdleThread();
        if(th != null){
            th.putTasks(tasks);
            th.startTasks();
        }
    }

    public void processTasksInSingleThread(Collection tasks){
        PooledThread th = getIdleThread();
        if(th != null){
            th.putTasks(tasks);
            th.startTasks();
        }
    }
}

