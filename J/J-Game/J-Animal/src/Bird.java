import java.util.*;

public class Bird extends Critter{
	public char getChar(){
	    return this.getClass().getName().charAt(0);
	}

	public Direction getMove(CritterInfo info) {
		return Direction.values()
			       [new Random().nextInt(Direction.values().length-1)];
	}
}
