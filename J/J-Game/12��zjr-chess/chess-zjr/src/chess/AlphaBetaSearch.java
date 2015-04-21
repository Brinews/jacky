package chess;

import chess.Resources;
import chess.Resources.Player;
import java.util.*;

public class AlphaBetaSearch {
	public AlphaBetaSearch() {
	}

	public Player getOther(Player p) {
		if (p == Player.WHITE) return Player.BLACK;
		else return Player.WHITE;
	}

	/*
	 * Evaluate function: caculate the weight
	 */
	public int ABsEvaluate(ChessBoard b, Player p) {
		if (b.isCastleOccupied(p)) return MAX;
		if (b.isCastleOccupied(getOther(p))) return MIN;
		if (b.isAllCaptured(p)) return MIN;
		if (b.isAllCaptured(getOther(p))) return MAX;

		int whiteNum = 0, blackNum = 0;
		int whiteDist = 0, blackDist = 0;

		whiteNum = b.statPlayerNum(p);
		blackNum = b.statPlayerNum(getOther(p));

		whiteDist = b.minDist(p);
		blackDist = b.minDist(p);

		return whiteNum - blackNum + whiteDist - blackDist;
	}

	/*
	 * Find all the moves can do
	 */
	public void ABsGetAvailableMoves(ChessBoard b,
			List<BoardLocation> from, List<BoardLocation> to, Player p)
	{
		b.getAvailableMoves(from, to, p);
	}

	/*
	 * Get a new board after do a move 
	 */
	public ChessBoard ABsMovedBoard(ChessBoard b, BoardLocation from,
			BoardLocation to) {
		return b.getMovedBoard(from, to);
	}

	/*
	 * alpha-beta search
	 */
	public int ABsValue(ChessBoard b, Player p, 
			BoardLocation[] changedLocation, int depth, 
			int alpha, int beta) {

		// 1. evaluate the target value
		int value = ABsEvaluate(b, p), bestValue;

		// 2. get the target move
		if (value == MAX || value == MIN || depth == 0) {
			if (p == Player.BLACK) return value; //assume white is user
			else return (-value);
		}

		// 3. get all the possible moves
		//    3.1 jumping must list
		//    3.2 cantering and plain moves
		List<BoardLocation> from = new ArrayList<BoardLocation>();
		List<BoardLocation> to = new ArrayList<BoardLocation>();

		ABsGetAvailableMoves(b, from, to, p);
		bestValue = MIN;

		// 4. do the move
		for (int i = 0; i < from.size() /*&& bestValue < alpha*/; i++) {

			// 4.1 change the board
			ChessBoard nb = ABsMovedBoard(b, from.get(i), to.get(i));

			// 4.2 calulate min value
			int v = ABsValue(nb, getOther(p), 
							changedLocation, depth-1, -bestValue, -alpha);

			// 4.3 alpha-beta optimization
			if (bestValue < -v) {
				bestValue = -v;
				changedLocation[0] = from.get(i);
				changedLocation[1] = to.get(i);
			}

			/*
			if (bestValue > alpha)
				alpha = bestValue;

			if (value >= beta) {
				break;
			}
			*/
		}

		return bestValue;
	}

	private final int MIN = -10000;
	private final int MAX = 10000;
}
