import cern.jet.random.Poisson;
import cern.jet.random.engine.RandomEngine;
import cern.jet.random.engine.MersenneTwister;
import java.util.Random;

/**
 * problem2:
 *  insertions and deletions with poisson random
 */
public class Problem2 {
    /**
     * ancestor:A , childs: B and C
     */
    private String A = "", B = "", C = "";
    private RandomEngine  random;
    private Poisson pois;
    private double L, u, t;

    /**
     * constructor
     * @param L DNA sequence's length
     * @param u mutation rate
     * @param t time
     */
    public Problem2 (double L_, double u_, double t_) {
        int seed = new Random().nextInt();
        random = new MersenneTwister(seed);

        L = L_; u = u_; t = t_;
        pois = new Poisson(L*u*t/10, random);
    }

    /**
     * uniformly get A,C,G,T
     * @return char random the next
     */
    public char uniformRand(char orig)
    {
        double sd;
        char next = ' ';

        double[] seg = {0.0, 0.25, 0.5, 0.75, 1.0};

        do {
            sd = random.raw();
            if (sd > seg[0] && sd <= seg[1]) next = 'A';
            if (sd > seg[1] && sd <= seg[2]) next = 'G';
            if (sd > seg[2] && sd <= seg[3]) next = 'C';
            if (sd > seg[3] && sd <= seg[4]) next = 'T';
        } while (orig == next);

        return next;
    }

    /**
     * get DNA sequence by JC Model
     * @param length DNA sequence's length
     * @param u mutation rate
     * @param t time
     */
    public void modelJC(double length, double u, double t) {
        /* make init */
        A = ""; B = ""; C = "";

        /* get A by mean distribute */
        for (int i = 0; i < length; i++) {
            A += uniformRand(' ');
        }

        B = C = "";

        String B1 = A, C1 = A;

        /* get B and C by mutation */
        for (int i = 0; i < t; i++) {

            B = C = "";

            for (int j = 0; j < length; j++) {
                double p = random.raw();
                if (p >= 0 && p <= 3.0*u/4.0) { /* mutation happened */
                    B += uniformRand(B1.charAt(j));
                    C += uniformRand(C1.charAt(j));
                } else {
                    B += B1.charAt(j);
                    C += C1.charAt(j);
                }
            }

            B1 = B;
            C1 = C;
        }
    }

    /**
     * insert by poisson random
     * @param insNum int
     */
    public void doInsertion(int insNum) {
        // copy
        String A1 = A, B1 = B, C1 = C;


        for (int i = 0; i < insNum; i++) {
            double pos = random.raw();
            int p = (int) (pos * L);

            B = B1.substring(0, p);
            B += uniformRand(' ');
            B += uniformRand(' ');
            B += uniformRand(' ');
            B += B1.substring(p);

            C = C1.substring(0, p);
            C += "---";
            C += C1.substring(p);

            B1 = B;
            C1 = C;
        }

        System.out.println("After Insertion:");
        System.out.println("B:"+B);
        System.out.println("C:"+C);
    }

    /**
     * deletions by poisson random
     * @param delNum int
     */
    public void doDeletion(int delNum) {
        String C1 = "";

        for (int i = 0; i < delNum; i++) {
            double pos = random.raw();
            int p = (int) (pos * L);

            while (C.charAt(p) == '-' && p < C.length()) p++;

            /* do delete */
            int j = 0;
            C1 = C.substring(0, p);
            while (p < C.length() && j++ < 3) {
                C1 += "-";
                p++;
            }

            if (p < C.length()) {
                C1 += C.substring(p);
            }

            C = C1;
        }

        System.out.println("After Deletion:");
        System.out.println("B:"+B);
        System.out.println("C:"+C);

    }

    /**
     * get DNA sequence by JC and insertion/deletion
     */
    public void mutationInsertDelete() {

        double insNum, delNum;

        modelJC(L, u, t);

        do {
            insNum = pois.nextDouble();
            delNum = pois.nextDouble();
        } while (insNum == 0 || delNum == 0);

        doInsertion((int)insNum);
        doDeletion((int)delNum);
    }

    /**
     * main method
     * @param args arguments
     */
    public static void main(String[] args) {
        Problem2 p2c = new Problem2(50, 0.01, 20);
        p2c.mutationInsertDelete();
    }
}
