module Portfolio where

import Types
import Market

import Data.List

showPortfolio :: Portfolio -> [StockHistory] -> String
showPortfolio p@(cash, stocks) prices = 
        replicate 25 '-' ++ "\n" ++ 
        "  Cash:       $" ++ show (roundPrices cash) ++ "\n" ++
        "  Stocks: \n" ++ intercalate "\n" (map ("     " ++ ) $ map showHolding stocks) ++ "\n" ++
        "  Wealth:      $" ++ show (roundPrices $ calculateWealth p prices) ++
        "\n" ++ replicate 25 '-'
        where
            showHolding :: (Stock, Quantity) -> String
            showHolding (s, q) = show s ++ replicate 5 ' ' ++ show q
            
roundPrices :: Price -> Price
roundPrices p = (fromIntegral $ round (p * 100)) / 100