
package chess;

import java.awt.Image;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

/**
 * Resources is composed entirely of static utility methods and fields as well
 * as enumerations, all of which are intended to be able to be used externally.
 * The Resources class interacts with the Direction class, as well as other
 * classes exposed through interfaces. It is mainly used to determine the
 * validity of moves depending on the piece and locations involved, but also
 * holds static data related to the ordering and representation of the different
 * pieces.
 * 
 * @author Simon Stannus
 */
public class Resources
{
    /****************/
    /* Final Fields */
    /**************************************************************************/
    
    /** The number of pieces per player on the board */
	public final static int NUM_PIECES_PER_PLAYER = 6;
	public final static int NUM_PIECES_PLAYER = 8;
	
	public static int WHITE_CAPTURES = 0;
	public static int BLACK_CAPTURES = 0;
    
    /** The size (width and height) of the board */
	public final static int BOARD_HEIGHT = 14;
	public final static int BOARD_WIDTH = 8;
    
    /** The index of the King piece in the PIECE_INSTANCE_TYPES array */
	public final static int KING_INDEX = 0;
    
    /** The images of the White chess pieces */
	public static Image[] WHITE_PIECE_IMAGES;
    
    /** The images of the Black chess pieces */
	public static Image[] BLACK_PIECE_IMAGES;
    
	/** The names of the pieces in English, in order according to PieceType */
 	//public final static String[] PIECE_NAMES = {"King", "Queen", "Rook", "Bishop", "Knight", "Pawn"};
 	public final static String[] PIECE_NAMES = {"Pawn", "Pawn", "Pawn", "Pawn", "Pawn", "Pawn"};
    
    /** The standard characters for representing the pieces in English, in order
     * according to PieceType */
	//public final static char[] PIECE_CHARACTERS = {'K', 'Q', 'R', 'B', 'N', 'P'};
	public final static char[] PIECE_CHARACTERS = {'P', 'P', 'P', 'P', 'P', 'P'};
    
    /** The types of each of the (6) pieces per player */
	public final static PieceType[] PIECE_INSTANCE_TYPES = new PieceType[]	
	{
		PieceType.PAWN,PieceType.PAWN,PieceType.PAWN,
		PieceType.PAWN,PieceType.PAWN,PieceType.PAWN
		/*
		PieceType.BISHOP, PieceType.BISHOP,
		PieceType.KING, PieceType.QUEEN, PieceType.ROOK, PieceType.ROOK, 
		, PieceType.KNIGHT, PieceType.KNIGHT,
		PieceType.PAWN, PieceType.PAWN, PieceType.PAWN, PieceType.PAWN, 
		PieceType.PAWN, PieceType.PAWN, PieceType.PAWN, PieceType.PAWN
		*/
	};
    /**************************************************************************/
	
    
    /****************/
    /* Enumerations */
    /**************************************************************************/
    
    /** The (two) possible players */
	public enum Player {
        /** The player controlling the white pieces */	WHITE,
        /** The player controlling the black pieces */	BLACK;
	}
    
    /** The different types (not instances) of pieces */
	public enum PieceType {
        /** The king piece */	KING,
        /** The queen piece */	QUEEN,
        /** The rook piece */	ROOK,
        /** The bishop piece */	BISHOP,
        /** The knight piece */	KNIGHT,
        /** The pawn piece */	PAWN;
	}
    
    /** The different possible classifications of a move by a given piece to a
     * given destination location */
	public enum MoveState
	{
		/** The destination is the same as the origin */	SELF,
		/** The move can be made */							MOVABLE,
		/** The move will capture an enemy piece */			CAPTURABLE,
		/** The move cannot be made */						NOT_MOVABLE,
		/** The destination is off the board */				OFF_BOARD,
		/** A MoveState does not apply in this context */	NOT_APPLICABLE;
		
        /**
         * Classifies whether or not this MoveState value represents a type of
         * successful move
         * @return a boolean representing the successfulness of the move
         */
		public boolean canMove()
		{
			if (this == MOVABLE || this == CAPTURABLE)
			{
				return true;
			}
			return false;
		}
	}
    /**************************************************************************/
	
    
    /**********/
    /* Fields */
    /**************************************************************************/
    
    /** Whether or not trace statements are displayed to the standard output */
	public static boolean tracing = true;
    /**************************************************************************/
    
    
    /***********/
    /* Methods */
    /**************************************************************************/
    
    /**
     * The constructor for Resources is not intended to be called, because
     * Resources entirely consists of static fields and methods; it has no
     * instance data.
     */
    private Resources()
    {
    }
    
