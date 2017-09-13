module Types where

type Portfolio = (Cash, Holdings)

type Holding  = (Stock, Quantity)
type Holdings = [Holding]

type Cash = Double

type Price = Double
type Quantity = Integer

type Stock = String
type StockHistory = (Stock, [Price])

-- Positive quantity indicates long position.
data Order = Order Stock Quantity deriving (Show)