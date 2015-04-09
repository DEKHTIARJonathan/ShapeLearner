// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#include <odb/pre.hxx>

#include "person-odb.hxx"

#include <cassert>
#include <cstring>  // std::memcpy


#include <odb/pgsql/traits.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/pgsql/transaction.hxx>
#include <odb/pgsql/connection.hxx>
#include <odb/pgsql/statement.hxx>
#include <odb/pgsql/statement-cache.hxx>
#include <odb/pgsql/simple-object-statements.hxx>
#include <odb/pgsql/section-statements.hxx>
#include <odb/pgsql/container-statements.hxx>
#include <odb/pgsql/exceptions.hxx>

namespace odb
{
  // person
  //

  const char access::object_traits_impl< ::person, id_pgsql >::
  persist_statement_name[] = "persist_person";

  const char access::object_traits_impl< ::person, id_pgsql >::
  find_statement_name[] = "find_person";

  const char access::object_traits_impl< ::person, id_pgsql >::
  update_statement_name[] = "update_person";

  const char access::object_traits_impl< ::person, id_pgsql >::
  erase_statement_name[] = "erase_person";

  const unsigned int access::object_traits_impl< ::person, id_pgsql >::
  persist_statement_types[] =
  {
    pgsql::text_oid,
    pgsql::text_oid,
    pgsql::text_oid
  };

  const unsigned int access::object_traits_impl< ::person, id_pgsql >::
  find_statement_types[] =
  {
    pgsql::int8_oid
  };

  const unsigned int access::object_traits_impl< ::person, id_pgsql >::
  update_statement_types[] =
  {
    pgsql::text_oid,
    pgsql::text_oid,
    pgsql::int8_oid
  };

  struct access::object_traits_impl< ::person, id_pgsql >::extra_statement_cache_type
  {
    pgsql::smart_container_statements_impl< nicknames_traits > nicknames_;

    pgsql::section_statements< ::person, extras_traits > extras_;

    extra_statement_cache_type (
      pgsql::connection& c,
      image_type& im,
      id_image_type& idim,
      pgsql::binding& id,
      pgsql::binding& idv,
      pgsql::native_binding& idn,
      const unsigned int* idt)
    : nicknames_ (c, id, idn, idt),
      extras_ (c, im, idim, id, idv, idn, idt)
    {
    }
  };

  // nicknames_
  //

  const char access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  select_name[] = "select_person_nicknames";

  const char access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  insert_name[] = "insert_person_nicknames";

  const char access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  update_name[] = "update_person_nicknames";

  const char access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  delete_name[] = "delete_person_nicknames";

  const unsigned int access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  insert_types[] =
  {
    pgsql::int8_oid,
    pgsql::int8_oid,
    pgsql::text_oid
  };

  const unsigned int access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  update_types[] =
  {
    pgsql::text_oid,
    pgsql::int8_oid,
    pgsql::int8_oid
  };

  const unsigned int access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  delete_types[] =
  {
    pgsql::int8_oid,
    pgsql::int8_oid
  };

  const char access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  select_statement[] =
  "SELECT "
  "\"section_person_nicknames\".\"index\", "
  "\"section_person_nicknames\".\"value\" "
  "FROM \"section_person_nicknames\" "
  "WHERE \"section_person_nicknames\".\"object_id\"=$1 ORDER BY \"section_person_nicknames\".\"index\"";

  const char access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  insert_statement[] =
  "INSERT INTO \"section_person_nicknames\" "
  "(\"object_id\", "
  "\"index\", "
  "\"value\") "
  "VALUES "
  "($1, $2, $3)";

  const char access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  update_statement[] =
  "UPDATE \"section_person_nicknames\" "
  "SET "
  "\"value\"=$1 "
  "WHERE \"object_id\"=$2 AND \"index\"=$3";

