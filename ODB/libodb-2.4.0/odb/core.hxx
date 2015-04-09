// file      : odb/core.hxx
// copyright : Copyright (c) 2009-2015 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_CORE_HXX
#define ODB_CORE_HXX

#ifdef _MSC_VER
	#pragma message("Compiling odb\\core.hxx - this should happen just once per project.\n")
#endif

#include <odb/pre.hxx>

#ifdef ODB_COMPILER
#  define PRAGMA_DB_IMPL(x) _Pragma (#x)
#  define PRAGMA_DB(x) PRAGMA_DB_IMPL (db x)
#else
#  define PRAGMA_DB(x)
#endif

#include <odb/forward.hxx>

#include <odb/post.hxx>

#endif // ODB_CORE_HXX
