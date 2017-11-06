module BFS (Graph, Vertex, ssp, asp) where

import Data.Bits
import Data.Array
import qualified Data.IntSet as S

-- Graphs

type Vertex = Int 
type Graph = Array Vertex [Vertex]

-- Breadth-first search

type BFSState = (S.IntSet, [Vertex])

step :: Graph -> BFSState -> BFSState
step g (seen, fringe) = foldl visit (seen, []) (concatMap (g!) fringe)
  where
    visit (seen, fringe) v
      | v `S.member` seen = (seen, fringe)
      | otherwise         = (S.insert v seen, v:fringe)

bfs :: Graph -> Vertex -> [[Vertex]]
bfs g x = fmap snd (iterate (step g) (S.singleton x, [x]))

levels :: Graph -> [[[Vertex]]]
levels g = fmap (bfs g) (indices g)

-- Average shortest path

--total :: [Set] -> Int
total = sum . zipWith (*) [0..] . takeWhile (/= 0) . fmap length

ssp :: Graph -> Int
ssp = sum . fmap total . levels

asp :: Graph -> Double
asp g = fromIntegral (ssp g) / fromIntegral (n*(n-1))
  where n = length (indices g)