  const char access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  delete_statement[] =
  "DELETE FROM \"section_person_nicknames\" "
  "WHERE \"object_id\"=$1 AND \"index\">=$2";

  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  bind (pgsql::bind* b,
        const pgsql::bind* id,
        std::size_t id_size,
        cond_image_type& c)
  {
    using namespace pgsql;

    statement_kind sk (statement_select);
    ODB_POTENTIALLY_UNUSED (sk);

    std::size_t n (0);

    // object_id
    //
    if (id != 0)
      std::memcpy (&b[n], id, id_size * sizeof (id[0]));
    n += id_size;

    // index
    //
    b[n].type = pgsql::bind::bigint;
    b[n].buffer = &c.index_value;
    b[n].is_null = &c.index_null;
  }

  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  bind (pgsql::bind* b,
        const pgsql::bind* id,
        std::size_t id_size,
        data_image_type& d)
  {
    using namespace pgsql;

    statement_kind sk (statement_select);
    ODB_POTENTIALLY_UNUSED (sk);

    size_t n (0);

    // object_id
    //
    if (id != 0)
      std::memcpy (&b[n], id, id_size * sizeof (id[0]));
    n += id_size;

    // index
    //
    b[n].type = pgsql::bind::bigint;
    b[n].buffer = &d.index_value;
    b[n].is_null = &d.index_null;
    n++;

    // value
    //
    b[n].type = pgsql::bind::text;
    b[n].buffer = d.value_value.data ();
    b[n].capacity = d.value_value.capacity ();
    b[n].size = &d.value_size;
    b[n].is_null = &d.value_null;
  }

  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  bind (pgsql::bind* b,
        const pgsql::bind* id,
        std::size_t id_size,
        cond_image_type& c,
        data_image_type& d)
  {
    using namespace pgsql;

    statement_kind sk (statement_insert);
    ODB_POTENTIALLY_UNUSED (sk);

    std::size_t n (0);

    // value
    //
    b[n].type = pgsql::bind::text;
    b[n].buffer = d.value_value.data ();
    b[n].capacity = d.value_value.capacity ();
    b[n].size = &d.value_size;
    b[n].is_null = &d.value_null;
    n++;

    // object_id
    //
    if (id != 0)
      std::memcpy (&b[n], id, id_size * sizeof (id[0]));
    n += id_size;

    // index
    //
    b[n].type = pgsql::bind::bigint;
    b[n].buffer = &c.index_value;
    b[n].is_null = &c.index_null;
  }

  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  grow (data_image_type& i,
        bool* t)
  {
    bool grew (false);

    // index
    //
    t[0UL] = 0;

    // value
    //
    if (t[1UL])
    {
      i.value_value.capacity (i.value_size);
      grew = true;
    }

    if (grew)
      i.version++;
  }

  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  init (data_image_type& i,
        index_type* j,
        const value_type& v)
  {
    using namespace pgsql;

    statement_kind sk (statement_insert);
    ODB_POTENTIALLY_UNUSED (sk);

    bool grew (false);

    // index
    //
    if (j != 0)
    {
      bool is_null (false);
      pgsql::value_traits<
          index_type,
          pgsql::id_bigint >::set_image (
        i.index_value, is_null, *j);
      i.index_null = is_null;
    }

    // value
    //
    {
      bool is_null (false);
      std::size_t size (0);
      std::size_t cap (i.value_value.capacity ());
      pgsql::value_traits<
          value_type,
          pgsql::id_string >::set_image (
        i.value_value,
        size,
        is_null,
        v);
      i.value_null = is_null;
      i.value_size = size;
      grew = grew || (cap != i.value_value.capacity ());
    }

    if (grew)
      i.version++;
  }

  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  init (cond_image_type& i, index_type j)
  {
    using namespace pgsql;

    statement_kind sk (statement_select);
    ODB_POTENTIALLY_UNUSED (sk);

    {
      bool is_null (false);
      pgsql::value_traits<
          index_type,
          pgsql::id_bigint >::set_image (
        i.index_value, is_null, j);
      i.index_null = is_null;
    }
  }


  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  init (index_type& j,
        value_type& v,
        const data_image_type& i,
        database* db)
  {
    ODB_POTENTIALLY_UNUSED (db);

    // index
    //
    {
      pgsql::value_traits<
          index_type,
          pgsql::id_bigint >::set_value (
        j,
        i.index_value,
        i.index_null);
    }

    // value
    //
    {
      pgsql::value_traits<
          value_type,
          pgsql::id_string >::set_value (
        v,
        i.value_value,
        i.value_size,
        i.value_null);
    }
  }

  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  insert (index_type i, const value_type& v, void* d)
  {
    using namespace pgsql;

    statements_type& sts (*static_cast< statements_type* > (d));
    data_image_type& di (sts.data_image ());

    init (di, &i, v);

    if (sts.data_binding_test_version ())
    {
      const binding& id (sts.id_binding ());
      bind (sts.data_bind (), id.bind, id.count, di);
      sts.data_binding_update_version ();
    }

    if (!sts.insert_statement ().execute ())
      throw object_already_persistent ();
  }

  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  update (index_type i, const value_type& v, void* d)
  {
    using namespace pgsql;

    statements_type& sts (*static_cast< statements_type* > (d));
    cond_image_type& ci (sts.cond_image ());
    data_image_type& di (sts.data_image ());

    init (ci, i);
    init (di, 0, v);

    if (sts.update_binding_test_version ())
    {
      const binding& id (sts.id_binding ());
      bind (sts.update_bind (), id.bind, id.count, ci, di);
      sts.update_binding_update_version ();
    }

    if (sts.update_statement ().execute () == 0)
      throw object_not_persistent ();
  }

