// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#include <odb/pre.hxx>

#include "Graph-odb.hxx"

#include <cassert>
#include <cstring>  // std::memcpy


#include <odb/pgsql/traits.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/pgsql/transaction.hxx>
#include <odb/pgsql/connection.hxx>
#include <odb/pgsql/statement.hxx>
#include <odb/pgsql/statement-cache.hxx>
#include <odb/pgsql/simple-object-statements.hxx>
#include <odb/pgsql/container-statements.hxx>
#include <odb/pgsql/exceptions.hxx>
#include <odb/pgsql/simple-object-result.hxx>

namespace odb
{
  // Graph
  //

  const char access::object_traits_impl< ::Graph, id_pgsql >::
  persist_statement_name[] = "persist_Graph";

  const char access::object_traits_impl< ::Graph, id_pgsql >::
  find_statement_name[] = "find_Graph";

  const char access::object_traits_impl< ::Graph, id_pgsql >::
  update_statement_name[] = "update_Graph";

  const char access::object_traits_impl< ::Graph, id_pgsql >::
  erase_statement_name[] = "erase_Graph";

  const char access::object_traits_impl< ::Graph, id_pgsql >::
  query_statement_name[] = "query_Graph";

  const char access::object_traits_impl< ::Graph, id_pgsql >::
  erase_query_statement_name[] = "erase_query_Graph";

  const unsigned int access::object_traits_impl< ::Graph, id_pgsql >::
  persist_statement_types[] =
  {
    pgsql::text_oid,
    pgsql::text_oid,
    pgsql::text_oid,
    pgsql::int4_oid
  };

  const unsigned int access::object_traits_impl< ::Graph, id_pgsql >::
  find_statement_types[] =
  {
    pgsql::int8_oid
  };

  const unsigned int access::object_traits_impl< ::Graph, id_pgsql >::
  update_statement_types[] =
  {
    pgsql::text_oid,
    pgsql::text_oid,
    pgsql::text_oid,
    pgsql::int4_oid,
    pgsql::int8_oid
  };

  const char alias_traits<  ::GraphClass,
    id_pgsql,
    access::object_traits_impl< ::Graph, id_pgsql >::graphClass_tag>::
  table_name[] = "\"graphClass\"";

  const char alias_traits<  ::ObjectClass,
    id_pgsql,
    access::object_traits_impl< ::Graph, id_pgsql >::objectClass_tag>::
  table_name[] = "\"objectClass\"";

  struct access::object_traits_impl< ::Graph, id_pgsql >::extra_statement_cache_type
  {
    extra_statement_cache_type (
      pgsql::connection&,
      image_type&,
      id_image_type&,
      pgsql::binding&,
      pgsql::binding&,
      pgsql::native_binding&,
      const unsigned int*)
    {
    }
  };

  access::object_traits_impl< ::Graph, id_pgsql >::id_type
  access::object_traits_impl< ::Graph, id_pgsql >::
  id (const id_image_type& i)
  {
    pgsql::database* db (0);
    ODB_POTENTIALLY_UNUSED (db);

    id_type id;
    {
      pgsql::value_traits<
          long unsigned int,
          pgsql::id_bigint >::set_value (
        id,
        i.id_value,
        i.id_null);
    }

    return id;
  }

  access::object_traits_impl< ::Graph, id_pgsql >::id_type
  access::object_traits_impl< ::Graph, id_pgsql >::
  id (const image_type& i)
  {
    pgsql::database* db (0);
    ODB_POTENTIALLY_UNUSED (db);

    id_type id;
    {
      pgsql::value_traits<
          long unsigned int,
          pgsql::id_bigint >::set_value (
        id,
        i.idGraph_value,
        i.idGraph_null);
    }

    return id;
  }

