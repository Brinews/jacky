/**
 *  Modified from version by Stuart Reges (12/03/04)
 */
public class Critter {
	/**
	 *  The current direction the critter is moving.
	 */
	protected Direction currentDirection = Direction.NONE;
	
	/**
	 *  Gets the character representation of the critter
	 *  (for the simulation).
	 *  
	 *  @return character representation of the critter
	 */
    public char getChar() {
		return '\0';
	}
	
	/**
	 *  Gets the critter's next move.
	 *  
	 *  @param info a represenation of the information relevant to the critter
	 *  @return the direction of the critter's next move
	 */
    public Direction getMove(CritterInfo info) {
		return currentDirection;
	}
	
	/**
	 *  Returns the critter's current direction (the last
	 *  direction given by getMove())
	 *  
	 *  @return the direction of the critter's previous move
	 */
	public Direction getCurrentDirection() {
		return currentDirection;
	}
}