  bool access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  select (index_type& i, value_type& v, void* d)
  {
    using namespace pgsql;
    using pgsql::select_statement;

    statements_type& sts (*static_cast< statements_type* > (d));
    data_image_type& di (sts.data_image ());

    init (i, v, di, &sts.connection ().database ());

    select_statement& st (sts.select_statement ());
    select_statement::result r (st.fetch ());
    return r != select_statement::no_data;
  }

  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  delete_ (index_type i, void* d)
  {
    using namespace pgsql;

    statements_type& sts (*static_cast< statements_type* > (d));
    cond_image_type& ci (sts.cond_image ());

    init (ci, i);

    if (sts.cond_binding_test_version ())
    {
      const binding& id (sts.id_binding ());
      bind (sts.cond_bind (), id.bind, id.count, ci);
      sts.cond_binding_update_version ();
    }

    sts.delete_statement ().execute ();
  }

  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  persist (const container_type& c,
           statements_type& sts)
  {
    using namespace pgsql;

    functions_type& fs (sts.functions ());
    container_traits_type::persist (c, fs);
  }

  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  load (container_type& c,
        statements_type& sts)
  {
    using namespace pgsql;
    using pgsql::select_statement;

    const binding& id (sts.id_binding ());

    if (sts.data_binding_test_version ())
    {
      bind (sts.data_bind (), id.bind, id.count, sts.data_image ());
      sts.data_binding_update_version ();
    }

    select_statement& st (sts.select_statement ());
    st.execute ();
    auto_result ar (st);
    select_statement::result r (st.fetch ());
    bool more (r != select_statement::no_data);

    functions_type& fs (sts.functions ());
    container_traits_type::load (c, more, fs);
  }

  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  update (const container_type& c,
          statements_type& sts)
  {
    using namespace pgsql;

    functions_type& fs (sts.functions ());
    container_traits_type::update (c, fs);
  }

  void access::object_traits_impl< ::person, id_pgsql >::nicknames_traits::
  erase (const container_type* c, statements_type& sts)
  {
    using namespace pgsql;

    functions_type& fs (sts.functions ());
    container_traits_type::erase (c, fs);
  }

  // extras_
  //

  std::size_t access::object_traits_impl< ::person, id_pgsql >::extras_traits::
  bind (pgsql::bind* b,
        const pgsql::bind* id,
        std::size_t id_size,
        image_type& i,
        pgsql::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace pgsql;

    std::size_t n (0);

    // bio_
    //
    b[n].type = pgsql::bind::text;
    b[n].buffer = i.bio_value.data ();
    b[n].capacity = i.bio_value.capacity ();
    b[n].size = &i.bio_size;
    b[n].is_null = &i.bio_null;
    n++;

    // object_id
    //
    if (id != 0)
      std::memcpy (&b[n], id, id_size * sizeof (id[0]));
    n += id_size;

    return n;
  }

