/**
 * @file debugstream.h
 * @author gxt_kt (gxt_kt@163.com)
 * @brief  If you have use the qDebug() function of "Qt" before, you must use this module easily.
 * And the qDebug is change name to gDebug here. The detail see the "@attention".
 * The github address is https://github.com/gxt-kt/debugstream
 * @version 1.4.0
 * @date 2023-10-09
 *
 * @copyright Copyright (c) 2022
 *
 * @attention You can use the default gDebug() function to output the debug stream.
 * Such as gDebug("hello world"); , Complexly you can write like gDebug("hello") << "world"; and so on.
 * And the default gDebug() has enable the space and newline.
 * If you use the class DebugStream create a new instantiation. The space funciton is exist but the
 * auto newline is invalid.
 */

#ifndef DEBUGSTREAM_H__
#define DEBUGSTREAM_H__


#include <string>
#include <functional>
#include <memory>
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <sstream>

// Reference : https://github.com/p-ranav/pprint

// There will no debug stream output if define the G_CONFIG_NO_DEBUG_OUTPUT
// #define G_CONFIG_NO_DEBUG_OUTPUT


#include <iostream>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <unordered_set>
#include <array>
#include <map>
#include <unordered_map>
#include <iomanip>
#include <variant>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iosfwd>
#include <limits>
#include <string_view>
#include <optional>
#include <utility>
#include <sstream>
#include <queue>
#include <stack>
#include <tuple>
#include <initializer_list>
#include <complex>
#include <cmath>
#include <memory>
#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>
#endif

namespace gxt {

// support c++17 variant and optional
#define SUPPORTS_CPP17 (__cplusplus >= 201703L)

// Check if a type is stream writable, i.e., std::cout << foo;
template<typename S, typename T, typename = void>
struct is_to_stream_writable: std::false_type {};

// gxt: solve compile bug for clangd or msvc caused by c++ version lower than 17
template <typename...>
using void_t = void;

template<typename S, typename T>
struct is_to_stream_writable<S, T,
           gxt::void_t<  decltype( std::declval<S&>()<<std::declval<T>() ) >>
  : std::true_type {};


// Printing std::tuple
// The indices trick: http://loungecpp.wikidot.com/tips-and-tricks:indices
namespace pprint {

  template<std::size_t...> struct seq{};

  template<std::size_t N, std::size_t... Is>
  struct gen_seq : gen_seq<N-1, N-1, Is...>{};

  template<std::size_t... Is>
  struct gen_seq<0, Is...> : seq<Is...>{};

  template<typename T>
  inline T to_string(T value) {
    return value;
  }

  inline std::string to_string(char value) {
    return "'" + std::string(1, value) + "'";
  }

  inline std::string to_string(const char * value) {
    return "\"" + std::string(value) + "\"";
  }

  inline std::string to_string(const std::string& value) {
    return "\"" + value + "\"";
  }

  template<class Ch, class Tr, class Tuple, std::size_t... Is>
  void print_tuple(std::basic_ostream<Ch,Tr>& os, Tuple const& t, seq<Is...>){
    using swallow = int[];
    (void)swallow{0, (void(os << (Is == 0? "" : ", ") << to_string(std::get<Is>(t))), 0)...};
  }

}

template<class Ch, class Tr, class... Args>
auto operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
  -> std::basic_ostream<Ch, Tr>& {
  os << "(";
  pprint::print_tuple(os, t, pprint::gen_seq<sizeof...(Args)>());
  return os << ")";
}

// enum imp
namespace detail {

// Enum value must be greater or equals than G_CONFIG_ENUM_RANGE_MIN. By default
// G_CONFIG_ENUM_RANGE_MIN = -128. If need another min range for all enum types
// by default, redefine the macro G_CONFIG_ENUM_RANGE_MIN.
#if !defined(G_CONFIG_ENUM_RANGE_MIN)
#define G_CONFIG_ENUM_RANGE_MIN -128
#endif

// Enum value must be less or equals than G_CONFIG_ENUM_RANGE_MAX. By default
// G_CONFIG_ENUM_RANGE_MAX = 128. If need another max range for all enum types
// by default, redefine the macro G_CONFIG_ENUM_RANGE_MAX.
#if !defined(G_CONFIG_ENUM_RANGE_MAX)
#define G_CONFIG_ENUM_RANGE_MAX 128
#endif

static_assert(G_CONFIG_ENUM_RANGE_MAX < std::numeric_limits<int>::max(),
              "G_CONFIG_ENUM_RANGE_MAX must be less than INT_MAX.");

static_assert(G_CONFIG_ENUM_RANGE_MIN > std::numeric_limits<int>::min(),
              "G_CONFIG_ENUM_RANGE_MIN must be greater than INT_MIN.");

template <typename T, T N>
inline std::string GetEnumNameImp() {
#if defined(__GNUC__) || defined(__clang__)
  std::string tmp = __PRETTY_FUNCTION__;
  auto first = tmp.find("T N = ");
  first += 6;
  auto end = tmp.find(";", first);
  return std::string(tmp, first, end - first);
#elif defined(_MSC_VER)
  // TODO: add support for msvc
#else
#endif
}

template <int begin, int end, typename F>
typename std::enable_if<begin == end>::type TemplateForLoop(const F &fun) {
  fun.template call<begin>();
}
template <int begin, int end, typename F>
typename std::enable_if<begin != end>::type TemplateForLoop(const F &fun) {
  fun.template call<begin>();
  TemplateForLoop<begin + 1, end>(fun);
}


template <typename T>
struct GetEnumClass {
  int n_;
  std::string &str_;
  GetEnumClass(int n, std::string &str) : n_(n), str_(str) {}

  template <int N>
  void call() const {
    if (n_ == N) {
      str_ = detail::GetEnumNameImp<T, T(N)>();
    }
  }
};

} // detail for enum imp

template <typename T, int min = G_CONFIG_ENUM_RANGE_MIN,
          int max = G_CONFIG_ENUM_RANGE_MAX>
inline std::string GetEnumName(T n) {
  std::string str;
  gxt::detail::TemplateForLoop<min, max>(
      gxt::detail::GetEnumClass<T>(static_cast<int>(n), str));
  if (str.empty()) {
    throw std::runtime_error("\nenum out of range\n");
  }
  return str;
}

template <typename T, int min = G_CONFIG_ENUM_RANGE_MIN,
          int max = G_CONFIG_ENUM_RANGE_MAX>
inline int GetNameEnum(std::string name) {
  std::string str;
  for (int i = G_CONFIG_ENUM_RANGE_MIN; i <= G_CONFIG_ENUM_RANGE_MAX; i++) {
    gxt::detail::TemplateForLoop<G_CONFIG_ENUM_RANGE_MIN,
                                 G_CONFIG_ENUM_RANGE_MAX>(
        gxt::detail::GetEnumClass<T>(static_cast<int>(i), str));
    if (!str.empty()) {  // solve bug that use class enum
      auto find = str.find("::");
      if (find != std::string::npos) {
        find += 2;
        str = std::string(str, find);
      }
    }
    if (!str.empty() && str == name) {
      return i;
    }
  }
  throw std::runtime_error("\nenum out of range\n");
  return 0;
}

namespace pprint {

  // Some utility structs to check template specialization
  template<typename Test, template<typename...> class Ref>
  struct is_specialization : std::false_type {};

  template<template<typename...> class Ref, typename... Args>
  struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

  template<typename ...>
  using to_void = void;

  template<typename T, typename = void>
  struct is_container : std::false_type
  {};

  template<typename T>
  struct is_container<T,
          to_void<decltype(std::declval<T>().begin()),
              decltype(std::declval<T>().end()),
              typename T::value_type
          >> : std::true_type // will  be enabled for iterable objects
  {};

  class PrettyPrinter {
  private:
    std::ostream& stream_;
    std::string line_terminator_;
    size_t indent_;
    bool quotes_;
    bool compact_;

  public:

    PrettyPrinter(std::ostream& stream = std::cout) :
      stream_(stream),
      line_terminator_(""),
      indent_(2),
      quotes_(false),
      compact_(false) {}

    PrettyPrinter& line_terminator(const std::string& value) {
      line_terminator_ = value;
      return *this;
    }

    PrettyPrinter& indent(size_t indent) {
      indent_ = indent;
      return *this;
    }

    PrettyPrinter& compact(bool value) {
      compact_ = value;
      return *this;
    }

    PrettyPrinter& quotes(bool value) {
      quotes_ = value;
      return *this;
    }

    template <typename T>
    void print(T value) {
      print_internal(value, 0, line_terminator_, 0);
    }

    template <typename T>
    void print(std::initializer_list<T> value) {
      print_internal(value, 0, line_terminator_, 0);
    }

    template<typename T, typename... Targs>
    void print(T value, Targs... Fargs) {
      print_internal(value, 0, "", 0);
      auto current_quotes = quotes_;
      quotes_ = false;
      print_internal(" ", 0, "", 0);
      quotes_ = current_quotes;
      print(Fargs...);
    }

