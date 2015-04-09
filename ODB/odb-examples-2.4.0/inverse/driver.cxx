// file      : inverse/driver.cxx
// copyright : not copyrighted - public domain

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/session.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "employee.hxx"
#include "employee-odb.hxx"

using namespace std;
using namespace odb::core;

void
print (const employee& e)
{
  cout << e.first () << " " << e.last () << endl
       << "  employer: " << e.employer ().load ()->name () << endl
       << "  position: " << e.position ().load ()->title () << endl;

  const projects& ps (e.projects ());

  for (projects::const_iterator i (ps.begin ()); i != ps.end (); ++i)
  {
    const lazy_shared_ptr<project>& p (*i);
    p.load ();

    cout << "  project: " << p->name () << endl;
  }

  cout << endl;
}

int
main (int argc, char* argv[])
{
  using tr1::shared_ptr;

  try
  {
    auto_ptr<database> db (create_database (argc, argv));

    // Create a few persistent objects.
    //
    {
      // Simple Tech Ltd.
      //
      {
        shared_ptr<employer> er (new employer ("Simple Tech Ltd"));

        shared_ptr<position> he (new position ("Hardware Engineer"));
        shared_ptr<position> se (new position ("Software Engineer"));

        shared_ptr<project> sh (new project ("Simple Hardware"));
        shared_ptr<project> ss (new project ("Simple Software"));

        shared_ptr<employee> john (new employee ("John", "Doe", er, he));
        shared_ptr<employee> jane (new employee ("Jane", "Doe", er, se));

        // Set the inverse side of the employee-employer relationship.
        //
        er->employees ().push_back (john);
        er->employees ().push_back (jane);

        // Set the inverse side of the employee-position relationship.
        //
        he->employee (john);
        se->employee (jane);

        // Set the employee-project relationship (both directions).
        //
        john->projects ().push_back (sh);
        john->projects ().push_back (ss);
        jane->projects ().push_back (ss);

        sh->employees ().push_back (john);
        ss->employees ().push_back (john);
        ss->employees ().push_back (jane);

        transaction t (db->begin ());

        db->persist (er);

        db->persist (he);
        db->persist (se);

        db->persist (sh);
        db->persist (ss);

        db->persist (john);
        db->persist (jane);

        t.commit ();
      }

      // Complex Systems Inc.
      //
      {
        shared_ptr<employer> er (new employer ("Complex Systems Inc"));

        shared_ptr<position> he (new position ("Hardware Engineer"));
        shared_ptr<position> se (new position ("Software Engineer"));

        shared_ptr<project> ch (new project ("Complex Hardware"));
        shared_ptr<project> cs (new project ("Complex Software"));

        shared_ptr<employee> john (new employee ("John", "Smith", er, se));
        shared_ptr<employee> jane (new employee ("Jane", "Smith", er, he));

        // Set the inverse side of the employee-employer relationship.
        //
        er->employees ().push_back (john);
        er->employees ().push_back (jane);

        // Set the inverse side of the employee-position relationship.
        //
        he->employee (john);
        se->employee (jane);

        // Set the employee-project relationship (both directions).
        //
        john->projects ().push_back (cs);
        jane->projects ().push_back (ch);
        jane->projects ().push_back (cs);

        ch->employees ().push_back (jane);
        cs->employees ().push_back (john);
        cs->employees ().push_back (jane);

        transaction t (db->begin ());

        db->persist (er);

        db->persist (he);
        db->persist (se);

        db->persist (ch);
        db->persist (cs);

        db->persist (john);
        db->persist (jane);

        t.commit ();
      }
    }

    // Load Simple Tech Ltd and print its employees. We use a session in this
    // and subsequent transactions to make sure that a single instance of any
    // particular object (e.g., employer) is shared among all objects (e.g.,
    // employee) that relate to it.
    //
    {
      session s;
      transaction t (db->begin ());

      shared_ptr<employer> stl (db->load<employer> ("Simple Tech Ltd"));

      employees& es (stl->employees ());

      for (employees::iterator i (es.begin ()); i != es.end (); ++i)
      {
        lazy_weak_ptr<employee>& lwp (*i);
        shared_ptr<employee> p (lwp.load ()); // Load and lock.
        print (*p);
      }

      t.commit ();
    }

    // Find all Software Engineers.
    //
    {
      typedef odb::query<position> query;
      typedef odb::result<position> result;

      session s;
      transaction t (db->begin ());

      result r (db->query<position> (query::title == "Software Engineer"));

      for (result::iterator i (r.begin ()); i != r.end (); ++i)
      {
        const lazy_weak_ptr<employee>& lwp (i->employee ());
        shared_ptr<employee> p (lwp.load ()); // Load and lock.

        // Employee can be NULL if the position is vacant.
        //
        if (p)
          print (*p);
      }

      t.commit ();
    }

    // John Doe has moved to Complex Systems Inc and is now working as
    // a Software Engineer on Complex Software.
    //
    {
      typedef odb::query<employee> query;

      session s;
      transaction t (db->begin ());

      // Create "unloaded" pointers to the employer and project objects.
      //
      lazy_shared_ptr<employer> csi (*db, std::string ("Complex Systems Inc"));
      lazy_shared_ptr<project> cs (*db, std::string ("Complex Software"));

      // Create a new Software Engineer position.
      //
      shared_ptr<position> se (new position ("Software Engineer"));

      shared_ptr<employee> john (
        db->query_one<employee> (query::first == "John" &&
                                 query::last == "Doe"));

      john->employer (csi);
      john->position (se);
      john->projects ().clear ();
      john->projects ().push_back (cs);

      db->persist (se);
      db->update (john);

      t.commit ();
    }

    // Print Complex Systems Inc's employees. This time, instead of loading
    // the employer object, we use a query which shows how we can use members
    // of the pointed-to objects in the queries.
    //
    {
      typedef odb::query<employee> query;
      typedef odb::result<employee> result;

      session s;
      transaction t (db->begin ());

      result r (db->query<employee> (
                  query::employer->name == "Complex Systems Inc"));

      for (result::iterator i (r.begin ()); i != r.end (); ++i)
        print (*i);

      t.commit ();
    }
  }
  catch (const odb::exception& e)
  {
    cerr << e.what () << endl;
    return 1;
  }
}
