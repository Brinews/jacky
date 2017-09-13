module Simulate where
    
import Order
import Types
import Market

import Data.List

runSimulation :: [StockHistory] -> Portfolio
runSimulation = simulate initialPortfolio
    where
        initialPortfolio = (500000, [])

simulate :: Portfolio -> [StockHistory] -> Portfolio
simulate p histories = 
    foldl (\cs hist -> executeOrders cs hist $ makeOrders cs hist) 
          p (unfoldHistories histories)

unfoldHistories :: [StockHistory] -> [[StockHistory]]
unfoldHistories histories = transpose $ convert $ unfoldHistoriesAux histories

unfoldHistoriesAux :: [StockHistory] -> [(Stock, [[Price]])]
unfoldHistoriesAux histories = case histories of
    [] -> []
    (stock,prices):hs -> (stock, unfoldPrices prices):unfoldHistoriesAux hs

convert :: [(Stock, [[Price]])] -> [[StockHistory]]
convert sps = case sps of
   [] -> []
   (s,ps):spss -> distribute s ps : convert spss

distribute :: Stock -> [[Price]] -> [StockHistory]
distribute st = map (\pr -> (st,pr))

unfoldPrices :: [Price] -> [[Price]]
unfoldPrices prices = reverse $ foldr (\p up -> case up of
                                                  [] -> [p]:up 
                                                  _ -> (p:head up):up 
                                     ) [] prices
     
