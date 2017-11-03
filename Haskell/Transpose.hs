module Transpose where

-- Lazy transpose, assumes all rows have same length
transpose [] = [[]]
transpose [xs] = [[x] | x <- xs]
transpose (xs : xss) = lzw (:) xs (transpose xss)

-- Left zipWith, assumes lists are same size
lzw op [] ys = []
lzw op (x:xs) ys = (x `op` head ys) : lzw op xs (tail ys)
