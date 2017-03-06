--
-- Date: 2017-03-03
--

import Data.Maybe
import Data.Char
import Data.List
import Data.Ord

--Define Grid definition
data Grid = Grid [[Int]] Int deriving Show
width = 7
height = 6
depth = 6
winnum = 4
empty = Grid (replicate width $ replicate height (0::Int)) 1

--Define column Move 
type Move = Int

-- Game trees

gametree :: Grid -> [Move]
gametree gs@(Grid state _)
    | isNothing (wins gs) = map fst $ filter (\(a, b) -> head b == 0) $ zip [0..width] state
    | otherwise = []

moves :: Grid -> Maybe Move -> Grid
moves gs@(Grid state movePlayer) Nothing = gs
moves gs@(Grid state movePlayer) (Just whichCol)
    | nonzeros == height = gs
    | not $ elem whichCol $ gametree gs = gs
    | otherwise = Grid (before ++ newCol ++ after) (-movePlayer)
    where
        col = state !! whichCol
        playerNodes = filter (/= 0) col
        nonzeros = length playerNodes
        before = take whichCol state
        after = reverse $ take (width-whichCol-1) $ reverse state
        newCol = [(replicate (height-nonzeros-1) (0::Int)) ++ [movePlayer] ++ playerNodes]

-- Grid utilities

wins :: Grid -> Maybe Int
wins (Grid state _) 
    | horizonMin <= -winnum = Just (-1)
    | verticalMin <= -winnum = Just (-1)
    | nwseMin <= -winnum = Just (-1)
    | neswMin <= -winnum = Just (-1)
    | horizonMax >= winnum = Just 1
    | verticalMax >= winnum = Just 1
    | nwseMax >= winnum = Just 1
    | neswMax >= winnum = Just 1
    | sum (map abs (concat state)) == width*height = Just 0
    | otherwise = Nothing
    where
        minMax xs = (minimum xs, maximum xs)
        (horizonMin, horizonMax) = minMax $ concatMap (map sum) $ map group $ transpose state
        (verticalMin, verticalMax) = minMax $ concatMap (map sum) $ map group state
        (neswMin, neswMax) = minMax $ concatMap (map sum) $ map group $ (transpose . diagWins) state
        (nwseMin, nwseMax) = minMax $ concatMap (map sum) $ map group $ (transpose . diagWins . reverse) state

-- diagonal wins
diagWins :: [[Int]] -> [[Int]]
diagWins xs = diagValid xs 0 $ (length $ head xs)-1

diagValid :: [[Int]] -> Int -> Int -> [[Int]]
diagValid [] _ _ = []
diagValid (x:xs) before after = ((replicate before (0::Int)) ++ x ++ (replicate after (0::Int))) : diagValid xs (before+1) (after-1)

-- Minimax

eval :: Grid -> Int
eval gs@(Grid state _)
    | isNothing win = sum $ zipWith (*) bit (map sum state)
    | otherwise = (div (maxBound::Int) 2) * (fromJust win)
    where
        win = wins gs
        bup = [1..(div (width+1) 2)]
        bdown = reverse [1..(div width 2)]
        bit = bup ++ bdown

type MinMaxData = (Int, Int, Int, Move, Bool)

minimax :: Grid -> Int -> Maybe Move
minimax gs depth 
    | 0 == length pms = Nothing
    | depth <= 0 = Just (head pms)
    | isNothing $ wins gs = Just move
    | otherwise = Nothing
    where
        pms = gametree gs
        bestMove = if (depth <= 0) 
                    then (head pms) 
                    else (fromJust $ minimax gs (depth-1))
        moves = bestMove:(delete bestMove pms)
        valueOfFoldue = (depth, div (minBound::Int) 4, div (maxBound::Int) 4, head moves, False)
        dstFoldValue = zip moves $ replicate (length moves) gs
        (_, _, _, move, _) = foldl minmaxUpdate valueOfFoldue dstFoldValue

minmaxUpdate :: MinMaxData -> (Move, Grid) -> MinMaxData
minmaxUpdate foldval@(_, _, _, _, True) _ = foldval
minmaxUpdate foldval@(depth, alpha, beta, bestMove, False) (move, gs)
    | score >= beta = (depth, alpha, beta, move, True)
    | score > alpha = (depth, score, beta, move, False)
    | otherwise = foldval
    where
        score = -(minimaxHelper (moves gs (Just move)) (-beta) (-alpha) (depth-1))


minimaxHelper :: Grid -> Int -> Int -> Int -> Int
minimaxHelper gs@(Grid _ player) _ _ 0 = player * (eval gs)
minimaxHelper gs alpha beta depth = val
    where
        pms = gametree gs
        valueOfFoldue = (depth, alpha, beta, alpha, False)
        dstFoldValue = zip pms $ replicate (length pms) gs
        (_, _, _, val, _) = foldl minmaxHelperUpdate valueOfFoldue dstFoldValue

type MinMaxHelper = (Int, Int, Int, Int, Bool)

minmaxHelperUpdate :: MinMaxHelper -> (Move, Grid) -> MinMaxHelper
minmaxHelperUpdate foldval@(_, _, _, _, True) _ = foldval
minmaxHelperUpdate foldval@(depth, alpha, beta, returnVal, False) (move, gs)
    | score >= beta = (depth, alpha, beta, score, True)
    | score > alpha = (depth, score, beta, score, False)
    | otherwise = foldval
    where
        score = -(minimaxHelper (moves gs (Just move)) (-beta) (-alpha) (depth-1))

-- print grid board

putGrid :: Grid -> String
putGrid (Grid state _) = (concat (intersperse ['\n'] (map ((intersperse ' ') . (map f)) (transpose state))))++"\n\n"
    where
        f a
            | a == 0 = 'B'
            | a == 1 = 'X'
            | otherwise = 'O'

-- Human vs computer
main :: IO ()
main = do
        putStrLn (putGrid empty)
        play empty
        putStrLn "Game Over"

prompt :: String
prompt = "Human's turn, enter your move number (0-6): "

-- Reading a natural number

getNat :: String -> IO Int
getNat prompt = do putStr prompt
                   xs <- getLine
                   if xs /= [] && all isDigit xs then
                      --return (read xs)
                      if (read xs) >= 0 && (read xs) < width then
                        return (read xs)
                      else
                        do putStrLn "ERROR: Number should between 0 and 6"
                           getNat prompt
                   else
                      do putStrLn "ERROR: Invalid number"
                         getNat prompt


play :: Grid -> IO ()
play gs@(Grid _ 1) = do
                            move <- getNat prompt 
                            let nextState = moves gs $ Just move
                            putStrLn $ putGrid nextState
                            if isNothing $ wins nextState
                            then play nextState
                            else return ()

play gs@(Grid _ (-1)) = do
                                putStrLn $ putGrid nextState
                                if isNothing $ wins nextState
                                then play nextState
                                else return ()
                                where
                                    move = minimax gs depth
                                    nextState = moves gs move
