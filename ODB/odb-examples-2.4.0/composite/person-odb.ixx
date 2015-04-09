// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

namespace odb
{
  // person
  //

  inline
  access::object_traits< ::person >::id_type
  access::object_traits< ::person >::
  id (const object_type& o)
  {
    return o.email_;
  }

  inline
  void access::object_traits< ::person >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  inline
  void access::object_traits< ::person >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }
}

namespace odb
{
  // basic_name
  //

  inline
  bool access::composite_value_traits< ::basic_name, id_pgsql >::
  get_null (const image_type& i)
  {
    bool r (true);
    r = r && i.first_null;
    r = r && i.last_null;
    return r;
  }

  inline
  void access::composite_value_traits< ::basic_name, id_pgsql >::
  set_null (image_type& i,
            pgsql::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace pgsql;

    i.first_null = true;
    i.last_null = true;
  }

  // name_extras
  //

  // name
  //

  // phone_numbers
  //

  inline
  bool access::composite_value_traits< ::phone_numbers, id_pgsql >::
  get_null (const image_type& i)
  {
    bool r (true);
    r = r && i.first_null;
    r = r && i.second_null;
    return r;
  }

  inline
  void access::composite_value_traits< ::phone_numbers, id_pgsql >::
  set_null (image_type& i,
            pgsql::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace pgsql;

    i.first_null = true;
    i.second_null = true;
  }

  // email_address
  //

  inline
  bool access::composite_value_traits< ::email_address, id_pgsql >::
  get_null (const image_type& i)
  {
    bool r (true);
    r = r && i.recipient_null;
    r = r && i.domain_null;
    return r;
  }

  inline
  void access::composite_value_traits< ::email_address, id_pgsql >::
  set_null (image_type& i,
            pgsql::statement_kind sk)
  {
    ODB_POTENTIALLY_UNUSED (sk);

    using namespace pgsql;

    i.recipient_null = true;
    i.domain_null = true;
  }

  // person
  //

  inline
  void access::object_traits_impl< ::person, id_pgsql >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }
}
