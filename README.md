# query-freaks

Copyright 2015 Maxim Musolov

SQL metalanguage in c++ context. Using template issue to define sql expression. As result query-freaks return qurey in SQL standard.

```c++
#include "include/query-freaks/sql.h"

// Simple example of query difinition in c++ context

   using namespace sql;

// Construct binder object
  sql::binder<
    sql::var<_sel::f_name>,
    sql::var<_sel::f_count>    
    > _args(
            sql::var<_sel::f_name>(std::string("'John'")),
            sql::var<_sel::f_count>(99)
            );

// Construct query
  query<
    select<
      field<QSTR("F_1")>,
      field<QSTR("F_2")>,
      field<QSTR("F_3")>,
      field<QSTR("F_4")>,
      field<QSTR("F_5")>,
      field<QSTR("F_6")>,
      field<QSTR("F_7")>,
      field<QSTR("F_8")>
      >,
    from<
      table<QSTR("T_1") >,
      table<alias<table<QSTR("T_2")>, QSTR("q") >, 
            join<INNER, alias<table<QSTR("T_3") >, QSTR("w") >,
                 on<
                   cond<point<QSTR("w"), QSTR("a1")>, EQ,
                        point<QSTR("q"), QSTR("a1") > >, 
                   cond<point<QSTR("w"), QSTR("a2")>, EQ,
                        point<QSTR("q"), QSTR("a2") > > 
                   > > > >,
    where<
      cond<
        cond<QSTR("F_1"), EQ, var<_sel::f_name > >, AND,
        cond<QSTR("F_2"), GT, var<_sel::f_count > >
        > >,
    groupby<
      field<QSTR("F_1") >,
      field<QSTR("F_2") >
      >,
    having<
      cond<
        cond<field<QSTR("F_2") >, GT, QSTR("100") >, AND,
        cond<field<QSTR("F_3") >, EQ, QSTR("'JB7'") >
        > >,
    orderby<
      expr<field<QSTR("F_1") >, DESC, LAST>,
      expr<field<QSTR("F_2") > >      
      >
    > qry_sel;

  std::cout << "result(): " << qry_sel(_args) << std::endl;

```

cout print as result this string:

```eshell
result(): SELECT F_1, F_2, F_3, F_4, F_5, F_6, F_7, F_8 FROM T_1, T_2 AS q  INNER JOIN T_3 AS w  ON ( w.a1 = q.a1 ) AND ( w.a2 = q.a2 )  WHERE ( ( F_1 = John ) AND ( F_2 > 99 ) ) GROUP BY F_1, F_2 HAVING ( ( F_2 > 100 ) AND ( F_3 = JB7 ) ) ORDER BY F_1 DESC NULLS LAST, F_2 ASC NULLS FIRST 
```

