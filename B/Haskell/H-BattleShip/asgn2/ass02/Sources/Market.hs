module Market where
    
import Types

-- Commission for a regular buy or sell order: 0.11%
regCommission :: Double
regCommission = 0.0011

-- Commission for a short sell order: 0.75%
ssCommission :: Double
ssCommission = 0.0075

-- Annual loan interest rate: 5.5%
loanRate :: Double
loanRate = - 0.055

-- Annual savings interest rate: 3%
cashRate :: Double
cashRate = 0.03

-- One day of order execution
--
-- | executeOrders: Extend short position.
-- >>> order=[(Order "TLS" (-3000))]
-- >>> portfolio=(500000.0, [("TLS", (-3000))])
-- >>> histories=[("TLS", [8.0])]
-- >>> output=executeOrders portfolio histories order
-- >>> (round $ fst output, snd output)
-- (523862,[("TLS",-6000)])
--
-- | executeOrders: Buy out short position.
-- >>> order=[(Order "TLS" (3000))]
-- >>> portfolio=(500000.0, [("TLS", (-3000))])
-- >>> histories=[("TLS", [8.0])]
-- >>> output=executeOrders portfolio histories order
-- >>> (round $ fst output, snd output)
-- (476012,[])
--
-- | executeOrders: Short more than holding.
-- >>> order=[(Order "TLS" (-5000))]
-- >>> portfolio=(500000.0, [("TLS", (3000))])
-- >>> histories=[("TLS", [8.0])]
-- >>> output=executeOrders portfolio histories order
-- >>> (round $ fst output, snd output)
-- (539897,[("TLS",-2000)])
--
-- | executeOrders: Regular Buy.
-- >>> order=[(Order "TLS" (5000))]
-- >>> portfolio=(500000.0, [("TLS", (3000))])
-- >>> histories=[("TLS", [8.0])]
-- >>> output=executeOrders portfolio histories order
-- >>> (round $ fst output, snd output)
-- (459993,[("TLS",8000)])
--
-- | executeOrders: Regular Sell.
-- >>> order=[(Order "TLS" (-5000))]
-- >>> portfolio=(500000.0, [("TLS", (10000))])
-- >>> histories=[("TLS", [8.0])]
-- >>> output=executeOrders portfolio histories order
-- >>> (round $ fst output, snd output)
-- (540000,[("TLS",5000)])
--
-- | executeOrders: Buy more than can afford.
-- >>> order=[(Order "TLS" (10000))]
-- >>> portfolio=(500.0, [("TLS", (10))])
-- >>> histories=[("TLS", [8.0])]
-- >>> output=executeOrders portfolio histories order
-- >>> (round $ fst output, snd output)
-- (500,[("TLS",10)])
--
-- | executeOrders: Buy into negative.
-- >>> order=[(Order "TLS" (125))]
-- >>> portfolio=(500.0, [("TLS", (100))])
-- >>> histories=[("TLS", [8.0])]
-- >>> output=executeOrders portfolio histories order
-- >>> (round $ fst output, snd output)
-- (-501,[("TLS",225)])

executeOrders :: Portfolio -> [StockHistory] -> [Order] -> Portfolio
executeOrders p@(cash, holdings) histories orders = case orders of
    []                    
        | cash < 0       -> (cash + interest loanRate cash, holdings)
        | otherwise      -> (cash + interest cashRate cash, holdings)
    Order s q : xs
        | s == "XJO"
          || invalidShort
          || invalidLong -> skipOrder
        | isShortingReg  -> executeOrders (cash - cost + commission ssCommission cost, updatedHoldings) histories xs
        | isShortingHeld -> executeOrders p histories $ regularSellOrder : shortSellOrder : xs
        | isSelling      -> executeOrders (cash - cost + commission regCommission cost, updatedHoldings) histories xs
        | otherwise      -> executeOrders (cash - cost - commission regCommission cost, updatedHoldings) histories xs
        where
            isShortingHeld   = isSelling && abs q > quantityHeld
            isSelling        = q < 0
            isShortingReg    = isSelling && quantityHeld <= 0
            invalidShort     = isSelling
                                && abs (cost - commission ssCommission cost) > currentWealth
            invalidLong      = not isSelling 
                                && (cost + commission regCommission cost) > currentWealth
            skipOrder        = executeOrders p histories xs
            quantityHeld
                | s `notElem` map fst holdings = 0
                | otherwise = head $ map snd $ filter (\a -> fst a == s) holdings
            cost             = fromIntegral q * price
            regularSellOrder = Order s (- quantityHeld)
            shortSellOrder   = Order s (quantityHeld + q)
            price            = getStockPrice s histories
            updatedHoldings  = updateHoldings (s, q) holdings
            currentWealth    = calculateWealth p histories
             
updateHoldings :: Holding -> Holdings -> Holdings
updateHoldings h hs = case hs of
    [] -> [h]
    y : ys
        | fst y == fst h 
            && snd y == - snd h -> ys
        | fst y == fst h        -> (fst y, snd y + snd h) : ys
        | otherwise             -> y : updateHoldings h ys

commission :: Double -> Double -> Cash
commission = (*)

interest :: Double -> Cash -> Cash
interest rate = (((1 + rate) ** (1 / 365) - 1) *)

getStockPrice :: Stock -> [StockHistory] -> Price
getStockPrice s histories = head $ snd getStock
    where
        getStock = head $ filter (\x -> fst x == s) histories

calculateWealth :: Portfolio -> [StockHistory] -> Cash
calculateWealth (cash, stocks) histories
        = cash + sum (map currentValue stocks)
    where
        currentValue :: (Stock, Quantity) -> Cash
        currentValue (ticker, quantity) 
            = fromIntegral quantity 
              * getStockPrice ticker histories