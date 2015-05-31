public class Tester {
    public static void main(String[] args)
    {
        System.out.println("Testing ThreadPool ");
        System.out.println("Creating ThreadPool ");
        ThreadPool pool = ThreadPool.instance();
        pool.setDebug(true);
        class TestRunner implements Runnable {
            public int count = 0;
            public void run() {
                System.out.println("Testrunner sleeping 5 seconds ");
                //�˷���ʹ���߳�˯��5��
                synchronized (this) {
                    try {
                        wait(5000);//�ȴ�5��ʱ��
                    }
                    catch (InterruptedException ioe) {
                    }
                }
                System.out.println("Testrunner leaving  ");
                count++;
            }
        }
        System.out.println("Starting a new thread ");
        TestRunner runner = new TestRunner();
        pool.start(runner, pool.HIGH_PRIORITY);
        System.out.println("count : " + runner.count);
        System.out.println("Thread count : " + pool.getCreatedThreadsCount());
        pool.shutdown();
    }
}

