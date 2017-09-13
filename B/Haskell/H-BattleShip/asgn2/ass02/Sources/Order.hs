module Order where

import Types
import Debug.Trace

-- Change this implementation to your own non-trivial trading strategy.
-- Do not modify the type signature of the function.
--
makeOrders :: Portfolio -> [StockHistory] -> [Order]
makeOrders (cash, m) history = 
    case history of
        []   -> []
        (s,p): xs ->  -- trace ("order:" ++ show [Order s (floor ((sellOrBuy p) * cash / 100 * (head p)))] ) 
               (Order s (floor ((sellOrBuy p) * cash / (100 * (head p))))) : makeOrders (cash, m) xs
              where
                sellOrBuy :: [Price] -> Double
                sellOrBuy p = if length p >= 2 && (head p) > (head $ tail p)*1.1 then 1
                              else if length p >= 2 && (head p) < (head $ tail p)*0.9 then -1*(fromInteger $ getStockNum s m)
                              else  0

getStockNum :: Stock -> Holdings -> Quantity
getStockNum s holdings = 
    if getStock == [] then 0
    else snd $ head getStock
    where
        getStock = filter (\x -> fst x == s) holdings