    template <typename T>
    void print_inline(T value) {
      print_internal(value, indent_, "", 0);
    }

    template <typename T>
    void print_inline(std::initializer_list<T> value) {
      print_internal(value, indent_, "", 0);
    }

    template<typename T, typename... Targs>
    void print_inline(T value, Targs... Fargs) {
      print_internal(value, indent_, "", 0);
      auto current_quotes = quotes_;
      quotes_ = false;
      print_internal(" ", 0, "", 0);
      quotes_ = current_quotes;
      print_inline(Fargs...);
    }

  private:

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value == true, void>::type
    print_internal(T value, size_t indent = 0, const std::string& line_terminator = "\n", size_t level = 0) {
      stream_ << std::string(indent, ' ') << value << line_terminator;
    }

    template <typename T>
    typename std::enable_if<std::is_null_pointer<T>::value == true, void>::type
    print_internal(T value, size_t indent = 0, const std::string& line_terminator = "\n", size_t level = 0) {
      stream_ << std::string(indent, ' ') << "nullptr" << line_terminator;
    }

    void print_internal(float value, size_t indent = 0, const std::string& line_terminator = "\n", size_t level = 0) {
      stream_ << std::string(indent, ' ') << value << 'f' << line_terminator;
    }

    void print_internal(double value, size_t indent = 0, const std::string& line_terminator = "\n", size_t level = 0) {
      stream_ << std::string(indent, ' ') << value << line_terminator;
    }

    void print_internal(const std::string& value, size_t indent = 0, const std::string& line_terminator = "\n",
      size_t level = 0) {
      if (!quotes_)
        print_internal_without_quotes(value, indent, line_terminator, level);
      else
        stream_ << std::string(indent, ' ') << "\"" << value << "\"" << line_terminator;
    }

    void print_internal(const char * value, size_t indent = 0, const std::string& line_terminator = "\n",
      size_t level = 0) {
      if (!quotes_)
        print_internal_without_quotes(value, indent, line_terminator, level);
      else
        stream_ << std::string(indent, ' ') << "\"" << value << "\"" << line_terminator;
    }

    void print_internal(char value, size_t indent = 0, const std::string& line_terminator = "\n", size_t level = 0) {
      if (!quotes_)
        print_internal_without_quotes(value, indent, line_terminator, level);
      else
        stream_ << std::string(indent, ' ') << "'" << value << "'" << line_terminator;
    }

    void print_internal_without_quotes(const std::string& value, size_t indent = 0,
      const std::string& line_terminator = "\n", size_t level = 0) {
      stream_ << std::string(indent, ' ') << value << line_terminator;
    }

    void print_internal_without_quotes(const char * value, size_t indent = 0,
      const std::string& line_terminator = "\n", size_t level = 0) {
      stream_ << std::string(indent, ' ') << value << line_terminator;
    }

    void print_internal_without_quotes(char value, size_t indent = 0, const std::string& line_terminator = "\n",
      size_t level = 0) {
      stream_ << std::string(indent, ' ') << value << line_terminator;
    }

    void print_internal(bool value, size_t indent = 0, const std::string& line_terminator = "\n", size_t level = 0) {
      stream_ << std::string(indent, ' ') << (value ? "true" : "false") << line_terminator;
    }

    template <typename T>
    typename std::enable_if<std::is_pointer<T>::value == true, void>::type
    print_internal(T value, size_t indent = 0, const std::string& line_terminator = "\n", size_t level = 0) {
      if (value == nullptr) {
        return print_internal(nullptr, indent, line_terminator, level);
      }
      stream_ << std::string(indent, ' ') << "<" << type(value) << " at "
              << value << ">" << line_terminator;
    }

    std::string demangle(const char* name) {
#ifdef __GNUG__
      int status = -4;
      std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free
      };
      return (status==0) ? res.get() : name;
#else
      return name;
#endif
    }

    template <class T>
    std::string type(const T& t) {
      return demangle(typeid(t).name());
    }


    template <typename T>
    typename std::enable_if<std::is_enum<T>::value == true, void>::type
    print_internal(T value, size_t indent = 0, const std::string& line_terminator = "\n", size_t level = 0) {
      stream_ << std::string(indent, ' ')
              << static_cast<typename std::underlying_type<T>::type>(value)
              << line_terminator;
    }

    template <typename T>
    typename std::enable_if<std::is_class<T>::value == true &&
        is_to_stream_writable<std::ostream, T>::value == true &&
        std::is_enum<T>::value == false &&
        is_specialization<T, std::unique_ptr>::value == false &&
        is_specialization<T, std::shared_ptr>::value == false &&
        is_specialization<T, std::weak_ptr>::value == false &&
        is_specialization<T, std::tuple>::value == false &&
        #if SUPPORTS_CPP17
        is_specialization<T, std::variant>::value == false &&
        #endif
        is_specialization<T, std::vector>::value == false &&
        is_specialization<T, std::list>::value == false &&
        is_specialization<T, std::deque>::value == false &&
        is_specialization<T, std::queue>::value == false &&
        is_specialization<T, std::priority_queue>::value == false &&
        is_specialization<T, std::stack>::value == false &&
        is_specialization<T, std::set>::value == false &&
        is_specialization<T, std::multiset>::value == false &&
        is_specialization<T, std::unordered_set>::value == false &&
        is_specialization<T, std::unordered_multiset>::value == false &&
        is_specialization<T, std::map>::value == false &&
        is_specialization<T, std::multimap>::value == false &&
        is_specialization<T, std::unordered_map>::value == false &&
        is_specialization<T, std::unordered_multimap>::value == false, void>::type
    print_internal(T value, size_t indent = 0, const std::string& line_terminator = "\n", size_t level = 0) {
      stream_ << std::string(indent, ' ') << value << line_terminator;
    }

    template <typename T>
    typename std::enable_if<std::is_class<T>::value == true &&
            is_to_stream_writable<std::ostream, T>::value == false &&
            std::is_enum<T>::value == false &&
            is_specialization<T, std::unique_ptr>::value == false &&
            is_specialization<T, std::shared_ptr>::value == false &&
            is_specialization<T, std::weak_ptr>::value == false &&
            is_specialization<T, std::tuple>::value == false &&
            #if SUPPORTS_CPP17
            is_specialization<T, std::variant>::value == false &&
            #endif
            is_specialization<T, std::vector>::value == false &&
            is_specialization<T, std::list>::value == false &&
            is_specialization<T, std::deque>::value == false &&
            is_specialization<T, std::queue>::value == false &&
            is_specialization<T, std::priority_queue>::value == false &&
            is_specialization<T, std::stack>::value == false &&
            is_specialization<T, std::set>::value == false &&
            is_specialization<T, std::multiset>::value == false &&
            is_specialization<T, std::unordered_set>::value == false &&
            is_specialization<T, std::unordered_multiset>::value == false &&
            is_specialization<T, std::map>::value == false &&
            is_specialization<T, std::multimap>::value == false &&
            is_specialization<T, std::unordered_map>::value == false &&
            is_specialization<T, std::unordered_multimap>::value == false, void>::type
            print_internal(T value, size_t indent = 0, const std::string& line_terminator = "\n", size_t level = 0) {
      stream_ << std::string(indent, ' ') << "<Object " << type(value) << ">"
              << line_terminator;
    }

    template <typename T>
    typename std::enable_if<std::is_member_function_pointer<T>::value == true, void>::type
    print_internal(T value, size_t indent = 0, const std::string& line_terminator = "\n", size_t level = 0) {
      stream_ << std::string(indent, ' ') << "<Object.method " << type(value)
              << " at " << &value << ">"
              << line_terminator;
    }

    template <typename Container>
    typename std::enable_if<is_specialization<Container, std::vector>::value, void>::type
            print_internal(const Container& value, size_t indent = 0, const std::string& line_terminator = "\n",
            size_t level = 0) {
      typedef typename Container::value_type T;
      if (level == 0 && !compact_) {
        if (value.size() == 0) {
          print_internal_without_quotes("[", 0, "");
        }
        else if (value.size() == 1) {
          print_internal_without_quotes("[", 0, "");
          print_internal(value.front(), 0, "", level + 1);
        }
        else if (value.size() > 0) {
          print_internal_without_quotes("[", 0, "\n");
          print_internal(value.front(), indent + indent_, "", level + 1);
          if (value.size() > 1 && is_container<T>::value == false)
            print_internal_without_quotes(", ", 0, "\n");
          else if (is_container<T>::value)
            print_internal_without_quotes(", ", 0, "\n");
          for (size_t i = 1; i < value.size() - 1; i++) {
            print_internal(value[i], indent + indent_, "", level + 1);
            if (is_container<T>::value == false)
              print_internal_without_quotes(", ", 0, "\n");
            else
              print_internal_without_quotes(", ", 0, "\n");
          }
          if (value.size() > 1) {
            print_internal(value.back(), indent + indent_, "\n", level + 1);
          }
        }
        if (value.size() == 0)
          print_internal_without_quotes("]", indent, "");
        else if (is_container<T>::value == false)
          print_internal_without_quotes("]", indent, "");
        else
          print_internal_without_quotes(line_terminator_ + "]", indent, "");
        print_internal_without_quotes(line_terminator_, 0, "");
      }
      else {
        if (value.size() == 0) {
          print_internal_without_quotes("[", indent, "");
        }
        else if (value.size() == 1) {
          print_internal_without_quotes("[", indent, "");
          print_internal(value.front(), 0, "", level + 1);
        }
        else if (value.size() > 0) {
          print_internal_without_quotes("[", indent, "");
          print_internal(value.front(), 0, "", level + 1);
          if (value.size() > 1)
            print_internal_without_quotes(", ", 0, "");
          for (size_t i = 1; i < value.size() - 1; i++) {
            print_internal(value[i], 0, "", level + 1);
            print_internal_without_quotes(", ", 0, "");
          }
          if (value.size() > 1) {
            print_internal(value.back(), 0, "", level + 1);
          }
        }
        print_internal_without_quotes("]", 0, "");
        if (level == 0 && compact_)
          print_internal_without_quotes(line_terminator_, 0, "");
      }

    }

