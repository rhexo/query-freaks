#ifndef _SQL_SELECT_HPP
#define _SQL_SELECT_HPP

/** 
  * Copyright 2015 Maxim Musolov
  * This code distribute under private license
  */


#include <string>
#include <type_traits>
#include <sql/sql_base.hpp>

namespace sql {

  /** define sql::field */
  template <class Entry>
  struct field
  {
  public:

    field() {}

    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return Entry()(b);
    }
    
    std::string operator()() {      
      return Entry()();
    }
  };

  namespace helper {

    /** select impl - helper class */
    template <std::size_t N, class... Fields>
    struct impl_select_part;

    /** add space at the end of expression */
    template <std::size_t N>
    struct impl_select_part<N> {
      static constexpr size_t value = N;
    
      // operator implementation for specialization
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string(" ");
      }

      std::string operator()() {
        return std::string(" ");
      }

    };
    
    /** mid expression for fields order */
    template <std::size_t N, class F, class ...Fields>
    struct impl_select_part<N,F,Fields...>
    {
      static constexpr size_t value = N;
    
      /** expression contains query parameters */
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string(", ") + F()(b) + impl_select_part<N+1, Fields...>()(b);
      }
      
      /** expression without parameters */
      std::string operator()() {
        return std::string(", ") + F()() + impl_select_part<N+1, Fields...>()();
      }

    };
  
    /** first entry of selecting fields order */
    template <class F, class ...Fields>
    struct impl_select_part<0,F,Fields...>
    {
      static constexpr size_t value = 0;
    
      // operator implementation of first entry
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string("SELECT ") + F()(b) + impl_select_part<1, Fields...>()(b);
      }
      
      /** expression without query parameters */
      std::string operator()() {
        return std::string("SELECT ") + F()() + impl_select_part<1, Fields...>()();
      }

    };
  };

  /** define sql::select */
  template <typename ...Fields>
  struct select
  {
    public:
    
    select() {}
    
    /** implement select directive */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return helper::impl_select_part<0,Fields...>()(b);
    }       
    
    /** expression without query parameters */
    std::string operator()() {      
      return helper::impl_select_part<0,Fields...>()();
    }       

  };
   
};

#endif
