import cern.jet.random.engine.RandomEngine;
import cern.jet.random.engine.MersenneTwister;
import java.util.Random;

/**
 * Jukes-Cantor model of DNA
 */
public class Problem1 {

    /**
     * A is ancestor, B and C are descendant
     */
    private String A = "", B = "", C = "";
    private RandomEngine random;

    public Problem1() {
        /**
         * colt-random
         */
        int seed = (new Random().nextInt());
        random = new MersenneTwister(seed);
    }

    /**
     * get different sites number
     * @param s1 one string
     * @param s2 the other string
     */
    public double sitesDiffCount(String s1, String s2) {
        double dsCount = 0;

        for (int i = 0; i < s1.length(); i++) {
            if (s1.charAt(i) != s2.charAt(i)) {
                dsCount++;
            }
        }

        return dsCount;
    }

    /**
     * return diff sites num
     * @return num
     */
    public double getDiffSitesNum() {
        return sitesDiffCount(B, C);
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
    public void modelJC(int length, double u, double t) {
        /* make init */
        A = ""; B = ""; C = "";

        /* get A by mean-distribute */
        for (int i = 0; i < length; i++) {
            A += uniformRand(' ');
        }

        B = C = "";

        String B1 = A, C1 = A;

        /* get B and C by mutation */
        for (int i = 0; i < t; i++) {

            B = C = "";

            for (int j = 0; j < length; j++) {
                double r = random.raw();
                if (r >= 0 && r <= 3.0*u/4.0) { /* mutation happened */
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
     * statistics of diff sites
     */
    public void showDiffs() {
        System.out.println("A:"+A);
        System.out.println("B:"+B);
        System.out.println("C:"+C);

        System.out.println("A-B diffs:" + sitesDiffCount(A, B));
        System.out.println("B-C diffs:" + sitesDiffCount(C, B));
    }

    /**
     * estimate Pab by theoretical
     */
    public void estimatePab() {

        /* Pab when a=b */
        double p1 = 0;
        for (int i = 0; i < B.length(); i++) {
            if (B.charAt(i) == C.charAt(i)) p1++;
        }

        System.out.println("Pab(a=b) = " + p1/B.length());

        /* Pab when a!=b */
        double p2 = 0;

        for (int i = 0; i < B.length(); i++) {
            for (int j = 0; j < C.length(); j++) {
                if (i == j) continue;

                if (B.charAt(i) == C.charAt(j)) {
                    p2++;
                }
            }
        }

        System.out.println("Pab(a!=b) = "+ 
                p2*2/(B.length()*(B.length()-1)));
    }

    /**
     * main method
     * @param args arguments
     */
    public static void main(String[] args) {
        Problem1 p1a = new Problem1();
        /* Q1-a */
        p1a.modelJC(50, 0.01, 10);
        p1a.showDiffs();

        /* Q1-b */
        double sum1 = 0.0, sum2 = 0.0;
        int total = 10000;

        /* it will cost a lot of time */
        for (int i = 0; i < total; i++) {
            p1a.modelJC(1000, 0.01, 25);
            double df = p1a.getDiffSitesNum();

            sum1 += df;
            sum2 += df*df;
        }

        System.out.printf("Simulate 10000 pairs Mean:%.2f\n", sum1/1000);
        System.out.printf("Simulate 10000   Variance:%.2f\n", 
                sum2/1000 - sum1*sum1/1000000);

        /* Q1-c */
        p1a.modelJC(10000, 0.03, 10);
        p1a.estimatePab();
    }
}
