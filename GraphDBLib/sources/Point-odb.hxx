// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef POINT_ODB_HXX
#define POINT_ODB_HXX

#include <odb/version.hxx>

#if (ODB_VERSION != 20400UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "Point.h"

#include "Edge-odb.hxx"
#include "Graph-odb.hxx"
#include "GraphClass-odb.hxx"
#include "Node-odb.hxx"
#include "ObjectClass-odb.hxx"

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
  // Point
  //
  template <>
  struct class_traits< ::Point >
  {
    static const class_kind kind = class_object;
  };

  template <>
  class access::object_traits< ::Point >
  {
    public:
    typedef ::Point object_type;
    typedef ::Point* pointer_type;
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

  // pointsInNode
  //
  template <>
  struct class_traits< ::pointsInNode >
  {
    static const class_kind kind = class_view;
  };

  template <>
  class access::view_traits< ::pointsInNode >
  {
    public:
    typedef ::pointsInNode view_type;
    typedef ::pointsInNode* pointer_type;

    static void
    callback (database&, view_type&, callback_event);
  };

  // PointIdViewByGraph
  //
  template <>
  struct class_traits< ::PointIdViewByGraph >
  {
    static const class_kind kind = class_view;
  };

  template <>
  class access::view_traits< ::PointIdViewByGraph >
  {
    public:
    typedef ::PointIdViewByGraph view_type;
    typedef ::PointIdViewByGraph* pointer_type;

    static void
    callback (database&, view_type&, callback_event);
  };

  // PointIdViewByNode
  //
  template <>
  struct class_traits< ::PointIdViewByNode >
  {
    static const class_kind kind = class_view;
  };

  template <>
  class access::view_traits< ::PointIdViewByNode >
  {
    public:
    typedef ::PointIdViewByNode view_type;
    typedef ::PointIdViewByNode* pointer_type;

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
  // Point
  //
  template <typename A>
  struct pointer_query_columns< ::Point, id_pgsql, A >
  {
    // idPoint
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        long unsigned int,
        pgsql::id_bigint >::query_type,
      pgsql::id_bigint >
    idPoint_type_;

    static const idPoint_type_ idPoint;

    // xCoord
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        double,
        pgsql::id_double >::query_type,
      pgsql::id_double >
    xCoord_type_;

    static const xCoord_type_ xCoord;

    // yCoord
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        double,
        pgsql::id_double >::query_type,
      pgsql::id_double >
    yCoord_type_;

    static const yCoord_type_ yCoord;

    // radius
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        double,
        pgsql::id_double >::query_type,
      pgsql::id_double >
    radius_type_;

    static const radius_type_ radius;

    // refGraph
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        long unsigned int,
        pgsql::id_bigint >::query_type,
      pgsql::id_bigint >
    refGraph_type_;

    static const refGraph_type_ refGraph;

    // refNode
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        long unsigned int,
        pgsql::id_bigint >::query_type,
      pgsql::id_bigint >
    refNode_type_;

    static const refNode_type_ refNode;
  };

  template <typename A>
  const typename pointer_query_columns< ::Point, id_pgsql, A >::idPoint_type_
  pointer_query_columns< ::Point, id_pgsql, A >::
  idPoint (A::table_name, "\"idPoint\"", 0);

  template <typename A>
  const typename pointer_query_columns< ::Point, id_pgsql, A >::xCoord_type_
  pointer_query_columns< ::Point, id_pgsql, A >::
  xCoord (A::table_name, "\"xCoord\"", 0);

  template <typename A>
  const typename pointer_query_columns< ::Point, id_pgsql, A >::yCoord_type_
  pointer_query_columns< ::Point, id_pgsql, A >::
  yCoord (A::table_name, "\"yCoord\"", 0);

  template <typename A>
  const typename pointer_query_columns< ::Point, id_pgsql, A >::radius_type_
  pointer_query_columns< ::Point, id_pgsql, A >::
  radius (A::table_name, "\"radius\"", 0);

  template <typename A>
  const typename pointer_query_columns< ::Point, id_pgsql, A >::refGraph_type_
  pointer_query_columns< ::Point, id_pgsql, A >::
  refGraph (A::table_name, "\"refGraph\"", 0);

  template <typename A>
  const typename pointer_query_columns< ::Point, id_pgsql, A >::refNode_type_
  pointer_query_columns< ::Point, id_pgsql, A >::
  refNode (A::table_name, "\"refNode\"", 0);

  template <>
  class access::object_traits_impl< ::Point, id_pgsql >:
    public access::object_traits< ::Point >
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
      // idPoint
      //
      long long idPoint_value;
      bool idPoint_null;

      // xCoord
      //
      double xCoord_value;
      bool xCoord_null;

      // yCoord
      //
      double yCoord_value;
      bool yCoord_null;

      // radius
      //
      double radius_value;
      bool radius_null;

      // refGraph
      //
      long long refGraph_value;
      bool refGraph_null;

      // refNode
      //
      long long refNode_value;
      bool refNode_null;

      std::size_t version;
    };

    struct extra_statement_cache_type;

    struct refGraph_tag;
    struct refNode_tag;

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

    static const std::size_t column_count = 6UL;
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
  class access::object_traits_impl< ::Point, id_common >:
    public access::object_traits_impl< ::Point, id_pgsql >
  {
  };

  // pointsInNode
  //
  template <>
  class access::view_traits_impl< ::pointsInNode, id_pgsql >:
    public access::view_traits< ::pointsInNode >
  {
    public:
    struct image_type
    {
      // value
      //
      int value_value;
      bool value_null;

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
  class access::view_traits_impl< ::pointsInNode, id_common >:
    public access::view_traits_impl< ::pointsInNode, id_pgsql >
  {
  };

  // PointIdViewByGraph
  //
  template <>
  class access::view_traits_impl< ::PointIdViewByGraph, id_pgsql >:
    public access::view_traits< ::PointIdViewByGraph >
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
  class access::view_traits_impl< ::PointIdViewByGraph, id_common >:
    public access::view_traits_impl< ::PointIdViewByGraph, id_pgsql >
  {
  };

  // PointIdViewByNode
  //
  template <>
  class access::view_traits_impl< ::PointIdViewByNode, id_pgsql >:
    public access::view_traits< ::PointIdViewByNode >
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
  class access::view_traits_impl< ::PointIdViewByNode, id_common >:
    public access::view_traits_impl< ::PointIdViewByNode, id_pgsql >
  {
  };

  // Point
  //
  template <>
  struct alias_traits<
    ::Graph,
    id_pgsql,
    access::object_traits_impl< ::Point, id_pgsql >::refGraph_tag>
  {
    static const char table_name[];
  };

  template <>
  struct alias_traits<
    ::Node,
    id_pgsql,
    access::object_traits_impl< ::Point, id_pgsql >::refNode_tag>
  {
    static const char table_name[];
  };

  template <>
  struct query_columns_base< ::Point, id_pgsql >
  {
    // refGraph
    //
    typedef
    odb::alias_traits<
      ::Graph,
      id_pgsql,
      access::object_traits_impl< ::Point, id_pgsql >::refGraph_tag>
    refGraph_alias_;

    // refNode
    //
    typedef
    odb::alias_traits<
      ::Node,
      id_pgsql,
      access::object_traits_impl< ::Point, id_pgsql >::refNode_tag>
    refNode_alias_;
  };

  template <typename A>
  struct query_columns< ::Point, id_pgsql, A >:
    query_columns_base< ::Point, id_pgsql >
  {
    // idPoint
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        long unsigned int,
        pgsql::id_bigint >::query_type,
      pgsql::id_bigint >
    idPoint_type_;

    static const idPoint_type_ idPoint;

    // xCoord
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        double,
        pgsql::id_double >::query_type,
      pgsql::id_double >
    xCoord_type_;

    static const xCoord_type_ xCoord;

    // yCoord
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        double,
        pgsql::id_double >::query_type,
      pgsql::id_double >
    yCoord_type_;

    static const yCoord_type_ yCoord;

    // radius
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        double,
        pgsql::id_double >::query_type,
      pgsql::id_double >
    radius_type_;

    static const radius_type_ radius;

    // refGraph
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        long unsigned int,
        pgsql::id_bigint >::query_type,
      pgsql::id_bigint >
    refGraph_column_type_;

    typedef
    odb::query_pointer<
      odb::pointer_query_columns<
        ::Graph,
        id_pgsql,
        refGraph_alias_ > >
    refGraph_pointer_type_;

    struct refGraph_type_: refGraph_pointer_type_, refGraph_column_type_
    {
      refGraph_type_ (const char* t, const char* c, const char* conv)
        : refGraph_column_type_ (t, c, conv)
      {
      }
    };

    static const refGraph_type_ refGraph;

    // refNode
    //
    typedef
    pgsql::query_column<
      pgsql::value_traits<
        long unsigned int,
        pgsql::id_bigint >::query_type,
      pgsql::id_bigint >
    refNode_column_type_;

    typedef
    odb::query_pointer<
      odb::pointer_query_columns<
        ::Node,
        id_pgsql,
        refNode_alias_ > >
    refNode_pointer_type_;

    struct refNode_type_: refNode_pointer_type_, refNode_column_type_
    {
      refNode_type_ (const char* t, const char* c, const char* conv)
        : refNode_column_type_ (t, c, conv)
      {
      }
    };

    static const refNode_type_ refNode;
  };

  template <typename A>
  const typename query_columns< ::Point, id_pgsql, A >::idPoint_type_
  query_columns< ::Point, id_pgsql, A >::
  idPoint (A::table_name, "\"idPoint\"", 0);

  template <typename A>
  const typename query_columns< ::Point, id_pgsql, A >::xCoord_type_
  query_columns< ::Point, id_pgsql, A >::
  xCoord (A::table_name, "\"xCoord\"", 0);

  template <typename A>
  const typename query_columns< ::Point, id_pgsql, A >::yCoord_type_
  query_columns< ::Point, id_pgsql, A >::
  yCoord (A::table_name, "\"yCoord\"", 0);

  template <typename A>
  const typename query_columns< ::Point, id_pgsql, A >::radius_type_
  query_columns< ::Point, id_pgsql, A >::
  radius (A::table_name, "\"radius\"", 0);

  template <typename A>
  const typename query_columns< ::Point, id_pgsql, A >::refGraph_type_
  query_columns< ::Point, id_pgsql, A >::
  refGraph (A::table_name, "\"refGraph\"", 0);

  template <typename A>
  const typename query_columns< ::Point, id_pgsql, A >::refNode_type_
  query_columns< ::Point, id_pgsql, A >::
  refNode (A::table_name, "\"refNode\"", 0);

  // pointsInNode
  //
  struct access::view_traits_impl< ::pointsInNode, id_pgsql >::query_columns:
    odb::pointer_query_columns<
      ::Point,
      id_pgsql,
      odb::access::object_traits_impl< ::Point, id_pgsql > >
  {
  };

  // PointIdViewByGraph
  //
  struct access::view_traits_impl< ::PointIdViewByGraph, id_pgsql >::query_columns:
    odb::pointer_query_columns<
      ::Point,
      id_pgsql,
      odb::access::object_traits_impl< ::Point, id_pgsql > >
  {
  };

  // PointIdViewByNode
  //
  struct access::view_traits_impl< ::PointIdViewByNode, id_pgsql >::query_columns:
    odb::pointer_query_columns<
      ::Point,
      id_pgsql,
      odb::access::object_traits_impl< ::Point, id_pgsql > >
  {
  };
}

#include "Point-odb.ixx"

#include <odb/post.hxx>

#endif // POINT_ODB_HXX