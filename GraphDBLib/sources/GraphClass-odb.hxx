// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef GRAPH_CLASS_ODB_HXX
#define GRAPH_CLASS_ODB_HXX

#include <odb/version.hxx>

#if (ODB_VERSION != 20400UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "GraphClass.h"

#include "Edge-odb.hxx"
#include "Graph-odb.hxx"
#include "Node-odb.hxx"
#include "ObjectClass-odb.hxx"
#include "Point-odb.hxx"

#include <memory>
#include <cstddef>

#include <odb/core.hxx>
#include <odb/traits.hxx>
#include <odb/callback.hxx>
#include <odb/wrapper-traits.hxx>
#include <odb/pointer-traits.hxx>
#include <odb/container-traits.hxx>
#include <odb/no-op-cache-traits.hxx>
#include <odb/prepared-query.hxx>
#include <odb/result.hxx>
#include <odb/simple-object-result.hxx>

#include <odb/details/unused.hxx>
#include <odb/details/shared-ptr.hxx>

namespace odb
{
  // GraphClass
  //
  template <>
  struct class_traits< ::graphDBLib::GraphClass >
  {
    static const class_kind kind = class_object;
  };

  template <>
  class access::object_traits< ::graphDBLib::GraphClass >
  {
    public:
    typedef ::graphDBLib::GraphClass object_type;
    typedef ::graphDBLib::GraphClass* pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    static const bool polymorphic = false;

    typedef ::std::string id_type;

    static const bool auto_id = false;

    static const bool abstract = false;

    static id_type
    id (const object_type&);

    typedef
    no_op_pointer_cache_traits<pointer_type>
    pointer_cache_traits;

    typedef
    no_op_reference_cache_traits<object_type>
    reference_cache_traits;

    static void
    callback (database&, object_type&, callback_event);

    static void
    callback (database&, const object_type&, callback_event);
  };
}

#include <odb/details/buffer.hxx>

#include <odb/pgsql/version.hxx>
#include <odb/pgsql/forward.hxx>
#include <odb/pgsql/binding.hxx>
#include <odb/pgsql/pgsql-types.hxx>
#include <odb/pgsql/query.hxx>

namespace odb
{
  // GraphClass
  //
  template <typename A>
  struct query_columns< ::graphDBLib::GraphClass, id_pgsql, A >
  {
    // graphClassName
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::std::string,
        pgsql::id_string >::query_type,
      pgsql::id_string >
    graphClassName_type_;

    static const graphClassName_type_ graphClassName;

    // directGraph
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        bool,
        pgsql::id_boolean >::query_type,
      pgsql::id_boolean >
    directGraph_type_;

    static const directGraph_type_ directGraph;

    // acyclicGraph
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        bool,
        pgsql::id_boolean >::query_type,
      pgsql::id_boolean >
    acyclicGraph_type_;

    static const acyclicGraph_type_ acyclicGraph;
  };

  template <typename A>
  const typename query_columns< ::graphDBLib::GraphClass, id_pgsql, A >::graphClassName_type_
  query_columns< ::graphDBLib::GraphClass, id_pgsql, A >::
  graphClassName (A::table_name, "\"graphClassName\"", 0);

  template <typename A>
  const typename query_columns< ::graphDBLib::GraphClass, id_pgsql, A >::directGraph_type_
  query_columns< ::graphDBLib::GraphClass, id_pgsql, A >::
  directGraph (A::table_name, "\"directGraph\"", 0);

  template <typename A>
  const typename query_columns< ::graphDBLib::GraphClass, id_pgsql, A >::acyclicGraph_type_
  query_columns< ::graphDBLib::GraphClass, id_pgsql, A >::
  acyclicGraph (A::table_name, "\"acyclicGraph\"", 0);

  template <typename A>
  struct pointer_query_columns< ::graphDBLib::GraphClass, id_pgsql, A >:
    query_columns< ::graphDBLib::GraphClass, id_pgsql, A >
  {
  };

  template <>
  class access::object_traits_impl< ::graphDBLib::GraphClass, id_pgsql >:
    public access::object_traits< ::graphDBLib::GraphClass >
  {
    public:
    struct id_image_type
    {
      details::buffer id_value;
      std::size_t id_size;
      bool id_null;

      std::size_t version;
    };

    struct image_type
    {
      // graphClassName
      //
      details::buffer graphClassName_value;
      std::size_t graphClassName_size;
      bool graphClassName_null;

      // directGraph
      //
      bool directGraph_value;
      bool directGraph_null;

      // acyclicGraph
      //
      bool acyclicGraph_value;
      bool acyclicGraph_null;

      std::size_t version;
    };

    struct extra_statement_cache_type;

    using object_traits<object_type>::id;

    static id_type
    id (const image_type&);

    static bool
    grow (image_type&,
          bool*);

    static void
    bind (pgsql::bind*,
          image_type&,
          pgsql::statement_kind);

    static void
    bind (pgsql::bind*, id_image_type&);

    static bool
    init (image_type&,
          const object_type&,
          pgsql::statement_kind);

    static void
    init (object_type&,
          const image_type&,
          database*);

    static void
    init (id_image_type&, const id_type&);

    typedef pgsql::object_statements<object_type> statements_type;

    typedef pgsql::query_base query_base_type;

    static const std::size_t column_count = 3UL;
    static const std::size_t id_column_count = 1UL;
    static const std::size_t inverse_column_count = 0UL;
    static const std::size_t readonly_column_count = 0UL;
    static const std::size_t managed_optimistic_column_count = 0UL;

    static const std::size_t separate_load_column_count = 0UL;
    static const std::size_t separate_update_column_count = 0UL;

    static const bool versioned = false;

    static const char persist_statement[];
    static const char find_statement[];
    static const char update_statement[];
    static const char erase_statement[];
    static const char query_statement[];
    static const char erase_query_statement[];

    static const char table_name[];

    static void
    persist (database&, const object_type&);

    static pointer_type
    find (database&, const id_type&);

    static bool
    find (database&, const id_type&, object_type&);

    static bool
    reload (database&, object_type&);

    static void
    update (database&, const object_type&);

    static void
    erase (database&, const id_type&);

    static void
    erase (database&, const object_type&);

    static result<object_type>
    query (database&, const query_base_type&);

    static unsigned long long
    erase_query (database&, const query_base_type&);

    static odb::details::shared_ptr<prepared_query_impl>
    prepare_query (connection&, const char*, const query_base_type&);

    static odb::details::shared_ptr<result_impl>
    execute_query (prepared_query_impl&);

    static const char persist_statement_name[];
    static const char find_statement_name[];
    static const char update_statement_name[];
    static const char erase_statement_name[];
    static const char query_statement_name[];
    static const char erase_query_statement_name[];

    static const unsigned int persist_statement_types[];
    static const unsigned int find_statement_types[];
    static const unsigned int update_statement_types[];

    public:
    static bool
    find_ (statements_type&,
           const id_type*);

    static void
    load_ (statements_type&,
           object_type&,
           bool reload);
  };

  template <>
  class access::object_traits_impl< ::graphDBLib::GraphClass, id_common >:
    public access::object_traits_impl< ::graphDBLib::GraphClass, id_pgsql >
  {
  };

  // GraphClass
  //
}

#include "GraphClass-odb.ixx"

#include <odb/post.hxx>

#endif // GRAPH_CLASS_ODB_HXX
