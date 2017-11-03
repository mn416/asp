module DFS (Graph, Vertex, ssp, asp) where

import Data.Bits
import Data.Array
import Transpose

-- Graphs

type Graph = Array Vertex [Vertex]

-- In range 0 .. |V|-1
type Vertex = Int 

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

-- Iterative deepening

table :: [Set] -> Array Vertex Set
table ss = listArray (0, length ss - 1) ss

initial :: Graph -> Array Vertex Set
initial g = table (map singleton (indices g))

step :: Graph -> Array Vertex Set -> Array Vertex Set
step g l = table (map (combine g (l !)) (indices g))

levels :: Graph -> [Array Vertex Set]
levels g = iterate (step g) (initial g)

-- Average shortest path

diffs :: [Int] -> [Int]
diffs xs = takeWhile (/= 0) (zipWith (-) (tail xs) xs)

total :: [Set] -> Int
total = sum . zipWith (*) [1..] . diffs . map size

ssp :: Graph -> Int
ssp = sum . map total . transpose . map elems . levels

asp :: Graph -> Double
asp g = fromIntegral (ssp g) / fromIntegral (n*(n-1))
  where n = length (indices g)
