#ifndef _SQL_VAR_HPP
#define _SQL_VAR_HPP

/** 
  * Copyright 2015 Maxim Musolov
  * This code distribute under private license
  */


#include <string>
#include <cstddef>

namespace sql {

  template <typename ...Ts>
  struct var;

  template <typename T, typename ...Ts>
  struct var<T, Ts...> : public var<Ts...> // Specialization
  {
    typedef var type;
    typedef T value_type;

    var(T entry, Ts ...ts) : value(entry), var<Ts...>(ts...){}
    var() {}
    T value;
    
    template <class ...Args>
    std::string operator()(binder<Args...>& b) 
    {return value();}
    
  };

  template <typename T>
  struct var<T>  // specialization
  {
    typedef var type;
    typedef T value_type;

    var(T entry) : value(entry) {}
    var() {}
    T value;
    
    // std::string operator()() {return value();}
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {return value();}

  };

  

};

#endif
