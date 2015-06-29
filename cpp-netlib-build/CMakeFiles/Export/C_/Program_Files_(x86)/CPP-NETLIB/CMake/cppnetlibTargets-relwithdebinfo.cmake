#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "cppnetlib-uri" for configuration "RelWithDebInfo"
set_property(TARGET cppnetlib-uri APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(cppnetlib-uri PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "C:/Program Files (x86)/CPP-NETLIB/lib/cppnetlib-uri.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS cppnetlib-uri )
list(APPEND _IMPORT_CHECK_FILES_FOR_cppnetlib-uri "C:/Program Files (x86)/CPP-NETLIB/lib/cppnetlib-uri.lib" )

# Import target "cppnetlib-server-parsers" for configuration "RelWithDebInfo"
set_property(TARGET cppnetlib-server-parsers APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(cppnetlib-server-parsers PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "C:/Program Files (x86)/CPP-NETLIB/lib/cppnetlib-server-parsers.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS cppnetlib-server-parsers )
list(APPEND _IMPORT_CHECK_FILES_FOR_cppnetlib-server-parsers "C:/Program Files (x86)/CPP-NETLIB/lib/cppnetlib-server-parsers.lib" )

# Import target "cppnetlib-client-connections" for configuration "RelWithDebInfo"
set_property(TARGET cppnetlib-client-connections APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(cppnetlib-client-connections PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELWITHDEBINFO "C:/OpenSSL-Win32/lib/VC/ssleay32MD.lib;C:/OpenSSL-Win32/lib/VC/libeay32MD.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "C:/Program Files (x86)/CPP-NETLIB/lib/cppnetlib-client-connections.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS cppnetlib-client-connections )
list(APPEND _IMPORT_CHECK_FILES_FOR_cppnetlib-client-connections "C:/Program Files (x86)/CPP-NETLIB/lib/cppnetlib-client-connections.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