    template <typename T, unsigned long int S>
    void print_internal(const std::array<T, S>& value, size_t indent = 0, const std::string& line_terminator = "\n",
            size_t level = 0) {
      if (level == 0 && !compact_) {
        if (value.size() == 0) {
          print_internal_without_quotes("[", 0, "");
        }
        else if (value.size() == 1) {
          print_internal_without_quotes("[", 0, "");
          print_internal(value.front(), 0, "", level + 1);
        }
        else if (value.size() > 0) {
          print_internal_without_quotes("[", 0, "\n");
          print_internal(value.front(), indent + indent_, "", level + 1);
          if (value.size() > 1 && is_container<T>::value == false)
            print_internal_without_quotes(", ", 0, "\n");
          else if (is_container<T>::value)
            print_internal_without_quotes(", ", 0, "\n");
          for (size_t i = 1; i < value.size() - 1; i++) {
            print_internal(value[i], indent + indent_, "", level + 1);
            if (is_container<T>::value == false)
              print_internal_without_quotes(", ", 0, "\n");
            else
              print_internal_without_quotes(", ", 0, "\n");
          }
          if (value.size() > 1) {
            print_internal(value.back(), indent + indent_, "\n", level + 1);
          }
        }
        if (value.size() == 0)
          print_internal_without_quotes("]", indent, "");
        else if (is_container<T>::value == false)
          print_internal_without_quotes("]", indent, "");
        else
          print_internal_without_quotes(line_terminator_ + "]", indent, "");
        print_internal_without_quotes(line_terminator_, 0, "");
      }
      else {
        if (value.size() == 0) {
          print_internal_without_quotes("[", indent, "");
        }
        else if (value.size() == 1) {
          print_internal_without_quotes("[", indent, "");
          print_internal(value.front(), 0, "", level + 1);
        }
        else if (value.size() > 0) {
          print_internal_without_quotes("[", indent, "");
          print_internal(value.front(), 0, "", level + 1);
          if (value.size() > 1)
            print_internal_without_quotes(", ", 0, "");
          for (size_t i = 1; i < value.size() - 1; i++) {
            print_internal(value[i], 0, "", level + 1);
            print_internal_without_quotes(", ", 0, "");
          }
          if (value.size() > 1) {
            print_internal(value.back(), 0, "", level + 1);
          }
        }
        print_internal_without_quotes("]", 0, "");
        if (level == 0 && compact_)
          print_internal_without_quotes(line_terminator_, 0, "");
      }

    }

    template <typename Container>
    typename std::enable_if<is_specialization<Container, std::list>::value ||
            is_specialization<Container, std::deque>::value,
            void>::type print_internal(const Container& value, size_t indent = 0,
            const std::string& line_terminator = "\n",
            size_t level = 0) {
      typedef typename Container::value_type T;
      if (level == 0 && !compact_) {
        if (value.size() == 0) {
          print_internal_without_quotes("[", 0, "");
        }
        else if (value.size() == 1) {
          print_internal_without_quotes("[", 0, "");
          print_internal(value.front(), 0, "", level + 1);
        }
        else if (value.size() > 0) {
          print_internal_without_quotes("[", 0, "\n");
          print_internal(value.front(), indent + indent_, "", level + 1);
          if (value.size() > 1 && is_container<T>::value == false)
            print_internal_without_quotes(", ", 0, "\n");
          else if (is_container<T>::value)
            print_internal_without_quotes(", ", 0, "\n");

          typename Container::const_iterator iterator;
          for (iterator = std::next(value.begin()); iterator != std::prev(value.end()); ++iterator) {
            print_internal(*iterator, indent + indent_, "", level + 1);
            if (is_container<T>::value == false)
              print_internal_without_quotes(", ", 0, "\n");
            else
              print_internal_without_quotes(", ", 0, "\n");
          }

          if (value.size() > 1) {
            print_internal(value.back(), indent + indent_, "\n", level + 1);
          }
        }
        if (value.size() == 0)
          print_internal_without_quotes("]", indent, "");
        else if (is_container<T>::value == false)
          print_internal_without_quotes("]", indent, "");
        else
          print_internal_without_quotes(line_terminator_ + "]", indent, "");
        print_internal_without_quotes(line_terminator_, 0, "");
      }
      else {
        if (value.size() == 0) {
          print_internal_without_quotes("[", indent, "");
        }
        else if (value.size() == 1) {
          print_internal_without_quotes("[", indent, "");
          print_internal(value.front(), 0, "", level + 1);
        }
        else if (value.size() > 0) {
          print_internal_without_quotes("[", indent, "");
          print_internal(value.front(), 0, "", level + 1);
          if (value.size() > 1)
            print_internal_without_quotes(", ", 0, "");

          typename Container::const_iterator iterator;
          for (iterator = std::next(value.begin()); iterator != std::prev(value.end()); ++iterator) {
            print_internal(*iterator, 0, "", level + 1);
            print_internal_without_quotes(", ", 0, "");
          }

          if (value.size() > 1) {
            print_internal(value.back(), 0, "", level + 1);
          }
        }
        print_internal_without_quotes("]", 0, "");
        if (level == 0 && compact_)
          print_internal_without_quotes(line_terminator_, 0, "");
      }

    }

    template <typename Container>
    typename std::enable_if<is_specialization<Container, std::set>::value ||
            is_specialization<Container, std::multiset>::value ||
            is_specialization<Container, std::unordered_set>::value ||
            is_specialization<Container, std::unordered_multiset>::value, void>::type
            print_internal(const Container& value, size_t indent = 0, const std::string& line_terminator = "\n",
            size_t level = 0) {
      typedef typename Container::value_type T;
      if (level == 0 && !compact_) {
        if (value.size() == 0) {
          print_internal_without_quotes("{", 0, "");
        }
        else if (value.size() == 1) {
          print_internal_without_quotes("{", 0, "");
          print_internal(*(value.begin()), 0, "", level + 1);
        }
        else {
          print_internal_without_quotes("{", 0, "\n");
          print_internal(*(value.begin()), indent + indent_, "", level + 1);
          if (value.size() > 1 && is_container<T>::value == false)
            print_internal_without_quotes(", ", 0, "\n");
          else if (is_container<T>::value)
            print_internal_without_quotes(", ", 0, "\n");

          typename Container::const_iterator iterator;
          for (iterator = std::next(value.begin()); (iterator != value.end()) && (std::next(iterator) != value.end()); ++iterator) {
            print_internal(*iterator, indent + indent_, "", level + 1);
            if (is_container<T>::value == false)
              print_internal_without_quotes(", ", 0, "\n");
            else
              print_internal_without_quotes(", ", 0, "\n");
          }

          if (value.size() > 1) {
            print_internal(*iterator, indent + indent_, "\n", level + 1);
          }
        }
        if (value.size() == 0)
          print_internal_without_quotes("}", indent, "");
        else if (is_container<T>::value == false)
          print_internal_without_quotes("}", indent, "");
        else
          print_internal_without_quotes(line_terminator_ + "}", indent, "");
        print_internal_without_quotes(line_terminator_, 0, "");
      }
      else {
        if (value.size() == 0) {
          print_internal_without_quotes("{", indent, "");
        }
        else if (value.size() == 1) {
          print_internal_without_quotes("{", indent, "");
          print_internal(*(value.begin()), 0, "", level + 1);
        }
        else {
          print_internal_without_quotes("{", indent, "");
          print_internal(*(value.begin()), 0, "", level + 1);
          if (value.size() > 1)
            print_internal_without_quotes(", ", 0, "");

          typename Container::const_iterator iterator;
          for (iterator = std::next(value.begin()); (iterator != value.end()) && (std::next(iterator) != value.end()); ++iterator) {
            print_internal(*iterator, 0, "", level + 1);
            print_internal_without_quotes(", ", 0, "");
          }

          if (value.size() > 1) {
            print_internal(*iterator, 0, "", level + 1);
          }
        }
        print_internal_without_quotes("}", 0, "");
        if (level == 0 && compact_)
          print_internal_without_quotes(line_terminator_, 0, "");
      }

    }

