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

#pragma once


// Reference : https://github.com/p-ranav/pprint

// There will no debug stream output if define the G_CONFIG_NO_DEBUG_OUTPUT
// #define G_CONFIG_NO_DEBUG_OUTPUT

#include "stdc++.h"
#include "definehelper.h"

#include "detail/time.h"
#include "detail/debugwaiting.h"
#include "detail/random.h"
#include "detail/logfile.h"
#include "detail/bitset.h"
#include "detail/debughelper.h"
#include "detail/type.h"
#include "detail/color.h"
#include "detail/enum.h"
#include "detail/leetcode.h"
#include "detail/leetcode_tree.h"
#include "detail/leetcode_list.h"

#define MY_LIB_VERSION TO_STRING(MY_LIBRARY_VERSION)
#define MY_LIB_VERSION_MAJOR MY_LIBRARY_VERSION
#define MY_LIB_VERSION_MINOR MY_LIBRARY_VERSION_MINOR
#define MY_LIB_VERSION_PATCH MY_LIBRARY_VERSION_MINOR


namespace gxt {

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
    // typename std::enable_if<std::is_null_pointer<T>::value == true, void>::type
    typename std::enable_if<std::is_same<std::nullptr_t, typename std::remove_cv<T>::type>::value == true, void>::type
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

    // gxt: support gDebug() << std::hex << 10;
    void print_internal(std::ios_base& (*value)(std::ios_base&), size_t indent = 0, const std::string& line_terminator = "\n", size_t level = 0) {
      stream_ << std::string(indent, ' ') << value << line_terminator;
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

// Set the endl compatible with std::endl;
namespace detail{
class DebugStreamEndl {};
}
inline void endl(detail::DebugStreamEndl){}


class DebugStream {
 public:
  //======================================
  explicit DebugStream(std::function<void(const std::string&)> func = nullptr)
      : func_(func), pp(pprint_stream) {
    if (func_ == nullptr) {
      func_ = [](const std::string& str) { std::cout << str; };
    }
  };
  DebugStream(const DebugStream &obj);
  DebugStream(DebugStream &&obj);
  DebugStream& operator=(const DebugStream &obj);
  DebugStream& operator=(DebugStream &&obj);
  ~DebugStream();
  //===============================================================
  inline DebugStream &OutEn(bool en)    {out_en_=en;       return *this;}
  inline DebugStream &NoNewLine()       {newline=false;   return *this;}
  inline DebugStream &NewLine()         {newline=true;    return *this;}
  inline DebugStream &Space()           {space_=true;      return *this;}
  inline DebugStream &NoSpace()         {space_=false;     return *this;}
  inline DebugStream &Terminate()       {terminate_=true;  return *this;}
  inline DebugStream &ClearColor()      {clear_color_=true;return *this;}
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

  DebugStream &operator<<(void(*)(detail::debug_general_t))     {(*this).NoSpace()<<detail::normal_fg<<detail::normal_bg; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::debug_status_t))      {(*this).NoSpace()<<detail::red_fg   <<detail::cyan_bg; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::debug_warning_t))     {(*this).NoSpace()<<detail::green_fg <<detail::yellow_bg; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::debug_error_t))       {(*this).NoSpace()<<detail::normal_fg<<detail::blue_bg; return (*this).Space();}
  DebugStream &operator<<(void(*)(detail::debug_fatal_error_t)) {(*this).NoSpace()<<detail::normal_fg<<detail::red_bg; return (*this).Space();}

  //======================================
  template <typename T>
  DebugStream& operator<<(const T& value) {
    pp.print(value);
    if(!pprint_stream.str().empty()) {
      print_string(pprint_stream.str());
      MayBeSpace();
      pprint_stream.str("");
    }
    return *this;
  }
  DebugStream& operator<<(DebugStream& stream) {
    stream.newline = false;
    return *this;
  }
  //======================================
 public:
  DebugStream &Output(const std::string& str) {
#ifdef G_CONFIG_NO_DEBUG_OUTPUT
  return *this;
#endif // G_CONFIG_NO_DEBUG_OUTPUT
    if (!this->out_en_) {
      return *this;
    }
    func_(str);
    return *this;
  }
  DebugStream &printf(const char *fmt, ...);
  DebugStream &print_string(const std::string& str) {
    Output(str);

    size_t size=str.size();
    // solve the bug that add newline still add space
    if(str[size-1]==0X0A||str[size-1]==0X0D) {
      newline_= true;
    }
    return *this;
  }
  inline DebugStream &operator()() {
     return *this;
  }
  inline DebugStream& operator()(const char* varName) {
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
  inline DebugStream& operator()(const char* varName, const char* fmt, Args&&... args) {
    (*this).printf(fmt,std::forward<Args>(args)...);
    MayBeSpace();
    return *this;
  }
  //======================================
 private:
  inline DebugStream &MayBeSpace() {
    if (!newline_&&space_) {
      print_string(" ");
    }
    newline_ = false;
    return *this;
  }
  //======================================
 private:
  std::function<void(const std::string&)> func_;
  bool out_en_{true};
  bool space_{true};
  bool newline{false};
  bool newline_{false}; // solve the bug that add newline still add space
  bool terminate_{false}; // if true will terminate program if true Use for debug error info
  bool clear_color_{false}; // if true will clear color when deconstruct object
 private:
  std::stringstream pprint_stream;
  pprint::PrettyPrinter pp;
};

