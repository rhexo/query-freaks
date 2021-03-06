#ifndef _SQL_HAVING_HPP
#define _SQL_HAVING_HPP

/** 
  * Copyright 2015 Maxim Musolov
  * This code distribute under private license
  */


#include <string>
#include <type_traits>
#include <query-freaks/sql_base.hpp>

namespace sql {

  namespace helper {

    /** select impl - helper class */
    template <std::size_t N, class... Ent>
    struct impl_having_part;

    /** add space at the end of expression */
    template <std::size_t N>
    struct impl_having_part<N>
    {
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
    template <std::size_t N, class F, class ...Ent>
    struct impl_having_part<N,F,Ent...>
    {
      static constexpr size_t value = N;
    
      /** expression contains query parameters */
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string(", ") + F()(b) + impl_having_part<N+1, Ent...>()(b);
      }
      
      /** expression without parameters */
      std::string operator()() {
        return std::string(", ") + F()() + impl_having_part<N+1, Ent...>()();
      }

    };
  
    /** first entry of selecting fields order */
    template <class F, class ...Ent>
    struct impl_having_part<0,F,Ent...>
    {
      static constexpr size_t value = 0;
    
      // operator implementation of first entry
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string("HAVING ") + F()(b) + impl_having_part<1, Ent...>()(b);
      }
      
      /** expression without query parameters */
      std::string operator()() {
        return std::string("HAVING ") + F()() + impl_having_part<1, Ent...>()();
      }

    };
  };

  /** define sql::select */
  template <typename ...Ent>
  struct having
  {
    public:
    /** constructor */
    having() {}
    
    template <class ...Args>
    std::string operator()(binder<Args...>& b) { 
      return helper::impl_having_part<0,Ent...>()(b); 
    }       
    
    /** expression without query parameters */
    std::string operator()() {      
      return helper::impl_having_part<0,Ent...>()();
    }       
  };
   

}

#endif
