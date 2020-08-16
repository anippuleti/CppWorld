Introduction
This example discusses a generic approach to inject memoize capability
to any dynamic programming (DP) problem. Also disccusses C++ Template
programming concepts combined with functional programming approach.
Soruce code is referenced from Functional programming in C++ book. 

Details
Memoize_helper method caches result of the function for a set of arguments.
For every recursive iteration, if it's a hit within cache for given set of
arguments then, returns result from cache. Or else, computes user provided
function-object, stores the result in cache before returning the value. 

Important concepts in this source code are 
  1. How a templated base class (Memoize helper) is declared and then latter
     specialized to crate a parital template for a generic function signature.
  2. How variadic template feature is used to capture any type of function signature.
  3. How a make_memoize() helper function is provided in constructing templated
     Memoize_helper object. 
  4. Functional programming techniques along with mutex support to support multiple
     threads to accesses same data without races.
     
References
  1. Functional programming in C++ Ch 6.2, 6.3
