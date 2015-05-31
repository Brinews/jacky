package polarman.threadpool;

import java.util.Collection;
import java.util.Vector;

/**
 * 接受线程池管理的线程
 * @author ryang
 * 2006-8-8
 */
public class PooledThread extends Thread {

    protected Vector tasks = new Vector();
    protected boolean running = false;
    protected boolean stopped = false;
    protected boolean paused = false;
    protected boolean killed = false;
    private ThreadPool pool;

    public PooledThread(ThreadPool pool){
        this.pool = pool;
    }

    public void putTask(ThreadTask task){
        tasks.add(task);
    }

    public void putTasks(ThreadTask[] tasks){
        for(int i=0; i<tasks.length; i++)
            this.tasks.add(tasks[i]);
    }

    public void putTasks(Collection tasks){
        this.tasks.addAll(tasks);
    }

    protected ThreadTask popTask(){
        if(tasks.size() > 0)
            return (ThreadTask)tasks.remove(0);
        else
            return null;
    }

    public boolean isRunning(){
        return running;
    }

    public void stopTasks(){
        stopped = true;
    }

    public void stopTasksSync(){
        stopTasks();
        while(isRunning()){
            try {
                sleep(5);
            } catch (InterruptedException e) {
            }
        }
    }

    public void pauseTasks(){
        paused = true;
    }

    public void pauseTasksSync(){
        pauseTasks();
        while(isRunning()){
            try {
                sleep(5);
            } catch (InterruptedException e) {
            }
        }
    }

    public void kill(){
        if(!running)
            interrupt();
        else
            killed = true;
    }

    public void killSync(){
        kill();
        while(isAlive()){
            try {
                sleep(5);
            } catch (InterruptedException e) {
            }
        }
    }

    public synchronized void startTasks(){
        running = true;
        this.notify();
    }

    public synchronized void run(){
        try{
            while(true){
                if(!running || tasks.size() == 0){
                    pool.notifyForIdleThread();
                    //System.out.println(Thread.currentThread().getId() + ": 空闲");
                    this.wait();
                }else{
                    ThreadTask task;
                    while((task = popTask()) != null){
                        task.run();
                        if(stopped){
                            stopped = false;
                            if(tasks.size() > 0){
                                tasks.clear();
                                System.out.println(Thread.currentThread().getId() + ": Tasks are stopped");
                                break;
                            }
                        }
                        if(paused){
                            paused = false;
                            if(tasks.size() > 0){
                                System.out.println(Thread.currentThread().getId() + ": Tasks are paused");
                                break;
                            }
                        }
                    }
                    running = false;
                }

                if(killed){
                    killed = false;
                    break;
                }
            }
        }catch(InterruptedException e){
            return;
        }
        //System.out.println(Thread.currentThread().getId() + ": Killed");
    }
}
