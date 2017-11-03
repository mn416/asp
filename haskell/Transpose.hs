module Transpose where

-- Lazy transpose
transpose [xs] = [[x] | x <- xs]
transpose (xs : xss) = lzw (:) xs (transpose xss)

-- Lazy zipWith
lzw op [] ys = []
lzw op xs ys = (head xs `op` head ys) : lzw op (tail xs) (tail ys)
