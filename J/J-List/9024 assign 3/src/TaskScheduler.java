import java.io.File;
import java.io.FileNotFoundException;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Task scheduler with EDF algorithm(greedy pattern)
 * Time cost : O(nlogn)
 * @author coder
 */

public class TaskScheduler {
    /**
     * inner class
     * @author coder
     */
    static class TaskItem {
        public String taskName;
        public int taskStart; /* release time */
        public int taskEnd;   /* deadline time */

        /**
         * constructor
         * @param tn name
         * @param ts start
         * @param te end
         */
        public TaskItem(String tn, int ts, int te) {
            taskName = tn;
            taskStart = ts;
            taskEnd = te;
        }
    }

    /**
     * just for debug
     * @param task TaskItem[]
     */
    public static void display(TaskItem[] task) {
        for (int i = 0; i < task.length; i++) {
            System.out.println(task[i].taskName+":"+
                    task[i].taskStart+":"+ task[i].taskEnd);
        }
    }

    /**
     * merge two array a[first...mid] and a[mid...last]
     * Time Cost: O(N)
     * @param a to merge TaskItem[]
     * @param first start position
     * @param mid mid position
     * @param last end position
     * @param temp helper TaskItem[]
     */
    public static void merge(TaskItem[] a, int first, int mid, 
            int last, TaskItem[] temp) {

        int i = first, j = mid + 1;  
        int m = mid,   n = last;  
        int k = 0;  

        while (i <= m && j <= n) {  
            if (a[i].taskEnd < a[j].taskEnd ||
                   (a[i].taskEnd == a[j].taskEnd 
                    && a[i].taskStart <= a[j].taskStart)
               )  
                temp[k++] = a[i++];  
            else  
                temp[k++] = a[j++];  
        }  

        while (i <= m)  
            temp[k++] = a[i++];  

        while (j <= n)  
            temp[k++] = a[j++];  

        for (i = 0; i < k; i++)  
            a[first + i] = temp[i];  
    }

    /**
     * merge sort by recursion mode
     * Time Cost: O(nlogn)
     * @param a to sort TaskItem[]
     * @param first start position
     * @param last end position
     * @param temp helper TaskItem[]
     */
    public static void mergeSort(TaskItem[] a, int first, int last,
            TaskItem[] temp) {
        if (first < last)  
        {  
            int mid = (first + last) / 2;  

            mergeSort(a, first, mid, temp);
            mergeSort(a, mid + 1, last, temp);
            merge(a, first, mid, last, temp);
        } 
    }

    /**
     * static method
     *
     * Time Complexity Analysis:
     *  merge sort cost : O(nlogn)
     *  EDF cost        : O(n)
     *  So time cost    : O(nlogn)
     *
     * @param file1 task set source file
     * @param file2 scheduler result file
     * @param m core number
     */
    public static void scheduler(String file1, String file2, 
            Integer m) throws IOException {
        /* 1. read in task set */
        File file = new File(file1);
        if (!file.exists())
            throw new FileNotFoundException();

        BufferedReader br = new BufferedReader(new FileReader(file));
        StringBuffer sb = new StringBuffer();

        String temp = null;

        temp = br.readLine();

        while (temp != null) {
            temp = temp.trim();
            sb.append(temp + " ");
            temp = br.readLine();
        }

        String taskLine = sb.toString();

        //System.out.println("IN=" + taskLine);

        /*2. split input line */
        String[] taskSets = taskLine.split(" ");

        /*
        System.out.println("len=" + taskSets.length);
        for (int i = 0; i < taskSets.length; i++) {
            System.out.println("["+taskSets[i]+"]");
        }
        */

        if (taskSets.length%3 != 0) {
            throw new IllegalArgumentException();
        }

        TaskItem[] taskArray = new TaskItem[taskSets.length/3];

        for (int i = 0; i < taskSets.length; i+=3) {
            try {
                int ts = Integer.parseInt(taskSets[i+1]);
                int te = Integer.parseInt(taskSets[i+2]);

                if (ts < 0 || te < 0 || ts >= te) 
                    throw new IllegalArgumentException();

                taskArray[i/3] = new TaskItem(taskSets[i], ts, te);

            } catch (Exception e) {
                //e.printStackTrace();
                throw new IllegalArgumentException();
            }
        }

        br.close();

        /* 3. merge sort task array */
        TaskItem[] tempArray = new TaskItem[taskArray.length];
        mergeSort(taskArray, 0, taskArray.length-1, tempArray);

        /* for debug */
        //display(taskArray);

        int lastTime = taskArray[taskArray.length-1].taskEnd;

        /* 4. EDF algorithm */
        String[][] schedTable = new String[lastTime][m];
        for (int i = 0; i < lastTime; i++) {
            for (int j = 0; j < m; j++) {
                schedTable[i][j] = "";
            }
        }

        int coreId = 0, timeIdx = 0;
        for (int i = 0; i < taskArray.length; i++) {
            String name = taskArray[i].taskName;
            int start = taskArray[i].taskStart;
            int end = taskArray[i].taskEnd;

            /* find a feasible core and time */
            boolean ok = false;
            for (int t = start; t < end; t++) {
                for (int c = 0; c < m; c++) {
                    if (schedTable[t][c] == "") {
                        ok = true;
                        schedTable[t][c] = name;
                        break;
                    }
                }
                if (ok == true) break;
            }

            if (ok == false) {
                System.out.println("No feasible schedule exists");
                return;
            }
        }

        /* 5. write schedule result */
        file = new File(file2);
        if (file.exists()) 
            throw new IllegalArgumentException();
        else
            file.createNewFile();

        FileOutputStream out = new FileOutputStream(file, true);

        StringBuffer obuf = new StringBuffer();

        boolean isFirst = true;
        for (int i = 0; i < lastTime; i++) {
            for (int j = 0; j < m; j++) {
                if (schedTable[i][j] != "") {
                    if (isFirst != true)
                        obuf.append(" ");
                    obuf.append(schedTable[i][j] + " " + i);
                    isFirst = false;
                }
            }
        }

        out.write(obuf.toString().getBytes("utf-8"));

        out.close();
    }

    /**
     * main function for test
     * @param args String[]
     */
    public static void main(String[] args) throws Exception{

        TaskScheduler.scheduler("samplefile1.txt", "feasibleschedule1", 4);
        /** There is a feasible schedule on 4 cores */      
        TaskScheduler.scheduler("samplefile1.txt", "feasibleschedule2", 3);
        /** There is no feasible schedule on 3 cores */
        TaskScheduler.scheduler("samplefile2.txt", "feasibleschedule3", 3);
        /** There is a feasible scheduler on 5 cores */ 
        TaskScheduler.scheduler("samplefile2.txt", "feasibleschedule4", 2);
        /** There is no feasible schedule on 4 cores */

        /** The sample task sets are sorted. You can shuffle the tasks and test your program again */  

    }
}