    template <typename T>
    typename std::enable_if<is_specialization<T, std::map>::value == true ||
            is_specialization<T, std::multimap>::value == true ||
            is_specialization<T, std::unordered_map>::value == true ||
            is_specialization<T, std::unordered_multimap>::value == true, void>::type
            print_internal(const T& value, size_t indent = 0, const std::string& line_terminator = "\n", size_t level = 0) {
      typedef typename T::mapped_type Value;
      if (level == 0 && !compact_) {
        if (value.size() == 0) {
          print_internal_without_quotes("{", 0, "");
        }
        else if (value.size() == 1) {
          print_internal_without_quotes("{", 0, "");
          for (auto& kvpair : value) {
            print_internal(kvpair.first, 0, "", level + 1);
            print_internal_without_quotes(" : ", 0, "");
            print_internal(kvpair.second, 0, "", level + 1);
          }
        }
        else if (value.size() > 0) {
          size_t count = 0;
          for (auto& kvpair : value) {
            if (count == 0) {
              print_internal_without_quotes("{", 0, "\n");
              print_internal(kvpair.first, indent + indent_, "", level + 1);
              print_internal_without_quotes(" : ", 0, "");
              print_internal(kvpair.second, 0, "", level + 1);
              if (value.size() > 1 && is_container<Value>::value == false)
                print_internal_without_quotes(", ", 0, "\n");
              else if (is_container<Value>::value)
                print_internal_without_quotes(", ", 0, "\n");
            }
            else if (count + 1 < value.size()) {
              print_internal(kvpair.first, indent + indent_, "", level + 1);
              print_internal_without_quotes(" : ", 0, "");
              print_internal(kvpair.second, 0, "", level + 1);
              if (is_container<Value>::value == false)
                print_internal_without_quotes(", ", 0, "\n");
              else
                print_internal_without_quotes(", ", 0, "\n");
            }
            else {
              print_internal(kvpair.first, indent + indent_, "", level + 1);
              print_internal_without_quotes(" : ", 0, "");
              print_internal(kvpair.second, 0, "\n", level + 1);
            }
            count += 1;
          }
        }
        if (value.size() == 0)
          print_internal_without_quotes("}", indent, "");
        else if (is_container<Value>::value == false)
          print_internal_without_quotes("}", indent, "");
        else
          print_internal_without_quotes(line_terminator_ + "}", indent, "");
        print_internal_without_quotes(line_terminator_, 0, "");
      }

      else {
        if (value.size() == 0) {
          print_internal_without_quotes("{", indent, "");
        }
        else if (value.size() == 1) {
          print_internal_without_quotes("{", indent, "");
          for (auto& kvpair : value) {
            print_internal(kvpair.first, 0, "", level + 1);
            print_internal_without_quotes(" : ", 0, "");
            print_internal(kvpair.second, 0, "", level + 1);
          }
        }
        else if (value.size() > 0) {
          size_t count = 0;
          for (auto& kvpair : value) {
            if (count == 0) {
              print_internal_without_quotes("{", indent, "");
              print_internal(kvpair.first, 0, "", level + 1);
              print_internal_without_quotes(" : ", 0, "");
              print_internal(kvpair.second, 0, "", level + 1);
              print_internal_without_quotes(", ", 0, "");
            }
            else if (count + 1 < value.size()) {
              print_internal(kvpair.first, indent + indent_, "", level + 1);
              print_internal_without_quotes(" : ", 0, "");
              print_internal(kvpair.second, 0, "", level + 1);
              print_internal_without_quotes(", ", 0, "");
            }
            else {
              print_internal(kvpair.first, 0, "", level + 1);
              print_internal_without_quotes(" : ", 0, "");
              print_internal(kvpair.second, 0, "", level + 1);
            }
            count += 1;
          }
        }
        print_internal_without_quotes("}", 0, "");
        if (level == 0 && compact_)
          print_internal_without_quotes(line_terminator_, 0, "");
      }
    }

    template <typename Key, typename Value>
    void print_internal(std::pair<Key, Value> value, size_t indent = 0, const std::string& line_terminator = "\n",
            size_t level = 0) {
      print_internal_without_quotes("(", indent, "");
      print_internal(value.first, 0, "");
      print_internal_without_quotes(", ", 0, "");
      print_internal(value.second, 0, "");
      print_internal_without_quotes(")", 0, line_terminator, level);
    }

    #if SUPPORTS_CPP17
    template <class ...Ts>
    void print_internal(std::variant<Ts...> value, size_t indent = 0,
        const std::string& line_terminator = "\n", size_t level = 0) {
      std::visit([=](const auto& value) { print_internal(value, indent, line_terminator, level); }, value);
    }

    template <typename T>
    void print_internal(std::optional<T> value, size_t indent = 0,
        const std::string& line_terminator = "\n", size_t level = 0) {
      if (value) {
        print_internal(value.value(), indent, line_terminator, level);
      }
      else {
        print_internal_without_quotes("nullopt", indent, line_terminator, level);
      }
    }
    #endif

    template <typename Container>
    typename std::enable_if<is_specialization<Container, std::queue>::value, void>::type
            print_internal(const Container& value, size_t indent = 0, const std::string& line_terminator = "\n",
            size_t level = 0) {
      auto current_compact = compact_;
      compact_ = true;
      typedef typename Container::value_type T;
      auto local = value;
      std::vector<T> local_vector;
      while (!local.empty()) {
        local_vector.push_back(local.front());
        local.pop();
      }
      print_internal(local_vector, indent, line_terminator, level);
      compact_ = current_compact;
    }

    template <typename Container>
    typename std::enable_if<is_specialization<Container, std::priority_queue>::value, void>::type
    print_internal(const Container& value, size_t indent = 0, const std::string& line_terminator = "\n",
            size_t level = 0) {
      auto current_compact = compact_;
      compact_ = true;
      typedef typename Container::value_type T;
      auto local = value;
      std::vector<T> local_vector;
      while (!local.empty()) {
        local_vector.push_back(local.top());
        local.pop();
      }
      print_internal(local_vector, indent, line_terminator, level);
      compact_ = current_compact;
    }

    template <typename T>
    void print_internal(std::initializer_list<T> value, size_t indent = 0,
            const std::string& line_terminator = "\n", size_t level = 0) {
      std::multiset<T> local;
      for(const T& x : value) {
        local.insert(x);
      }
      print_internal(local, indent, line_terminator_, level);
    }

    template <typename Container>
    typename std::enable_if<is_specialization<Container, std::stack>::value, void>::type
    print_internal(const Container& value, size_t indent = 0, const std::string& line_terminator = "\n",
            size_t level = 0) {
      bool current_compact = compact_;
      compact_ = false; // Need to print a stack like its a stack, i.e., vertical
      typedef typename Container::value_type T;
      auto local = value;
      std::vector<T> local_vector;
      while (!local.empty()) {
        local_vector.push_back(local.top());
        local.pop();
      }
      print_internal(local_vector, indent, line_terminator, level);
      compact_ = current_compact;
    }

    template<class... Args>
    void print_internal(const std::tuple<Args...>& value, size_t indent = 0, const std::string& line_terminator = "\n",
            size_t level = 0) {
      stream_ << std::string(indent, ' ') << value
              << line_terminator;
    }

    template<typename T>
    void print_internal(const std::complex<T>& value, size_t indent = 0, const std::string& line_terminator = "\n",
        size_t level = 0) {
      stream_ << std::string(indent, ' ') << "(" <<
      value.real() << " + " << value.imag() << "i)"
                   << line_terminator;
    }

    template<typename Pointer>
    typename std::enable_if<is_specialization<Pointer, std::unique_ptr>::value ||
        is_specialization<Pointer, std::shared_ptr>::value ||
        is_specialization<Pointer, std::weak_ptr>::value, void>::type
        print_internal(const Pointer& value, size_t indent = 0, const std::string& line_terminator = "\n",
        size_t level = 0) {
      stream_ << std::string(indent, ' ') << "<" <<
      type(value) << " at " << &value << ">"
                  << line_terminator;
    }

  };

}

// The default call back function : use the printf to send the stream
// inline void DebugSendStringCallBack_Default_(const char *str, int num) {
//   std::printf("%.*s",num,str);
// }
inline void DebugSendStringCallBack_Default_(std::string str) {
  std::cout << str;
  // std::printf("%s",str.c_str());
}

// Set the endl compatible with std::endl;
namespace detail{
class DebugStreamEndl {};
}
inline void endl(detail::DebugStreamEndl){}

