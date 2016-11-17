import java.util.*;

/**
 * Author: coder
 * Date: 2016-11-12
 */

public class Player {
    private String playerName;
    private Pokeball pokeball;
    private Potion potion;

    private ArrayList<Pokemon> pokeList = new ArrayList<Pokemon>();
    public Player(String name) {
        playerName = name;
        pokeList.clear();

        Type atype = new Fire("fire");
        double r = Math.random();
        if (r < 0.3) {
            atype = new Grass("grass");
        } else if (r > 0.6) {
            atype = new Water("water");
        }

        pokeList.add(new Pokemon("ownpoke", Math.random()*15+1.0, "abi", (int)(Math.random()*20), atype, new Water("water")));

        potion = new Potion("potion1", 100);
        pokeball = new Pokeball("ball1", 0.6);
    }

    public String getName() {
        return playerName;
    }

    public void setName(String name) {
        playerName = name;
    }

    public Pokeball getPokeball() {
        return pokeball;
    }

    public void setPokeball(Pokeball pb) {
        pokeball = pb;
    }

    public Potion getPotion() {
        return potion;
    }

    public void setPotion(Potion p) {
        potion = p;
    }

    public boolean capturePokemon(Pokemon p) {
        if (pokeball.getProbability() > Math.random() 
                && pokeList.size() < 4) { /* at most 4 pokemon */
            addPokemon(p);
            return true;
        }
        return false;
    }

    public boolean healPokemon(int k) {
        if (potion.doHeal()) {
            pokeList.get(k).reduceHP(-10);
            return true;
        }
        return false;
    }

    /**
     * @return 0: win, 1: lost, 2: no win or lost
     */
    public int fightPokemon(int k, Pokemon p) {
        Pokemon pk = pokeList.get(k);

        pk.fightPokemon(p);
        System.out.println("Left HP:" + pk.getHP());
        if (p.getHP() <= 0) {
            /* win */
            return 0;
        }

        System.out.println("Enmy's HP:" + pk.getHP());
        p.fightPokemon(pk);
        if (pk.getHP() <= 0) {
            /* lost */
            pokeList.remove(k);
            return 1;
        }

        return 2;
    }

    public int getTotalHealth() {
        int health = 0;
        for (int i = 0; i < pokeList.size(); i++) {
            health += pokeList.get(i).getHP();
        }
        return health;
    }

    public Pokemon getPokemon(String name) {
        for (int i = 0; i < pokeList.size(); i++) {
            if (pokeList.get(i).getName().equals(name)) {
                return pokeList.get(i);
            }
        }
        return null;
    }

    public Pokemon getPokemon(int i) {
        if (i < pokeList.size()) {
            return pokeList.get(i);
        }
        return null;
    }

    public int getPokemonNum() {
        return pokeList.size();
    }

    public void addPokemon(Pokemon p) {
        pokeList.add(p);
    }

    public void removePokemon(String name) {
        int found = -1;
        for (int i = 0; i < pokeList.size(); i++) {
            if (pokeList.get(i).getName().equals(name)) {
                found = i;
                break;
            }
        }

        if (found != -1) {
            pokeList.remove(found);
        }
    }
}