  bool access::object_traits_impl< ::Graph, id_pgsql >::
  grow (image_type& i,
        bool* t)
  {
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (t);

    bool grew (false);

    // idGraph
    //
    t[0UL] = 0;

    // graphClass
    //
    if (t[1UL])
    {
      i.graphClass_value.capacity (i.graphClass_size);
      grew = true;
    }

    // objectClass
    //
    if (t[2UL])
    {
      i.objectClass_value.capacity (i.objectClass_size);
      grew = true;
    }

    // objectName
    //
    if (t[3UL])
    {
      i.objectName_value.capacity (i.objectName_size);
      grew = true;
    }

    // viewNumber
    //
    t[4UL] = 0;

    return grew;
  }

  void access::object_traits_impl< ::Graph, id_pgsql >::
  bind (pgsql::bind* b,
        image_type& i,
        pgsql::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace pgsql;

    std::size_t n (0);

    // idGraph
    //
    if (sk != statement_insert && sk != statement_update)
    {
      b[n].type = pgsql::bind::bigint;
      b[n].buffer = &i.idGraph_value;
      b[n].is_null = &i.idGraph_null;
      n++;
    }

    // graphClass
    //
    b[n].type = pgsql::bind::text;
    b[n].buffer = i.graphClass_value.data ();
    b[n].capacity = i.graphClass_value.capacity ();
    b[n].size = &i.graphClass_size;
    b[n].is_null = &i.graphClass_null;
    n++;

    // objectClass
    //
    b[n].type = pgsql::bind::text;
    b[n].buffer = i.objectClass_value.data ();
    b[n].capacity = i.objectClass_value.capacity ();
    b[n].size = &i.objectClass_size;
    b[n].is_null = &i.objectClass_null;
    n++;

    // objectName
    //
    b[n].type = pgsql::bind::text;
    b[n].buffer = i.objectName_value.data ();
    b[n].capacity = i.objectName_value.capacity ();
    b[n].size = &i.objectName_size;
    b[n].is_null = &i.objectName_null;
    n++;

    // viewNumber
    //
    b[n].type = pgsql::bind::integer;
    b[n].buffer = &i.viewNumber_value;
    b[n].is_null = &i.viewNumber_null;
    n++;
  }

  void access::object_traits_impl< ::Graph, id_pgsql >::
  bind (pgsql::bind* b, id_image_type& i)
  {
    std::size_t n (0);
    b[n].type = pgsql::bind::bigint;
    b[n].buffer = &i.id_value;
    b[n].is_null = &i.id_null;
  }

  bool access::object_traits_impl< ::Graph, id_pgsql >::
  init (image_type& i,
        const object_type& o,
        pgsql::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (o);
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace pgsql;

    bool grew (false);

    // graphClass
    //
    {
      ::GraphClass* const& v =
        o.graphClass;

      typedef object_traits< ::GraphClass > obj_traits;
      typedef odb::pointer_traits< ::GraphClass* > ptr_traits;

      bool is_null (ptr_traits::null_ptr (v));
      if (!is_null)
      {
        const obj_traits::id_type& id (
          obj_traits::id (ptr_traits::get_ref (v)));

        std::size_t size (0);
        std::size_t cap (i.graphClass_value.capacity ());
        pgsql::value_traits<
            obj_traits::id_type,
            pgsql::id_string >::set_image (
          i.graphClass_value,
          size,
          is_null,
          id);
        i.graphClass_null = is_null;
        i.graphClass_size = size;
        grew = grew || (cap != i.graphClass_value.capacity ());
      }
      else
        throw null_pointer ();
    }

    // objectClass
    //
    {
      ::ObjectClass* const& v =
        o.objectClass;

      typedef object_traits< ::ObjectClass > obj_traits;
      typedef odb::pointer_traits< ::ObjectClass* > ptr_traits;

      bool is_null (ptr_traits::null_ptr (v));
      if (!is_null)
      {
        const obj_traits::id_type& id (
          obj_traits::id (ptr_traits::get_ref (v)));

        std::size_t size (0);
        std::size_t cap (i.objectClass_value.capacity ());
        pgsql::value_traits<
            obj_traits::id_type,
            pgsql::id_string >::set_image (
          i.objectClass_value,
          size,
          is_null,
          id);
        i.objectClass_null = is_null;
        i.objectClass_size = size;
        grew = grew || (cap != i.objectClass_value.capacity ());
      }
      else
        throw null_pointer ();
    }

    // objectName
    //
    {
      ::std::string const& v =
        o.objectName;

      bool is_null (false);
      std::size_t size (0);
      std::size_t cap (i.objectName_value.capacity ());
      pgsql::value_traits<
          ::std::string,
          pgsql::id_string >::set_image (
        i.objectName_value,
        size,
        is_null,
        v);
      i.objectName_null = is_null;
      i.objectName_size = size;
      grew = grew || (cap != i.objectName_value.capacity ());
    }

    // viewNumber
    //
    {
      unsigned int const& v =
        o.viewNumber;

      bool is_null (false);
      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_image (
        i.viewNumber_value, is_null, v);
      i.viewNumber_null = is_null;
    }

    return grew;
  }