//--------------------------------------------------
namespace detail{
static const char* DEBUG_STREAM_COLOR_FG_NORMAL = "\x1B[0m";
static const char* DEBUG_STREAM_COLOR_FG_BLACK  = "\x1B[30m";
static const char* DEBUG_STREAM_COLOR_FG_RED    = "\x1B[31m";
static const char* DEBUG_STREAM_COLOR_FG_GREEN  = "\x1B[32m";
static const char* DEBUG_STREAM_COLOR_FG_YELLOW = "\x1B[33m";
static const char* DEBUG_STREAM_COLOR_FG_BLUE   = "\x1B[34m";
static const char* DEBUG_STREAM_COLOR_FG_MAGENTA= "\x1B[35m";
static const char* DEBUG_STREAM_COLOR_FG_CYAN   = "\x1B[36m";
static const char* DEBUG_STREAM_COLOR_FG_WHITE  = "\x1B[37m";
//--------------------------------------------------
static const char* DEBUG_STREAM_COLOR_BG_NORMAL = "\x1B[49m";
static const char* DEBUG_STREAM_COLOR_BG_BLACK  = "\x1b[40m";
static const char* DEBUG_STREAM_COLOR_BG_RED    = "\x1b[41m";
static const char* DEBUG_STREAM_COLOR_BG_GREEN  = "\x1B[42m";
static const char* DEBUG_STREAM_COLOR_BG_YELLOW = "\x1B[43m";
static const char* DEBUG_STREAM_COLOR_BG_BLUE   = "\x1B[44m";
static const char* DEBUG_STREAM_COLOR_BG_MAGENTA= "\x1B[45m";
static const char* DEBUG_STREAM_COLOR_BG_CYAN   = "\x1B[46m";
static const char* DEBUG_STREAM_COLOR_BG_WHITE  = "\x1B[47m";
}
//--------------------------------------------------
namespace detail{
class normal_fg_t {}; 
class black_fg_t  {}; 
class red_fg_t    {}; 
class green_fg_t  {}; 
class yellow_fg_t {}; 
class blue_fg_t   {}; 
class magenta_fg_t{}; 
class cyan_fg_t   {}; 
class white_fg_t  {}; 
//--------------------------------------------------
class normal_bg_t {}; 
class black_bg_t  {}; 
class red_bg_t    {}; 
class green_bg_t  {}; 
class yellow_bg_t {}; 
class blue_bg_t   {}; 
class magenta_bg_t{}; 
class cyan_bg_t   {}; 
class white_bg_t  {}; 
//--------------------------------------------------
class debug_general_t     {};
class debug_status_t      {};
class debug_warning_t     {};
class debug_error_t       {};
class debug_fatal_error_t {};
}
inline void normal_fg (detail::normal_fg_t ) {}
inline void black_fg  (detail::black_fg_t  ) {}
inline void red_fg    (detail::red_fg_t    ) {}
inline void green_fg  (detail::green_fg_t  ) {}
inline void yellow_fg (detail::yellow_fg_t ) {}
inline void blue_fg   (detail::blue_fg_t   ) {}
inline void magenta_fg(detail::magenta_fg_t) {}
inline void cyan_fg   (detail::cyan_fg_t   ) {}
inline void white_fg  (detail::white_fg_t  ) {}
//---------------------detail::-----------------------------
inline void normal_bg (detail::normal_bg_t ) {}
inline void black_bg  (detail::black_bg_t  ) {}
inline void red_bg    (detail::red_bg_t    ) {}
inline void green_bg  (detail::green_bg_t  ) {}
inline void yellow_bg (detail::yellow_bg_t ) {}
inline void blue_bg   (detail::blue_bg_t   ) {}
inline void magenta_bg(detail::magenta_bg_t) {}
inline void cyan_bg   (detail::cyan_bg_t   ) {}
inline void white_bg  (detail::white_bg_t  ) {}

// set the debug color
inline void GENERAL     (detail::debug_general_t)     {}
inline void STATUS      (detail::debug_status_t)      {}
inline void WARNING     (detail::debug_warning_t)     {}
inline void ERROR       (detail::debug_error_t)       {}
inline void FATAL_ERROR (detail::debug_fatal_error_t) {}


class DebugStream {
 public:
  //======================================
  explicit DebugStream \
  (std::function<void(const std::string&)>  fun_ = DebugSendStringCallBack_Default_, \
  int buf_len_ = 2048);
  DebugStream(const DebugStream &obj);
  DebugStream(DebugStream &&obj);
  DebugStream& operator=(const DebugStream &obj);
  DebugStream& operator=(DebugStream &&obj);
  ~DebugStream();
  //===============================================================
  inline DebugStream &OutEn(bool en)    {out_en=en;       return *this;}
  inline DebugStream &NoNewLine()       {newline=false;   return *this;}
  inline DebugStream &NewLine()         {newline=true;    return *this;}
  inline DebugStream &Space()           {space=true;      return *this;}
  inline DebugStream &NoSpace()         {space=false;     return *this;}
  inline DebugStream &Terminate()       {terminate=true;  return *this;}
  inline DebugStream &ClearColor()      {clear_color=true;return *this;}
  //===============================================================

  //=========================================
  // inline DebugStream &print()                                    {return *this;}
  DebugStream &operator<<(void(*)(detail::DebugStreamEndl)) {(*this)<<"\n"; return *this;}

  DebugStream &operator<<(void(*)(detail::normal_fg_t)) {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_FG_NORMAL; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::black_fg_t))  {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_FG_BLACK; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::red_fg_t))    {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_FG_RED; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::green_fg_t))  {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_FG_GREEN; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::yellow_fg_t)) {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_FG_YELLOW; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::blue_fg_t))   {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_FG_BLUE; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::magenta_fg_t)){(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_FG_MAGENTA; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::cyan_fg_t))   {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_FG_CYAN; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::white_fg_t))  {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_FG_WHITE; return (*this).Space();}

  DebugStream &operator<<(void(*)(detail::normal_bg_t)) {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_BG_NORMAL; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::black_bg_t))  {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_BG_BLACK; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::red_bg_t))    {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_BG_RED; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::green_bg_t))  {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_BG_GREEN; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::yellow_bg_t)) {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_BG_YELLOW; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::blue_bg_t))   {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_BG_BLUE; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::magenta_bg_t)){(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_BG_MAGENTA; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::cyan_bg_t))   {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_BG_CYAN; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::white_bg_t))  {(*this).NoSpace()<<detail::DEBUG_STREAM_COLOR_BG_WHITE; return (*this).Space();}

  DebugStream &operator<<(void(*)(detail::debug_general_t))     {(*this).NoSpace()<<normal_fg<<normal_bg; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::debug_status_t))      {(*this).NoSpace()<<red_fg<<cyan_bg; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::debug_warning_t))     {(*this).NoSpace()<<green_fg<<yellow_bg; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::debug_error_t))       {(*this).NoSpace()<<normal_fg<<blue_bg; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::debug_fatal_error_t)) {(*this).NoSpace()<<normal_fg<<red_bg; return (*this).Space();}

  //======================================
  #if 1
  template <typename T>
  DebugStream& operator<<(const T& value) {
    pp.print(value);
    if(!pprint_stream.str().empty()) {
      // std::cout <<  pprint_stream.str();
      printf(pprint_stream.str().c_str());
      MayBeSpace();
      pprint_stream.str("");
    }
    return *this;
  }
  DebugStream& operator<<(DebugStream& stream) {
    stream.newline = false;
    return *this;
  }
  #else
  template <typename T,typename D=typename std::enable_if<!std::is_same<std::decay<DebugStream>,std::decay<T>>::value,T>::type>
  DebugStream& operator<<(T&& value) {
    pp.print(value);
    if(!pprint_stream.str().empty()) {
      // std::cout <<  pprint_stream.str();
      printf(pprint_stream.str().c_str());
      MayBeSpace();
      pprint_stream.str("");
    }
    return *this;
  }
  DebugStream& operator<<(DebugStream& stream) {
    stream.newline = false;
    return *this;
  }
  #endif
  //======================================
 public:
  DebugStream &printf(const char *fmt, ...);
  inline DebugStream &operator()() {
     return *this;
  }
  inline DebugStream& operator()(const char* varName) {
    // (*this).printf(varName);
    (*this) << varName;
    return *this;
  }
  template <typename T>
  inline DebugStream& operator()(const char* varName, const T& value) {
    (*this) << varName << "=" << value;
    return *this;
  }
  inline DebugStream& operator()(const char* varName, const char* value) {
    (*this) << value;
    return *this;
  }
  template <typename... Args>
  inline DebugStream& operator()(const char* varName, const char* fmt, Args... args) {
    (*this).printf(fmt,args...);
    MayBeSpace();
    return *this;
  }
  //======================================
 private:
  inline DebugStream &MayBeSpace() {
    if (!newline_&&space) {
      // (*this)(" ");
      printf(" ");
      // pprint_stream<<" ";
    } 
    newline_ = false;
    return *this;
  }
  //======================================
 private:
  std::function<void(const std::string&)> fun;
  int buf_len;
  std::unique_ptr<char[]> buffer;
  bool out_en{true};
  bool space{true};
  bool newline{false};
  bool newline_{false}; // solve the bug that add newline still add space
  bool terminate{false}; // if true will terminate program if true Use for debug error info
  bool clear_color{false}; // if true will clear color when deconstruct object
 private:
  std::stringstream pprint_stream;
  pprint::PrettyPrinter pp;
};

