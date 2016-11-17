import java.util.*;

/**
 * Pokemon class
 * Author : coder
 * Date : 2016-11-10
 */
public class Pokemon {
	
	private String name;
	private double combatPower;
	private List<Ability> abilityList = new ArrayList<Ability>();
    private Type pmtype1;
    private Type pmtype2;
	private int HP;

    private int level; /* leveling & evolution system */
    private int experience;
	
	public Pokemon(String name, double combatPower, String abilityName, int HP, Type ptype, Type atype) {

        abilityList.clear();

		this.name=name;
		this.combatPower=combatPower;
		Ability ability=new Ability(abilityName,1+(int)(Math.random()*19),
                atype);
        abilityList.add(ability);
		this.HP=HP;
        this.pmtype1 = ptype;
	}
	
	public String getName(){
		return this.name;
	}
	
	public void setName(String name){
		this.name=name;
	}

	public double getCP(){
		return this.combatPower;
	}
	
	public void setCP(double cp){
		combatPower=cp;
	}

    public Type getType1() { return pmtype1; }
    public Type getType2() { return pmtype2; }
    public void setType1(Type t1) { pmtype1 = t1; }
    public void setType2(Type t2) { pmtype2 = t2; }
	
	public List<Ability> getAbilityList(){
		return this.abilityList;
	}

    public Ability getAbility() {
        return this.abilityList.get(0);
    }
	
	public void addAbility(String abilityName, Type t){
		Ability ability=new Ability(abilityName,1+(int)(Math.random()*19),
                t);
        abilityList.add(ability);

	}

    public Ability findAbility(String abiName) {
        for (int i = 0; i < abilityList.size(); i++) {
            if (abiName.equals(abilityList.get(i).getAbilityName())) {
                return abilityList.get(i);
            }
        }

        /* not found */
        return null;
    }
	
	public int getHP(){
		return this.HP;
	}
	
	public void reduceHP(int hp){
		this.HP-=hp;
	}

    public int getLevel() {
        return level;
    }

    public void addLevel(int l) {
        level += l;
    }

    public int getExperience() {
        return experience;
    }

    public int addExperience(int e) {
        experience += e;
        level = (experience/100); /* every 100 experience is a level */
        return experience;
    }

    public void fightPokemon(Pokemon p) {
        double damage = 0;
        if (this.getType1().getClass() == p.getType1().getClass()) {
            damage = 1.5*combatPower;
        }
        if ((this.getType1() instanceof Fire) && (p.getType1() instanceof
                    Grass)) {
            damage = 2.0*combatPower;
        }
        if ((this.getType1() instanceof Fire) && (p.getType1() instanceof
                    Water)) {
            damage = 0.5*combatPower;
        }
        if ((this.getType1() instanceof Water) && (p.getType1() instanceof
                    Fire)) {
            damage = 2.0*combatPower;
        }
        if ((this.getType1() instanceof Grass) && (p.getType1() instanceof
                    Fire)) {
            damage = 0.5*combatPower;
        }

        p.HP -= (int)damage;
    }
}
