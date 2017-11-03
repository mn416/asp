--import BFS
import MID
import Data.Set
import Data.Array
import System.CPUTime
import System.IO
import System.Environment
import Control.DeepSeq
import Control.Exception (evaluate)
import Text.Printf

graph :: Int -> [(Vertex, Vertex)] -> Graph
graph n edges = fmap (toList) (accumArray union empty (0, n-1) es)
  where es = concat [[(a, singleton b), (b, singleton a)] | (a, b) <- edges]

pairs :: [a] -> [(a, a)]
pairs [] = []
pairs [x] = []
pairs (x:y:ys) = (x, y) : pairs ys

readGraph :: FilePath -> IO Graph
readGraph path = do
  contents <- readFile path
  let n:e:edges = words contents
  return (graph (read n) (pairs (fmap read edges)))

main :: IO ()
main = do
  [filename] <- getArgs
  g <- readGraph filename
  evaluate (rnf g)
  start <- getCPUTime
  putStr "Sum of shortest paths = " >> print (ssp g)
  finish <- getCPUTime
  let duration = fromInteger (finish - start) / 1000000000000
  printf "Time = %.04fs\n" (duration :: Double)