inline DebugStream::DebugStream(std::function<void(const std::string&)> fun_,
                                int buf_len_)
    : pp(pprint_stream) {
  fun = fun_;
  buf_len = buf_len_;
  buffer = std::unique_ptr<char[]>(new char[buf_len]);
}

inline DebugStream::DebugStream(const DebugStream &obj) {
  this->buf_len = obj.buf_len;
  this->out_en = obj.out_en;
  this->fun = obj.fun;
  this->buffer = std::unique_ptr<char[]>(new char[buf_len]);
}

inline DebugStream::DebugStream(DebugStream &&obj) {
  this->buf_len = obj.buf_len;
  this->out_en = obj.out_en;
  this->fun = obj.fun;
  buffer=std::move(obj.buffer);
}

inline DebugStream &DebugStream::operator=(const DebugStream &obj) {
  if (this != &obj) {
    this->buf_len = obj.buf_len;
    this->out_en = obj.out_en;
    this->fun = obj.fun;
    this->buffer = std::unique_ptr<char[]>(new char[buf_len]);
  }
  return *this;
}

inline DebugStream &DebugStream::operator=(DebugStream &&obj) {
  if (this != &obj) {
    this->buf_len = obj.buf_len;
    this->out_en = obj.out_en;
    this->fun = obj.fun;
    this->buffer = std::move(obj.buffer);
  }
  return *this;
}

inline DebugStream::~DebugStream() {
  if(terminate) {
    (*this)<<normal_fg<<normal_bg; // mandatory clear the color
    (*this)("\n"); // mandatory put a new line in case the error not output
    std::terminate();
  }
  if(buffer==nullptr) return; // If buffer is nullptr, then cannot use print
  if(clear_color) (*this)<<normal_fg<<normal_bg; 
  if(newline) (*this)("\n"); // send a "\n"
}

inline DebugStream &DebugStream::printf(const char *fmt, ...) {
#ifdef G_CONFIG_NO_DEBUG_OUTPUT
  return *this;
#endif // G_CONFIG_NO_DEBUG_OUTPUT
  if (!this->out_en) {
    return *this;
  }

  va_list ap;
  va_start(ap, fmt);
  std::vsprintf((char *) buffer.get() , fmt, ap);
  int size = std::strlen(buffer.get());
  va_end(ap);

  fun(buffer.get());

  // solve the bug that add newline still add space
  if(buffer.get()[size-1]==0X0A||buffer.get()[size-1]==0X0D) {
    newline_= true;
  }

  return *this;
}

namespace detail{
// FILE_LINE
inline std::string FileLine(const std::string& file_name="",int line_num=-1) {
  std::string res;
  if(line_num<0) {
    res = file_name;
  } else if (file_name=="") {
    res = std::to_string(line_num);
  } else {
    res = file_name+":"+std::to_string(line_num);
  }
  return res;
}
}
#define G_FILE_LINE gxt::detail::FileLine(__FILE__, __LINE__)
#define G_FILE gxt::detail::FileLine(__FILE__, -1)
#define G_LINE gxt::detail::FileLine("", __LINE__)


namespace detail {
// Type Name Implement
template <typename T>
inline std::string TypeImpl() {
#ifdef _MSC_VER // msvc support
  std::string str=__FUNCSIG__;
  auto posi_start = str.find(",");
  posi_start += 1;
  auto posi_end=str.find_first_of(">",posi_start);
#else // gcc and clangd
  std::string str=__PRETTY_FUNCTION__;
  auto posi_start = str.find("T = ");
  posi_start += 4;
  auto posi_end=str.find_first_of(";",posi_start);
#endif
  return str.substr(posi_start,posi_end-posi_start);
}
}
#define TYPET(type) (gxt::detail::TypeImpl<type>())
#define TYPE(type) (gxt::detail::TypeImpl<decltype(type)>())

namespace g_var {

#define COUNT_ARGS_IMPL(_null, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(0 __VA_OPT__(, ) __VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define VARIMP(x) #x << "=" << x

#define G_VAR0() ""
#define G_VAR1(_1) VARIMP(_1)
#define G_VAR2(_1, _2) VARIMP(_1) << "," << G_VAR1(_2)
#define G_VAR3(_1, _2, _3) VARIMP(_1) << "," << G_VAR2(_2, _3)
#define G_VAR4(_1, _2, _3, _4) VARIMP(_1) << "," << G_VAR3(_2, _3, _4)
#define G_VAR5(_1, _2, _3, _4, _5) VARIMP(_1) << "," << G_VAR4(_2, _3, _4, _5)
#define G_VAR6(_1, _2, _3, _4, _5, _6) VARIMP(_1) << "," << G_VAR5(_2, _3, _4, _5, _6)
#define G_VAR7(_1, _2, _3, _4, _5, _6, _7) \
  VARIMP(_1) << "," << G_VAR6(_2, _3, _4, _5, _6, _7)
#define G_VAR8(_1, _2, _3, _4, _5, _6, _7, _8) \
  VARIMP(_1) << "," << G_VAR7(_2, _3, _4, _5, _6, _7, _8)
#define G_VAR9(_1, _2, _3, _4, _5, _6, _7, _8, _9) \
  VARIMP(_1) << "," << G_VAR8(_2, _3, _4, _5, _6, _7, _8, _9)
#define G_VAR10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
  VARIMP(_1) << "," << G_VAR9(_2, _3, _4, _5, _6, _7, _8, _9, _10)

#define G_VARHELPIMP(N, ...) G_VAR##N(__VA_ARGS__)
#define G_VARHELP(N, ...) G_VARHELPIMP(N __VA_OPT__(, ) __VA_ARGS__)

// Usage: gDebug() << VAR(a,b) // stdout: a = ${a} , b = ${b}
#define VAR(...) G_VARHELP(COUNT_ARGS(__VA_ARGS__) __VA_OPT__(, ) __VA_ARGS__)

}  // namespace g_var

// Define a macro to get the parameter at the specified position in the parameter list
#define GET_ARG(N, ...) GET_ARG_##N (__VA_ARGS__)
#define GET_ARG_1(arg, ...) arg
#define GET_ARG_2(arg, ...) GET_ARG_1(__VA_ARGS__) 
#define GET_ARG_3(arg, ...) GET_ARG_2(__VA_ARGS__) 
#define GET_ARG_4(arg, ...) GET_ARG_3(__VA_ARGS__) 
#define GET_ARG_5(arg, ...) GET_ARG_4(__VA_ARGS__) 


namespace detail{
// Get the number of input parameters
template <typename ...T>
__attribute__((deprecated))
inline int GetParaNumber(T ...para){ return sizeof...(para); }

// Prevent input null parameter
template <typename T,T value>
__attribute__((deprecated))
inline T PreventNULL(const T& para){ return para;}
template <typename T,T value>
__attribute__((deprecated))
inline T PreventNULL(){return value;}
}

} // namespace gxt

#define gDebugN(a,...) \
    [=](int max_print_cnt = -1,int cnt = 1) { \
      static int cnt_i{cnt}; \
      static int max_print_i{0}; \
      if(max_print_cnt>=0&&max_print_i>=max_print_cnt) return; \
      if (cnt_i++ >= cnt) { \
        gDebug(a); \
        cnt_i = 1; \
        ++max_print_i; \
      } \
    }(__VA_ARGS__)

#define gDebugCol(col_fg,col_bg,...) ((gxt::DebugStream(gxt::DebugSendStringCallBack_Default_).NewLine().ClearColor()<<col_fg<<col_bg)(#__VA_ARGS__ __VA_OPT__(,) __VA_ARGS__))

#define gDebug(...) gDebugCol(gxt::normal_fg,gxt::normal_bg,##__VA_ARGS__)
#define gDebugWarn(...) gDebugCol(gxt::black_fg,gxt::yellow_bg,##__VA_ARGS__)
#define gDebugError(...) gDebugCol(gxt::white_fg,gxt::red_bg,##__VA_ARGS__).Terminate()
#define gDebugCol1(...) gDebugCol(gxt::green_fg,gxt::normal_bg,##__VA_ARGS__)
#define gDebugCol2(...) gDebugCol(gxt::blue_fg,gxt::normal_bg,##__VA_ARGS__)
#define gDebugCol3(...) gDebugCol(gxt::magenta_fg,gxt::normal_bg,##__VA_ARGS__)
#define gDebugCol4(...) gDebugCol(gxt::cyan_fg,gxt::normal_bg,##__VA_ARGS__)
#define gDebugCol5(...) gDebugCol(gxt::red_fg,gxt::normal_bg,##__VA_ARGS__)


// Print split line such as "=============================="
namespace gxt {
namespace detail {
class SplitLine {
 public:
  std::string operator()(std::string str="", size_t size = size_,char c=default_char_) const {
    size_t len=str.size();
    if(len>size) return "";
    size_t left_len=(size-len)/2;
    std::string ret;
    ret=std::string(left_len,c)+str+std::string((size-left_len-len),c);
    return ret;
  }
  friend std::ostream& operator<<(std::ostream& os, const SplitLine& obj) {
    std::string res;
    res = obj.operator()();
    os << res;
    return os;
  }
 private:
  static const int size_ = 30;
  static const char default_char_ = '=';
};
}  // namespace detail
}  // namespace gxt

