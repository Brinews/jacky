public class PooledThread extends Thread 
{
    private ThreadPool pool_;  // �����߳���Ҫ֪���Լ����ڵĳ�
    private Runnable target_;   // �̵߳�����
    private boolean shutDown_ = false;
    private boolean idle_ = false;//�����Ƿ����̴߳��ڵȴ�״̬

    private PooledThread() {
        super();
    }

    private PooledThread(Runnable target)
    {
        super(target); //��ʼ������
    }

    private PooledThread(Runnable target, String name) 
    {
        super(target, name);
    }

    public PooledThread(Runnable target, String name, ThreadPool pool)
    {
        super(name);
        pool_ = pool;
        target_ = target;
    }

    private PooledThread(String name) 
    {
        super(name);//��ʼ������
    }

    private PooledThread(ThreadGroup group, Runnable target)
    {
        super(group, target);
    }

    private PooledThread(ThreadGroup group, Runnable target, String name) 
    {
        super(group, target, name);
    }

    private PooledThread(ThreadGroup group, String name) 
    {
        super(group, name);
    }

    public java.lang.Runnable getTarget() 
    {
        return target_;
    }

    public boolean isIdle() 
    {
        return idle_;//���ص�ǰ��״̬
    }

    //�������߳���ͨ���̲߳�֮ͬ������run()�����Ĳ�ͬ��ͨ�����̣߳�
    //����߳�Ӧ��ִ�еĴ������Ȼ�˳����߳̽�����
    //��������߳̽������ջط�����̵߳���Դ���̶߳����������ա�]
    //�����ڳػ��Ĺ������߳�����Ӧ�ñ���ģ������̳߳ؾ�ʧȥ�����塣
    //��Ϊ���Ա�������в��������õĹ������̣߳�����run()������Ӧ�ý�����
    //�������Կ�����ʵ���У�run()����ִ����target����Ĵ����
    //�ͽ�����repool()��Ȼ�����wait()������ʹ�Լ�˯�߶������˳�ѭ����run()��
    //���ʹ�̳߳�ʵ�ֵ�Ҫ�㡣
    public void run() 
    {
        // ���ѭ�����ܽ��������ǳ���Ҫ���߳̽���
        // ÿһ��ѭ������ִ��һ�γ�������������target
        while (!shutDown_) 
        {  
            idle_ = false;
            if (target_ != null) 
            {
                target_.run();  // ����target�еĴ���
            }
            idle_ = true;
            try 
            {
                //�߳�֪ͨ�����½��Լ��Żص�����
                pool_.repool(this);  // 
                //������к�˯�ߣ��ȴ�������ִ���µ�����
                //�������̳߳����߳�����ͨ�̵߳�run()��ͬ�ĵط���
                synchronized (this) 
                {
                    wait();
                }
            }
            catch (InterruptedException ie)
            {
            }
            idle_ = false;
        }
        //ѭ�����ﲻ�ܽ����������߳̽�������Դ��VM�ջأ�
        //���޷����̳߳ص�������
    }


    public synchronized void setTarget(java.lang.Runnable newTarget) 
    {//�����µ�target��������˯���е��߳�
        target_ = newTarget;  // ������
        notifyAll();          // ����˯�ߵ��߳�
    }

    public synchronized void shutDown()
    {
        shutDown_ = true;
        notifyAll();
    }
}