    /**
     * Loads the chess piece images into WHITE_PIECE_IMAGES and
     * BLACK_PIECE_IMAGES
     */
	public static void loadImages()
	{
		Resources.trace("ENTERING Resources.loadImages()...");
        
		WHITE_PIECE_IMAGES = new Image[PIECE_NAMES.length];
		BLACK_PIECE_IMAGES = new Image[PIECE_NAMES.length];
		
        //For each type of piece...
		for (int i = 0; i < PIECE_NAMES.length; i++)
		{
            // ...construct the appropriate file name..
			String filePieceName = PIECE_NAMES[i].toLowerCase();
			File whiteImageFile = new File("images" + File.separator + "w_" + filePieceName + ".png");
			File blackImageFile = new File("images" + File.separator + "b_" + filePieceName + ".png");
            
            // ...and try to load the corresponding image file.
			try
			{
				WHITE_PIECE_IMAGES[i] = ImageIO.read(whiteImageFile);
				BLACK_PIECE_IMAGES[i] = ImageIO.read(blackImageFile);
			}
			catch (IOException e)
			{
				e.printStackTrace();
			}
		}
		Resources.trace("...EXITING Resources.loadImages()");
	}
	
    
    /**
     * Returns the character that represents the type of piece at the given
     * index pieceIndex in a rack of pieces
     * 
     * @param pieceInstanceIndex an integer representing which of the 16 pieces
     * is being referred to
     * @return a char representing the type of piece at the given rack index
     */
	public static char getPieceCharacter(int pieceInstanceIndex)
	{
		return PIECE_CHARACTERS[PIECE_INSTANCE_TYPES[pieceInstanceIndex].ordinal()];
	}
	
    
    /**
     * Returns the String that represents the type of piece at the given index
     * in a rack of pieces.
     * 
     * @param pieceInstanceIndex an integer representing which of the 16 pieces
     * is being referred to
     * @return a String that represents the type of piece at the given index in
     * a rack of pieces.
     */
	public static String getPieceString(int pieceInstanceIndex)
	{
		return PIECE_NAMES[PIECE_INSTANCE_TYPES[pieceInstanceIndex].ordinal()];
	}
    
    
    /**
     * Calculates whether or not a piece of the given type that belongs to the
     * given player can be moved from one location to another on the given
     * board.
     * 
     * @param type the type of piece being moved
     * @param origin where the piece is moved from
     * @param destination where the piece is moved to
     * @param board the board, which contains references to other pieces in play
     * @param player the player who is making the move
     * @return a boolean represents whether or not the move can be made
     */
	public static boolean canMove(PieceType type, BoardLocation origin, BoardLocation destination, 
			BoardInterface board, Player player)
	{
		return getMove(type, origin, destination, board, player).canMove();
	}
	
    
    /**
     * Returns a MoveState that represents what type of move (or failure to
     * move) would result if a piece of the given type that belongs to
     * the given player was attempted to be moved from one location
     * to another on the given board.
     * 
     * @param type the type of piece being moved
     * @param origin the starting point
     * @param destination the destination
     * @param board the board, which contains references to other pieces in play
     * @param player the current player who is making the move
     * @return a MoveState representing the type of move or failure that would
     * result
     */
	public static MoveState getMove(PieceType type, BoardLocation origin, 
			BoardLocation destination, BoardInterface board, Player player)
	{
		Resources.trace("ENTERING Resources.getMove(" + type + ", " + origin + ", " + destination + ", " + board + ", " + player + ")...");
		if (origin.equals(destination))
		{
            Resources.trace("...EXITING Resources.getMove(...) early with a value of " + MoveState.SELF);
			return MoveState.SELF;
		}
		
        // If either the origin or destination are not on the board...
		if (!board.isWithinBoard(origin) || !board.isWithinBoard(destination))
		{
            Resources.trace("...EXITING Resources.getMove(...) early with a value of " + MoveState.OFF_BOARD);
            // ...return the appropriate MoveState.
			return MoveState.OFF_BOARD;
		}
		
        // If the destination cell is not empty...
		if (!board.isEmpty(destination))
		{
			return MoveState.NOT_MOVABLE;
		}
		
		boolean canMove = false;
		
        // Calculate the MoveState by calling the method appropriate to the type of the piece being moved.
		switch (type) {
			case KING: canMove = isAdjacentTo(origin, destination); break;
			case QUEEN: canMove = (isAlignedWith(origin, destination) || isDiagonalTo(origin, destination)) && hasClearPathTo(origin, destination, board, player); break;
			case ROOK: canMove = isAlignedWith(origin, destination) && hasClearPathTo(origin, destination, board, player); break;
			case BISHOP: canMove = isDiagonalTo(origin, destination) && hasClearPathTo(origin, destination, board, player); break;
			case KNIGHT: canMove = isKnightJumpable(origin, destination); break;
			case PAWN: canMove = isPawnMovable(origin, destination, board, player); break;
		}
		
		int midX = (origin.getX()+destination.getX())/2;
		int midY = (origin.getY()+destination.getY())/2;
		
		MoveState successfulMoveState = board.isEnemy(new BoardLocation(midX, midY), player) 
											? MoveState.CAPTURABLE : MoveState.MOVABLE;
		
        // Convert the MoveState to a boolean representing movability and return it
        MoveState returnMoveState = canMove ? successfulMoveState : MoveState.NOT_MOVABLE;
        Resources.trace("...EXITING Resources.getMove(...) with a value of " + returnMoveState);
		return returnMoveState;
	}
    
	
    /**
     * Calculates whether or not there is a clear path between two points on a
     * board.
     * 
     * @param origin the starting point
     * @param destination the destination
     * @param board the board, which contains references to other pieces in play
     * @param currentPlayer the current player
     * @return a boolean representing whether or not there is a clear path
     */
	public static boolean hasClearPathTo(BoardLocation origin, BoardLocation destination, BoardInterface board, Player currentPlayer)
	{
        Resources.trace("ENTERING Resources.hasClearPathTo(" + origin + ", " + destination + ", " + board + ", " + currentPlayer + ")...");
		if (board.isFriendly(destination, currentPlayer))
		{
            Resources.trace("...EXITING Resources.hasClearPathTo(...) with a value of false (friendly piece at destination)");
			return false;
		}
		Direction direction = new Direction(origin, destination);   // Create a new direction based on the difference between the two locations.
		BoardLocation currentPosition = origin;
        
        // Start moving, one step at a time, in that direction.
		while (board.isWithinBoard(currentPosition))
		{
			currentPosition = direction.getLocationAfterMoveInThisDirection(currentPosition);
            
            // If we have reached the destination without hindrance...
			if (currentPosition.equals(destination))
			{
                Resources.trace("...EXITING Resources.hasClearPathTo(...) with a value of true");
				return true;
			}
            // If there is a piece blocking our path to the destination...
			if (!board.isEmpty(currentPosition))
			{
                Resources.trace("...EXITING Resources.hasClearPathTo(...) with a value of false (directional movement blocked)");
				return false;
			}
		}
        // If we have reached the edge of the board then the move is not valid.
        Resources.trace("...EXITING Resources.hasClearPathTo(...) with a value of false (directional movement ran off the board)");
		return false;
	}
	
    
    /**
     * Calculates whether or not a pawn that belongs to the given player can be
     * moved from one location to another on the given board.
     * 
     * @param origin the starting point
     * @param destination the destination
     * @param board the board, which contains references to other pieces in play
     * @param currentPlayer the current player
     * @return a boolean representing whether or not the pawn can make the move
     */
	public static boolean isPawnMovable(BoardLocation origin, BoardLocation destination, BoardInterface board,
			Player currentPlayer)
	{
        // Calculate some intermediate values
		int differenceX = getDifferenceX(origin, destination);
		int differenceY = getDifferenceY(origin, destination);
		
		// capturing move
		if ((Math.abs(differenceX) == 2 && Math.abs(differenceY) == 2)
			|| (Math.abs(differenceX) == 0 && Math.abs(differenceY) == 2)
			|| (Math.abs(differenceX) == 2 && Math.abs(differenceY) == 0)) {
		
			int midX = (origin.getX() + destination.getX())/2;
			int midY = (origin.getY() + destination.getY())/2;
			
			if (board.isEnemy(new BoardLocation(midX, midY), currentPlayer))
				return true;
		}
		
		// find capture move(which is a must) then return false
		int eight_direction[][] = {{1,0}, {1,-1}, {0,-1},{-1,-1},
				{-1,0}, {-1, 1}, {0, 1}, {1, 1}};
		
		for (int i = 0; i < eight_direction.length; i++) {
			BoardLocation neighber1 = new BoardLocation(origin.getX()+eight_direction[i][0], 
					origin.getY()+eight_direction[i][1]);
			
			BoardLocation neighber2 = new BoardLocation(origin.getX()+2*eight_direction[i][0], 
					origin.getY()+2*eight_direction[i][1]);
			
			if (board.isWithinBoard(neighber1) && board.isWithinBoard(neighber2)) {
				if (board.isEmpty(neighber2) && board.isEnemy(neighber1, currentPlayer)) {
					return false;
				}
			}
		}
		
		// eight direction, one plain move
		if (Math.abs(differenceX) <= 1 && Math.abs(differenceY) <= 1) return true;
		
		// capturing move
		if ((Math.abs(differenceX) == 2 && Math.abs(differenceY) == 2)
			|| (Math.abs(differenceX) == 0 && Math.abs(differenceY) == 2)
			|| (Math.abs(differenceX) == 2 && Math.abs(differenceY) == 0)) {
		
			int midX = (origin.getX() + destination.getX())/2;
			int midY = (origin.getY() + destination.getY())/2;
			
			if (board.isFriendly(new BoardLocation(midX, midY), currentPlayer))
				return true;
		}
		
		return false;
	}
	
	
    /**
     * Calculates whether or not a knight that belongs to the given player can
     * be moved from one location to another on the given board.
     * 
     * @param origin the starting point
     * @param destination the destination
     * @return a boolean representing whether or not the knight can make the
     * move
     */
	public static boolean isKnightJumpable(BoardLocation origin, BoardLocation destination)
	{
		return (getAbsoluteDifferenceX(origin, destination) == 1 && getAbsoluteDifferenceY(origin, destination) == 2) || (getAbsoluteDifferenceX(origin, destination) == 2 && getAbsoluteDifferenceY(origin, destination) == 1);
	}
	
    
    /**
     * Calculates whether or not one point is adjacent to another. This is true
     * if it is within one space horizontally or diagonally of the other.
     * 
     * @param origin the starting point
     * @param destination the destination
     * @return a boolean representing adjacency
     */
	public static boolean isAdjacentTo(BoardLocation origin, BoardLocation destination)
	{
		return (getAbsoluteDifferenceX(origin, destination) <= 1 && getAbsoluteDifferenceY(origin, destination) <= 1 && !origin.equals(destination));
	}
	
