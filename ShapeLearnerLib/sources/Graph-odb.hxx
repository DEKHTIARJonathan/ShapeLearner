// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef GRAPH_ODB_HXX
#define GRAPH_ODB_HXX

#include <odb/version.hxx>

#if (ODB_VERSION != 20400UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "Graph.h"

#include "Edge-odb.hxx"
#include "GraphClass-odb.hxx"
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
#include <odb/view-image.hxx>
#include <odb/view-result.hxx>

#include <odb/details/unused.hxx>
#include <odb/details/shared-ptr.hxx>

namespace odb
{
  // Graph
  //
  template <>
  struct class_traits< ::Graph >
  {
    static const class_kind kind = class_object;
  };

  template <>
  class access::object_traits< ::Graph >
  {
    public:
    typedef ::Graph object_type;
    typedef ::Graph* pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    static const bool polymorphic = false;

    typedef long unsigned int id_type;

    static const bool auto_id = true;

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

  // GraphIdViewByGraphClass
  //
  template <>
  struct class_traits< ::GraphIdViewByGraphClass >
  {
    static const class_kind kind = class_view;
  };

  template <>
  class access::view_traits< ::GraphIdViewByGraphClass >
  {
    public:
    typedef ::GraphIdViewByGraphClass view_type;
    typedef ::GraphIdViewByGraphClass* pointer_type;

    static void
    callback (database&, view_type&, callback_event);
  };

  // GraphIdViewByObjectClass
  //
  template <>
  struct class_traits< ::GraphIdViewByObjectClass >
  {
    static const class_kind kind = class_view;
  };

  template <>
  class access::view_traits< ::GraphIdViewByObjectClass >
  {
    public:
    typedef ::GraphIdViewByObjectClass view_type;
    typedef ::GraphIdViewByObjectClass* pointer_type;

    static void
    callback (database&, view_type&, callback_event);
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
  // Graph
  //
  template <typename A>
  struct pointer_query_columns< ::Graph, id_pgsql, A >
  {
    // idGraph
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        long unsigned int,
        pgsql::id_bigint >::query_type,
      pgsql::id_bigint >
    idGraph_type_;

    static const idGraph_type_ idGraph;

    // refGraphClass
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::std::string,
        pgsql::id_string >::query_type,
      pgsql::id_string >
    refGraphClass_type_;

    static const refGraphClass_type_ refGraphClass;

    // refObjectClass
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::std::string,
        pgsql::id_string >::query_type,
      pgsql::id_string >
    refObjectClass_type_;

    static const refObjectClass_type_ refObjectClass;

    // objectName
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::std::string,
        pgsql::id_string >::query_type,
      pgsql::id_string >
    objectName_type_;

    static const objectName_type_ objectName;

    // viewNumber
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        unsigned int,
        pgsql::id_integer >::query_type,
      pgsql::id_integer >
    viewNumber_type_;

    static const viewNumber_type_ viewNumber;
  };

  template <typename A>
  const typename pointer_query_columns< ::Graph, id_pgsql, A >::idGraph_type_
  pointer_query_columns< ::Graph, id_pgsql, A >::
  idGraph (A::table_name, "\"idGraph\"", 0);

  template <typename A>
  const typename pointer_query_columns< ::Graph, id_pgsql, A >::refGraphClass_type_
  pointer_query_columns< ::Graph, id_pgsql, A >::
  refGraphClass (A::table_name, "\"refGraphClass\"", 0);

  template <typename A>
  const typename pointer_query_columns< ::Graph, id_pgsql, A >::refObjectClass_type_
  pointer_query_columns< ::Graph, id_pgsql, A >::
  refObjectClass (A::table_name, "\"refObjectClass\"", 0);

  template <typename A>
  const typename pointer_query_columns< ::Graph, id_pgsql, A >::objectName_type_
  pointer_query_columns< ::Graph, id_pgsql, A >::
  objectName (A::table_name, "\"objectName\"", 0);

  template <typename A>
  const typename pointer_query_columns< ::Graph, id_pgsql, A >::viewNumber_type_
  pointer_query_columns< ::Graph, id_pgsql, A >::
  viewNumber (A::table_name, "\"viewNumber\"", 0);

