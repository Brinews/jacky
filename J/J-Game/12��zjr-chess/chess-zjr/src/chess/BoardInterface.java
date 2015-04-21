
package chess;

import chess.Resources.Player;

/**
 *
 * @author Simon Stannus
 */
public interface BoardInterface
{
	public static int SIZE = 8;
	
	public boolean isWithinBoard(BoardLocation location);
	public boolean isEmpty(BoardLocation location);
	public boolean isFriendly(BoardLocation location, Player currentPlayer);
	public boolean isEnemy(BoardLocation location, Player currentPlayer);
}
