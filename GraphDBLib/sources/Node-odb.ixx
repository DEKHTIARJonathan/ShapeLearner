// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

namespace odb
{
  // Node
  //

  inline
  access::object_traits< ::graphDBLib::Node >::id_type
  access::object_traits< ::graphDBLib::Node >::
  id (const object_type& o)
  {
    return o.idNode;
  }

  inline
  void access::object_traits< ::graphDBLib::Node >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  inline
  void access::object_traits< ::graphDBLib::Node >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  // NodeIdViewByGraph
  //

  inline
  void access::view_traits< ::graphDBLib::NodeIdViewByGraph >::
  callback (database& db, view_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }
}

namespace odb
{
  // Node
  //

  inline
  void access::object_traits_impl< ::graphDBLib::Node, id_pgsql >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }

  inline
  void access::object_traits_impl< ::graphDBLib::Node, id_pgsql >::
  load_ (statements_type& sts,
         object_type& obj,
         bool)
  {
    ODB_POTENTIALLY_UNUSED (sts);
    ODB_POTENTIALLY_UNUSED (obj);
  }

  // NodeIdViewByGraph
  //
}

