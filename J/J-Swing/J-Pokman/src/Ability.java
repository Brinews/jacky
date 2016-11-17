/**
 * Ability class
 * Author: coder
 */
public class Ability {
	private String abilityName;
	private int abilityStrength;
    private Type type;
	
	public Ability(String name, int strength, Type t){
		this.abilityName=name;
		this.abilityStrength=strength;
        type = t;
	}
	
	public void setAbilityStrength(int strength){
		this.abilityStrength=strength;
	}
	
	public int getAbilityStrength(){
		return this.abilityStrength;
	}
	
	public void setAbilityName(String name){
		this.abilityName=name;
	}
	
	public String getAbilityName(){
		return this.abilityName;
	}

    public Type getType() {
        return type;
    }
    
    public void setType(Type t) {
        type = t;
    }
}
