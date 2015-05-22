package chess;

import chess.Resources;
import chess.Resources.Player;

public class ChessPlay
{
	public ChessPlay(ChessBoard brd, ChessBoard blkbrd, ChessBoard whtbrd, feedBack fdbk)
	{
		board = brd;
		blackBoard = blkbrd;
		whiteBoard = whtbrd;
		selectPiece = null;
		selectLocation = null;
		capturedPiece = null;
		feedback = fdbk;
		inPlay = false;
		start = 0;
		currentPlayer = Player.WHITE;
		computerPlayer = Player.BLACK;
		Resources.loadImages();

		/* init search stragty */
		abs = new AlphaBetaSearch();
	}
	
	public void initialise()
	{
		board.clearBoard();
		blackBoard.clearBoard();
		whiteBoard.clearBoard();
		selectPiece = null;
		selectLocation = null;
		capturedPiece = null;
		inPlay = true;
		start = 0;
		feedback.clear();
		currentPlayer = Player.WHITE;
		computerPlayer = Player.BLACK;
		feedback.showTurn(currentPlayer);
		
		Resources.BLACK_CAPTURES = Resources.WHITE_CAPTURES = 0;

		
		Player player = Player.WHITE;
		for (int pieceIndex = 0; pieceIndex < Resources.NUM_PIECES_PER_PLAYER; pieceIndex ++)
		{
			BoardLocation location = Resources.getStartingLocation(player, pieceIndex);
			board.setPiece(location, new Piece(player, pieceIndex));
		}
		
		player = Player.BLACK;
		for (int pieceIndex = 0; pieceIndex < Resources.NUM_PIECES_PER_PLAYER; pieceIndex ++)
		{
			BoardLocation location = Resources.getStartingLocation(player, pieceIndex);
			board.setPiece(location, new Piece(player, pieceIndex));
		}
	}
	
	public void restart()
	{
		feedback.Hint("Start a new game.");
		this.initialise();
		board.repaint();
	}
	
	public BoardLocation computerMove(Player nextPlayer) {
		
		start = 1;
		
		/*
		 * When user do a place, then the computer's turn, 
		 *   from oldLocation->newLocation
		 */
		feedback.showTurn(nextPlayer);
		
		//Thread.sleep(1000);
		
		abs.reInit(computerPlayer, 7);
		abs.ABsValue(board, nextPlayer, changeLocation, 7, 10000, -10000);
		
		feedback.Hint("\nMax Depth:" + abs.getDepths());
		feedback.Hint("Total Nodes Generated:" + abs.getTotalSteps());
		feedback.Hint("Number of pruning within MAX-VALUE:" + abs.getAlphaSteps());
		feedback.Hint("Number of pruning within MIN-VALUE:" + abs.getBetaSteps());
		
		feedback.trace(nextPlayer, 
				board.getPiece(changeLocation[0]), 
				changeLocation[0], changeLocation[1]);
		
		capturedPiece = board.moveLocation(changeLocation[0], changeLocation[1], nextPlayer);
		
		// captured piece move out of board
		while (capturedPiece != null) {
			if (capturedPiece.getPlayer() == Player.WHITE) {
				whiteBoard.setPiece(new BoardLocation(-1, capturedPiece.getIndex()), capturedPiece);
				Resources.WHITE_CAPTURES ++;
			}
			else {
				blackBoard.setPiece(new BoardLocation(-1, capturedPiece.getIndex()), capturedPiece);
				Resources.BLACK_CAPTURES ++;
			}
			
			BoardLocation mLoc = new BoardLocation((changeLocation[0].getX()+changeLocation[1].getX())/2,
					(changeLocation[0].getY()+changeLocation[1].getY())/2);
			
			board.setLocationEmpty(mLoc);
			board.flushLocation(mLoc);
			
			if (Resources.WHITE_CAPTURES == Resources.NUM_PIECES_PER_PLAYER
					|| Resources.BLACK_CAPTURES == Resources.NUM_PIECES_PER_PLAYER) {
				inPlay = false;
				feedback.Hint(nextPlayer + " player win !");
			}
			
			BoardLocation next = null;
			if ((next = board.getNextCaptured(nextPlayer, changeLocation[1])) != null) {
				capturedPiece = board.moveLocation(changeLocation[1], next, nextPlayer);
				changeLocation[0] = changeLocation[1];
				changeLocation[1] = next;
			}
			else {
				capturedPiece = null;
			}
		}
		
		return changeLocation[1];
	}
	
