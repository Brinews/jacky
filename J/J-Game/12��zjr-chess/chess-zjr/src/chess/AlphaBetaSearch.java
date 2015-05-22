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
		
		if (p == computerSide && b.isCastleOccupied(p)) {
			return MAX;
		}

		if (p == computerSide && b.isCastleOccupied(getOther(p))) {
			return MIN;
		}

		if (getOther(p) == computerSide && 
				b.isCastleOccupied(getOther(p))) 
			return MIN;

		if (getOther(p) == computerSide &&
				b.isCastleOccupied(p))
			return MAX;

		if (getOther(p) == computerSide && 
				b.isAllCaptured(getOther(p))) return MAX;
		
		if (p == computerSide && 
				b.isAllCaptured(getOther(p))) return MAX;
		
		int aiNum, manNum;
		int aiDist, manDist;

		aiNum = b.statPlayerNum(computerSide);
		manNum = b.statPlayerNum(getOther(computerSide));

		aiDist = b.minDist(computerSide);
		manDist = b.minDist(getOther(computerSide));

		int value;
		if (p == computerSide){
			value =  120*aiNum+180*(13-aiDist);
			value = -value;
		}
		else {
			value =  130*(7 - aiNum) + 80*aiDist;
		}
		
		return value;
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
			BoardLocation to, Player p) {
		return b.getMovedBoard(from, to, p);
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
		if (value == MIN || value == MAX || depth == 0) {
			if (maxDepths < originDepth - depth)
				maxDepths = originDepth - depth;
			
			if (value == MIN) alphaSteps++;
			if (value == MAX) betaSteps++;
			
			return value;
		}

		// 3. get all the possible moves
		//    3.1 jumping must list
		//    3.2 cantering and plain moves
		List<BoardLocation> from = new ArrayList<BoardLocation>();
		List<BoardLocation> to = new ArrayList<BoardLocation>();

		ABsGetAvailableMoves(b, from, to, p);
		bestValue = MIN;

		// 4. do the move
		for (int i = 0; i < from.size(); i++) {
			
			totalSteps ++;

			// 4.1 change the board
			ChessBoard nb = ABsMovedBoard(b, from.get(i), to.get(i), p);

			// 4.2 calulate min value
			int v = -ABsValue(nb, getOther(p), 
							changedLocation, depth-1, -beta, -alpha);
			
			for (int j = 0; j < depth-1; j++)
				System.out.printf("+");
			System.out.printf("[%d]\n",v);

			// 4.3 alpha-beta optimization
			if (bestValue < v) {
				bestValue = v;
				changedLocation[0] = from.get(i);
				changedLocation[1] = to.get(i);
			}
			
			if (bestValue > alpha) {
				alpha = bestValue;
			}
			
			if (bestValue >= beta) {
				if (v < 0)
					betaSteps += (from.size()-i);
				else
					alphaSteps += (from.size()-i);
				
				totalSteps += betaSteps;
				
				break;
			}
		}

		return bestValue;
	}
	
	public int getTotalSteps() {
		return totalSteps;
	}
	
	public int getAlphaSteps() {
		return alphaSteps;
	}
	
	public int getBetaSteps() {
		return betaSteps;
	}
	
	public int getDepths() {
		return maxDepths;
	}
	
	public void reInit(Player p, int od) {
		totalSteps = 0;
		alphaSteps = 0;
		betaSteps = 0;
		maxDepths = 0;
		originDepth = od;
		computerSide = p;
	}

	private final int MIN = -10000;
	private final int MAX = 10000;
	
	public int originDepth = 0;
	public int maxDepths = 0;
	public int totalSteps = 0;
	public int alphaSteps = 0;
	public int betaSteps = 0;
	
	private Player computerSide;
}