    /**
     * Calculates whether or not one point is diagonal to another. This is true
     * if the number of spaces between them vertically is the same as the number
     * of spaces between them horizontally.
     * 
     * @param origin the starting point
     * @param destination the destination
     * @return a boolean representing diagonality
     */
	public static boolean isDiagonalTo(BoardLocation origin, BoardLocation destination)
	{
		return getAbsoluteDifferenceX(origin, destination) == getAbsoluteDifferenceY(origin, destination) && !origin.equals(destination);
	}
	
    /**
     * Calculates whether or not one point aligned with another either
     * vertically or horizontally. This is true if either the vertical or
     * horizontal coordinate values are the same between the two points.
     * 
     * @param origin the starting point
     * @param destination the destination
     * @return a boolean representing whether or not the two points are aligned
     */
	public static boolean isAlignedWith(BoardLocation origin, BoardLocation destination)
	{
		return (getAbsoluteDifferenceX(origin, destination) == 0 || getAbsoluteDifferenceY(origin, destination) == 0) && !origin.equals(destination);
	}
	
    /**
     * Calculates the horizontal difference between two points.
     * 
     * @param origin the starting point
     * @param destination the destination
     * @return an integer representing the horizontal difference
     */
	public static int getDifferenceX(BoardLocation origin, BoardLocation destination)
	{
		return destination.getX() - origin.getX();
	}
	
