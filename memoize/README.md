This example discusses a generic approach to add memoize method to any 
dynamic programming (DP) problem. Also shows C++ Template programming 
combined with functional programming approach. This code is referenced 
from Functional programming in C++ book. Memoize_helper method caches
result of the function for a set of arguments. For every recursive 
iteration, if its a hit within cache for given set of arguments then 
return results from cache or else computes user provided function-object, 
stores the result in cache and returns result. 

Important concept is how a Base Memoize helper class is declared and then 
latter specialized for function signature. We use variadic template feature
to capture any type of function signature
