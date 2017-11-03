module MID (Graph, Vertex, ssp, asp) where

import Data.Bits
import Data.Array
import Transpose

-- Graphs

type Vertex = Int 
type Graph = Array Vertex [Vertex]

-- Sets

type Set = Integer

empty :: Set
empty = 0

singleton :: Vertex -> Set
singleton v = 1 `shiftL` v

size :: Set -> Int
size = popCount

union :: Set -> Set -> Set
union = (.|.)

unions :: [Set] -> Set
unions = foldr union empty

-- Bounded depth-first search

combine :: Graph -> (Vertex -> Set) -> Vertex -> Set
combine g f v = unions (singleton v : map f (g!v))

dfs :: Graph -> Int -> Vertex -> Set
dfs g 0 v = singleton v
dfs g d v = combine g (dfs g (d-1)) v

-- Memoised iterative deepening

toArray :: [Set] -> Array Vertex Set
toArray ss = listArray (0, length ss - 1) ss

initial :: Graph -> Array Vertex Set
initial g = toArray (map singleton (indices g))

step :: Graph -> Array Vertex Set -> Array Vertex Set
step g l = toArray (map (combine g (l !)) (indices g))

levels :: Graph -> [[Set]]
levels g = map elems (iterate (step g) (initial g))

-- Average shortest path

diffs :: [Int] -> [Int]
diffs xs = takeWhile (/= 0) (zipWith (-) (tail xs) xs)

total :: [Set] -> Int
total = sum . zipWith (*) [1..] . diffs . map size

ssp :: Graph -> Int
ssp = sum . map total . transpose . levels

asp :: Graph -> Double
asp g = fromIntegral (ssp g) / fromIntegral (n*(n-1))
  where n = length (indices g)
