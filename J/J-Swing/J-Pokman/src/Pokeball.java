
public class Pokeball {
    private String ballName;
    private double ballProbability;

    public Pokeball(String bn, double bp) {
        ballName = bn;
        ballProbability = bp;
    }

    public String getName() {
        return ballName;
    }

    public void setName(String s) {
        ballName = s;
    }

    public double getProbability() {
        return ballProbability;
    }

    public void setProbability(double p) {
        ballProbability = p;
    }
}
