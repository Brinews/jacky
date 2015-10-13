/*
 * wolf walk around (EAST->SOUTH->WEST->SOUTH)
 */

public class Wolf extends Critter{

	public int steps = 0;

	public int[] dir = {2, 1, 3, 1};

	public char getChar() {
	    return this.getClass().getName().charAt(0);
	}

	public Direction getMove(CritterInfo info) {

		currentDirection = Direction.values()[dir[steps]];

		steps = (++steps)%4;

		return currentDirection;

	}
}
