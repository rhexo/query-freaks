#ifndef _SQL_BASE_HPP
#define _SQL_BASE_HPP

/** 
  * Copyright 2015 Maxim Musolov
  * This code distribute under private license
  */

#include <type_traits>
#include <tuple>

/**
 * Base type definitions for sql helper metafunctions
 */

namespace sql {
  // denote a select field tag type 
  struct field_tag {};
  // denote a select operator tag type
  struct select_tag {};
  // denote a from operator tag type
  struct from_tag {};
  // denote a table tag type
  struct table_tag {};
  // denote a where operator tag type
  struct where_tag {};
  // denote a where condition tag type
  struct condition_tag {};

  template <class ...Args>
  struct binder;

  // denote a none element as template parameter
  struct none {
    std::string operator()() { return std::string(""); }

    template <class ...Args>
    std::string operator()(binder<Args...>& b) {return std::string("");}

  };


  /** 
   * Variadic template support definitions
   */
  template <template <char...> class T, typename U>
  struct is_template_of_string
  {
    template <char... TT>
    static std::true_type test(T<TT...>*);

    static std::false_type test(...);

    static constexpr bool value = decltype(test((U*)nullptr)){};
  };

  /**
    * Get tuple element by element::type
    * c++14 implementation used
    */
  
  namespace detail {

    template <class T, std::size_t N, std::size_t M, class... Args>
    struct get_number_of_element_from_tuple_by_type_impl;
    
    template <class T, std::size_t N, std::size_t M>
    struct get_number_of_element_from_tuple_by_type_impl<T,N,M>
    {
      static constexpr size_t value = 0;
    };
  
    
    template <class T, std::size_t N, std::size_t M, class... Args>
    struct get_number_of_element_from_tuple_by_type_impl<T,N,M,T,Args...>
    {
      static constexpr size_t value = N;
    };

    template <class T, std::size_t N, std::size_t M, class U, class... Args>
    struct get_number_of_element_from_tuple_by_type_impl<T,N,M,U,Args...>
    {
      static constexpr size_t value = get_number_of_element_from_tuple_by_type_impl<T, N+1, M, Args...>::value;
    };
  };
  
  // define result type as aliase to type
  template <class T, class... Args>
  using get_element_by_type_result = typename std::tuple_element<
    detail::get_number_of_element_from_tuple_by_type_impl<
      T,
      0,
      std::tuple_size<std::tuple<Args...> >::value,
      Args...
      >::value,
    std::tuple<Args...> 
    >::type;

  // get_element_by_type template implementation
  template <class T, class... Args>
  get_element_by_type_result<T,Args...>& get_element_by_type(std::tuple<Args...>& t)
  {
    // bind tuple element  
    return std::get<( detail::get_number_of_element_from_tuple_by_type_impl<T,
                                                            0,
                                                            std::tuple_size<std::tuple<Args...> >::value,
                                                            Args...>::value )>(t);
  };

  /**
    * Check for a tuple has type
    */
  template <class T, typename Tuple>
  struct tuple_has_type;
  
  template <typename T>
  struct tuple_has_type<T, std::tuple<>> : std::false_type {};
  
  template <typename T, typename U, typename ...Ts>
  struct tuple_has_type<T, std::tuple<U, Ts...> > : tuple_has_type<T, std::tuple<Ts...> > {};

  template <typename T, typename ...Ts>
  struct tuple_has_type<T, std::tuple<T, Ts...> > : std::true_type {};

  namespace helper {
    
  };



  // template <class F, class ...Ent, int Order = 0, int ONull = 0>
  // struct expr<F,Ent...,Order,ONull> : public expr<Ent...,Order,ONull>
  // {
  // public:
  //   /** constructor */
  //   expr() : expr<Ent...,Order,ONull>() {}

  //   template <class ...Args>
  //   std::string operator()(binder<Args...>& b) 
  //   {      
  //     return F()(b) + std::string(" ") + expr<Ent...,Order,ONull>::operator()(b);
  //   }
    
  //   std::string operator()() 
  //   {      
  //     return  F()() + std::string(" ") + expr<Ent...,Order,ONull>::operator()();
  //   }
  // };


  // template <class F, int Order = 0, int ONull = 0>
  // struct expr<F,Order,ONull>
  // {
  // public:
  //   /** constructor */
  //   expr() {}

  //   template <class ...Args>
  //   std::string operator()(binder<Args...>& b) 
  //   {      
  //     return F()(b);
  //   }
    
  //   std::string operator()() 
  //   {      
  //     return  F()();
  //   }
  // };



};

#include <sql/binder.hpp>
#include <sql/string.hpp>
#include <sql/var.hpp>


#endif