// Print Restricted Cnt
namespace gxt {
namespace detail {
class PrintCnt {
 public:
  /*
   * max_print_cnt : 最大打印次数
   * print_interval : 每print_interval次打印一次
   * terminate : 达到最大打印次数是否终止程序（方便调试）
   */
  PrintCnt& operator()(int max_print_cnt = -1, size_t print_interval = 1,
                       bool terminate = false) {
    max_print_cnt_ = max_print_cnt;
    print_interval_ = print_interval;
    terminate_ = terminate;
    if (!exec_once) {
      exec_once = true;
      cnt_interval = print_interval_;
    }
    return *this;
  }
  friend gxt::DebugStream& operator<<(gxt::DebugStream& os, PrintCnt& obj) {
    if (obj.max_print_cnt_ >= 0 && obj.cnt >= obj.max_print_cnt_) {
      if (obj.terminate_) os.Terminate();
      os.OutEn(false);
      return os;
    }
    if (obj.cnt_interval++ >= obj.print_interval_) {
      os.OutEn(true);
      ++(obj.cnt);
      if (obj.max_print_cnt_ >= 0 && obj.cnt >= obj.max_print_cnt_) {
        if (obj.terminate_) os.Terminate();
      }
      obj.cnt_interval = 1;
    } else {
      os.OutEn(false);
    };
    return os;
  }

 private:
  bool exec_once = false;
  int cnt = 0;
  size_t cnt_interval = 1;
  int max_print_cnt_ = -1;
  size_t print_interval_ = 1;
  bool terminate_ = false;
};
}  // namespace detail
}  // namespace gxt

#define G_PRINT_CNT []()->gxt::detail::PrintCnt& \
  {static gxt::detail::PrintCnt print_cnt; return print_cnt;}()

#define G_SPLIT_LINE gxt::detail::SplitLine()

namespace gxt {
namespace detail {
// print class attributes
template <class T, class D = typename std::enable_if<std::is_class<T>::value, T>::type>
[[nodiscard]] inline std::string ClassDetail(std::string class_name) {
  // static_assert(std::is_same<T,D>::value,"T must be class type");

  std::stringstream str;
  str << std::boolalpha;
  str << "\n";

#define CLASS_DETAIL_SPLIT(str_) str << G_SPLIT_LINE(str_, 44) << "\n";

#define CLASS_DETAIL_VAR(tt)                                       \
  str << std::setw(35) << std::left << #tt << "| " << std::setw(5) \
      << std::tt<T>() << " |\n"

  CLASS_DETAIL_SPLIT(">")
  str << "class name: " << class_name << "\n";
  CLASS_DETAIL_SPLIT("=")

  CLASS_DETAIL_VAR(is_constructible);
  CLASS_DETAIL_VAR(is_trivially_constructible);
  CLASS_DETAIL_VAR(is_nothrow_constructible);

  CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(is_default_constructible);
  CLASS_DETAIL_VAR(is_trivially_default_constructible);
  CLASS_DETAIL_VAR(is_nothrow_default_constructible);

  CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(is_copy_constructible);
  CLASS_DETAIL_VAR(is_trivially_copy_constructible);
  CLASS_DETAIL_VAR(is_nothrow_copy_constructible);

  CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(is_move_constructible);
  CLASS_DETAIL_VAR(is_trivially_move_constructible);
  CLASS_DETAIL_VAR(is_nothrow_move_constructible);

  CLASS_DETAIL_SPLIT("-")

  // CLASS_DETAIL_VAR(is_assignable);
  // CLASS_DETAIL_VAR(is_trivially_assignable);
  // CLASS_DETAIL_VAR(is_nothrow_assignable);
  //
  // CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(is_copy_assignable);
  CLASS_DETAIL_VAR(is_trivially_copy_assignable);
  CLASS_DETAIL_VAR(is_nothrow_copy_assignable);

  CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(is_move_assignable);
  CLASS_DETAIL_VAR(is_trivially_move_assignable);
  CLASS_DETAIL_VAR(is_nothrow_move_assignable);

  CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(is_destructible);
  CLASS_DETAIL_VAR(is_trivially_destructible);
  CLASS_DETAIL_VAR(is_nothrow_destructible);

  CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(has_virtual_destructor);

  #if SUPPORTS_CPP17
  // CLASS_DETAIL_SPLIT("-")
  // CLASS_DETAIL_VAR(is_swappable_with);
  CLASS_DETAIL_VAR(is_swappable);
  // CLASS_DETAIL_VAR(is_nothrow_swappable_with);
  CLASS_DETAIL_VAR(is_nothrow_swappable);
  #endif

  CLASS_DETAIL_SPLIT("=")
  CLASS_DETAIL_SPLIT("<")

#undef CLASS_DETAIL_VAR
#undef CLASS_DETAIL_SPLIT

  return str.str();
}

}  // namespace detail
}  // namespace gxt

#define CLASS(type) gxt::detail::ClassDetail<type>(#type)

#include <chrono>
#include <thread>

// time lib
namespace gxt{
#if !defined(G_CONFIG_TIME_COLOR_NAME)
#define G_CONFIG_TIME_COLOR_NAME gDebugCol3
#endif
#if !defined(G_CONFIG_TIME_COLOR_TIME)
#define G_CONFIG_TIME_COLOR_TIME gDebugCol4
#endif
namespace detail {

class TimeCount {
  using time_type = std::chrono::high_resolution_clock::time_point;
 public:
  TimeCount(std::string str, time_type begin) {
    name_ = str;
    start_time_ = begin;
  }
  ~TimeCount() {
    if (print_) {
      auto end_time = std::chrono::high_resolution_clock::now();
      auto duration_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time_).count();
      std::string name;
      std::string time;
      if (std::string(name_).empty()) name += std::string("Default");
      else time += name_;
      time += " Time: " + std::to_string(duration_time) + " ms";
      G_CONFIG_TIME_COLOR_NAME().NoSpace() << name << G_CONFIG_TIME_COLOR_TIME() << time;
    }
  }
  time_type GetStartTime() const{ return start_time_; };
  void SetNotPrint() { print_=false; };
 private:
  std::string name_;
  bool print_{true};
  time_type start_time_;
};

}


// 定义宏 TIME_FUNCTION 来计算一个函数耗时
#define TIME_FUNCTION(func) \
 [&]()->decltype(func){ \
    auto start = std::chrono::high_resolution_clock::now(); \
    decltype(func) result = func; \
    auto end = std::chrono::high_resolution_clock::now(); \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); \
    std::string name; \
    std::string time; \
    name += std::string(#func); \
    time += std::string(" Time: ") + std::to_string(duration) + " ms"; \
    G_CONFIG_TIME_COLOR_NAME().NoSpace() << name << G_CONFIG_TIME_COLOR_TIME() << time; \
    return result; \
}()

