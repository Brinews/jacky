package polarman;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import polarman.threadpool.ThreadPool;
import polarman.threadpool.ThreadTask;

public class ThreadPoolTest {

    public static void main(String[] args) {
        System.out.println("quit �˳�");
        System.out.println("task A 10 ��������A��ʱ��Ϊ10��");
        System.out.println("size 2 ���õ�ǰ�̳߳ش�СΪ2");
        System.out.println("max 3 �����̳߳�����߳���Ϊ3");
        System.out.println();

        final ThreadPool pool = new ThreadPool(3, 2);
        pool.init();

        Thread cmdThread = new Thread(){
            public void run(){

                BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

                while(true){
                    try {
                        String line = reader.readLine();
                        String words[] = line.split(" ");
                        if(words[0].equalsIgnoreCase("quit")){
                            System.exit(0);
                        }else if(words[0].equalsIgnoreCase("size") && words.length >= 2){
                            try{
                                int size = Integer.parseInt(words[1]);
                                pool.setPoolSize(size);
                            }catch(Exception e){
                            }
                        }else if(words[0].equalsIgnoreCase("max") && words.length >= 2){
                            try{
                                int max = Integer.parseInt(words[1]);
                                pool.setMaxPoolSize(max);
                            }catch(Exception e){
                            }
                        }else if(words[0].equalsIgnoreCase("task") && words.length >= 3){
                            try{
                                int timelen = Integer.parseInt(words[2]);
                                SimpleTask task = new SimpleTask(words[1], timelen * 1000);
                                pool.processTask(task);
                            }catch(Exception e){
                            }
                        }

                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        };

        cmdThread.start();
        /*
           for(int i=0; i<10; i++){
           SimpleTask task = new SimpleTask("Task" + i, (i+10)*1000);
           pool.processTask(task);
           }*/
    }
}

class SimpleTask implements ThreadTask{

    private String taskName;
    private int timeLen;

    public SimpleTask(String taskName, int timeLen){
        this.taskName = taskName;
        this.timeLen = timeLen;
    }

    public void run() {
        System.out.println(Thread.currentThread().getId() +
                ": START TASK " + taskName + "");
        try {
            Thread.sleep(timeLen);
        } catch (InterruptedException e) {
        }

        System.out.println(Thread.currentThread().getId() +
                ": END TASK " + taskName + "");
    }
}

/*

   ʹ�ô��̳߳��൱�򵥣��������д����ʼ���̳߳أ�

   ThreadPool pool = new ThreadPool(3, 2);
   pool.init();

   Ҫ���������ʵ��ThreadTask�ӿڼ��ɣ�����Դ������SimpleTask��������ӿ�ֻ��һ������run()
   ���д��뼴�ɵ��ã�

   ThreadTask task = ... //ʵ��������������
   pool.processTask(task);

*/
