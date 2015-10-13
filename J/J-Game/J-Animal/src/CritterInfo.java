/**
 *  Modified from version by Stuart Reges (5/22/06)
 *  
 *  The CritterInfo interface defines a set of methods for querying the
 *  state of a critter simulation.
 */

public interface CritterInfo {
	/**
	 *  Gives a critter's current X location in the world.
	 *  
	 *  @return X location in the world
	 */
    public int getX();
	
	/**
	 *  Gives a critter's current Y location in the world.
	 *  
	 *  @return Y location in the world
	 */
    public int getY();
	
	/**
	 *  Gets the critter in the neighboring position
	 *  character.
	 *  
	 *  @param direction to look
	 *  @return null if there is no critter next to it, else the critter
	 */
    public Critter getNeighbor(Direction direction);
	
	/**
	 *  Gives the height of the critter's world.
	 *  
	 *  @return height of the critter's world
	 */
    public int getHeight();
	
	/**
	 *  Gives the width of the critter's world.
	 *  
	 *  @return width of the critter's world
	 */
    public int getWidth();
}
