package chess;

import java.awt.GridLayout;

import javax.swing.JPanel;

import chess.Resources.Player;
import chess.Resources;
import java.util.*;

public class ChessBoard extends JPanel implements BoardInterface
{
	public ChessBoard(MouseListener listener)
	{
		this.setSize(400, 406);
		this.setLayout(new GridLayout(Resources.BOARD_HEIGHT, Resources.BOARD_WIDTH));
		grids = new BoardGrid[Resources.BOARD_HEIGHT][Resources.BOARD_WIDTH];

		for (int i = 0; i < Resources.BOARD_HEIGHT; i ++) {
			for (int j = 0; j < Resources.BOARD_WIDTH; j ++) {
				BoardLocation location = new BoardLocation(i, j);
				grids[i][j] = new BoardGrid(location);
				grids[i][j].setVisible(true);
				grids[i][j].addMouseListener(listener);
				this.add(grids[i][j]);
			}
		}
	}
	
	public ChessBoard()
	{
		this.setSize(400, 58);
		this.setLayout(new GridLayout(2, Resources.BOARD_WIDTH));
		outsideGrids = new BoardGrid[Resources.NUM_PIECES_PLAYER];
		for (int i = 0; i < Resources.NUM_PIECES_PLAYER; i ++) {
			outsideGrids[i] = new BoardGrid(i);
			this.add(outsideGrids[i]);
		}
	}
	
	public void clearBoard()
	{
		if (grids != null) {
			for (int i = 0; i < Resources.BOARD_HEIGHT; i ++) {
				for (int j = 0; j < Resources.BOARD_WIDTH; j ++) {
					grids[i][j].setEmpty();
				}
			}
		}
		
		if (outsideGrids != null) {
			for (int i = 0; i < Resources.NUM_PIECES_PLAYER; i ++) {
				outsideGrids[i].setEmpty();
			}
		}
	}

	public Piece moveLocation(BoardLocation from, BoardLocation to, Player p)
	{
		Piece capturedPiece = null;
		
		BoardLocation mid = new BoardLocation((from.getX()+to.getX())/2, (from.getY()+to.getY())/2);
		
		if (isEnemy(mid, p)) {
			capturedPiece = getPiece(mid);
			grids[mid.getX()][mid.getY()].setEmpty();
		}
		
		if (grids[from.getX()][from.getY()].getPiece() == null) {
			System.out.println("exception here..");
		}
		
		grids[to.getX()][to.getY()].setPiece(grids[from.getX()][from.getY()].getPiece());
		grids[from.getX()][from.getY()].setEmpty();

		flushLocation(from);
		flushLocation(to);
		
		return capturedPiece;
	}
	
	public void setLocationSelected(BoardLocation location)
	{
		grids[location.getX()][location.getY()].setSelected();
	}
	
	public void setLocationUnSelected(BoardLocation location)
	{
		grids[location.getX()][location.getY()].setUnSelected();
	}
	
	public void setLocationEmpty(BoardLocation location)
	{
		grids[location.getX()][location.getY()].setEmpty();
	}
	
	public void setPiece(BoardLocation location, Piece piece)
	{
		if (location.getX() >= 0)
			grids[location.getX()][location.getY()].setPiece(piece);
		else
			outsideGrids[location.getY()].setPiece(piece);
	}
	
	public Piece getPiece(BoardLocation location)
	{
		return grids[location.getX()][location.getY()].getPiece();
	}
	
	public void flushLocation(BoardLocation location)
	{
		grids[location.getX()][location.getY()].repaint();
	}
	
	public boolean isWithinBoard(BoardLocation location)
	{
		if (location.getX() < 0 || location.getX() >= Resources.BOARD_HEIGHT 
				|| location.getY() < 0 || location.getY() >= Resources.BOARD_WIDTH)
			return false;
		
		if (location.getX() + location.getY() <= 2)
			return false;
		
		if (location.getY()-location.getX() >= 5) 
			return false;
		
		if (location.getX()-location.getY() >= 11)
			return false;
		
		if (location.getX() + location.getY() >= 18)
			return false;
		
		return true;
	}
	
	public boolean isEmpty(BoardLocation location)
	{
		return grids[location.getX()][location.getY()].getPiece() == null;
	}
	
	public boolean isFriendly(BoardLocation location, Player currentPlayer)
	{
		Piece piece = grids[location.getX()][location.getY()].getPiece();
		if (piece != null && piece.getPlayer() == currentPlayer)
			return true;
		return false;
	}
	
	public boolean isEnemy(BoardLocation location, Player currentPlayer)
	{
		Piece piece = grids[location.getX()][location.getY()].getPiece();
		if (piece != null && piece.getPlayer() != currentPlayer)
			return true;
		return false;
	}

	public boolean isCastleOccupied(Player py)
	{
		Piece p;

		if (py == Player.WHITE) {
			p = grids[0][3].getPiece();
			if (p != null && p.getPlayer() == Player.WHITE) return true;
			p = grids[0][4].getPiece();
			if (p != null && p.getPlayer() == Player.WHITE) return true;
		}

		if (py == Player.BLACK) {
			p = grids[13][3].getPiece();
			if (p != null && p.getPlayer() == Player.BLACK) return true;
			p = grids[13][4].getPiece();
			if (p != null && p.getPlayer() == Player.BLACK) return true;
		}

		return false;
	}

	public boolean isAllCaptured(Player p)
	{
		int total = 0;

		for (int i = 0; i < Resources.BOARD_HEIGHT; i++)
			for (int j = 0; j < Resources.BOARD_WIDTH; j++) {
				if (grids[i][j].getPiece() != null &&
						grids[i][j].getPiece().getPlayer() == p)
					total++;
			}

		if (total == 0) return true;

		return false;
	}
	