  bool access::object_traits_impl< ::person, id_pgsql >::extras_traits::
  grow (image_type& i,
        bool* t)
  {
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (t);

    bool grew (false);

    // id_
    //
    t[0UL] = 0;

    // first_
    //
    if (t[1UL])
    {
      i.first_value.capacity (i.first_size);
      grew = true;
    }

    // last_
    //
    if (t[2UL])
    {
      i.last_value.capacity (i.last_size);
      grew = true;
    }

    // bio_
    //
    if (t[3UL])
    {
      i.bio_value.capacity (i.bio_size);
      grew = true;
    }

    return grew;
  }

  void access::object_traits_impl< ::person, id_pgsql >::extras_traits::
  init (object_type& o,
        const image_type& i,
        database* db)
  {
    ODB_POTENTIALLY_UNUSED (db);

    // bio_
    //
    {
      ::std::string& v =
        o.bio_;

      pgsql::value_traits<
          ::std::string,
          pgsql::id_string >::set_value (
        v,
        i.bio_value,
        i.bio_size,
        i.bio_null);
    }
  }

  bool access::object_traits_impl< ::person, id_pgsql >::extras_traits::
  init (image_type& i,
        const object_type& o)
  {
    using namespace pgsql;

    statement_kind sk (statement_insert);
    ODB_POTENTIALLY_UNUSED (sk);

    bool grew (false);

    // bio_
    //
    {
      ::std::string const& v =
        o.bio_;

      bool is_null (false);
      std::size_t size (0);
      std::size_t cap (i.bio_value.capacity ());
      pgsql::value_traits<
          ::std::string,
          pgsql::id_string >::set_image (
        i.bio_value,
        size,
        is_null,
        v);
      i.bio_null = is_null;
      i.bio_size = size;
      grew = grew || (cap != i.bio_value.capacity ());
    }

    return grew;
  }

  const char access::object_traits_impl< ::person, id_pgsql >::extras_traits::
  select_statement[] =
  "SELECT "
  "\"section_person\".\"bio\" "
  "FROM \"section_person\" "
  "WHERE \"section_person\".\"id\"=$1";

  const char access::object_traits_impl< ::person, id_pgsql >::extras_traits::
  update_statement[] =
  "UPDATE \"section_person\" "
  "SET "
  "\"bio\"=$1 "
  "WHERE \"id\"=$2";

  void access::object_traits_impl< ::person, id_pgsql >::extras_traits::
  load (extra_statement_cache_type& esc, object_type& obj)
  {
    using namespace pgsql;
    using pgsql::select_statement;

    statements_type& sts (esc.extras_);

    image_type& im (sts.image ());
    binding& imb (sts.select_image_binding ());

    if (im.version != sts.select_image_version () ||
        imb.version == 0)
    {
      bind (imb.bind, 0, 0, im, statement_select);
      sts.select_image_version (im.version);
      imb.version++;
    }

    select_statement& st (sts.select_statement ());
    st.execute ();
    auto_result ar (st);
    select_statement::result r (st.fetch ());

    if (r == select_statement::no_data)
      throw object_not_persistent ();

    if (r == select_statement::truncated)
    {
      if (grow (im, sts.select_image_truncated ()))
        im.version++;

      if (im.version != sts.select_image_version ())
      {
        bind (imb.bind, 0, 0, im, statement_select);
        sts.select_image_version (im.version);
        imb.version++;
        st.refetch ();
      }
    }

    init (obj, im, &sts.connection ().database ());

    // nicknames_
    //
    {
      ::person::names& v =
        obj.nicknames_;

      nicknames_traits::load (
        v,
        esc.nicknames_);
    }
  }

  void access::object_traits_impl< ::person, id_pgsql >::extras_traits::
  update (extra_statement_cache_type& esc, const object_type& obj)
  {
    using namespace pgsql;
    using pgsql::update_statement;

    statements_type& sts (esc.extras_);

    image_type& im (sts.image ());
    const binding& id (sts.idv_binding ());
    binding& imb (sts.update_image_binding ());

    if (init (im, obj))
      im.version++;

    if (im.version != sts.update_image_version () ||
        id.version != sts.update_id_binding_version () ||
        imb.version == 0)
    {
      bind (imb.bind, id.bind, id.count, im, statement_update);
      sts.update_image_version (im.version);
      sts.update_id_binding_version (id.version);
      imb.version++;
    }

    update_statement& st (sts.update_statement ());
    if (st.execute () == 0)
      throw object_not_persistent ();

    // nicknames_
    //
    {
      ::person::names const& v =
        obj.nicknames_;

      nicknames_traits::update (
        v,
        esc.nicknames_);
    }
  }

