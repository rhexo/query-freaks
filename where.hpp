#ifndef _SQL_WHERE_HPP
#define _SQL_WHERE_HPP

/** 
  * Copyright 2015 Maxim Musolov
  * This code distribute under private license
  */

#include <string>
#include <type_traits>

#include <sql/sql_base.hpp>
#include <sql/binder.hpp>

namespace sql {
  
   
  /** ----------------------------------------------------------------------*/
  
  // where operator enumeration 
  enum  _operator { EQ, GT, LT, GE, LE, NE, AND, OR, LIKE};

  // define structure for cond definition
  template <class T1, int E, class T2>
  struct cond;

  /** specify cond for each operator */
  // LIKE
  template <class T1, class T2>
  struct cond<T1,LIKE,T2>
  {
    public:
    
    cond() {}

    /** apply expression */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return 
        std::string("( ") +
        ( b.template check<T1>() ? 
          b.template get<T1>()(b) : 
          T1()(b)
          ) +
        std::string(" LIKE ") + 
        ( b.template check<T2>() ?
          b.template get<T2>()(b) : 
          T2()(b) 
          ) +
        std::string(" )");
    } 
  };

  // EQ
  template <class T1, class T2>
  struct cond<T1,EQ,T2>
  {
    public:
    
    cond() {}

    /** apply expression */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return 
        std::string("( ") +
        ( b.template check<T1>() ? 
          b.template get<T1>()(b) : 
          T1()(b)
          ) +
        std::string(" = ") + 
        ( b.template check<T2>() ?
          b.template get<T2>()(b) : 
          T2()(b) 
          ) +
        std::string(" )");
    } 
  };
  
  // GT
  template <class T1, class T2>
  struct cond<T1,GT,T2>
  {
    public:
    
    cond() {}

    /** application of exprassion */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return 
        std::string("( ") +
        ( b.template check<T1>() ? 
          b.template get<T1>()(b) : 
          T1()(b)
          ) +
        std::string(" > ") + 
        ( b.template check<T2>() ?
          b.template get<T2>()(b) : 
          T2()(b) 
          ) +
        std::string(" )");
    } 
  };

  // LT
  template <class T1, class T2>
  struct cond<T1,LT,T2>
  {
    public:
    
    cond() {}

    /** application of exprassion */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return 
        std::string("( ") +
        ( b.template check<T1>() ? 
          b.template get<T1>()(b) : 
          T1()(b)
          ) +
        std::string(" < ") + 
        ( b.template check<T2>() ?
          b.template get<T2>()(b) : 
          T2()(b) 
          ) +
        std::string(" )");
    } 
  };

  // GE
  template <class T1, class T2>
  struct cond<T1,GE,T2>
  {
    public:
    
    cond() {}

    /** application of exprassion */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return 
        std::string("( ") +
        ( b.template check<T1>() ? 
          b.template get<T1>()(b) : 
          T1()(b)
          ) +
        std::string(" >= ") + 
        ( b.template check<T2>() ?
          b.template get<T2>()(b) : 
          T2()(b) 
          ) +
        std::string(" )");
    } 
  };

  // LE
  template <class T1, class T2>
  struct cond<T1,LE,T2>
  {
    public:
    
    cond() {}

    /** application of exprassion */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return 
        std::string("( ") +
        ( b.template check<T1>() ? 
          b.template get<T1>()(b) : 
          T1()(b)
          ) +
        std::string(" <= ") + 
        ( b.template check<T2>() ?
          b.template get<T2>()(b) : 
          T2()(b) 
          ) +
        std::string(" )");
    } 
  };

  // NE
  template <class T1, class T2>
  struct cond<T1,NE,T2>
  {
    public:
    
    cond() {}

    /** application of exprassion */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return 
        std::string("( ") +
        ( b.template check<T1>() ? 
          b.template get<T1>()(b) : 
          T1()(b)
          ) +
        std::string(" <> ") + 
        ( b.template check<T2>() ?
          b.template get<T2>()(b) : 
          T2()(b) 
          ) +
        std::string(" )");
    } 
  };

  // AND
  template <class T1, class T2>
  struct cond<T1,AND,T2>
  {
    public:
    
    cond() {}

    /** application of exprassion */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return 
        std::string("( ") +
        ( b.template check<T1>() ? 
          b.template get<T1>()(b) : 
          T1()(b)
          ) + 
        std::string(" AND ") +  
        ( b.template check<T2>() ?
          b.template get<T2>()(b) : 
          T2()(b) 
          ) +
        std::string(" )");
    } 
  };

  // OR
  template <class T1, class T2>
  struct cond<T1,OR,T2>
  {
  public:
    
    cond() {}

    /** application of exprassion */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return 
        std::string("( ") +
        ( b.template check<T1>() ? 
          b.template get<T1>()(b) : 
          T1()(b)
          ) +
        std::string(" OR ") + 
        ( b.template check<T2>() ?
          b.template get<T2>()(b) : 
          T2()(b) 
          ) +
        std::string(" )");
    } 
  };

  /** ----------------------------------------------------------------------*/
  
  template <class Cond>
  struct where
  {

  public:
    
    where() {}

    /** application of exprassion */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return std::string("WHERE ") + Cond()(b) + std::string(" ");
    } 
  };
 

};

#endif