inline DebugStream::~DebugStream() {
  if (terminate_) {
    (*this) << detail::normal_fg << detail::normal_bg;  // mandatory clear the color
    (*this)("\n");  // mandatory put a new line in case the error not output
    std::terminate();
  }
  if(clear_color_) (*this)<<detail::normal_fg<<detail::normal_bg; 
  if(newline) (*this)("\n"); // send a "\n"
}

inline DebugStream& DebugStream::printf(const char* fmt, ...) {

  va_list ap;
  va_start(ap, fmt);
  char buffer[1024];
  std::vsnprintf((char*)buffer, 1024, fmt, ap);
  int size = std::strlen(buffer);
  va_end(ap);

  Output(buffer);

  // solve the bug that add newline still add space
  if (buffer[size - 1] == 0X0A || buffer[size - 1] == 0X0D) {
    newline_ = true;
  }
  return *this;
}

} // namespace gxt

#define gDebugColFun(col_fg,col_bg,func,...) ((gxt::DebugStream(func).NewLine().ClearColor()<<col_fg<<col_bg)(#__VA_ARGS__ __VA_OPT__(,) __VA_ARGS__))
#define gDebug(...) gDebugColFun(gxt::detail::normal_fg,gxt::detail::normal_bg,nullptr,##__VA_ARGS__)
#define gDebugWarn(...) gDebugColFun(gxt::detail::black_fg,gxt::detail::yellow_bg,nullptr,##__VA_ARGS__)
#define gDebugError(...) gDebugColFun(gxt::detail::white_fg,gxt::detail::red_bg,nullptr,##__VA_ARGS__).Terminate()
#define gDebugCol1(...) gDebugColFun(gxt::detail::green_fg,gxt::detail::normal_bg,nullptr,##__VA_ARGS__)
#define gDebugCol2(...) gDebugColFun(gxt::detail::blue_fg,gxt::detail::normal_bg,nullptr,##__VA_ARGS__)
#define gDebugCol3(...) gDebugColFun(gxt::detail::magenta_fg,gxt::detail::normal_bg,nullptr,##__VA_ARGS__)
#define gDebugCol4(...) gDebugColFun(gxt::detail::cyan_fg,gxt::detail::normal_bg,nullptr,##__VA_ARGS__)
#define gDebugCol5(...) gDebugColFun(gxt::detail::red_fg,gxt::detail::normal_bg,nullptr,##__VA_ARGS__)



// #define gDebugCol(col_fg,col_bg) (gxt::DebugStream().NewLine().ClearColor()<<col_fg<<col_bg)
// #define gDebug(...) (gDebugCol(gxt::normal_fg,gxt::normal_bg) << VAR(__VA_ARGS__))
// #define gDebugWarn(...) (gDebugCol(gxt::black_fg,gxt::yellow_bg) << VAR(__VA_ARGS__))
// #define gDebugError(...) (gDebugCol(gxt::white_fg,gxt::red_bg).Terminate() << VAR(__VA_ARGS__))
// #define gDebugCol1(...) (gDebugCol(gxt::green_fg,gxt::normal_bg) << VAR(__VA_ARGS__))
// #define gDebugCol2(...) (gDebugCol(gxt::blue_fg,gxt::normal_bg) << VAR(__VA_ARGS__))
// #define gDebugCol3(...) (gDebugCol(gxt::magenta_fg,gxt::normal_bg) << VAR(__VA_ARGS__))
// #define gDebugCol4(...) (gDebugCol(gxt::cyan_fg,gxt::normal_bg) << VAR(__VA_ARGS__))
// #define gDebugCol5(...) (gDebugCol(gxt::red_fg,gxt::normal_bg) << VAR(__VA_ARGS__))


// Print Restricted Cnt
namespace gxt{
namespace detail{
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
    if (!exec_once_) {
      exec_once_ = true;
      cnt_interval_ = print_interval_;
    }
    return *this;
  }
  friend gxt::DebugStream& operator<<(gxt::DebugStream& os, PrintCnt& obj) {
    if (obj.max_print_cnt_ >= 0 && obj.cnt_ >= obj.max_print_cnt_) {
      if (obj.terminate_) os.Terminate();
      os.OutEn(false);
      return os;
    }
    if (obj.cnt_interval_++ >= obj.print_interval_) {
      os.OutEn(true);
      ++(obj.cnt_);
      if (obj.max_print_cnt_ >= 0 && obj.cnt_ >= obj.max_print_cnt_) {
        if (obj.terminate_) os.Terminate();
      }
      obj.cnt_interval_ = 1;
    } else {
      os.OutEn(false);
    };
    return os;
  }

 private:
  bool exec_once_ = false;
  int cnt_ = 0;
  size_t cnt_interval_ = 1;
  int max_print_cnt_ = -1;
  size_t print_interval_ = 1;
  bool terminate_ = false;
};
}
}
#define G_PRINT_CNT                         \
  []() -> gxt::detail::PrintCnt& {          \
    static gxt::detail::PrintCnt print_cnt; \
    return print_cnt;                       \
  }()

// rename fmt::format to gxt::format
#define FMT_HEADER_ONLY
#include "detail/fmt/format.h"
namespace gxt {
template <typename... T>
FMT_NODISCARD FMT_INLINE auto format(fmt::format_string<T...> fmt, T&&... args)
    -> std::string {
  return vformat(fmt, fmt::make_format_args(args...));
}

template <typename Locale, typename... T,
          FMT_ENABLE_IF(fmt::detail::is_locale<Locale>::value)>
inline auto format(const Locale& loc, fmt::format_string<T...> fmt, T&&... args)
    -> std::string {
  return fmt::vformat(loc, string_view(fmt), fmt::make_format_args(args...));
}
}  // namespace gxt