  const char access::object_traits_impl< ::person, id_pgsql >::extras_traits::
  select_name[] = "select_person_extras";

  const char access::object_traits_impl< ::person, id_pgsql >::extras_traits::
  update_name[] = "update_person_extras";

  const unsigned int access::object_traits_impl< ::person, id_pgsql >::extras_traits::
  update_types[] =
  {
    pgsql::text_oid,
    pgsql::int8_oid
  };

  access::object_traits_impl< ::person, id_pgsql >::id_type
  access::object_traits_impl< ::person, id_pgsql >::
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

  bool access::object_traits_impl< ::person, id_pgsql >::
  grow (image_type& i,
        bool* t)
  {
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (t);

    bool grew (false);

    // id_
    //
    t[0UL] = 0;

    // first_
    //
    if (t[1UL])
    {
      i.first_value.capacity (i.first_size);
      grew = true;
    }

    // last_
    //
    if (t[2UL])
    {
      i.last_value.capacity (i.last_size);
      grew = true;
    }

    // bio_
    //
    if (t[3UL])
    {
      i.bio_value.capacity (i.bio_size);
      grew = true;
    }

    return grew;
  }

  void access::object_traits_impl< ::person, id_pgsql >::
  bind (pgsql::bind* b,
        image_type& i,
        pgsql::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace pgsql;

    std::size_t n (0);

    // id_
    //
    if (sk != statement_insert && sk != statement_update)
    {
      b[n].type = pgsql::bind::bigint;
      b[n].buffer = &i.id_value;
      b[n].is_null = &i.id_null;
      n++;
    }

    // first_
    //
    b[n].type = pgsql::bind::text;
    b[n].buffer = i.first_value.data ();
    b[n].capacity = i.first_value.capacity ();
    b[n].size = &i.first_size;
    b[n].is_null = &i.first_null;
    n++;

    // last_
    //
    b[n].type = pgsql::bind::text;
    b[n].buffer = i.last_value.data ();
    b[n].capacity = i.last_value.capacity ();
    b[n].size = &i.last_size;
    b[n].is_null = &i.last_null;
    n++;

    // bio_
    //
    if (sk == statement_insert)
    {
      b[n].type = pgsql::bind::text;
      b[n].buffer = i.bio_value.data ();
      b[n].capacity = i.bio_value.capacity ();
      b[n].size = &i.bio_size;
      b[n].is_null = &i.bio_null;
      n++;
    }
  }

  void access::object_traits_impl< ::person, id_pgsql >::
  bind (pgsql::bind* b, id_image_type& i)
  {
    std::size_t n (0);
    b[n].type = pgsql::bind::bigint;
    b[n].buffer = &i.id_value;
    b[n].is_null = &i.id_null;
  }

  bool access::object_traits_impl< ::person, id_pgsql >::
  init (image_type& i,
        const object_type& o,
        pgsql::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (o);
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace pgsql;

    bool grew (false);

    // first_
    //
    {
      ::std::string const& v =
        o.first_;

      bool is_null (false);
      std::size_t size (0);
      std::size_t cap (i.first_value.capacity ());
      pgsql::value_traits<
          ::std::string,
          pgsql::id_string >::set_image (
        i.first_value,
        size,
        is_null,
        v);
      i.first_null = is_null;
      i.first_size = size;
      grew = grew || (cap != i.first_value.capacity ());
    }

    // last_
    //
    {
      ::std::string const& v =
        o.last_;

      bool is_null (false);
      std::size_t size (0);
      std::size_t cap (i.last_value.capacity ());
      pgsql::value_traits<
          ::std::string,
          pgsql::id_string >::set_image (
        i.last_value,
        size,
        is_null,
        v);
      i.last_null = is_null;
      i.last_size = size;
      grew = grew || (cap != i.last_value.capacity ());
    }

    // bio_
    //
    if (sk == statement_insert)
    {
      ::std::string const& v =
        o.bio_;

      bool is_null (false);
      std::size_t size (0);
      std::size_t cap (i.bio_value.capacity ());
      pgsql::value_traits<
          ::std::string,
          pgsql::id_string >::set_image (
        i.bio_value,
        size,
        is_null,
        v);
      i.bio_null = is_null;
      i.bio_size = size;
      grew = grew || (cap != i.bio_value.capacity ());
    }

    return grew;
  }