	/**
	 * continue jumping
	 * @param p player
	 * @param from previous location
	 * @return next location
	 */
	public BoardLocation getNextCaptured(Player p, BoardLocation from) {
		BoardLocation next = null;
		
		int eight_direction[][] = {{1,0}, {1,-1}, {0,-1},{-1,-1},
				{-1,0}, {-1, 1}, {0, 1}, {1, 1}};
		
		int i = from.getX();
		int j = from.getY();
		
		// jumping moves
		for (int k = 0; k < eight_direction.length; k++) {
			BoardLocation n1 = new BoardLocation(
					i+eight_direction[k][0],
					j+eight_direction[k][1]);
			BoardLocation n2 = new BoardLocation(
					i+2*eight_direction[k][0],
					j+2*eight_direction[k][1]);

			if (isWithinBoard(n1) && isWithinBoard(n2)) {
				if (isEmpty(n2) && isEnemy(n1, p)) {
					return n2;
				}
			}
		} // 
		
		return next;
	}

	public void getAvailableMoves(List<BoardLocation> fromList,
			List<BoardLocation> toList, Player p) 
	{
		int eight_direction[][] = {{1,0}, {1,-1}, {0,-1},{-1,-1},
				{-1,0}, {-1, 1}, {0, 1}, {1, 1}};
		
		boolean capturedAvaiable = false;
		
		for (int i = 0; i < Resources.BOARD_HEIGHT; i++) {
			for (int j = 0; j < Resources.BOARD_WIDTH; j++) {
				BoardLocation from = new BoardLocation(i, j);
				Piece pie = grids[i][j].getPiece();

				if (isWithinBoard(from) && pie != null 
						&& pie.getPlayer() == p ) {

					// jumping moves
					for (int k = 0; k < eight_direction.length; k++) {
						BoardLocation n1 = new BoardLocation(
								i+eight_direction[k][0],
								j+eight_direction[k][1]);
						BoardLocation n2 = new BoardLocation(
								i+2*eight_direction[k][0],
								j+2*eight_direction[k][1]);

						if (isWithinBoard(n1) && isWithinBoard(n2)) {
							if (isEmpty(n2) && isEnemy(n1, p)) {
								fromList.add(from);
								toList.add(n2);
								capturedAvaiable = true;
							}
						}
					} // end for
				}
			}
		}
		
		if (capturedAvaiable == true)
			return ;

		for (int i = 0; i < Resources.BOARD_HEIGHT; i++) {
			for (int j = 0; j < Resources.BOARD_WIDTH; j++) {
				BoardLocation from = new BoardLocation(i, j);
				Piece pie = grids[i][j].getPiece();

				if (isWithinBoard(from) && pie != null 
						&& pie.getPlayer() == p ) {

					boolean must = false;

					// jumping moves
					for (int k = 0; k < eight_direction.length; k++) {
						BoardLocation n1 = new BoardLocation(
								i+eight_direction[k][0],
								j+eight_direction[k][1]);
						BoardLocation n2 = new BoardLocation(
								i+2*eight_direction[k][0],
								j+2*eight_direction[k][1]);

						if (isWithinBoard(n1) && isWithinBoard(n2)) {
							if (isEmpty(n2) && isEnemy(n1, p)) {
								fromList.add(from);
								toList.add(n2);
								must = true;
							}
						}
					} // end for

					// canter moves and plain moves
					if (must == false) {
						for (int k = 0; k < eight_direction.length; k++) {
							BoardLocation n1 = new BoardLocation(
									i+eight_direction[k][0],
									j+eight_direction[k][1]);
							BoardLocation n2 = new BoardLocation(
									i+2*eight_direction[k][0],
									j+2*eight_direction[k][1]);

							// canter
							if (isWithinBoard(n1) && isWithinBoard(n2)) {
								if (isEmpty(n2) && isFriendly(n1, p)) {
									fromList.add(from);
									toList.add(n2);
								}
								else if (isEmpty(n1)) { // plain move
									fromList.add(from);
									toList.add(n1);
								}
							}
						}
					}// end if
				}// end if
			}
		}
	}

	public ChessBoard getMovedBoard(BoardLocation from, 
										BoardLocation to, Player p) {
		MouseListener ml = null;
		ChessBoard newBoard = new ChessBoard(ml);

		for (int i = 0; i < Resources.BOARD_HEIGHT; i++)
			for (int j = 0; j < Resources.BOARD_WIDTH; j++) {
				if (grids[i][j].getPiece() != null)
				newBoard.setPiece(new BoardLocation(i,j),
						this.getPiece(new BoardLocation(
								i, j)));
			}

		newBoard.moveLocation(from, to, p);

		return newBoard;
	}

	public int statPlayerNum(Player p)
	{
		int num = 0;

		for (int i = 0; i < Resources.BOARD_HEIGHT; i++)
			for (int j = 0; j < Resources.BOARD_WIDTH; j++) {
				if (grids[i][j].getPiece() != null
						&& grids[i][j].getPiece().getPlayer() == p)
					num++;
			}

		return num;
	}

	public int minDist(Player p) {
		int dist = 1000;

		for (int i = 0; i < Resources.BOARD_HEIGHT; i++)
			for (int j = 0; j < Resources.BOARD_WIDTH; j++) {
				if (grids[i][j].getPiece() != null
						&& grids[i][j].getPiece().getPlayer() == p) {
					int d = 0;
					if (p == Player.WHITE) {
						d = i;
					}
					else {
						d = 13 - i;
					}

					if (d < dist)
						dist = d;
				}
			}

		return dist;
	}

	private BoardGrid [][] grids = null;
	private BoardGrid [] outsideGrids = null; 	// for captured pieces
}
