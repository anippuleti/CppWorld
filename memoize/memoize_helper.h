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

#ifndef MEMOIZE_MEMOIZE_HELPER_H
#define MEMOIZE_MEMOIZE_HELPER_H

#include <map>
#include <mutex>

namespace nan {

///Dummy class used to avoid overload resolution with copy constructor
namespace detail {
struct Null_param {};
} //end of namespace detail

template<class Arg, class Function>
class Memoize_helper;

template<class Result, class... Args, class Function>
class Memoize_helper<Result(Args...), Function> {
 public:
  using function_type = Function;
  using args_type     = std::tuple<std::decay_t<Args>...>;

  function_type fun;
 private:
  mutable std::map<args_type, Result> m_cache;
  mutable std::recursive_mutex        m_cache_mutex;

 public:
  ///constructor. Passing Null param type to avoid overload resolution
  ///wit copy constructor
  template<typename F>
  Memoize_helper(F&&, detail::Null_param);

  ///Copy Constructor
  Memoize_helper(Memoize_helper const& other);

  ///calling operator
  template<class... InnerArgs>
  Result operator()(InnerArgs&&... args) const;

};

///Helper function to construct Memoize_helper
template<class Sig, class Function>
Memoize_helper<Sig, std::decay_t<Function>> make_memoize(Function&& fun)
{
  return {std::forward<Function>(fun), detail::Null_param{}};
}

/////////////////////////////////////////////////////
///API Implementation
/////////////////////////////////////////////////////

///constructor
template<class Result, class... Args, class Function>
template<typename F>
Memoize_helper<Result(Args...), Function>::Memoize_helper(F&& f, detail::Null_param):
    fun{f} { }

//copy constructor
template<class Result, class... Args, class Function>
Memoize_helper<Result(Args...), Function>::Memoize_helper(Memoize_helper const& other):
    fun{other.fun} { }

//call operator
template<class Result, class... Args, class Function>
template<class... InnerArgs>
Result Memoize_helper<Result(Args...), Function>::operator()(InnerArgs&&... args) const
{
  std::unique_lock<std::recursive_mutex> lock{m_cache_mutex};

  auto const args_key = std::make_tuple(args...);
  auto exists = m_cache.find(args_key);
  if (exists != m_cache.cend()) {
    return exists->second;
  } else {
    auto&& result = fun(*this, std::forward<InnerArgs>(args)...);
    m_cache.emplace(args_key, result);
    return result;
  }
}

} //end of namespace nan

#endif //MEMOIZE_MEMOIZE_HELPER_H