  void access::object_traits_impl< ::person, id_pgsql >::
  init (object_type& o,
        const image_type& i,
        database* db)
  {
    ODB_POTENTIALLY_UNUSED (o);
    ODB_POTENTIALLY_UNUSED (i);
    ODB_POTENTIALLY_UNUSED (db);

    // id_
    //
    {
      long unsigned int& v =
        o.id_;

      pgsql::value_traits<
          long unsigned int,
          pgsql::id_bigint >::set_value (
        v,
        i.id_value,
        i.id_null);
    }

    // first_
    //
    {
      ::std::string& v =
        o.first_;

      pgsql::value_traits<
          ::std::string,
          pgsql::id_string >::set_value (
        v,
        i.first_value,
        i.first_size,
        i.first_null);
    }

    // last_
    //
    {
      ::std::string& v =
        o.last_;

      pgsql::value_traits<
          ::std::string,
          pgsql::id_string >::set_value (
        v,
        i.last_value,
        i.last_size,
        i.last_null);
    }
  }

  void access::object_traits_impl< ::person, id_pgsql >::
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

  const char access::object_traits_impl< ::person, id_pgsql >::persist_statement[] =
  "INSERT INTO \"section_person\" "
  "(\"id\", "
  "\"first\", "
  "\"last\", "
  "\"bio\") "
  "VALUES "
  "(DEFAULT, $1, $2, $3) "
  "RETURNING \"id\"";

  const char access::object_traits_impl< ::person, id_pgsql >::find_statement[] =
  "SELECT "
  "\"section_person\".\"id\", "
  "\"section_person\".\"first\", "
  "\"section_person\".\"last\" "
  "FROM \"section_person\" "
  "WHERE \"section_person\".\"id\"=$1";

  const char access::object_traits_impl< ::person, id_pgsql >::update_statement[] =
  "UPDATE \"section_person\" "
  "SET "
  "\"first\"=$1, "
  "\"last\"=$2 "
  "WHERE \"id\"=$3";

  const char access::object_traits_impl< ::person, id_pgsql >::erase_statement[] =
  "DELETE FROM \"section_person\" "
  "WHERE \"id\"=$1";

  void access::object_traits_impl< ::person, id_pgsql >::
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

    obj.id_ = id (sts.id_image ());

    id_image_type& i (sts.id_image ());
    init (i, obj.id_);

    binding& idb (sts.id_image_binding ());
    if (i.version != sts.id_image_version () || idb.version == 0)
    {
      bind (idb.bind, i);
      sts.id_image_version (i.version);
      idb.version++;
    }

    extra_statement_cache_type& esc (sts.extra_statement_cache ());

    // nicknames_
    //
    {
      ::person::names const& v =
        obj.nicknames_;

      nicknames_traits::persist (
        v,
        esc.nicknames_);
    }

    obj.extras_.reset (true, false);