  void access::object_traits_impl< ::Graph, id_pgsql >::
  init (object_type& o,
        const image_type& i,
        database* db)
  {
    ODB_POTENTIALLY_UNUSED (o);
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (db);

    // idGraph
    //
    {
      long unsigned int& v =
        o.idGraph;

      pgsql::value_traits<
          long unsigned int,
          pgsql::id_bigint >::set_value (
        v,
        i.idGraph_value,
        i.idGraph_null);
    }

    // graphClass
    //
    {
      ::GraphClass*& v =
        o.graphClass;

      typedef object_traits< ::GraphClass > obj_traits;
      typedef odb::pointer_traits< ::GraphClass* > ptr_traits;

      if (i.graphClass_null)
        v = ptr_traits::pointer_type ();
      else
      {
        obj_traits::id_type id;
        pgsql::value_traits<
            obj_traits::id_type,
            pgsql::id_string >::set_value (
          id,
          i.graphClass_value,
          i.graphClass_size,
          i.graphClass_null);

        // If a compiler error points to the line below, then
        // it most likely means that a pointer used in a member
        // cannot be initialized from an object pointer.
        //
        v = ptr_traits::pointer_type (
          static_cast<pgsql::database*> (db)->load<
            obj_traits::object_type > (id));
      }
    }

    // objectClass
    //
    {
      ::ObjectClass*& v =
        o.objectClass;

      typedef object_traits< ::ObjectClass > obj_traits;
      typedef odb::pointer_traits< ::ObjectClass* > ptr_traits;

      if (i.objectClass_null)
        v = ptr_traits::pointer_type ();
      else
      {
        obj_traits::id_type id;
        pgsql::value_traits<
            obj_traits::id_type,
            pgsql::id_string >::set_value (
          id,
          i.objectClass_value,
          i.objectClass_size,
          i.objectClass_null);

        // If a compiler error points to the line below, then
        // it most likely means that a pointer used in a member
        // cannot be initialized from an object pointer.
        //
        v = ptr_traits::pointer_type (
          static_cast<pgsql::database*> (db)->load<
            obj_traits::object_type > (id));
      }
    }

    // objectName
    //
    {
      ::std::string& v =
        o.objectName;

      pgsql::value_traits<
          ::std::string,
          pgsql::id_string >::set_value (
        v,
        i.objectName_value,
        i.objectName_size,
        i.objectName_null);
    }

    // viewNumber
    //
    {
      unsigned int& v =
        o.viewNumber;

      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_value (
        v,
        i.viewNumber_value,
        i.viewNumber_null);
    }
  }

  void access::object_traits_impl< ::Graph, id_pgsql >::
  init (id_image_type& i, const id_type& id)
  {
    {
      bool is_null (false);
      pgsql::value_traits<
          long unsigned int,
          pgsql::id_bigint >::set_image (
        i.id_value, is_null, id);
      i.id_null = is_null;
    }
  }

