module FGL (Graph, Vertex, ssp) where

import Data.Array
import qualified Data.Graph.Inductive as G
import qualified Data.Graph.Inductive.Query.BFS as BFS

-- Graphs
type Vertex = Int 
type Graph = Array Vertex [Vertex]

-- FGL Graphs
type G = G.Gr () ()

toG :: Graph -> G
toG g = G.mkGraph nodes edges
  where
    nodes = [(v, ()) | v <- indices g]
    edges = [(v, w, ()) | v <- indices g, w <- g!v]

-- Breadth-first search
levels :: Graph -> [(Vertex, Int)]
levels g = concat [G.level v g' | v <- indices g]
  where g' = toG g

-- Average shortest path
ssp :: Graph -> Int
ssp g = sum [n | (v, n) <- levels g]
