#ifndef _SQL_ORDERBY_HPP
#define _SQL_ORDERBY_HPP

/** 
  * Copyright 2015 Maxim Musolov
  * This code distribute under private license
  */


#include <string>
#include <type_traits>
#include <sql/sql_base.hpp>

namespace sql {


  namespace helper {

    /** orderby impl - helper class */
    template <std::size_t N, class... Conds>
    struct impl_orderby_part;

    /** add space at the end of expression */
    template <std::size_t N>
    struct impl_orderby_part<N>
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
    template <std::size_t N, class F, class ...Conds>
    struct impl_orderby_part<N,F,Conds...>
    {
      static constexpr size_t value = N;
    
      /** expression contains query parameters */
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string(", ") + F()(b) + impl_orderby_part<N+1, Conds...>()(b);
      }
      
      /** expression without parameters */
      std::string operator()() {
        return std::string(", ") + F()() + impl_orderby_part<N+1, Conds...>()();
      }

    };
  
    /** first entry of selecting fields order */
    template <class F, class ...Conds>
    struct impl_orderby_part<0,F,Conds...>
    {
      static constexpr size_t value = 0;
    
      // operator implementation of first entry
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string("ORDER BY ") + F()(b) + impl_orderby_part<1, Conds...>()(b);
      }
      
      /** expression without query parameters */
      std::string operator()() {
        return std::string("ORDER BY ") +F()() + impl_orderby_part<1, Conds...>()();
      }

    };
  };
  
  // ordering result
  enum _order {ASC, DESC};
    
  // ordering null entries
  enum _order_nulls {FIRST, LAST};

  template <class F, int Order = 0, int ONull = 0>
  struct expr {
  public:
    /** constructor */
    expr() {}

    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      std::string _order;
      std::string _onull;

      switch(Order){
        case ASC:
          _order = std::string(" ASC");
          break;
        case DESC:
          _order = std::string(" DESC");
          break;
        default:
          _order = std::string("");          
      };

      switch(ONull){
        case FIRST:
          _onull = std::string(" NULLS FIRST");
          break;
        case LAST:
          _onull = std::string(" NULLS LAST");
          break;
        default:
          _onull = std::string("");          
      };     

      return F()(b) + _order + _onull;
    }
    
    std::string operator()() {      
      std::string _order;
      std::string _onull;

      switch(Order){
        case ASC:
          _order = std::string(" ASC");
          break;
        case DESC:
          _order = std::string(" DESC");
          break;
        default:
          _order = std::string("");          
      };

      switch(ONull){
        case FIRST:
          _onull = std::string(" NULLS FIRST");
          break;
        case LAST:
          _onull = std::string(" NULLS LAST");
          break;
        default:
          _onull = std::string("");          
      };     

      return F()() + _order + _onull;
    }
  };

  /** define sql::select */
  template <typename ...Ent>
  struct orderby
  {
    public:
    /** constructor */
    orderby() {}
    
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return helper::impl_orderby_part<0,Ent...>()(b);
    }       
    
    std::string operator()() {      
      return helper::impl_orderby_part<0,Ent...>()();
    }       
  };
   
};

#endif
