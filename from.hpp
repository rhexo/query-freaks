#ifndef _SQL_FROM_HPP
#define _SQL_FROM_HPP

/** 
  * Copyright 2015 Maxim Musolov
  * This code distribute under private license
  */

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector_c.hpp>
#include <string>

#include <query-freaks/sql_base.hpp>

namespace sql {


  /**
   * pieces of FROM clause:
   * table_name
   * alias
   * join_type
   */


  /** alias */
  template <class A, class Alias>
  struct alias
  {
  public:

    /** constructor */
    alias() {}
    
    /** Expression with parameters */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return A()(b) + std::string(" AS ") + Alias()(b) + std::string(" ");
    }
    
    /** Empty parameter list */
    std::string operator()() {      
      return A()() + std::string(" AS ") + Alias()() + std::string(" ");
    }

  };
  
  // point< [table <...> | string<"...">], field< string<"..."> > => T.Feild
  template <class T, class Field>
  struct point
  {
  public:    
    /** constructor */
    point() {}

    template <class ...Args>
    std::string operator()(binder<Args...>& b) {
      return T()(b) + std::string(".") + Field()(b);
    }

    std::string operator()() {
      return T()() + std::string(".") + Field()();
    }

  };

  /** join */  
  enum join_type { NONE, INNER, LEFT, RIGHT, FULL, CROSS};
    
  namespace helper {

    /** select impl - helper class */
    template <std::size_t N, class... Cond>
    struct impl_on_part;

    /** add space at the end of expression */
    template <std::size_t N>
    struct impl_on_part<N>
    {
      static constexpr size_t value = N;
    
      // operator implementation for specialization
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string(" ");
      }
      // without parameters expression
      std::string operator()() {
        return std::string(" ");
      }

    };   

    /** mid expression for fields order */
    template <std::size_t N, class F, class ...Cond>
    struct impl_on_part<N,F,Cond...>
    {
      static constexpr size_t value = N;
    
      // operator implementatiaon for specialization 
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string(" AND ") + F()(b) + impl_on_part<N+1, Cond...>()(b);
      }

      // operator implementatiaon for specialization 
      std::string operator()() {
        return std::string(" AND ") + F()() + impl_on_part<N+1, Cond...>()();
      }

    };
  
    /** first entry of selecting fields order */
    template <class F, class ...Cond>
    struct impl_on_part<0,F,Cond...>
    {
      static constexpr size_t value = 0;
    
      /** operator implementation of first entry */
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string("ON ") + F()(b) + impl_on_part<1, Cond...>()(b);
      }

      /** without template parameters */
      std::string operator()() {
        return std::string("ON ") + F()() + impl_on_part<1, Cond...>()();
      }

    };

  };


  // on<
  //   cond< point< [table <...> | string<"...">], field< string<"..."> > , EQ, 
  //         point< [table <...> | string<"...">], field< string<"..."> >  
  //         >
  //   >

  template <typename ...Cond>
  struct on
  {
  public:
    
    on() {}
    
    /** expression contains query parameters */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return helper::impl_on_part<0,Cond...>()(b);
    }       
    
    /** expression without query parameters */
    std::string operator()() {      
      return helper::impl_on_part<0,Cond...>()();
    }       

  };

  
  // join < join_type (int),
  //        T,
  //        on<...>
  //      >
  template <int Type, class T, class On>
  struct join
  {
  public:
    
    join() {}

    /** expression contains query parameters */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      // choose join type
      std::string _type;

      switch(Type) {
      case INNER:
        _type = " INNER JOIN ";
        break;

      case LEFT:
        _type = " LEFT OUTER JOIN ";
        break;          

      case RIGHT:
        _type = " RIGHT OUTER JOIN ";
        break;

      case FULL:
        _type = " FULL OUTER JOIN ";
        break;

      case CROSS:
        _type = " CROSS JOIN ";
        break;
      }
      return _type + T()(b) + std::string(" ") + On()(b);
    }       
    
    /** expression without query parameters */
    std::string operator()() {      
      // choose join type
      std::string _type;

      switch(Type) {
      case INNER:
        _type = " INNER JOIN ";
        break;

      case LEFT:
        _type = " LEFT OUTER JOIN ";
        break;          

      case RIGHT:
        _type = " RIGHT OUTER JOIN ";
        break;

      case FULL:
        _type = " FULL OUTER JOIN ";
        break;

      case CROSS:
        _type = " CROSS JOIN ";
        break;
      }
      return _type + T()() + std::string(" ") + On()();

    };
  
  };

  /** table */
  template <class Entry, class Join = none>
  struct table
  {
  public:
    /** constructor */
    table() {}
    
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      // field template parameter data
      if (std::is_same<Join,none>::value)
        return Entry()(b); // construct without join condition

      return Entry()(b) + Join()(b);
    }
    
    /** Empty parameter list */
    std::string operator()() {      
      // field template parameter data
      if (std::is_same<Join,none>::value)
        return Entry()(); // construct without join condition

      return Entry()() + Join()();
    }

  };


  namespace helper {

    /** select impl - helper class */
    template <std::size_t N, class... Ts>
    struct impl_from_part;

    /** add space at the end of expression */
    template <std::size_t N>
    struct impl_from_part<N>
    {
      static constexpr size_t value = N;
    
      // operator implementation for specialization
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string(" ");
      }

      // without parameters expression
      std::string operator()() {
        return std::string(" ");
      }

    };
    
    /** mid expression for tables order */
    template <std::size_t N, class T, class ...Ts>
    struct impl_from_part<N,T,Ts...>
    {
      static constexpr size_t value = N;
    
      // operator implementatiaon for specialization 
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string(", ") + T()(b) + impl_from_part<N+1, Ts...>()(b);
      }

      // operator implementatiaon for specialization 
      std::string operator()() {
        return std::string(", ") + T()() + impl_from_part<N+1, Ts...>()();
      }

    };
  
    /** first entry of selecting fields order */
    template <class T, class ...Ts>
    struct impl_from_part<0,T,Ts...>
    {
      static constexpr size_t value = 0;
    
      /** operator implementation of first entry */
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string("FROM ") + T()(b) + impl_from_part<1, Ts...>()(b);
      }

      /** without template parameters */
      std::string operator()() {
        return std::string("FROM ") + T()() + impl_from_part<1, Ts...>()();
      }

    };
  };

  

  /** from */

  // from table<...>, table<...>, ...
  template <class ...Ts>
  struct from
  {
  public:

    from() {}

    /** Expression with parameters */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return helper::impl_from_part<0,Ts...>()(b);
    }
    
    /** Empty parameter list */
    std::string operator()() {      
      return helper::impl_from_part<0,Ts...>()();    
    }
    
  };


};

#endif
