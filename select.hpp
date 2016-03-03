#ifndef _SQL_SELECT_HPP
#define _SQL_SELECT_HPP

/** 
  * Copyright 2015 Maxim Musolov
  * This code distribute under private license
  */


#include <string>
#include <type_traits>
#include <query-freaks/sql_base.hpp>

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

  // namespace helper {

  //   /** select impl - helper class */
  //   template <std::size_t N, class... Fields>
  //   struct impl_select_part;

  //   /** add space at the end of expression */
  //   template <std::size_t N>
  //   struct impl_select_part<N> {
  //     static constexpr size_t value = N;
    
  //     // operator implementation for specialization
  //     template <class ...Args>
  //     std::string operator()(binder<Args...>& b) {
  //       return std::string(" ");
  //     }

  //     std::string operator()() {
  //       return std::string(" ");
  //     }

  //   };
    
  //   /** mid expression for fields order */
  //   template <std::size_t N, class F, class ...Fields>
  //   struct impl_select_part<N,F,Fields...>
  //   {
  //     static constexpr size_t value = N;
    
  //     /** expression contains query parameters */
  //     template <class ...Args>
  //     std::string operator()(binder<Args...>& b) {
  //       return std::string(", ") + F()(b) + impl_select_part<N+1, Fields...>()(b);
  //     }
      
  //     /** expression without parameters */
  //     std::string operator()() {
  //       return std::string(", ") + F()() + impl_select_part<N+1, Fields...>()();
  //     }

  //   };
  
  //   /** first entry of selecting fields order */
  //   template <class F, class ...Fields>
  //   struct impl_select_part<0,F,Fields...>
  //   {
  //     static constexpr size_t value = 0;
    
  //     // operator implementation of first entry
  //     template <class ...Args>
  //     std::string operator()(binder<Args...>& b) {
  //       return F()(b) + impl_select_part<1, Fields...>()(b);
  //     }
      
  //     /** expression without query parameters */
  //     std::string operator()() {
  //       return F()() + impl_select_part<1, Fields...>()();
  //     }

  //   };
  // };

  /** define sql::select */
  template <class Fields, class From, class Where, class GroupBy = none, class Having = none, class OrderBy = none>
  struct _select
  {
    public:
    
    _select() {}
    
    /** implement select directive */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return 
        std::string("SELECT ") + 
        //helper::impl_select_part<0,Fields...>()(b) +
        Fields()(b) + std::string(" ") + 
        From()(b) + 
        Where()(b) + 
        (std::is_same<GroupBy,none>::value ?
         std::string("") :
         GroupBy()(b) + 
         (std::is_same<Having,none>::value ?
          std::string("") :
          Having()(b))) +
        (std::is_same<OrderBy,none>::value ? 
         std::string("") : 
         OrderBy()(b));
        
    }       
    
    /** expression without query parameters */
    std::string operator()() {      
      return 
        std::string("SELECT ") + 
        //helper::impl_select_part<0,Fields...>()() +
        Fields()() +
        From()() + 
        Where()() + 
        (std::is_same<GroupBy,none>::value ?
         std::string("") :
         GroupBy()() + 
         (std::is_same<Having,none>::value ?
          std::string("") :
          Having()())) +
        (std::is_same<OrderBy,none>::value ? 
         std::string("") : 
         OrderBy()());
    }       

  };
   
};

#endif
