// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

namespace odb
{
  // Edge
  //

  inline
  access::object_traits< ::graphDBLib::Edge >::id_type
  access::object_traits< ::graphDBLib::Edge >::
  id (const object_type& o)
  {
    return o.idEdge;
  }

  inline
  void access::object_traits< ::graphDBLib::Edge >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);

    static_cast< ::graphDBLib::Edge&> (x).checkCorrectness (e, db);
  }

  inline
  void access::object_traits< ::graphDBLib::Edge >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);

    static_cast<const ::graphDBLib::Edge&> (x).checkCorrectness (e, db);
  }

  // EdgeIdViewBySource
  //

  inline
  void access::view_traits< ::graphDBLib::EdgeIdViewBySource >::
  callback (database& db, view_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  // EdgeIdViewByTarget
  //

  inline
  void access::view_traits< ::graphDBLib::EdgeIdViewByTarget >::
  callback (database& db, view_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  // EdgeIdViewByGraph
  //

  inline
  void access::view_traits< ::graphDBLib::EdgeIdViewByGraph >::
  callback (database& db, view_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }
}

namespace odb
{
  // Edge
  //

  inline
  void access::object_traits_impl< ::graphDBLib::Edge, id_pgsql >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }

  inline
  void access::object_traits_impl< ::graphDBLib::Edge, id_pgsql >::
  load_ (statements_type& sts,
         object_type& obj,
         bool)
  {
    ODB_POTENTIALLY_UNUSED (sts);
    ODB_POTENTIALLY_UNUSED (obj);
  }

  // EdgeIdViewBySource
  //

  // EdgeIdViewByTarget
  //

  // EdgeIdViewByGraph
  //
}