  const char access::object_traits_impl< ::Graph, id_pgsql >::persist_statement[] =
  "INSERT INTO \"Graph\" "
  "(\"idGraph\", "
  "\"graphClass\", "
  "\"objectClass\", "
  "\"objectName\", "
  "\"viewNumber\") "
  "VALUES "
  "(DEFAULT, $1, $2, $3, $4) "
  "RETURNING \"idGraph\"";

  const char access::object_traits_impl< ::Graph, id_pgsql >::find_statement[] =
  "SELECT "
  "\"Graph\".\"idGraph\", "
  "\"Graph\".\"graphClass\", "
  "\"Graph\".\"objectClass\", "
  "\"Graph\".\"objectName\", "
  "\"Graph\".\"viewNumber\" "
  "FROM \"Graph\" "
  "WHERE \"Graph\".\"idGraph\"=$1";

  const char access::object_traits_impl< ::Graph, id_pgsql >::update_statement[] =
  "UPDATE \"Graph\" "
  "SET "
  "\"graphClass\"=$1, "
  "\"objectClass\"=$2, "
  "\"objectName\"=$3, "
  "\"viewNumber\"=$4 "
  "WHERE \"idGraph\"=$5";

  const char access::object_traits_impl< ::Graph, id_pgsql >::erase_statement[] =
  "DELETE FROM \"Graph\" "
  "WHERE \"idGraph\"=$1";

  const char access::object_traits_impl< ::Graph, id_pgsql >::query_statement[] =
  "SELECT\n"
  "\"Graph\".\"idGraph\",\n"
  "\"Graph\".\"graphClass\",\n"
  "\"Graph\".\"objectClass\",\n"
  "\"Graph\".\"objectName\",\n"
  "\"Graph\".\"viewNumber\"\n"
  "FROM \"Graph\"\n"
  "LEFT JOIN \"GraphClass\" AS \"graphClass\" ON \"graphClass\".\"graphClassName\"=\"Graph\".\"graphClass\"\n"
  "LEFT JOIN \"ObjectClass\" AS \"objectClass\" ON \"objectClass\".\"objectClassName\"=\"Graph\".\"objectClass\"";

  const char access::object_traits_impl< ::Graph, id_pgsql >::erase_query_statement[] =
  "DELETE FROM \"Graph\"";

  const char access::object_traits_impl< ::Graph, id_pgsql >::table_name[] =
  "\"Graph\"";

  void access::object_traits_impl< ::Graph, id_pgsql >::
  persist (database& db, object_type& obj)
  {
    ODB_POTENTIALLY_UNUSED (db);

    using namespace pgsql;

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    callback (db,
              static_cast<const object_type&> (obj),
              callback_event::pre_persist);

    image_type& im (sts.image ());
    binding& imb (sts.insert_image_binding ());

    if (init (im, obj, statement_insert))
      im.version++;

    if (im.version != sts.insert_image_version () ||
        imb.version == 0)
    {
      bind (imb.bind, im, statement_insert);
      sts.insert_image_version (im.version);
      imb.version++;
    }

    {
      id_image_type& i (sts.id_image ());
      binding& b (sts.id_image_binding ());
      if (i.version != sts.id_image_version () || b.version == 0)
      {
        bind (b.bind, i);
        sts.id_image_version (i.version);
        b.version++;
      }
    }

    insert_statement& st (sts.persist_statement ());
    if (!st.execute ())
      throw object_already_persistent ();

    obj.idGraph = id (sts.id_image ());

    callback (db,
              static_cast<const object_type&> (obj),
              callback_event::post_persist);
  }