  template <>
  class access::object_traits_impl< ::Graph, id_pgsql >:
    public access::object_traits< ::Graph >
  {
    public:
    struct id_image_type
    {
      long long id_value;
      bool id_null;

      std::size_t version;
    };

    struct image_type
    {
      // idGraph
      //
      long long idGraph_value;
      bool idGraph_null;

      // refGraphClass
      //
      details::buffer refGraphClass_value;
      std::size_t refGraphClass_size;
      bool refGraphClass_null;

      // refObjectClass
      //
      details::buffer refObjectClass_value;
      std::size_t refObjectClass_size;
      bool refObjectClass_null;

      // objectName
      //
      details::buffer objectName_value;
      std::size_t objectName_size;
      bool objectName_null;

      // viewNumber
      //
      int viewNumber_value;
      bool viewNumber_null;

      std::size_t version;
    };

    struct extra_statement_cache_type;

    struct refGraphClass_tag;
    struct refObjectClass_tag;

    using object_traits<object_type>::id;

    static id_type
    id (const id_image_type&);

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

    static const std::size_t column_count = 5UL;
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
    persist (database&, object_type&);

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
  class access::object_traits_impl< ::Graph, id_common >:
    public access::object_traits_impl< ::Graph, id_pgsql >
  {
  };

  // GraphIdViewByGraphClass
  //
  template <>
  class access::view_traits_impl< ::GraphIdViewByGraphClass, id_pgsql >:
    public access::view_traits< ::GraphIdViewByGraphClass >
  {
    public:
    struct image_type
    {
      // id
      //
      long long id_value;
      bool id_null;

      std::size_t version;
    };

    typedef pgsql::view_statements<view_type> statements_type;

    typedef pgsql::query_base query_base_type;
    struct query_columns;

    static const bool versioned = false;

    static bool
    grow (image_type&,
          bool*);

    static void
    bind (pgsql::bind*,
          image_type&);

    static void
    init (view_type&,
          const image_type&,
          database*);

    static const std::size_t column_count = 1UL;

    static query_base_type
    query_statement (const query_base_type&);

    static result<view_type>
    query (database&, const query_base_type&);

    static odb::details::shared_ptr<prepared_query_impl>
    prepare_query (connection&, const char*, const query_base_type&);

    static odb::details::shared_ptr<result_impl>
    execute_query (prepared_query_impl&);

    static const char query_statement_name[];
  };

  template <>
  class access::view_traits_impl< ::GraphIdViewByGraphClass, id_common >:
    public access::view_traits_impl< ::GraphIdViewByGraphClass, id_pgsql >
  {
  };

  // GraphIdViewByObjectClass
  //
  template <>
  class access::view_traits_impl< ::GraphIdViewByObjectClass, id_pgsql >:
    public access::view_traits< ::GraphIdViewByObjectClass >
  {
    public:
    struct image_type
    {
      // id
      //
      long long id_value;
      bool id_null;

      std::size_t version;
    };

    typedef pgsql::view_statements<view_type> statements_type;

    typedef pgsql::query_base query_base_type;
    struct query_columns;

    static const bool versioned = false;

    static bool
    grow (image_type&,
          bool*);

    static void
    bind (pgsql::bind*,
          image_type&);

    static void
    init (view_type&,
          const image_type&,
          database*);

    static const std::size_t column_count = 1UL;

    static query_base_type
    query_statement (const query_base_type&);

    static result<view_type>
    query (database&, const query_base_type&);

    static odb::details::shared_ptr<prepared_query_impl>
    prepare_query (connection&, const char*, const query_base_type&);

    static odb::details::shared_ptr<result_impl>
    execute_query (prepared_query_impl&);

    static const char query_statement_name[];
  };

  template <>
  class access::view_traits_impl< ::GraphIdViewByObjectClass, id_common >:
    public access::view_traits_impl< ::GraphIdViewByObjectClass, id_pgsql >
  {
  };

  // Graph
  //
  template <>
  struct alias_traits<
    ::GraphClass,
    id_pgsql,
    access::object_traits_impl< ::Graph, id_pgsql >::refGraphClass_tag>
  {
    static const char table_name[];
  };

  template <>
  struct alias_traits<
    ::ObjectClass,
    id_pgsql,
    access::object_traits_impl< ::Graph, id_pgsql >::refObjectClass_tag>
  {
    static const char table_name[];
  };

  template <>
  struct query_columns_base< ::Graph, id_pgsql >
  {
    // refGraphClass
    //
    typedef
    odb::alias_traits<
      ::GraphClass,
      id_pgsql,
      access::object_traits_impl< ::Graph, id_pgsql >::refGraphClass_tag>
    refGraphClass_alias_;

    // refObjectClass
    //
    typedef
    odb::alias_traits<
      ::ObjectClass,
      id_pgsql,
      access::object_traits_impl< ::Graph, id_pgsql >::refObjectClass_tag>
    refObjectClass_alias_;
  };

  template <typename A>
  struct query_columns< ::Graph, id_pgsql, A >:
    query_columns_base< ::Graph, id_pgsql >
  {
    // idGraph
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        long unsigned int,
        pgsql::id_bigint >::query_type,
      pgsql::id_bigint >
    idGraph_type_;

    static const idGraph_type_ idGraph;

    // refGraphClass
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::std::string,
        pgsql::id_string >::query_type,
      pgsql::id_string >
    refGraphClass_column_type_;

    typedef
    odb::query_pointer<
      odb::pointer_query_columns<
        ::GraphClass,
        id_pgsql,
        refGraphClass_alias_ > >
    refGraphClass_pointer_type_;

    struct refGraphClass_type_: refGraphClass_pointer_type_, refGraphClass_column_type_
    {
      refGraphClass_type_ (const char* t, const char* c, const char* conv)
        : refGraphClass_column_type_ (t, c, conv)
      {
      }
    };

    static const refGraphClass_type_ refGraphClass;

    // refObjectClass
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::std::string,
        pgsql::id_string >::query_type,
      pgsql::id_string >
    refObjectClass_column_type_;

    typedef
    odb::query_pointer<
      odb::pointer_query_columns<
        ::ObjectClass,
        id_pgsql,
        refObjectClass_alias_ > >
    refObjectClass_pointer_type_;

    struct refObjectClass_type_: refObjectClass_pointer_type_, refObjectClass_column_type_
    {
      refObjectClass_type_ (const char* t, const char* c, const char* conv)
        : refObjectClass_column_type_ (t, c, conv)
      {
      }
    };

    static const refObjectClass_type_ refObjectClass;

    // objectName
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        ::std::string,
        pgsql::id_string >::query_type,
      pgsql::id_string >
    objectName_type_;

    static const objectName_type_ objectName;

    // viewNumber
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        unsigned int,
        pgsql::id_integer >::query_type,
      pgsql::id_integer >
    viewNumber_type_;

    static const viewNumber_type_ viewNumber;
  };

  template <typename A>
  const typename query_columns< ::Graph, id_pgsql, A >::idGraph_type_
  query_columns< ::Graph, id_pgsql, A >::
  idGraph (A::table_name, "\"idGraph\"", 0);

  template <typename A>
  const typename query_columns< ::Graph, id_pgsql, A >::refGraphClass_type_
  query_columns< ::Graph, id_pgsql, A >::
  refGraphClass (A::table_name, "\"refGraphClass\"", 0);

  template <typename A>
  const typename query_columns< ::Graph, id_pgsql, A >::refObjectClass_type_
  query_columns< ::Graph, id_pgsql, A >::
  refObjectClass (A::table_name, "\"refObjectClass\"", 0);

  template <typename A>
  const typename query_columns< ::Graph, id_pgsql, A >::objectName_type_
  query_columns< ::Graph, id_pgsql, A >::
  objectName (A::table_name, "\"objectName\"", 0);

  template <typename A>
  const typename query_columns< ::Graph, id_pgsql, A >::viewNumber_type_
  query_columns< ::Graph, id_pgsql, A >::
  viewNumber (A::table_name, "\"viewNumber\"", 0);

  // GraphIdViewByGraphClass
  //
  struct access::view_traits_impl< ::GraphIdViewByGraphClass, id_pgsql >::query_columns:
    odb::pointer_query_columns<
      ::Graph,
      id_pgsql,
      odb::access::object_traits_impl< ::Graph, id_pgsql > >
  {
  };

  // GraphIdViewByObjectClass
  //
  struct access::view_traits_impl< ::GraphIdViewByObjectClass, id_pgsql >::query_columns:
    odb::pointer_query_columns<
      ::Graph,
      id_pgsql,
      odb::access::object_traits_impl< ::Graph, id_pgsql > >
  {
  };
}

#include "Graph-odb.ixx"

#include <odb/post.hxx>

#endif // GRAPH_ODB_HXX
