
public class Mouse extends Critter{

	public int switcher = 0;

	public char getChar() {
	    return this.getClass().getName().charAt(0);
	}

	public Direction getMove(CritterInfo info) {

		currentDirection = Direction.values()[switcher];

		switcher = 3 - switcher; /* WEST/NORTH */

		return currentDirection;
	}
}
