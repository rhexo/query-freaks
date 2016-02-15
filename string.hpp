#ifndef _SQL_STRING_HPP
#define _SQL_STRING_HPP

/** 
  * Copyright 2015 Maxim Musolov
  * This code distribute under private license
  */

#include <string>
#include <cstddef>

namespace sql {
  
  /**
   * declare a sql::string
   */

  template <char... c>
  struct string{
    typedef string type;
    
    enum {N = sizeof...(c)};
    char value[N] = {c...};

    string() {}

    template <class ...Args>
    std::string operator()(binder<Args...>& b) {return std::string(value);}

    template <class ...Args>
    std::string operator()() {return std::string(value);}

  };

};

/**
 * using macroses to implement "text" string syntax in query definitions
 */

#define E(L,I) \
    (I < sizeof(L)) ? L[I] : '\0'

#define QSTR(L)                                                        \
  sql::string<E(L,0),E(L,1),E(L,2),E(L,3),E(L,4),E(L,5),E(L,6),E(L,7),E(L,8),E(L,9), \
              E(L,10),E(L,11),E(L,12),E(L,13),E(L,14),E(L,15),E(L,16),E(L,17),E(L,18),E(L,19), \
              E(L,20),E(L,21),E(L,22),E(L,23),E(L,24),E(L,25),E(L,26),E(L,27),E(L,28),E(L,29), \
              E(L,30),E(L,31),E(L,32),E(L,33),E(L,34),E(L,35),E(L,36),E(L,37),E(L,38),E(L,39), \
              E(L,40),E(L,41),E(L,42),E(L,43),E(L,44),E(L,45),E(L,46),E(L,47),E(L,48),E(L,49), \
              E(L,50),E(L,51),E(L,52),E(L,53),E(L,54),E(L,55),E(L,56),E(L,57),E(L,58),E(L,59), \
              E(L,60),E(L,61),E(L,62),E(L,63),E(L,64),E(L,65),E(L,66),E(L,67),E(L,68),E(L,69), \
              E(L,70),E(L,71),E(L,72),E(L,73),E(L,74),E(L,75),E(L,76),E(L,77),E(L,78),E(L,79), \
              E(L,80),E(L,81),E(L,82),E(L,83),E(L,84),E(L,85),E(L,86),E(L,87),E(L,88),E(L,89), \
              E(L,90),E(L,91),E(L,92),E(L,93),E(L,94),E(L,95),E(L,96),E(L,97),E(L,98),E(L,99), \
              E(L,100),E(L,101),E(L,102),E(L,103),E(L,104),E(L,105),E(L,106),E(L,107),E(L,108),E(L,109), \
              E(L,110),E(L,111),E(L,112),E(L,113),E(L,114),E(L,115),E(L,116),E(L,117),E(L,118),E(L,119), \
              E(L,120),E(L,121),E(L,122),E(L,123),E(L,124),E(L,125),E(L,126),E(L,127),'\0' \
>::type


#endif
