// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#include <odb/pre.hxx>

#include "Node-odb.hxx"

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
#include <odb/pgsql/prepared-query.hxx>
#include <odb/pgsql/simple-object-result.hxx>

namespace odb
{
  // Node
  //

  const char access::object_traits_impl< ::Node, id_pgsql >::
  persist_statement_name[] = "persist_Node";

  const char access::object_traits_impl< ::Node, id_pgsql >::
  find_statement_name[] = "find_Node";

  const char access::object_traits_impl< ::Node, id_pgsql >::
  update_statement_name[] = "update_Node";

  const char access::object_traits_impl< ::Node, id_pgsql >::
  erase_statement_name[] = "erase_Node";

  const char access::object_traits_impl< ::Node, id_pgsql >::
  query_statement_name[] = "query_Node";

  const char access::object_traits_impl< ::Node, id_pgsql >::
  erase_query_statement_name[] = "erase_query_Node";

  const unsigned int access::object_traits_impl< ::Node, id_pgsql >::
  persist_statement_types[] =
  {
    pgsql::int4_oid,
    pgsql::int4_oid,
    pgsql::int4_oid,
    pgsql::int4_oid,
    pgsql::text_oid,
    pgsql::int8_oid
  };

  const unsigned int access::object_traits_impl< ::Node, id_pgsql >::
  find_statement_types[] =
  {
    pgsql::int8_oid
  };

  const unsigned int access::object_traits_impl< ::Node, id_pgsql >::
  update_statement_types[] =
  {
    pgsql::int4_oid,
    pgsql::int4_oid,
    pgsql::int4_oid,
    pgsql::int4_oid,
    pgsql::text_oid,
    pgsql::int8_oid,
    pgsql::int8_oid
  };

  const char alias_traits<  ::Graph,
    id_pgsql,
    access::object_traits_impl< ::Node, id_pgsql >::refGraph_tag>::
  table_name[] = "\"refGraph\"";

  struct access::object_traits_impl< ::Node, id_pgsql >::extra_statement_cache_type
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

  access::object_traits_impl< ::Node, id_pgsql >::id_type
  access::object_traits_impl< ::Node, id_pgsql >::
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