    callback (db,
              static_cast<const object_type&> (obj),
              callback_event::post_persist);
  }

  void access::object_traits_impl< ::person, id_pgsql >::
  update (database& db, const object_type& obj)
  {
    ODB_POTENTIALLY_UNUSED (db);

    using namespace pgsql;
    using pgsql::update_statement;

    callback (db, obj, callback_event::pre_update);

    // extras_
    //
    {
      const odb::section& s = obj.extras_;

      if (s.loaded () && !s.changed ())
      {
        // nicknames_
        //
        {
          ::person::names const& v =
            obj.nicknames_;

          if (nicknames_traits::container_traits_type::changed (v))
            s.reset (true, true);
        }
      }
    }

    pgsql::transaction& tr (pgsql::transaction::current ());
    pgsql::connection& conn (tr.connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    const id_type& id (
      obj.id_);
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

    extra_statement_cache_type& esc (sts.extra_statement_cache ());

    if (obj.extras_.loaded () && obj.extras_.changed ())
    {
      extras_traits::update (esc, obj);
      obj.extras_.reset (true, false, &tr);
    }

    callback (db, obj, callback_event::post_update);
    pointer_cache_traits::update (db, obj);
  }

  void access::object_traits_impl< ::person, id_pgsql >::
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

    extra_statement_cache_type& esc (sts.extra_statement_cache ());

    // nicknames_
    //
    nicknames_traits::erase (
      0,
      esc.nicknames_);

    if (sts.erase_statement ().execute () != 1)
      throw object_not_persistent ();

    pointer_cache_traits::erase (db, id);
  }

  void access::object_traits_impl< ::person, id_pgsql >::
  erase (database& db, const object_type& obj)
  {
    ODB_POTENTIALLY_UNUSED (db);

    using namespace pgsql;

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    callback (db, obj, callback_event::pre_erase);

    const id_type& id  (
      obj.id_);

    id_image_type& i (sts.id_image ());
    init (i, id);

    binding& idb (sts.id_image_binding ());
    if (i.version != sts.id_image_version () || idb.version == 0)
    {
      bind (idb.bind, i);
      sts.id_image_version (i.version);
      idb.version++;
    }

    extra_statement_cache_type& esc (sts.extra_statement_cache ());

    // nicknames_
    //
    {
      ::person::names const& v =
        obj.nicknames_;

      nicknames_traits::erase (
        &v,
        esc.nicknames_);
    }

    if (sts.erase_statement ().execute () != 1)
      throw object_not_persistent ();

    pointer_cache_traits::erase (db, id);
    callback (db, obj, callback_event::post_erase);
  }

  access::object_traits_impl< ::person, id_pgsql >::pointer_type
  access::object_traits_impl< ::person, id_pgsql >::
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

  bool access::object_traits_impl< ::person, id_pgsql >::
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

  bool access::object_traits_impl< ::person, id_pgsql >::
  reload (database& db, object_type& obj)
  {
    using namespace pgsql;

    pgsql::connection& conn (
      pgsql::transaction::current ().connection ());
    statements_type& sts (
      conn.statement_cache ().find_object<object_type> ());

    statements_type::auto_lock l (sts);

    const id_type& id  (
      obj.id_);

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

  bool access::object_traits_impl< ::person, id_pgsql >::
  load (connection& conn, object_type& obj, section& s)
  {
    using namespace pgsql;

    pgsql::connection& c (static_cast<pgsql::connection&> (conn));
    statements_type& sts (c.statement_cache ().find_object<object_type> ());
    extra_statement_cache_type& esc (sts.extra_statement_cache ());

    id_image_type& i (sts.id_image ());
    init (i, obj.id_);

    binding& idb (sts.id_image_binding ());
    if (i.version != sts.id_image_version () || idb.version == 0)
    {
      bind (idb.bind, i);
      sts.id_image_version (i.version);
      idb.version++;
    }

    if (&s == &obj.extras_)
      extras_traits::load (esc, obj);
    else
      return false;

    return true;
  }

  bool access::object_traits_impl< ::person, id_pgsql >::
  update (connection& conn, const object_type& obj, const section& s)
  {
    using namespace pgsql;

    pgsql::connection& c (static_cast<pgsql::connection&> (conn));
    statements_type& sts (c.statement_cache ().find_object<object_type> ());
    extra_statement_cache_type& esc (sts.extra_statement_cache ());

    id_image_type& i (sts.id_image ());
    init (i, obj.id_);

    binding& idb (sts.id_image_binding ());
    if (i.version != sts.id_image_version () || idb.version == 0)
    {
      bind (idb.bind, i);
      sts.id_image_version (i.version);
      idb.version++;
    }

    if (&s == &obj.extras_)extras_traits::update (esc, obj);
    else
      return false;

    return true;
  }

  bool access::object_traits_impl< ::person, id_pgsql >::
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

  void access::object_traits_impl< ::person, id_pgsql >::
  load_ (statements_type& sts,
         object_type& obj,
         bool reload)
  {
    ODB_POTENTIALLY_UNUSED (reload);

    extra_statement_cache_type& esc (sts.extra_statement_cache ());

    if (reload)
    {
      if (obj.extras_.loaded ())
      {
        extras_traits::load (esc, obj);
        obj.extras_.reset (true, false);
      }
    }
    else
      obj.extras_.reset ();
  }
}

#include <odb/post.hxx>