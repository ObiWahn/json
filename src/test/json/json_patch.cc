// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <iostream>

#include <tao/json.hh>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
        const value n = null;
        const value ea = empty_array;
        const value eo = empty_object;

        const value a = { { "a", { { "foo", 1 } } } };
        const value b = { { "b", value::array( { 1, 2, 3, 4 } ) } };

        const value q = { { "q", { { "bar", 2 } } } };

        TEST_ASSERT( patch( n, ea ) == n );
        TEST_ASSERT( patch( ea, ea ) == ea );
        TEST_ASSERT( patch( eo, ea ) == eo );
        TEST_ASSERT( patch( a, ea ) == a );
        TEST_ASSERT( patch( b, ea ) == b );
        TEST_ASSERT( patch( q, ea ) == q );

        TEST_ASSERT( patch( n, value::array( { { { "op", "test" }, { "path", "" }, { "value", null } } } ) ) == null );
        TEST_THROWS( patch( n, value::array( { { { "op", "test" }, { "path", "" }, { "value", 42 } } } ) ) );

        TEST_ASSERT( patch( a, value::array( { { { "op", "test" }, { "path", "/a" }, { "value", { { "foo", 1 } } } } } ) ) == a );
        TEST_ASSERT( patch( a, value::array( { { { "op", "test" }, { "path", "/a/foo" }, { "value", 1 } } } ) ) == a );

        TEST_THROWS( patch( a, value::array( { { { "op", "test" }, { "path", "" }, { "value", 42 } } } ) ) );
        TEST_THROWS( patch( a, value::array( { { { "op", "test" }, { "path", "/a" }, { "value", 42 } } } ) ) );

        TEST_ASSERT( patch( a, value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) == empty_object );
        TEST_ASSERT( patch( a, value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) == eo );
        TEST_ASSERT( patch( a, value::array( { { { "op", "remove" }, { "path", "/a/foo" } } } ) ) == value( { { "a", empty_object } } ) );
        TEST_THROWS( patch( a, value::array( { { { "op", "remove" }, { "path", "/q" } } } ) ) );
        TEST_THROWS( patch( a, value::array( { { { "op", "remove" }, { "path", "" } } } ) ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "remove" }, { "path", "/b" } } } ) ) == empty_object );
        TEST_ASSERT( patch( b, value::array( { { { "op", "remove" }, { "path", "/b" } } } ) ) == eo );
        TEST_ASSERT( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/0" } } } ) ) == value( { { "b", value::array( { 2, 3, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/1" } } } ) ) == value( { { "b", value::array( { 1, 3, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/2" } } } ) ) == value( { { "b", value::array( { 1, 2, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/3" } } } ) ) == value( { { "b", value::array( { 1, 2, 3 } ) } } ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/4" } } } ) ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/-" } } } ) ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "remove" }, { "path", "/b/a" } } } ) ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) );
        TEST_THROWS( patch( q, value::array( { { { "op", "remove" }, { "path", "/a" } } } ) ) );

        TEST_ASSERT( patch( a, value::array( { { { "op", "add" }, { "path", "/b" }, { "value", 42 } } } ) ) == value( { { "a", { { "foo", 1 } } }, { "b", 42 } } ) );
        TEST_ASSERT( patch( a, value::array( { { { "op", "add" }, { "path", "/a/b" }, { "value", 42 } } } ) ) == value( { { "a", { { "foo", 1 }, { "b", 42 } } } } ) );
        TEST_THROWS( patch( a, value::array( { { { "op", "add" }, { "path", "" }, { "value", 42 } } } ) ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "add" }, { "path", "/b/0" }, { "value", 42 } } } ) ) == value( { { "b", value::array( { 42, 1, 2, 3, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "add" }, { "path", "/b/1" }, { "value", 42 } } } ) ) == value( { { "b", value::array( { 1, 42, 2, 3, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "add" }, { "path", "/b/2" }, { "value", 42 } } } ) ) == value( { { "b", value::array( { 1, 2, 42, 3, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "add" }, { "path", "/b/3" }, { "value", 42 } } } ) ) == value( { { "b", value::array( { 1, 2, 3, 42, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "add" }, { "path", "/b/-" }, { "value", 42 } } } ) ) == value( { { "b", value::array( { 1, 2, 3, 4, 42 } ) } } ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "add" }, { "path", "/b/4" }, { "value", 42 } } } ) ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "add" }, { "path", "/b/a" }, { "value", 42 } } } ) ) );
        TEST_THROWS( patch( q, value::array( { { { "op", "add" }, { "path", "/a/b" }, { "value", 42 } } } ) ) );

        TEST_ASSERT( patch( a, value::array( { { { "op", "replace" }, { "path", "" }, { "value", 42 } } } ) ) == 42 );
        TEST_ASSERT( patch( a, value::array( { { { "op", "replace" }, { "path", "/a" }, { "value", 42 } } } ) ) == value( { { "a", 42 } } ) );
        TEST_ASSERT( patch( a, value::array( { { { "op", "replace" }, { "path", "/a/foo" }, { "value", 42 } } } ) ) == value( { { "a", { { "foo", 42 } } } } ) );
        TEST_THROWS( patch( a, value::array( { { { "op", "replace" }, { "path", "/b" }, { "value", 42 } } } ) ) );
        TEST_THROWS( patch( a, value::array( { { { "op", "replace" }, { "path", "/a/b" }, { "value", 42 } } } ) ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "replace" }, { "path", "/b/0" }, { "value", 42 } } } ) ) == value( { { "b", value::array( { 42, 2, 3, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "replace" }, { "path", "/b/1" }, { "value", 42 } } } ) ) == value( { { "b", value::array( { 1, 42, 3, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "replace" }, { "path", "/b/2" }, { "value", 42 } } } ) ) == value( { { "b", value::array( { 1, 2, 42, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "replace" }, { "path", "/b/3" }, { "value", 42 } } } ) ) == value( { { "b", value::array( { 1, 2, 3, 42 } ) } } ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "replace" }, { "path", "/b/4" }, { "value", 42 } } } ) ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "replace" }, { "path", "/b/-" }, { "value", 42 } } } ) ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "replace" }, { "path", "/b/a" }, { "value", 42 } } } ) ) );
        TEST_THROWS( patch( q, value::array( { { { "op", "replace" }, { "path", "/a/foo" }, { "value", 42 } } } ) ) );

        TEST_THROWS( patch( a, value::array( { { { "op", "move" }, { "from", "" }, { "path", "" } } } ) ) );
        TEST_THROWS( patch( a, value::array( { { { "op", "move" }, { "from", "/a" }, { "path", "" } } } ) ) );
        TEST_ASSERT( patch( a, value::array( { { { "op", "move" }, { "from", "/a" }, { "path", "/a" } } } ) ) == a );
        TEST_ASSERT( patch( a, value::array( { { { "op", "move" }, { "from", "/a/foo" }, { "path", "/a/foo" } } } ) ) == a );
        TEST_ASSERT( patch( a, value::array( { { { "op", "move" }, { "from", "/a" }, { "path", "/b" } } } ) ) == value( { { "b", { { "foo", 1 } } } } ) );
        TEST_ASSERT( patch( a, value::array( { { { "op", "move" }, { "from", "/a/foo" }, { "path", "/b" } } } ) ) == value( { { "a", empty_object }, { "b", 1 } } ) );
        TEST_ASSERT( patch( a, value::array( { { { "op", "move" }, { "from", "/a/foo" }, { "path", "/a" } } } ) ) == value( { { "a", 1 } } ) );
        TEST_THROWS( patch( a, value::array( { { { "op", "move" }, { "from", "/a" }, { "path", "/a/foo" } } } ) ) );
        TEST_THROWS( patch( a, value::array( { { { "op", "move" }, { "from", "/b" }, { "path", "/a" } } } ) ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/b/0" } } } ) ) == b );
        TEST_ASSERT( patch( b, value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/b/1" } } } ) ) == value( { { "b", value::array( { 2, 1, 3, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "move" }, { "from", "/b/1" }, { "path", "/b/0" } } } ) ) == value( { { "b", value::array( { 2, 1, 3, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/b/2" } } } ) ) == value( { { "b", value::array( { 2, 3, 1, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "move" }, { "from", "/b/2" }, { "path", "/b/0" } } } ) ) == value( { { "b", value::array( { 3, 1, 2, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/b/-" } } } ) ) == value( { { "b", value::array( { 2, 3, 4, 1 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "move" }, { "from", "/b/2" }, { "path", "/b/-" } } } ) ) == value( { { "b", value::array( { 1, 2, 4, 3 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "move" }, { "from", "/b/3" }, { "path", "/b/-" } } } ) ) == b );
        TEST_THROWS( patch( b, value::array( { { { "op", "move" }, { "from", "/b/4" }, { "path", "/b/-" } } } ) ) );
        TEST_THROWS( patch( b, value::array( { { { "op", "move" }, { "from", "/b/-" }, { "path", "/b/-" } } } ) ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "move" }, { "from", "/b/0" }, { "path", "/a" } } } ) ) == value( { { "a", 1 }, { "b", value::array( { 2, 3, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "move" }, { "from", "/b/2" }, { "path", "/a" } } } ) ) == value( { { "a", 3 }, { "b", value::array( { 1, 2, 4 } ) } } ) );
        TEST_ASSERT( patch( b, value::array( { { { "op", "move" }, { "from", "/b/3" }, { "path", "/a" } } } ) ) == value( { { "a", 4 }, { "b", value::array( { 1, 2, 3 } ) } } ) );

        // TODO: copy

      }

   } // json

} // tao

#include "main.hh"