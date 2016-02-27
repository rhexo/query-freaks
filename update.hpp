#ifndef _SQL_UPDATE_HPP
#define _SQL_UPDATE_HPP

/** 
 * Copyright 2015 Maxim Musolov
 * This code distribute under private license
 */

#include <string>

#include <query-freaks/sql_base.hpp>

namespace sql {

  template <typename Left, typename Right>
  struct pair
  {
  public:
    
    pair() {}

    /** Expression with parameters */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return Left()(b) + std::string(" = ") + Right()(b);
    }
    
    /** Empty parameter list */
    std::string operator()() {      
      return Left()() + std::string(" = ") + Right()();
    }
    
  };


  namespace helper {

  
    /** values impl - helper class */
    template <std::size_t N, class... Pairs >
    struct impl_pair_part;

    /** add space at the end of expression */
    template <std::size_t N>
    struct impl_pair_part<N> {
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
    template <std::size_t N, class F, class ...Pairs>
    struct impl_pair_part<N,F,Pairs...>
    {
      static constexpr size_t value = N;
    
      /** expression contains query parameters */
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string(", ") + F()(b) + impl_pair_part<N+1, Pairs...>()(b);
      }
      
      /** expression without parameters */
      std::string operator()() {
        return std::string(", ") + F()() + impl_pair_part<N+1, Pairs...>()();
      }

    };
  
    /** first entry of selecting fields order */
    template <class F, class ...Pairs>
    struct impl_pair_part<0,F,Pairs...>
    {
      static constexpr size_t value = 0;
    
      // operator implementation of first entry
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return  F()(b) + impl_pair_part<1, Pairs...>()(b);
      }
      
      /** expression without query parameters */
      std::string operator()() {
        return F()() + impl_pair_part<1, Pairs...>()();
      }

    };
  };


  template <class ...EqSet> // 1st dimension, fields(i) is the 2nd dimension
  struct set
  {
  public:
    
    set() {}

    /** Expression with parameters */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return std::string("SET ") + helper::impl_pair_part<0,EqSet...>()(b);
    }
    
    /** Empty parameter list */
    std::string operator()() {      
      return std::string("SET ") + helper::impl_pair_part<0,EqSet...>()();    
    }
    
  };

  template <class ...Fields> // 1st dimension, fields(i) is the 2nd dimension
  struct returning
  {
  public:
    
    returning() {}

    /** Expression with parameters */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return std::string("RETURNING ") + helper::impl_fields_part<0,Fields...>()(b);
    }
    
    /** Empty parameter list */
    std::string operator()() {      
      return std::string("RETURNING ") + helper::impl_fields_part<0,Fields...>()();    
    }
    
  };


  template <class Table, class Set, class From = none, class Where = none, class Returning = none>
  struct update
  {
  public:
    
    update() {}

    /** Expression with parameters */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return 
        std::string("UPDATE ") + 
        Table()(b) +  
        Set()(b) + 
        (std::is_same<From,none>::value ?
         std::string("") : 
         From()(b)) + 
        Where()(b) + 
        Returning()(b);
    }
    
    /** Empty parameter list */
    std::string operator()() {      
      return
        std::string("UPDATE ") + 
        Table()() + std::string(" ") + 
        Set()() + 
        (std::is_same<From,none>::value ?
         std::string("") : 
         From()()) + 
        Where()() + 
        Returning()();
    }
    
  };

};

#endif
