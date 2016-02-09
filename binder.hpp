#ifndef _SQL_BINDER_HPP
#define _SQL_BINDER_HPP

/** 
  * Copyright 2015 Maxim Musolov
  * This code distribute under private license
  */

#include <type_traits>
#include <tuple>
#include <string>
#include <utility>
#include <query-freaks/sql_base.hpp>  

namespace sql {
  
  /**
    * Common interface object for application variables that using in query construction process
    */

  template<class ...Args>
  struct binder
  {
    public:

    typedef binder type;
    
    binder(Args&&... args) : 
      // construc none element as first for utility reason
      _data(std::tuple_cat(std::make_tuple(std::forward<none>(none())), 
                           std::tuple<Args...>(std::forward<Args>(args)...)))
    {}

    binder(Args&... args) : 
      // construct none element as first for utility reason
      _data(std::tuple_cat(std::make_tuple(std::forward<none>(none())), 
                           std::tuple<Args...>(std::forward<Args>(args)...))) 
    {}
    
    std::string operator() () { return std::string(""); }
    
    /**
      * get stored data element by type
      * return reference to data element
      */
    
    // usign type alias
    template <class Type>
    using get_result_type = typename std::tuple_element<
      detail::get_number_of_element_from_tuple_by_type_impl<
        Type,
        0,
        std::tuple_size<std::tuple<none,Args...> >::value,
        none,
        Args...
        >::value,
      std::tuple<none,Args...> 
      >::type;

    template <class Type>
    get_result_type<Type>& get()
    {
       return get_element_by_type<Type>(_data);
    }
    
    /**
      * check for type existence in stored data tuple
      */
    template <class Type>
    bool check()
    {
      return typename tuple_has_type<Type, std::tuple<Args...> >::type();
    }

  private:

    /**
      * store data as tuple expression
      * using none type for the reference to none. In common case 
      * get<TYPE>() may be missing in stored data, than return none for the reference to nothing
      */
    std::tuple<none,Args...> _data;

  };  

};
#endif