  access::object_traits_impl< ::Node, id_pgsql >::id_type
  access::object_traits_impl< ::Node, id_pgsql >::
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
        i.idNode_value,
        i.idNode_null);
    }

    return id;
  }

  bool access::object_traits_impl< ::Node, id_pgsql >::
  grow (image_type& i,
        bool* t)
  {
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (t);

    bool grew (false);

    // idNode
    //
    t[0UL] = 0;

    // index
    //
    t[1UL] = 0;

    // level
    //
    t[2UL] = 0;

    // mass
    //
    t[3UL] = 0;

    // type
    //
    t[4UL] = 0;

    // label
    //
    if (t[5UL])
    {
      i.label_value.capacity (i.label_size);
      grew = true;
    }

    // refGraph
    //
    t[6UL] = 0;

    return grew;
  }

  void access::object_traits_impl< ::Node, id_pgsql >::
  bind (pgsql::bind* b,
        image_type& i,
        pgsql::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace pgsql;

    std::size_t n (0);

    // idNode
    //
    if (sk != statement_insert && sk != statement_update)
    {
      b[n].type = pgsql::bind::bigint;
      b[n].buffer = &i.idNode_value;
      b[n].is_null = &i.idNode_null;
      n++;
    }

    // index
    //
    b[n].type = pgsql::bind::integer;
    b[n].buffer = &i.index_value;
    b[n].is_null = &i.index_null;
    n++;

    // level
    //
    b[n].type = pgsql::bind::integer;
    b[n].buffer = &i.level_value;
    b[n].is_null = &i.level_null;
    n++;

    // mass
    //
    b[n].type = pgsql::bind::integer;
    b[n].buffer = &i.mass_value;
    b[n].is_null = &i.mass_null;
    n++;

    // type
    //
    b[n].type = pgsql::bind::integer;
    b[n].buffer = &i.type_value;
    b[n].is_null = &i.type_null;
    n++;

    // label
    //
    b[n].type = pgsql::bind::text;
    b[n].buffer = i.label_value.data ();
    b[n].capacity = i.label_value.capacity ();
    b[n].size = &i.label_size;
    b[n].is_null = &i.label_null;
    n++;

    // refGraph
    //
    b[n].type = pgsql::bind::bigint;
    b[n].buffer = &i.refGraph_value;
    b[n].is_null = &i.refGraph_null;
    n++;
  }

  void access::object_traits_impl< ::Node, id_pgsql >::
  bind (pgsql::bind* b, id_image_type& i)
  {
    std::size_t n (0);
    b[n].type = pgsql::bind::bigint;
    b[n].buffer = &i.id_value;
    b[n].is_null = &i.id_null;
  }

  bool access::object_traits_impl< ::Node, id_pgsql >::
  init (image_type& i,
        const object_type& o,
        pgsql::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (o);
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace pgsql;

    bool grew (false);

    // index
    //
    {
      unsigned int const& v =
        o.index;

      bool is_null (false);
      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_image (
        i.index_value, is_null, v);
      i.index_null = is_null;
    }

    // level
    //
    {
      unsigned int const& v =
        o.level;

      bool is_null (false);
      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_image (
        i.level_value, is_null, v);
      i.level_null = is_null;
    }

    // mass
    //
    {
      unsigned int const& v =
        o.mass;

      bool is_null (false);
      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_image (
        i.mass_value, is_null, v);
      i.mass_null = is_null;
    }

    // type
    //
    {
      unsigned int const& v =
        o.type;

      bool is_null (false);
      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_image (
        i.type_value, is_null, v);
      i.type_null = is_null;
    }

    // label
    //
    {
      ::std::string const& v =
        o.label;

      bool is_null (false);
      std::size_t size (0);
      std::size_t cap (i.label_value.capacity ());
      pgsql::value_traits<
          ::std::string,
          pgsql::id_string >::set_image (
        i.label_value,
        size,
        is_null,
        v);
      i.label_null = is_null;
      i.label_size = size;
      grew = grew || (cap != i.label_value.capacity ());
    }

    // refGraph
    //
    {
      ::odb::boost::lazy_weak_ptr< ::Graph > const& v =
        o.refGraph;

      typedef object_traits< ::Graph > obj_traits;
      typedef odb::pointer_traits< ::odb::boost::lazy_weak_ptr< ::Graph > > wptr_traits;
      typedef odb::pointer_traits< wptr_traits::strong_pointer_type > ptr_traits;

      wptr_traits::strong_pointer_type sp (wptr_traits::lock (v));
      bool is_null (ptr_traits::null_ptr (sp));
      if (!is_null)
      {
        const obj_traits::id_type& id (
          ptr_traits::object_id< ptr_traits::element_type  > (sp));

        pgsql::value_traits<
            obj_traits::id_type,
            pgsql::id_bigint >::set_image (
          i.refGraph_value, is_null, id);
        i.refGraph_null = is_null;
      }
      else
        throw null_pointer ();
    }

    return grew;
  }

  void access::object_traits_impl< ::Node, id_pgsql >::
  init (object_type& o,
        const image_type& i,
        database* db)
  {
    ODB_POTENTIALLY_UNUSED (o);
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (db);

    // idNode
    //
    {
      long unsigned int& v =
        o.idNode;

      pgsql::value_traits<
          long unsigned int,
          pgsql::id_bigint >::set_value (
        v,
        i.idNode_value,
        i.idNode_null);
    }

    // index
    //
    {
      unsigned int& v =
        o.index;

      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_value (
        v,
        i.index_value,
        i.index_null);
    }

    // level
    //
    {
      unsigned int& v =
        o.level;

      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_value (
        v,
        i.level_value,
        i.level_null);
    }

    // mass
    //
    {
      unsigned int& v =
        o.mass;

      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_value (
        v,
        i.mass_value,
        i.mass_null);
    }

    // type
    //
    {
      unsigned int& v =
        o.type;

      pgsql::value_traits<
          unsigned int,
          pgsql::id_integer >::set_value (
        v,
        i.type_value,
        i.type_null);
    }

    // label
    //
    {
      ::std::string& v =
        o.label;

      pgsql::value_traits<
          ::std::string,
          pgsql::id_string >::set_value (
        v,
        i.label_value,
        i.label_size,
        i.label_null);
    }

    // refGraph
    //
    {
      ::odb::boost::lazy_weak_ptr< ::Graph >& v =
        o.refGraph;

      typedef object_traits< ::Graph > obj_traits;
      typedef odb::pointer_traits< ::odb::boost::lazy_weak_ptr< ::Graph > > ptr_traits;

      if (i.refGraph_null)
        v = ptr_traits::pointer_type ();
      else
      {
        obj_traits::id_type id;
        pgsql::value_traits<
            obj_traits::id_type,
            pgsql::id_bigint >::set_value (
          id,
          i.refGraph_value,
          i.refGraph_null);

        v = ptr_traits::pointer_type (
          *static_cast<pgsql::database*> (db), id);
      }
    }
  }

  void access::object_traits_impl< ::Node, id_pgsql >::
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

  const char access::object_traits_impl< ::Node, id_pgsql >::persist_statement[] =
  "INSERT INTO \"Node\" "
  "(\"idNode\", "
  "\"index\", "
  "\"level\", "
  "\"mass\", "
  "\"type\", "
  "\"label\", "
  "\"refGraph\") "
  "VALUES "
  "(DEFAULT, $1, $2, $3, $4, $5, $6) "
  "RETURNING \"idNode\"";

  const char access::object_traits_impl< ::Node, id_pgsql >::find_statement[] =
  "SELECT "
  "\"Node\".\"idNode\", "
  "\"Node\".\"index\", "
  "\"Node\".\"level\", "
  "\"Node\".\"mass\", "
  "\"Node\".\"type\", "
  "\"Node\".\"label\", "
  "\"Node\".\"refGraph\" "
  "FROM \"Node\" "
  "WHERE \"Node\".\"idNode\"=$1";

  const char access::object_traits_impl< ::Node, id_pgsql >::update_statement[] =
  "UPDATE \"Node\" "
  "SET "
  "\"index\"=$1, "
  "\"level\"=$2, "
  "\"mass\"=$3, "
  "\"type\"=$4, "
  "\"label\"=$5, "
  "\"refGraph\"=$6 "
  "WHERE \"idNode\"=$7";

  const char access::object_traits_impl< ::Node, id_pgsql >::erase_statement[] =
  "DELETE FROM \"Node\" "
  "WHERE \"idNode\"=$1";

  const char access::object_traits_impl< ::Node, id_pgsql >::query_statement[] =
  "SELECT\n"
  "\"Node\".\"idNode\",\n"
  "\"Node\".\"index\",\n"
  "\"Node\".\"level\",\n"
  "\"Node\".\"mass\",\n"
  "\"Node\".\"type\",\n"
  "\"Node\".\"label\",\n"
  "\"Node\".\"refGraph\"\n"
  "FROM \"Node\"\n"
  "LEFT JOIN \"Graph\" AS \"refGraph\" ON \"refGraph\".\"idGraph\"=\"Node\".\"refGraph\"";

  const char access::object_traits_impl< ::Node, id_pgsql >::erase_query_statement[] =
  "DELETE FROM \"Node\"";

  const char access::object_traits_impl< ::Node, id_pgsql >::table_name[] =
  "\"Node\"";

  void access::object_traits_impl< ::Node, id_pgsql >::
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

    obj.idNode = id (sts.id_image ());

    callback (db,
              static_cast<const object_type&> (obj),
              callback_event::post_persist);
  }

  void access::object_traits_impl< ::Node, id_pgsql >::
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
      obj.idNode);
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

  void access::object_traits_impl< ::Node, id_pgsql >::
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

  access::object_traits_impl< ::Node, id_pgsql >::pointer_type
  access::object_traits_impl< ::Node, id_pgsql >::
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

  bool access::object_traits_impl< ::Node, id_pgsql >::
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

  bool access::object_traits_impl< ::Node, id_pgsql >::
  reload (database& db, object_type& obj)
  {
    using namespace pgsql;

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    statements_type::auto_lock l (sts);

    const id_type& id  (
      obj.idNode);

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

  bool access::object_traits_impl< ::Node, id_pgsql >::
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

  result< access::object_traits_impl< ::Node, id_pgsql >::object_type >
  access::object_traits_impl< ::Node, id_pgsql >::
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

  unsigned long long access::object_traits_impl< ::Node, id_pgsql >::
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

  odb::details::shared_ptr<prepared_query_impl>
  access::object_traits_impl< ::Node, id_pgsql >::
  prepare_query (connection& c, const char* n, const query_base_type& q)
  {
    using namespace pgsql;
    using odb::details::shared;
    using odb::details::shared_ptr;

    pgsql::connection& conn (
      static_cast<pgsql::connection&> (c));

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

    shared_ptr<pgsql::prepared_query_impl> r (
      new (shared) pgsql::prepared_query_impl (conn));
    r->name = n;
    r->execute = &execute_query;
    r->query = q;
    r->stmt.reset (
      new (shared) select_statement (
        sts.connection (),
        n,
        text,
        true,
        true,
        r->query.parameter_types (),
        r->query.parameter_count (),
        r->query.parameters_binding (),
        imb));

    return r;
  }

  odb::details::shared_ptr<result_impl>
  access::object_traits_impl< ::Node, id_pgsql >::
  execute_query (prepared_query_impl& q)
  {
    using namespace pgsql;
    using odb::details::shared;
    using odb::details::shared_ptr;

    pgsql::prepared_query_impl& pq (
      static_cast<pgsql::prepared_query_impl&> (q));
    shared_ptr<select_statement> st (
      odb::details::inc_ref (
        static_cast<select_statement*> (pq.stmt.get ())));

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());

    // The connection used by the current transaction and the
    // one used to prepare this statement must be the same.
    //
    assert (&conn == &st->connection ());

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

    pq.query.init_parameters ();
    st->execute ();

    return shared_ptr<result_impl> (
      new (shared) pgsql::object_result_impl<object_type> (
        pq.query, st, sts, 0));
  }
}

#include <odb/post.hxx>