	public void handleClick(BoardGrid grid) throws InterruptedException
	{
		Piece piece = grid.getPiece();
		
		if (!inPlay) return;
		
		if (selectPiece == null) {
			if (piece != null && piece.getPlayer() == currentPlayer) {
				selectPiece = grid.getPiece();
				selectLocation = grid.getlocation();
				board.setLocationSelected(selectLocation);
				return;
			}
		} else {
			Resources.MoveState moveState = Resources.getMove(selectPiece.getType(),
													selectLocation, grid.getlocation(), board, currentPlayer);

			if (moveState.canMove() && inPlay) {
				//doMove
				BoardLocation midLocation = null;
				
				if (moveState == Resources.MoveState.CAPTURABLE) {
					BoardLocation dstLocation = grid.getlocation();
					midLocation = new BoardLocation((selectLocation.getX()+dstLocation.getX())/2,
							(selectLocation.getY()+dstLocation.getY())/2);
					
					capturedPiece = board.getPiece(midLocation); //captured pawn
				}
				
				grid.setPiece(selectPiece);
				
				board.setLocationEmpty(selectLocation);
				board.setLocationUnSelected(selectLocation);
				
				board.flushLocation(selectLocation);
				board.flushLocation(grid.getlocation());
				
				feedback.trace(currentPlayer, selectPiece, selectLocation, grid.getlocation());
				
				selectPiece = null;
				selectLocation = null;
				
				// captured piece move out of board
				if (capturedPiece != null) {
					if (capturedPiece.getPlayer() == Player.WHITE) {
						whiteBoard.setPiece(new BoardLocation(-1, capturedPiece.getIndex()), capturedPiece);
						Resources.WHITE_CAPTURES ++;
					}
					else {
						blackBoard.setPiece(new BoardLocation(-1, capturedPiece.getIndex()), capturedPiece);
						Resources.BLACK_CAPTURES ++;
					}
					
					board.setLocationEmpty(midLocation);
					board.flushLocation(midLocation);
					
					if (Resources.WHITE_CAPTURES == Resources.NUM_PIECES_PER_PLAYER
							|| Resources.BLACK_CAPTURES == Resources.NUM_PIECES_PER_PLAYER) {
						inPlay = false;
						feedback.Hint(currentPlayer + " player win !");
						return;
					}
					
					capturedPiece = null;
				}
				
				if (Resources.reachCastle(grid.getPiece().getPlayer(), grid.getlocation())) {
					inPlay = false;
					feedback.Hint(currentPlayer + " player win !");
					return;
				}
				
				Player np;
				
				if (currentPlayer == Player.WHITE) {
					np = Player.BLACK;
				}
				else {
					np = Player.WHITE;
				}
				
				start = 1;

				BoardLocation bc;
				do {
					bc = computerMove(np);
				} while (false);
				
				if (Resources.reachCastle(np, bc)) {
					inPlay = false;
					feedback.Hint(np + " player win !");
					return;
				}
				
				//currentPlayer = nextPlayer;

				if (!inPlay)
					currentPlayer = null;
				
				feedback.showTurn(currentPlayer);
			}
			else {
				feedback.Hint("Can not move " + selectPiece.getName() + " from " + selectLocation.toString() + " to " + grid.getlocation().toString());
			}
			
			if (moveState == Resources.MoveState.SELF) {
				board.setLocationUnSelected(selectLocation);
				selectPiece = null;
				selectLocation = null;
			}
		}
	}
	
	void setPlayer(Player p) {
		currentPlayer = p;
		
		if (p == Player.WHITE) computerPlayer = Player.BLACK;
		else computerPlayer = Player.WHITE;
		
		feedback.showTurn(currentPlayer);
	}
	
	boolean inPlay() {
		if (start != 0) return true;
		else return false;
	}
	
	private ChessBoard board;
	private ChessBoard blackBoard; //outside pieces
	private ChessBoard whiteBoard; //outside pieces
	private Piece selectPiece = null;
	private Piece capturedPiece = null;
	private boolean inPlay;
	private int start = 0;
	private BoardLocation selectLocation = null;
	private Player currentPlayer;
	private Player computerPlayer;
	private feedBack feedback;

	private BoardLocation[] changeLocation = new BoardLocation[2];
	private AlphaBetaSearch abs;
}