    /**
     * Calculates the vertical difference between two points.
     * 
     * @param origin the starting point
     * @param destination the destination
     * @return an integer representing the vertical difference
     */
	public static int getDifferenceY(BoardLocation origin, BoardLocation destination)
	{
		return destination.getY() - origin.getY();
	}
	
    /**
     * Calculates the absolute horizontal difference between two points.
     * 
     * @param origin the starting point
     * @param destination the destination
     * @return an integer representing the absolute horizontal difference
     */
	public static int getAbsoluteDifferenceX(BoardLocation origin, BoardLocation destination)
	{
		return Math.abs(destination.getX() - origin.getX());
	}
	
    /**
     * Calculates the absolute vertical difference between two points.
     * 
     * @param origin the starting point
     * @param destination the destination
     * @return an integer representing the absolute vertical difference
     */
	public static int getAbsoluteDifferenceY(BoardLocation origin, BoardLocation destination)
	{
		return Math.abs(destination.getY() - origin.getY());
	}
    
	/**
     * Gets a value representing whether or not tracing is turned on.
     * @return a boolean representing whether or not tracing is turned on
     */
	public static boolean getTracing()
	{
		return tracing;
	}
	
	/**
     * Sets tracing either on or off.
     * @param tracingValue a boolean that determines whether or not tracing is
     * turned on
     */
	public static void setTracing(boolean tracingValue)
	{
		tracing = tracingValue;
	}
	
