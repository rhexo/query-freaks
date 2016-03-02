#ifndef _SQL_DELETE_HPP
#define _SQL_DELETE_HPP

/** 
 * Copyright 2015 Maxim Musolov
 * This code distribute under private license
 */

#include <string>

#include <query-freaks/sql_base.hpp>

namespace sql {


  template <class From, class Where>
  struct _delete
  {
  public:
    
    _delete() {}

    /** Expression with parameters */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return 
        std::string("DELETE ") + 
        From()(b) +  
        Where()(b);
    }
    
    /** Empty parameter list */
    std::string operator()() {      
      return
        std::string("DELETE ") + 
        From()() +  
        Where()();
    }
    
  };

};

#endif
