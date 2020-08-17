#ifndef EXPRESSION_TEMPLATES_STRING_CONCAT_H
#define EXPRESSION_TEMPLATES_STRING_CONCAT_H

#include <string>
#include <iostream>

namespace nan {

///Primary template compile time multi-string concatenation
///without creating temprories
template<typename... Strings>
class String_concat;

///Specialized template for string concatenation
template<typename LastStr, typename... Strings>
class String_concat<LastStr, Strings...> {
 private:
  LastStr data;
  String_concat<Strings...> tail;

 public:
  String_concat(LastStr str, String_concat<Strings...> rest) noexcept:
      data{str},
      tail{rest}
  {

  }

  [[nodiscard]] std::size_t size() const { return data.size() + tail.size(); }

  template<typename T>
  void save(T end) const
  {
    auto const begin = end - data.size();
    std::copy(data.cbegin(), data.cend(), begin);
    tail.save(begin);
  }

  operator std::string() const
  {
    std::string str(size(), '\0');
    save(str.end());
    return str;
  }

  String_concat<std::string, LastStr, Strings...>
  operator+(std::string const &other) const
  {
    return String_concat<std::string, LastStr, Strings...>{
        other,
        *this
    };
  }

  String_concat<std::string, LastStr, Strings...>
  operator+(char c) const
  {
    return String_concat<std::string, LastStr, Strings...>{
        std::string{c},
        *this
    };
  }

  String_concat<std::string_view, LastStr, Strings...>
  operator+(std::string_view view) const
  {
    return String_concat<std::string_view, LastStr, Strings...>{view, *this};
  }

};

///Base case
template<>
class String_concat<> {
 public:
  String_concat() noexcept = default;
  [[nodiscard]] std::size_t size() const { return 0; }
  template<typename T>
  void save(T end) const { }

  String_concat<std::string> operator+(std::string const &other) const {
    return String_concat<std::string>{other, *this};
  }

  String_concat<std::string> operator+(char c) const {
    return String_concat<std::string>{std::string{c}, *this};
  }

  String_concat<std::string_view>
  operator+(std::string_view view) const
  {
    return String_concat<std::string_view>{view, *this};
  }
};

} //end of namespace

#endif //EXPRESSION_TEMPLATES_STRING_CONCAT_H
