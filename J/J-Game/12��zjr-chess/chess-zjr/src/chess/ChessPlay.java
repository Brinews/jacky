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
		currentPlayer = Player.WHITE;
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
		feedback.clear();
		currentPlayer = Player.WHITE;
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
	
	public void handleClick(BoardGrid grid)
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

			if (moveState.canMove()) {
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
					}
					
					capturedPiece = null;
				}
				
				if (Resources.reachCastle(grid.getPiece().getPlayer(), grid.getlocation())) {
					inPlay = false;
					feedback.Hint(currentPlayer + " player win !");
				}
				
				if (currentPlayer == Player.WHITE) {
					nextPlayer = Player.BLACK;
				}
				else {
					nextPlayer = Player.WHITE;
				}

				/*
				 * When user do a place, then the computer's turn, 
				 *   from oldLocation->newLocation
				 */
				
				abs.ABsValue(board, nextPlayer, changeLocation, 3, 10000, -10000);
				board.moveLocation(changeLocation[0], changeLocation[1]);
				
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
	
	private ChessBoard board;
	private ChessBoard blackBoard; //outside pieces
	private ChessBoard whiteBoard; //outside pieces
	private Piece selectPiece = null;
	private Piece capturedPiece = null;
	private boolean inPlay;
	private BoardLocation selectLocation = null;
	private Player currentPlayer;
	private Player nextPlayer;
	private feedBack feedback;

	private BoardLocation[] changeLocation = new BoardLocation[2];
	private AlphaBetaSearch abs;
}