// 定义宏 TIME_BEGIN 来开始计时(如果不执行TIME_END，就会在析构时自动输出时间)
#define TIME_BEGIN(...) \
  std::unique_ptr<gxt::detail::TimeCount>  __time_count_##__VA_ARGS__= \
  std::unique_ptr<gxt::detail::TimeCount>(new gxt::detail::TimeCount(#__VA_ARGS__,std::chrono::high_resolution_clock::now()));

// 定义宏 TIME_END 来打印输出执行时间
#define TIME_END(...) \
  __time_count_##__VA_ARGS__.reset();

// 定义宏 TIME_CODE 来开始计算代码执行时间
#define TIME_CODE(...) \
  [&](){ \
    auto __start_time__code__ = std::chrono::high_resolution_clock::now(); \
    __VA_ARGS__; \
    auto __end_time__code__ = std::chrono::high_resolution_clock::now(); \
    auto __duration_time__code__ = std::chrono::duration_cast<std::chrono::milliseconds>(__end_time__code__ - __start_time__code__).count(); \
    std::string name; \
    std::string time; \
    if(std::string(#__VA_ARGS__).empty()) name=""; \
    else \
    name = std::string(#__VA_ARGS__); \
    time += std::string(" Time: ") + std::to_string(__duration_time__code__) + " ms"; \
    G_CONFIG_TIME_COLOR_NAME().NoSpace() << name << G_CONFIG_TIME_COLOR_TIME() << time; \
  }();

// 定义宏 TIME_LOOP 来计算一个循环耗时
#define TIME_LOOP(...) \
  []()->std::string{ \
    static auto __time_loop_begin_##__VA_ARGS__ = std::chrono::high_resolution_clock::now(); \
    static size_t __time_loop_i__##__VA_ARGS__ = 0; \
    auto __time_loop_end_##__VA_ARGS__ = std::chrono::high_resolution_clock::now(); \
    auto __loop_duration_time__##__VA_ARGS__ = std::chrono::duration_cast<std::chrono::milliseconds>(__time_loop_end_##__VA_ARGS__ - __time_loop_begin_##__VA_ARGS__).count(); \
    __time_loop_begin_##__VA_ARGS__=__time_loop_end_##__VA_ARGS__;\
    std::string name; \
    std::string time; \
    if(__time_loop_i__##__VA_ARGS__==0) { \
      name= std::string("TIME_LOOP(") + #__VA_ARGS__ + "): " + std::to_string(__time_loop_i__##__VA_ARGS__); \
      time= std::string(" Time: ")  + "initialize"; \
    } else { \
      name= std::string("TIME_LOOP(") + #__VA_ARGS__ + "): "+std::to_string(__time_loop_i__##__VA_ARGS__); \
      time = std::string(" Time: ") + std::to_string(__loop_duration_time__##__VA_ARGS__) + " ms"; \
    } \
    ++__time_loop_i__##__VA_ARGS__; \
    G_CONFIG_TIME_COLOR_NAME().NoSpace() << name << G_CONFIG_TIME_COLOR_TIME() << time; \
    return name+time; \
  }()

inline void Sleep(std::int64_t time) {
  std::this_thread::sleep_for(std::chrono::seconds(time));  
}
inline void SleepMs(int64_t time) {
  std::this_thread::sleep_for(std::chrono::milliseconds(time));  
}
inline void SleepUs(int64_t time) {
  std::this_thread::sleep_for(std::chrono::microseconds(time));  
}
inline void SleepNs(int64_t time) {
  std::this_thread::sleep_for(std::chrono::nanoseconds(time));  
}

inline long GetTime() {
  auto time = std::chrono::duration_cast<std::chrono::seconds>(
                  std::chrono::system_clock::now().time_since_epoch())
                  .count();
  return time;
}
inline long GetTimeMs() {
  auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::system_clock::now().time_since_epoch())
                  .count();
  return time;
}
inline long GetTimeUs() {
  auto time = std::chrono::duration_cast<std::chrono::microseconds>(
                  std::chrono::system_clock::now().time_since_epoch())
                  .count();
  return time;
}
inline long GetTimeNs() {
  auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                  std::chrono::system_clock::now().time_since_epoch())
                  .count();
  return time;
}

}


// random number lib
#include <cstdlib>
#include <random>
namespace gxt {

// get random number method : pool scalability
/* 
template <typename T=int>
inline typename std::enable_if<std::is_integral<T>::value == true, T>::type
Random(T min = std::numeric_limits<T>::min(),
       T max = std::numeric_limits<T>::max()) {
  if (min > max) {
    throw std::invalid_argument("Invalid range: min > max");
  }
  std::random_device rd;
  std::mt19937 gen(0);
  std::uniform_int_distribution<T> dist(min, max);
  return dist(gen);
}

template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value == true, T>::type
Random(T min = std::numeric_limits<T>::min(),
       T max = std::numeric_limits<T>::max()) {
  if (min > max) {
    throw std::invalid_argument("Invalid range: min > max");
  }
  std::random_device rd;
  std::mt19937 gen(0);
  std::uniform_real_distribution<T> dist(min, max);
  return dist(gen);
}
*/

namespace detail{
template <typename T>
struct RandomTypeTraits {
  static T Min() { return std::numeric_limits<T>::min(); }
  static T Max() { return std::numeric_limits<T>::max(); }

  template <typename U>
  typename std::enable_if<std::is_integral<U>::value == true, U>::type
  GetValImpl_(U min, U max) {
    std::uniform_int_distribution<U> dist(min, max);
    return dist(gen_);
  }
  template <typename U>
  typename std::enable_if<std::is_floating_point<U>::value == true, U>::type
  GetValImpl_(U min, U max) {
    std::uniform_real_distribution<U> dist(min, max);
    return dist(gen_);
  }

  T GetVal(T min, T max) { return GetValImpl_<T>(min, max); }
  RandomTypeTraits(std::mt19937& gen) : gen_(gen){};
  std::mt19937& gen_;
};

template <>
struct RandomTypeTraits<bool> {
  static double Min() { return 0.0; }
  static double Max() { return 1.0; }
  using DistributionType = std::uniform_real_distribution<double>;
  bool GetVal(bool min, bool max) {
    DistributionType dist(Min(), Max());
    return dist(gen_) > 0.5;
  }
  RandomTypeTraits(std::mt19937& gen) : gen_(gen){};
  std::mt19937& gen_;
};

inline std::mt19937& GenerateRandomGen(
    unsigned int value = std::numeric_limits<unsigned int>::max()) {
  static std::random_device rd;
  // static std::mt19937 gen(0);
  static std::mt19937 gen(rd());
  return gen;
}
}

template <typename T = int, int rd_val = -1>
inline T Random(T min = gxt::detail::RandomTypeTraits<T>::Min(),
                T max = gxt::detail::RandomTypeTraits<T>::Max()) {
  if (min > max) {
    throw std::invalid_argument("Invalid range: min > max");
  }
  if (rd_val==-1) {
    return gxt::detail::RandomTypeTraits<T>(gxt::detail::GenerateRandomGen()).GetVal(min, max);
  } else {
    static std::mt19937 gen(rd_val);
    return gxt::detail::RandomTypeTraits<T>(gen).GetVal(min, max);
  }
}

}  // namespace gxt

// string function
namespace gxt{
inline std::string PadStringToDesignChars(const std::string& input, size_t n = 8) {
  // 检查原始字符串的长度
  if (input.length() >= n) {
    return input.substr(0, n);  // 只保留前n个字符
  }
  // 计算需要补全的字符数
  int padding_cnt = n - input.length();
  // 计算前半部分和后半部分的补全字符数
  int left_pad_cnt = padding_cnt / 2;
  int right_pad_cnt = padding_cnt - left_pad_cnt;
  // 构建补全后的字符串
  std::string padded_str;
  padded_str.reserve(n);  // 预分配空间以提高性能
  // 添加前半部分的补全字符
  padded_str.append(left_pad_cnt, ' ');
  // 添加原始字符串
  padded_str += input;
  // 添加后半部分的补全字符
  padded_str.append(right_pad_cnt, ' ');
  return padded_str;
}
}

namespace gxt{
namespace algorithm{

/*Print Tree
  Example:
  std::string res = PrintTree(
      root, [](Node* node){ return node->left; },
      [](Node* node){ return node->right; },
      [](Node* node){ return node->value; });
  gDebugCol1() << res;
*/
template <typename T, typename F_LEFT, typename F_RIGHT, typename F_VAL>
inline std::string PrintTree(T* node, F_LEFT f_left, F_RIGHT f_right, F_VAL f_val,
                      int level = 0, char branch = ' ') {
  if (node == nullptr) return "";
  std::string res;

  res += PrintTree(f_right(node), f_left, f_right, f_val, level + 1, '/');
  for (int i = 0; i < level; i++) {
    res += "    ";
  }
  res += (branch + std::string("--") + std::to_string(node->key) + "\n");
  res += PrintTree(f_left(node), f_left, f_right, f_val, level + 1, '\\');
  return res;
}

template <typename T, typename F_LEFT, typename F_RIGHT, typename F_VAL>
inline std::vector<std::vector<std::string>> CreateBinaryTreeStructure(T* root,
                                                                F_LEFT f_left,
                                                                F_RIGHT f_right,
                                                                F_VAL f_val) {
  std::vector<std::vector<std::string>> treeStructure;
  if (root == nullptr) {
    return treeStructure;
  }

  std::vector<T*> currentLevel;
  std::vector<T*> nextLevel;
  currentLevel.push_back(root);

  while (!currentLevel.empty()) {
    std::vector<std::string> levelValues;

    bool null = true;
    for (const auto& node : currentLevel) {
      if (node != nullptr) {
        null = false;
        break;
      }
    }
    if (null == true) break;

    for (const auto& node : currentLevel) {
      if (node == nullptr) {
        levelValues.push_back("NULL");
        nextLevel.push_back(nullptr);
        nextLevel.push_back(nullptr);
        continue;
      }
      levelValues.push_back(std::to_string(f_val(node)));
      nextLevel.push_back(f_left(node));
      nextLevel.push_back((node->right));
    }
    treeStructure.push_back(levelValues);
    currentLevel = nextLevel;
    nextLevel.clear();
  }

  return treeStructure;
}

/* Example: 
 DrawTree(gxt::algorithm::CreateBinaryTreeStructure(root, [](Node* node){ return node->left; },
      [](Node* node){ return node->right; },
      [](Node* node){ return node->value; }));
*/
inline void DrawTree(const std::vector<std::vector<std::string>>& tree, size_t pad = 6,
              size_t space = 2) {
  for (size_t i = 0; i < tree.size(); i++) {
    std::cout << std::string(pad * (tree.size() - i - 1), ' ');
    for (const auto& val : tree.at(i)) {
      std::cout << gxt::PadStringToDesignChars(val, pad)
                << std::string(space, ' ');
    }
    std::cout << "\n";
  }
}
}
}

#endif //DEBUGSTREAM_H__

