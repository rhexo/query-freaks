#ifndef _SQL_HPP
#define _SQL_HPP

/** 
  * Copyright 2015 Maxim Musolov
  * This code distribute under private license
  */


/**
 * Definition of sql helper metafunctions
 */

#include <query-freaks/sql_base.hpp>
#include <query-freaks/select.hpp>
#include <query-freaks/from.hpp>
#include <query-freaks/where.hpp>
#include <query-freaks/groupby.hpp>
#include <query-freaks/having.hpp>
#include <query-freaks/orderby.hpp>

namespace sql {

  /** Define query type constructor */
  template <class Select, 
            class From, 
            class Where,  
            class GroupBy = none, 
            class Having = none, 
            class OrderBy = none>
  struct query
  {
  public:
    /** constructor */
    query() {}
  
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {    
      /** construct query expression */
      return 
        Select()(b) + 
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

    std::string operator()() {
      /** construct query expression */
      return 
        Select()() + 
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