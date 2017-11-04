module BFS (Graph, Vertex, ssp, asp) where

import Data.Bits
import Data.Array
import Data.IntSet as S

-- Graphs

type Graph = Array Vertex [Vertex]
type Graph' = Array Vertex Set

-- In range 0 .. |V|-1
type Vertex = Int 

-- Sets

type Set = S.IntSet

-- Breadth-first search

type BFSState = (Set, Set)

step :: Graph' -> BFSState -> BFSState
step g (seen, fringe) = (seen', fringe')
  where
    seen'   = seen `union` fringe
    fringe' = extend g fringe \\ seen'

extend :: Graph' -> Set -> Set
extend g = unions . fmap (g!) . toList

bfs :: Graph' -> Vertex -> [Set]
bfs g x = fmap snd $ iterate (step g) (empty, singleton x)

levels :: Graph -> [[Set]]
levels g = fmap (bfs g') (indices g')
  where g' = fmap fromList g

-- Average shortest path

total :: [Set] -> Int
total = sum . zipWith (*) [0..] . takeWhile (/= 0) . fmap size

ssp :: Graph -> Int
ssp = sum . fmap total . levels

asp :: Graph -> Double
asp g = fromIntegral (ssp g) / fromIntegral (n*(n-1))
  where n = length (indices g)
