module Main where

import Data.List.Split
import System.Environment
import System.Directory

import Portfolio
import Simulate
import Types

main :: IO ()
main = do
    file      <- getArgs
    files     <- listDirectory $ head file
    -- Get only the CSVs
    let csvs = filter (\x -> length x == 7 && drop 3 x == ".csv") files
    histories <- mapM (\x -> readnParseFile (head file ++ "/" ++ x)) csvs
    let prices = zipWith convertToStockHistory (map (take 3) csvs) histories
    let limitedRange = limitRange prices
    putStrLn "Simulation Started:"
    putStrLn $ showPortfolio (runSimulation limitedRange) limitedRange
    putStrLn "Simulation Ended."

-- Limit all stocks to have the same number of datum points.
limitRange :: [StockHistory] -> [StockHistory]
limitRange prices = map (\(s, list) -> (s, take minLength list)) prices
    where
        minLength = minimum $ map (length . snd) prices
        
-- Given a file path, read the file.
readnParseFile :: String -> IO [String]
readnParseFile path = do
    content <- readFile path
    return $ lines content
    
-- Given a ticker code, and a list of string representing prices, output StockHistory.
convertToStockHistory :: Stock -> [String] -> StockHistory
convertToStockHistory s content = 
    (s, reverse $ map (\y -> read y :: Price) $ unNull $ map (\x -> x !! 5) $ map (splitOn ",") content)
    where
        -- Replaces "null" in data with last available price.
        unNull :: [String] -> [String]
        unNull list = case list of
            [] -> []
            x : [] -> [x]
            x : y : xs
                | x == "null" -> "0" : unNull (y : xs)
                | y == "null" -> x   : unNull (x : xs)
                | otherwise   -> x   : unNull (y : xs)
