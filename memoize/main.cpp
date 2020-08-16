//////////////////////////////////////////////////////////////////////////////
//The MIT License
//
//Copyright (c) 2010-2019 Google, Inc. http://angularjs.org
//
//Permission is hereby granted, free of charge, to any person obtaining
//a copy of this software and associated documentation files
//(the "Software"), to deal in the Software without restriction, including
//without limitation the rights to use, copy, modify, merge, publish,
//distribute, sublicense, and/or sell copies of the Software, and to
//permit persons to whom the Software is furnished to do so, subject to the
//following conditions:
//
//The above copyright notice and this permission notice shall be included
//in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//////////////////////////////////////////////////////////////////////////////

#include "memoize_helper.h"
#include <algorithm>
#include <iostream>
#include <chrono>

namespace Fib {
unsigned fib(unsigned int val) {
  //std::cout << "Calculating " << val << "\n";
  return val == 0 ? 0 :
         val == 1 ? 1 :
         fib(val - 1) + fib(val - 2);
}

struct Fib_wrapper {
  template<typename Function>
  inline unsigned operator()(Function& fib_memo, unsigned val) const
  {
    return val == 0 ? 0 :
           val == 1 ? 1 :
           fib_memo(val - 1) + fib_memo(val - 2);
  }
};

} //end of namesapce

namespace Lev {
using Citr = std::string::const_iterator;
unsigned levenshtein_dist(Citr m_loc, Citr n_loc, Citr m_end, Citr n_end)
{
  auto v = [&m_loc, &n_loc]() {
    return static_cast<unsigned>(*m_loc != *n_loc);
  };

  return m_loc == m_end ? std::distance(n_loc, n_end) :
         n_loc == n_end ? std::distance(m_loc, m_end) :
         std::min({
             levenshtein_dist(std::next(m_loc), n_loc, m_end, n_end)   + 1,
             levenshtein_dist(m_loc, std::next(n_loc), m_end, n_end)   + 1,
             levenshtein_dist(std::next(m_loc), std::next(n_loc), m_end, n_end) + v()
         });
}

}

///Fibonacci sequence calculator with out memoization
void raw_fib(unsigned val)
{
  std::cout << "Fib without  memozie: " << Fib::fib(val) << std::endl;
}

///Fibonacci sequence calculator with memoization using
///Memoize_helper with (passing a lamada)
void mem_lamada_fib(unsigned val)
{
  ///Passing a Lamda to Memoize_helper
  ///Param1: 1st paramter to lamda is Memoize_helper object itself.
  ///Param2: 2nd paramter to lamda is fibonacci value for current invocation
  auto fib_memo = nan::make_memoize<unsigned(unsigned)>([](auto& fib, unsigned val) {
    //std::cout << "Calculating " << val << "\n";
    return val == 0 ? 0 :
           val == 1 ? 1 :
           fib(val - 1) + fib(val - 2);
  });

  ///When fib_memo(35) is invoked, its invoking call operator with arg 35 in
  ///Memoize_helper{} function object. If its a miss in cache then the above
  ///lamada is invoked. If val is neither 1 nor 0 then the Memoize_helper{}
  ///function object is recursively invoked for arg : val -1.
  std::cout << "Fib with lmd memoize: " << fib_memo(val) << std::endl;
}

///Fibonacci sequence calculator with memoization using
///Memoize_helper with (passing a Function Object).
///How to pass a FP (Function Pointer)
void mem_function_fib(unsigned val)
{
  auto fib_memo = nan::make_memoize<unsigned(unsigned)>(Fib::Fib_wrapper{});

  std::cout << "Fib with fun memoize: " << fib_memo(val) << std::endl;
}

void raw_lev(std::string const& m, std::string const& n)
{
  std::cout << "edit distance without memoize: " <<
  Lev::levenshtein_dist(m.cbegin(), n.cbegin(), m.cend(), n.cend()) <<
  std::endl;
}

void mem_lev(std::string const& m, std::string const& n)
{
  using Citr = Lev::Citr;
  auto lev_memo = nan::make_memoize<unsigned(Citr, Citr, Citr, Citr)>(
      [](auto& fun_memo, Citr m_loc, Citr n_loc, Citr m_end, Citr n_end) {
          auto v = [&m_loc, &n_loc]() {
            return static_cast<unsigned>(*m_loc != *n_loc);
          };

          return m_loc == m_end ? std::distance(n_loc, n_end) :
                 n_loc == n_end ? std::distance(m_loc, m_end) :
                 std::min({
                     fun_memo(std::next(m_loc), n_loc, m_end, n_end)   + 1,
                     fun_memo(m_loc, std::next(n_loc), m_end, n_end)   + 1,
                     fun_memo(std::next(m_loc), std::next(n_loc), m_end, n_end) + v()
                 });
  });

  std::cout << "edit distance with memoize: "
            << lev_memo(m.cbegin(), n.cbegin(), m.cend(), n.cend())
            << std::endl;
}

int main()
{
  using namespace std::chrono;
  unsigned ip = 40;

  ///Fib without Memoization
  {
    auto t1 = high_resolution_clock::now();
    raw_fib(ip);
    auto t2 = high_resolution_clock::now();
    std::cout << "raw-fib execution time: "
              << duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << std::endl;
  }

  ///Fib with Memoization (lamada)
  {
    auto t1 = high_resolution_clock::now();
    mem_lamada_fib(ip);
    auto t2 = high_resolution_clock::now();
    std::cout << "mem-fib execution time: "
              << duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << std::endl;
  }

  ///Fib with Memoization (Function Object)
  {
    auto t1 = high_resolution_clock::now();
    mem_function_fib(ip);
    auto t2 = high_resolution_clock::now();
    std::cout << "mem-fib execution time: "
              << duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << std::endl;
  }

  //Levenshtein distance algorithm without memoize
  {
    //std::string a = "0x7c682a13b4d456e567f9678234098d";
    //std::string b = "0x7d642b13b4e546e566782183037c89";
    std::string a = "0x7c682a13b4d";
    std::string b = "0x7d642b13b4e";
    auto t1 = high_resolution_clock::now();
    raw_lev(a, b);
    auto t2 = high_resolution_clock::now();
    std::cout << "raw_lev execution time: "
        << duration_cast<std::chrono::milliseconds>(t2 - t1).count()
        << std::endl;
  }

  ///Levenshtein distance algorithm with memoize
  {
    //std::string a = "0x7c682a13b4d456e567f9678234098d";
    //std::string b = "0x7d642b13b4e546e566782183037c89";
    std::string a = "0x7c682a13b4d";
    std::string b = "0x7d642b13b4e";
    auto t1 = high_resolution_clock::now();
    mem_lev(a, b);
    auto t2 = high_resolution_clock::now();
    std::cout << "mem_lev execution time: "
              << duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << std::endl;
  }

  ///Levenshtein distance algorithm with memoize (extremely long string)
  {
    std::string a = "0x7c682a13b4d456e567f9678234098d";
    std::string b = "0x7d642b13b4e546e566782183037c89";
    auto t1 = high_resolution_clock::now();
    mem_lev(a, b);
    auto t2 = high_resolution_clock::now();
    std::cout << "mem_lev execution time: "
              << duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << std::endl;
  }

  return 0;
}
