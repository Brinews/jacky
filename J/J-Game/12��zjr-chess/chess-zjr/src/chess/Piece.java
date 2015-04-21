package chess;

import java.awt.Image;
import chess.Resources.PieceType;
import chess.Resources.Player;

public class Piece
{
	public Piece(Player plr, int pieceIdx)
	{
		player = plr;
		pieceIndex = pieceIdx;
		name = Resources.getPieceString(pieceIndex);
		character = Resources.getPieceCharacter(pieceIndex);
		type = Resources.PIECE_INSTANCE_TYPES[pieceIndex];
		if (player == Player.WHITE)
		{
			image = Resources.WHITE_PIECE_IMAGES[Resources.PIECE_INSTANCE_TYPES[pieceIndex].ordinal()];
		}else
		{
			image = Resources.BLACK_PIECE_IMAGES[Resources.PIECE_INSTANCE_TYPES[pieceIndex].ordinal()];
		}
	}
	
	public Player getPlayer()
	{
		return player;
	}
	
	public String getName()
	{
		return name;
	}
	
	public char getCharacter()
	{
		return character;
	}
	
	public PieceType getType()
	{
		return type;
	}
	
	public Image getImage()
	{
		return image;
	}
	
	public int getIndex()
	{
		return pieceIndex;
	}
	
	private Image image;
	private Player player;
	private String name;
	private char character;
	private PieceType type;
	private int pieceIndex;
}