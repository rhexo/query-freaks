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


  /** define sql::select */
  template <class Fields, class From, class Where = none, class GroupBy = none, class Having = none, class OrderBy = none>
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
        (std::is_same<Where,none>::value ?
         std::string("") :
         Where()(b) + 
         (std::is_same<GroupBy,none>::value ?
          std::string("") :
          GroupBy()(b) + 
          (std::is_same<Having,none>::value ?
           std::string("") :
           Having()(b)))) +
        (std::is_same<OrderBy,none>::value ? 
         std::string("") : 
         OrderBy()(b));
        
    }       
    
    /** expression without query parameters */
    std::string operator()() {      
      return 
        std::string("SELECT ") + 
        //helper::impl_select_part<0,Fields...>()() +
        Fields()() + std::string(" ") + 
        From()() + 
        (std::is_same<Where,none>::value ?
         std::string("") :
         Where()() + 
         (std::is_same<GroupBy,none>::value ?
          std::string("") :
          GroupBy()() + 
          (std::is_same<Having,none>::value ?
           std::string("") :
           Having()()))) +
        (std::is_same<OrderBy,none>::value ? 
         std::string("") : 
         OrderBy()());
    }       

  };
   
};

#endif
