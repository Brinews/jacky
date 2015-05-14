/**
 * overlap alignment algorithm
 * reference: global alignment
 */
public class Problem3 {
    private String A = "", B = "";
    private int[][] F;
    private int d;
    private int[] score;

    /**
     * constructor
     * @param a sequence A
     * @param b sequence B
     * @param d_ gap penalty
     * @param sc_ score matrix
     */
    public Problem3(String a, String b, int d_, int[] sc_) {
        A = a;
        B = b;

        d = d_;
        score = new int[sc_.length];
        for (int i = 0; i < sc_.length; i++) {
            score[i] = sc_[i];
        }
    }

    /**
     * get align matrix
     * @return int[][] matrix of dynamic plan
     */
    public int[][] makeAlignMatrix() {
        int alen, blen;
        int match, dele, ins, max;

        alen = A.length();
        blen = B.length();

        F = new int[alen+1][blen+1];

        for (int i = 0; i <= alen; i++) F[i][0] = d*i;
        for (int i = 0; i <= blen; i++) F[0][i] = d*i;

        for (int i = 1; i <= alen; i++) {
            for (int j = 1; j <= blen; j++) {
                match = F[i-1][j-1] + 
                    scoreAB(A.charAt(i-1), B.charAt(j-1));
                dele = F[i-1][j] + d;
                ins = F[i][j-1] + d;

                max = Math.max(match, dele);
                max = Math.max(max, ins);

                F[i][j] = max;
            }
        }

        return F;
    }

    /**
     * restore the alignment string
     * @return String[] two string of aligned
     */
    public String[] backTrackAlign() {
        String[] ret = new String[2];

        ret[0] = ret[1] = "";

        int i = A.length();
        int j = B.length();

        while (i > 0 || j > 0) {
            if (i > 0 && j > 0 
                    && F[i][j] == F[i-1][j-1] 
                    + scoreAB(A.charAt(i-1), B.charAt(j-1))) {
                ret[0] = A.charAt(i-1) + ret[0];
                ret[1] = B.charAt(j-1) + ret[1];
                i = i-1;
                j = j-1;
            } else if (i > 0 && F[i][j] == F[i-1][j] + d) {
                ret[0] = A.charAt(i-1) + ret[0];
                ret[1] = "-" + ret[1];
                i = i-1;
            } else if (j > 0 && F[i][j] == F[i][j-1] + d) {
                ret[0] = "-" + ret[0];
                ret[1] = B.charAt(j-1) + ret[1];
                j = j-1;
            }
        }

        System.out.println("After alignment:");
        System.out.println("A:"+ret[0]);
        System.out.println("B:"+ret[1]);

        return ret;
    }


    /**
     * update gap penalty
     * @param d_ new penalty
     */
    public void updateGapPenalty(int d_) {
        d = d_;
    }

    /**
     * score function
     * @param a char
     * @param b char
     * @return int score
     */
    private int scoreAB(char a, char b) {
        if (a == b) return score[0];
        else return score[1];
    }

    /**
     * after alignment, caculate the score
     * @param ret String[] of two sequences
     */
    public double scoreCaculate(String[] ret) {
        double total = 0;

        for (int i = 0; i < ret[0].length(); i++) {
            if (ret[0].charAt(i) == '-' 
                    || ret[1].charAt(i) == '-') {
                total += d;
            } else {
                total += scoreAB(ret[0].charAt(i), ret[1].charAt(i));
            }
        }

        return total;
    }

    /**
     * do compare test
     * @param d gap penalty
     */
    public void testCase(int d) {
        updateGapPenalty(d);
        makeAlignMatrix();
        String[] ret = backTrackAlign();
        System.out.println("Align Total Score:"+scoreCaculate(ret));
    }

    public static void main(String[] args) {
        /* Q3b */
        String a = "ATCAGTTTGGAGAGGCCAGCTAAATCGCTGATGGTCACTGCCTCGTGCTT";
        String b = "AGGAATGTTCAGCTAAAACGCTGATGGCCACTGACTTGTGATT";
        int d = -3;
        int[] sc = {2, -2};

        Problem3 p3a = new Problem3(a, b, d, sc);

        p3a.testCase(-3);
        p3a.testCase(-1);
        p3a.testCase(-2);
        p3a.testCase(-4);
    }
}