    /**
     * Displays a message to the standard system output if tracing is currently
     * turned on.
     * @param message a String message to be traced
     */
	public static void trace(String message)
	{
		if (tracing)
		{
			System.out.println(message);
		}
	}
	
    /**
     * Gets the starting location of a given piece.
     * @param piece the piece whose starting location must be returned
     * @return the starting location of the given piece.
     */
	public static BoardLocation getStartingLocation(Player owner, int pieceIndex)
	{
        Resources.trace("ENTERING Resources.hasClearPathTo(" + owner + ", " + pieceIndex + ")...");
		
		int index = (owner==Player.BLACK) ? 0 : 1;
		
		int myXLocation[][] = {
				{4,4,4,4,5,5},
				{8,8,9,9,9,9}
		};
		
		int myYLocation[][] = {
				{2,3,4,5,3,4},
				{3,4,2,3,4,5}
		};
		
		int x = myXLocation[index][pieceIndex];
		int y = myYLocation[index][pieceIndex];
       		
        BoardLocation returnLocation = new BoardLocation(x, y);
        Resources.trace("...EXITING Resources.hasClearPathTo(" + owner + ", " + pieceIndex 
        		+ ") with a value of " + returnLocation);
		return returnLocation;
	}
	
	/*
	 * caslte reached by other player
	 */
	public static boolean reachCastle(Player player, BoardLocation location) 
	{
		int x = location.getX();
		int y = location.getY();
		
		if (player == Player.WHITE) {
			if (x == 0 && (y == 3 || y == 4))
				return true;
		}
		else {
			if (x == 13 && (y == 3 || y == 4))
				return true;
		}
		return false;
	}
}

/**
 * Direction represents a direction, such as one in which a chess piece might
 * move. The main purpose of this class is to model the change in coordinates as
 * a piece moves in a given direction.
 * 
 * @author Simon Stannus
 */
class Direction
{
	private int directionX; // the minimum integral horizontal component of the direction
	private int directionY; // the minimum integral vertical component of the direction

    /**
     * Constructs a new Direction based on the difference between two points.
     * Only the 8 neighbourhood directions can be represented.
     * 
     * @param origin the starting point
     * @param destination the destination
     */
	public Direction(BoardLocation origin, BoardLocation destination)
	{
        // Calculate the horizontal and vertical differences betweeen the two points.
		int differenceX = Resources.getDifferenceX(origin, destination);
		int differenceY = Resources.getDifferenceY(origin, destination);
        
        // Reduce the horizontal and vertical values to just be -1, 0 or 1.
		if (differenceX > 1) {differenceX = 1;}
		if (differenceX < -1) {differenceX = -1;}
		if (differenceY > 1) {differenceY = 1;}
		if (differenceY < -1) {differenceY = -1;}
        
        // Store the horizontal and vertical components.
		directionX = differenceX;
		directionY = differenceY;
	}
    
    /**
     * Creates a new Direction using the given horizontal and vertical
     * components.
     * 
     * @param directionX the horizontal component
     * @param directionY the vertical component
     */
    public Direction(int directionX, int directionY)
    {
        this.directionX = directionX;
        this.directionY = directionY;
    }

    /**
     * Gets the horizontal component of the direction.
     * @return the horizontal component of the direction
     */
	public int getDirectionX()
	{
		return directionX;
	}

    /**
     * Gets the vertical component of the direction.
     * @return the vertical component of the direction
     */
	public int getDirectionY()
	{
		return directionY;
	}

    /**
     * Calculates whether or not this Direction is equal to another. It is
     * considered to be equal if the horizontal and vertical components are
     * identical.
     * 
     * @param otherDirection the other Direction with which to compare this one
     * @return a boolean representing whether or not the two Directions are
     * equal
     */
	public boolean equals(Direction otherDirection)
	{
		return (this.directionX == otherDirection.directionX && this.directionY == otherDirection.directionY);
	}

    /**
     * Gets the location that one step from a given location in the direction
     * represented by this object will result in.
     * 
     * @param location the starting location
     * @return a BoardLocation representing the resulting location
     */
	public BoardLocation getLocationAfterMoveInThisDirection(BoardLocation location)
	{
		return new BoardLocation(location.getX() + directionX, location.getY() + directionY);
	}
	
	
}
