/**
 * Potion class
 * @Author : coder
 * @Date: 2016-11-01
 */
public class Potion {
    private String potionName;
    private int healVal;

    public Potion() {
    }

    public Potion(String pn, int hv) {
        potionName = pn;
        healVal = hv;
    }

    public int getHealVal() {
        return healVal;
    }

    public void setHealVal(int hv) {
        healVal = hv;
    }

    public String getPotionName() {
        return potionName;
    }

    public void setPotionName(String s) {
        potionName = s;
    }

    public boolean doHeal() {
        if (healVal > 10) {
            healVal -= 10;
            return true;
        }
        return false;
    }
}
