#ifndef _SQL_INSERT_HPP
#define _SQL_INSERT_HPP

/** 
 * Copyright 2015 Maxim Musolov
 * This code distribute under private license
 */

#include <string>

#include <query-freaks/sql_base.hpp>

namespace sql {

  

  namespace helper {

    // INTO section <table, fields...>

    /** select impl - helper class */
    template <std::size_t N, class... Fields>
    struct impl_fields_part;

    /** add space at the end of expression */
    template <std::size_t N>
    struct impl_fields_part<N> {
      static constexpr size_t value = N;
    
      // operator implementation for specialization
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string();
      }

      std::string operator()() {
        return std::string();
      }

    };
    
    /** mid expression for fields order */
    template <std::size_t N, class F, class ...Fields>
    struct impl_fields_part<N,F,Fields...>
    {
      static constexpr size_t value = N;
    
      /** expression contains query parameters */
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string(", ") + 
          ( b.template check<F>() ? 
            b.template get<F>()(b) : 
            F()(b) )
          + impl_fields_part<N+1, Fields...>()(b);
      }
      
      /** expression without parameters */
      std::string operator()() {
        return std::string(", ") + F()() + impl_fields_part<N+1, Fields...>()();
      }

    };
  
    /** first entry of selecting fields order */
    template <class F, class ...Fields>
    struct impl_fields_part<0,F,Fields...>
    {
      static constexpr size_t value = 0;
    
      // operator implementation of first entry
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return  ( b.template check<F>() ? 
            b.template get<F>()(b) : 
            F()(b) ) + impl_fields_part<1, Fields...>()(b);
      }
      
      /** expression without query parameters */
      std::string operator()() {
        return F()() + impl_fields_part<1, Fields...>()();
      }

    };
  };


  /** define sql::fields */

  template <class ...Fields>
  struct fields
  {
  public:
    
    fields() {}

    /** Expression with parameters */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return std::string("(") + helper::impl_fields_part<0,Fields...>()(b) + std::string(")");
    }
    
    /** Empty parameter list */
    std::string operator()() {      
      return std::string("(") + helper::impl_fields_part<0,Fields...>()() + std::string(")");    
    }
    
  };


  /** define sql::into */
  template <typename Table, typename Fields = none>
  struct into
  {
    public:
    
    into() {}
    
    /** implement select directive */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) { 

      if (std::is_same<Fields,none>::value)
        return std::string("INTO ") + Table()(b) + std::string(" ");

      return std::string("INTO ") + Table()(b) + std::string(" ") + Fields()(b) + std::string(" ") ;

    }       
    
    /** expression without query parameters */
    std::string operator()() {      
 //     return helper::impl_into_part<Table,0,Fields...>()();

      if (std::is_same<Fields,none>::value)
        return std::string("INTO ") + Table()() + std::string(" ");

      return std::string("INTO ") + Table()() + std::string(" ") + Fields()() + std::string(" ") ;

    }       

  };



  // VALUES section <fields...>...

  namespace helper {

  
    /** values impl - helper class */
    template <std::size_t N, class... Fields>
    struct impl_values_part;

    /** add space at the end of expression */
    template <std::size_t N>
    struct impl_values_part<N> {
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
    template <std::size_t N, class F, class ...Fields>
    struct impl_values_part<N,F,Fields...>
    {
      static constexpr size_t value = N;
    
      /** expression contains query parameters */
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return std::string(", ") + 
          ( b.template check<F>() ? 
            b.template get<F>()(b) : 
            F()(b) ) + impl_values_part<N+1, Fields...>()(b);
      }
      
      /** expression without parameters */
      std::string operator()() {
        return std::string(", ") + F()() + impl_values_part<N+1, Fields...>()();
      }

    };
  
    /** first entry of selecting fields order */
    template <class F, class ...Fields>
    struct impl_values_part<0,F,Fields...>
    {
      static constexpr size_t value = 0;
    
      // operator implementation of first entry
      template <class ...Args>
      std::string operator()(binder<Args...>& b) {
        return  
          ( b.template check<F>() ? 
            b.template get<F>()(b) : 
            F()(b) ) + impl_values_part<1, Fields...>()(b);
      }
      
      /** expression without query parameters */
      std::string operator()() {
        return F()() + impl_values_part<1, Fields...>()();
      }

    };
  };


  template <class ...Fields> // 1st dimension, fields(i) is the 2nd dimension
  struct values
  {
  public:
    
    values() {}

    /** Expression with parameters */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return std::string("VALUES ") + helper::impl_values_part<0,Fields...>()(b);
    }
    
    /** Empty parameter list */
    std::string operator()() {      
      return std::string("VALUES ") + helper::impl_values_part<0,Fields...>()();    
    }
    
  };

  // INSERT section <INTO<...>, VALUES<...> >
  template <class Into, class Values> 
  struct insert
  {
  public:
    
    insert() {}

    /** Expression with parameters */
    template <class ...Args>
    std::string operator()(binder<Args...>& b) {      
      return std::string("INSERT ") + Into()(b) + Values()(b);
    }
    
    /** Empty parameter list */
    std::string operator()() {      
      return std::string("INSERT ") + Into()() + Values()();
    }
    
  };


};

#endif
