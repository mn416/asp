module BFS2 (Graph, Vertex, ssp) where

import Data.Bits
import Data.Array
import qualified Data.IntSet as S

-- Graphs

type Vertex = Int 
type Graph = Array Vertex [Vertex]

-- Convert list of bools to an array of bools for fast indexing
toArray :: [Bool] -> Array Vertex Bool
toArray ss = listArray (0, length ss - 1) ss

-- Breadth-first search
-- For each level, return a bitmap of visited vertices
bfs :: Graph -> Vertex -> [Array Vertex Bool]
bfs g v = iterate step initial
  where
    initial = toArray [v == w | w <- indices g]
    step rs = toArray [any (rs!) (w:g!w) | w <- indices g]

-- Count number of trues
count :: Array Vertex Bool -> Int
count = length . filter id . elems

-- Sum of all shortest paths
diffs :: [Int] -> [Int]
diffs xs = takeWhile (/= 0) (zipWith (-) (tail xs) xs)

total :: Graph -> Vertex -> Int
total g = sum . zipWith (*) [1..] . diffs . map count . bfs g

ssp :: Graph -> Int
ssp g = sum [total g v | v <- indices g]
