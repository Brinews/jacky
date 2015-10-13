
public class Turtle extends Critter{
	public int steps = 0;

	public char getChar() {
	    return this.getClass().getName().charAt(0);
	}

	public Direction getMove(CritterInfo info) {
		if (steps < 5) {
			currentDirection = Direction.SOUTH;
		} else {
			currentDirection = Direction.NORTH;
		}

		steps = (++steps)%10;

		return currentDirection;
	}
}
