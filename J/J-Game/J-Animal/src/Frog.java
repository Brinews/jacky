import java.util.*;

public class Frog extends Critter{
	public int recorder = 0;

	public char getChar() {
	    return this.getClass().getName().charAt(0);
	}

	public Direction getMove(CritterInfo info) {
		if (recorder == 0) {
			currentDirection = Direction.values()
			       [new Random().nextInt(Direction.values().length-1)];
		}

		recorder = (++recorder)%3;

		return currentDirection;
	}
}
