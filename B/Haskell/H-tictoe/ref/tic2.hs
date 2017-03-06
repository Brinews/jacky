import Data.Maybe (isNothing, fromJust)
import Data.List (intercalate)
import Char (toUpper)
import System.IO (hFlush, stdout)

data Move        = O | X 
                 deriving (Eq, Show, Enum, Ord)
type Position    = (Char, Int)
data BoardMove   = BoardMove 
                   { bMove :: Maybe Move, bPos :: Position } 
                 deriving (Eq, Show)
type Board       = [BoardMove]
type InvalidMove = String

bsize = 3
coord = (['A'..], [1..])

empty :: Int -> Board
empty size = do
  x <- take size (fst coord)
  y <- take size (snd coord)
  return $ BoardMove Nothing (x,y)

move :: BoardMove -> Board -> Either InvalidMove Board
move (BoardMove _ (c,r)) [] = 
  Left $ "Could not make the move to given position " ++ [c] ++ (show r)
move bm@(BoardMove nmov npos) (x:xs) 
  | findMove x = Right $ bm:xs
  | otherwise  = 
    case move bm xs of
      Right r -> Right $ x:r
      err     -> err
  where findMove (BoardMove m p) = 
          p == npos && isNothing m && nmov /= Nothing

win :: BoardMove -> Board -> Bool
win (BoardMove Nothing _) _ = False
win (BoardMove m (c,r)) b = row || col || diag' cb || diag' (reverse cb)
  where row = length 
              (filter (\(BoardMove m2 (_,r2)) -> 
                        m2 == m && r2 == r) b) == bsize
        col = length
              (filter (\(BoardMove m2 (c2,_)) -> 
                        m2 == m && c2 == c) b) == bsize
        diag' xss = all (\(BoardMove m2 _) -> 
                          m2 == m) $ diag xss
        cb = chop bsize b

draw :: BoardMove -> Board -> Bool
draw bm b = not (any (isNothing . bMove) b)
         && not (win bm b)

printBoard :: Board -> String
printBoard b = intercalate "\n" $
                 map (\row-> [(fst . bPos) (row !! 0)] ++ ")   | " ++ 
                             (intercalate " | " 
                                $ map (\bm-> maybe " " show $ bMove bm) row)
                             ++ " |")
                 (chop bsize b)

chop :: Int -> [a] -> [[a]]
chop n [] =  []
chop n xs =  take n xs : chop n (drop n xs)

diag :: [[a]] -> [a]
diag xss = [xss !! n !! n | n <- [0 .. length xss - 1]]

main = do
  putStrLn "Starting new game..."
  putStrLn "Type 'quit' to exit game"
  let newBoard = empty bsize
      in do (putStrLn . (\s->"\n"++s++"\n") . printBoard) newBoard
            gameloop Nothing newBoard

gameloop prevMove board = do
  let currPlayer = maybe X (\(BoardMove mv _) -> 
                               case mv of
                                 Just X -> O
                                 Just O -> X) prevMove
  putStr $ "Player '" ++ (show currPlayer) ++ "': "
  hFlush stdout
  playerMove <- getLine
  case (playerMove, (map toUpper playerMove) `elem` allCoord) of
    ("quit", _) ->
        putStrLn "Goodbye!"
    (_, False)  -> do
        putStrLn $ "Possible options: " ++ intercalate ", " allCoord
        gameloop prevMove board
    otherwise   -> do
        let pos = (toUpper $ playerMove !! 0, 
                   read [(playerMove !! 1)] :: Int)
            currMove = BoardMove (Just currPlayer) pos
            currBoard = move currMove board
        either putStrLn (putStrLn . (\s->"\n"++s++"\n") . printBoard) currBoard
        case currBoard of
          Right r  -> if win currMove r
                        then do putStrLn $ "Player '"
                                           ++ (show currPlayer) ++"' wins!"
                                main
                        else if draw currMove r
                                then do putStrLn $ "It's a draw!"
                                        main
                                else gameloop (Just currMove) r
          Left err -> gameloop prevMove board
  where allCoord = [[x] ++ show y | x <- take bsize (fst coord), 
                                    y <- take bsize (snd coord)]