  void access::object_traits_impl< ::Graph, id_pgsql >::
  update (database& db, const object_type& obj)
  {
    ODB_POTENTIALLY_UNUSED (db);

    using namespace pgsql;
    using pgsql::update_statement;

    callback (db, obj, callback_event::pre_update);

    pgsql::transaction& tr (pgsql::transaction::current ());
    pgsql::connection& conn (tr.connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    const id_type& id (
      obj.idGraph);
    id_image_type& idi (sts.id_image ());
    init (idi, id);

    image_type& im (sts.image ());
    if (init (im, obj, statement_update))
      im.version++;

    bool u (false);
    binding& imb (sts.update_image_binding ());
    if (im.version != sts.update_image_version () ||
        imb.version == 0)
    {
      bind (imb.bind, im, statement_update);
      sts.update_image_version (im.version);
      imb.version++;
      u = true;
    }

    binding& idb (sts.id_image_binding ());
    if (idi.version != sts.update_id_image_version () ||
        idb.version == 0)
    {
      if (idi.version != sts.id_image_version () ||
          idb.version == 0)
      {
        bind (idb.bind, idi);
        sts.id_image_version (idi.version);
        idb.version++;
      }

      sts.update_id_image_version (idi.version);

      if (!u)
        imb.version++;
    }

    update_statement& st (sts.update_statement ());
    if (st.execute () == 0)
      throw object_not_persistent ();

    callback (db, obj, callback_event::post_update);
    pointer_cache_traits::update (db, obj);
  }

  void access::object_traits_impl< ::Graph, id_pgsql >::
  erase (database& db, const id_type& id)
  {
    using namespace pgsql;

    ODB_POTENTIALLY_UNUSED (db);

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    id_image_type& i (sts.id_image ());
    init (i, id);

    binding& idb (sts.id_image_binding ());
    if (i.version != sts.id_image_version () || idb.version == 0)
    {
      bind (idb.bind, i);
      sts.id_image_version (i.version);
      idb.version++;
    }

    if (sts.erase_statement ().execute () != 1)
      throw object_not_persistent ();

    pointer_cache_traits::erase (db, id);
  }

  access::object_traits_impl< ::Graph, id_pgsql >::pointer_type
  access::object_traits_impl< ::Graph, id_pgsql >::
  find (database& db, const id_type& id)
  {
    using namespace pgsql;

    {
      pointer_type p (pointer_cache_traits::find (db, id));

      if (!pointer_traits::null_ptr (p))
        return p;
    }

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    statements_type::auto_lock l (sts);

    if (l.locked ())
    {
      if (!find_ (sts, &id))
        return pointer_type ();
    }

    pointer_type p (
      access::object_factory<object_type, pointer_type>::create ());
    pointer_traits::guard pg (p);

    pointer_cache_traits::insert_guard ig (
      pointer_cache_traits::insert (db, id, p));

    object_type& obj (pointer_traits::get_ref (p));

    if (l.locked ())
    {
      select_statement& st (sts.find_statement ());
      ODB_POTENTIALLY_UNUSED (st);

      callback (db, obj, callback_event::pre_load);
      init (obj, sts.image (), &db);
      load_ (sts, obj, false);
      sts.load_delayed (0);
      l.unlock ();
      callback (db, obj, callback_event::post_load);
      pointer_cache_traits::load (ig.position ());
    }
    else
      sts.delay_load (id, obj, ig.position ());

    ig.release ();
    pg.release ();
    return p;
  }

  bool access::object_traits_impl< ::Graph, id_pgsql >::
  find (database& db, const id_type& id, object_type& obj)
  {
    using namespace pgsql;

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    statements_type::auto_lock l (sts);

    if (!find_ (sts, &id))
      return false;

    select_statement& st (sts.find_statement ());
    ODB_POTENTIALLY_UNUSED (st);

    reference_cache_traits::position_type pos (
      reference_cache_traits::insert (db, id, obj));
    reference_cache_traits::insert_guard ig (pos);

    callback (db, obj, callback_event::pre_load);
    init (obj, sts.image (), &db);
    load_ (sts, obj, false);
    sts.load_delayed (0);
    l.unlock ();
    callback (db, obj, callback_event::post_load);
    reference_cache_traits::load (pos);
    ig.release ();
    return true;
  }

  bool access::object_traits_impl< ::Graph, id_pgsql >::
  reload (database& db, object_type& obj)
  {
    using namespace pgsql;

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    statements_type::auto_lock l (sts);

    const id_type& id  (
      obj.idGraph);

    if (!find_ (sts, &id))
      return false;

    select_statement& st (sts.find_statement ());
    ODB_POTENTIALLY_UNUSED (st);

    callback (db, obj, callback_event::pre_load);
    init (obj, sts.image (), &db);
    load_ (sts, obj, true);
    sts.load_delayed (0);
    l.unlock ();
    callback (db, obj, callback_event::post_load);
    return true;
  }

  bool access::object_traits_impl< ::Graph, id_pgsql >::
  find_ (statements_type& sts,
         const id_type* id)
  {
    using namespace pgsql;

    id_image_type& i (sts.id_image ());
    init (i, *id);

    binding& idb (sts.id_image_binding ());
    if (i.version != sts.id_image_version () || idb.version == 0)
    {
      bind (idb.bind, i);
      sts.id_image_version (i.version);
      idb.version++;
    }

    image_type& im (sts.image ());
    binding& imb (sts.select_image_binding ());

    if (im.version != sts.select_image_version () ||
        imb.version == 0)
    {
      bind (imb.bind, im, statement_select);
      sts.select_image_version (im.version);
      imb.version++;
    }

    select_statement& st (sts.find_statement ());

    st.execute ();
    auto_result ar (st);
    select_statement::result r (st.fetch ());

    if (r == select_statement::truncated)
    {
      if (grow (im, sts.select_image_truncated ()))
        im.version++;

      if (im.version != sts.select_image_version ())
      {
        bind (imb.bind, im, statement_select);
        sts.select_image_version (im.version);
        imb.version++;
        st.refetch ();
      }
    }

    return r != select_statement::no_data;
  }

  result< access::object_traits_impl< ::Graph, id_pgsql >::object_type >
  access::object_traits_impl< ::Graph, id_pgsql >::
  query (database&, const query_base_type& q)
  {
    using namespace pgsql;
    using odb::details::shared;
    using odb::details::shared_ptr;

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());

    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    image_type& im (sts.image ());
    binding& imb (sts.select_image_binding ());

    if (im.version != sts.select_image_version () ||
        imb.version == 0)
    {
      bind (imb.bind, im, statement_select);
      sts.select_image_version (im.version);
      imb.version++;
    }

    std::string text (query_statement);
    if (!q.empty ())
    {
      text += "\n";
      text += q.clause ();
    }

    q.init_parameters ();
    shared_ptr<select_statement> st (
      new (shared) select_statement (
        sts.connection (),
        query_statement_name,
        text,
        true,
        true,
        q.parameter_types (),
        q.parameter_count (),
        q.parameters_binding (),
        imb));

    st->execute ();
    st->deallocate ();

    shared_ptr< odb::object_result_impl<object_type> > r (
      new (shared) pgsql::object_result_impl<object_type> (
        q, st, sts, 0));

    return result<object_type> (r);
  }

  unsigned long long access::object_traits_impl< ::Graph, id_pgsql >::
  erase_query (database&, const query_base_type& q)
  {
    using namespace pgsql;

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());

    std::string text (erase_query_statement);
    if (!q.empty ())
    {
      text += ' ';
      text += q.clause ();
    }

    q.init_parameters ();
    delete_statement st (
      conn,
      erase_query_statement_name,
      text,
      q.parameter_types (),
      q.parameter_count (),
      q.parameters_binding ());

    return st.execute ();
  }
}

#include <odb/post.hxx>
