#ifndef _SQL_INSERT_HPP
#define _SQL_INSERT_HPP

/** 
 * Copyright 2015 Maxim Musolov
 * This code distribute under private license
 */

#include <string>

#include <query-freaks/sql_base.hpp>

namespace sql {

  

  namespace helper {

    // INTO section <table, fields...>

    /** select impl - helper class */
    template <class Table, std::size_t N, class... Fields>
    struct impl_into_part;

    /** add space at the end of expression */
    template <class Table, std::size_t N>
    struct impl_into_part<Table,N> {
      static constexpr size_t value = N;
    
      // operator implementation for specialization
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string(") ");
      }

      std::string operator()() {
        return std::string(") ");
      }

    };
    
    /** mid expression for fields order */
    template <class Table, std::size_t N, class F, class ...Fields>
    struct impl_into_part<Table,N,F,Fields...>
    {
      static constexpr size_t value = N;
    
      /** expression contains query parameters */
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string(", ") + F()(b) + impl_into_part<Table, N+1, Fields...>()(b);
      }
      
      /** expression without parameters */
      std::string operator()() {
        return std::string(", ") + F()() + impl_into_part<Table, N+1, Fields...>()();
      }

    };
  
    /** first entry of selecting fields order */
    template <class Table, class F, class ...Fields>
    struct impl_select_part<Table,0,F,Fields...>
    {
      static constexpr size_t value = 0;
    
      // operator implementation of first entry
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string("INTO ") + Table()(b) + std::string(" (") +  F()(b) + impl_into_part<Table, 1, Fields...>()(b);
      }
      
      /** expression without query parameters */
      std::string operator()() {
        return std::string("INTO ")  + Table()() + std::string(" (") + F()() + impl_into_part<Table, 1, Fields...>()();
      }

    };
  };

  /** define sql::into */
  template <typename Table, typename ...Fields>
  struct into
  {
    public:
    
    into() {}
    
    /** implement select directive */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return helper::impl_into_part<Table,0,Fields...>()(b);
    }       
    
    /** expression without query parameters */
    std::string operator()() {      
      return helper::impl_into_part<Table,0,Fields...>()();
    }       

  };


  /** define sql::into */
  template <typename Table>
  struct into
  {
    public:
    
    into() {}
    
    /** implement select directive */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return std::string("INTO ") + Table()(b) + std::string(" ");
    }       
    
    /** expression without query parameters */
    std::string operator()() {      
      return std::string("INTO ") + Table()() + std::string(" ");
    }       

  };


  // VALUES section <fields...>...


  // INSERT section <INTO<...>, VALUES<...> >


};

#endif
