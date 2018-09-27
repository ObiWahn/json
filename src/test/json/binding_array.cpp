// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "../../test/json/test.hpp"

#include <tao/json.hpp>
#include <tao/json/binding.hpp>

namespace tao
{
   namespace json
   {
      using type_1 = std::pair< int, std::string >;  // traits< std::pair< ... > > is implemented with binding::array and binding::element.
      using type_2 = std::tuple< int, std::string, bool >;  // traits< std::tuple< ... > > is implemented with binding::array and binding::element2.

      class type_3
      {
      public:
         int get_int() const noexcept
         {
            return 7;
         }

         const std::string& get_string() const noexcept
         {
            return m_string;
         }

      private:
         std::string m_string = "dangerous";
      };

      template<>
      struct traits< type_3 >
         : binding::array< TAO_JSON_BIND_ELEMENT( &type_3::get_int ),
                           TAO_JSON_BIND_ELEMENT( &type_3::get_string ) >
      {
      };

      void unit_test()
      {
         {
            const value v = value::array( { 142, "hallo" } );
            const auto a = v.as< type_1 >();
            TEST_ASSERT( a.first == 142 );
            TEST_ASSERT( a.second == "hallo" );
            TEST_THROWS( v.as< type_2 >() );
         }
         {
            const value v = value::array( { 142, "hallo", 143 } );
            TEST_THROWS( v.as< type_1 >() );
            TEST_THROWS( v.as< type_2 >() );
         }
         {
            const value v = value::array( { 142 } );
            TEST_THROWS( v.as< type_1 >() );
            TEST_THROWS( v.as< type_2 >() );
         }
         {
            const value v = value::array( { "ciao", "hallo" } );
            TEST_THROWS( v.as< type_1 >() );
         }
         {
            const value v = value::array( { 142, 143 } );
            TEST_THROWS( v.as< type_1 >() );
         }
         {
            const value v = value::array( { 142, "hallo", true } );
            const auto a = v.as< type_2 >();
            TEST_ASSERT( std::get< 0 >( a ) == 142 );
            TEST_ASSERT( std::get< 1 >( a ) == "hallo" );
            TEST_ASSERT( std::get< 2 >( a ) == true );
            TEST_THROWS( v.as< type_1 >() );
         }
         {
            const value v = value::array( { "ciao", "hallo", true } );
            TEST_THROWS( v.as< type_1 >() );
            TEST_THROWS( v.as< type_2 >() );
         }
         {
            const value v = value::array( { 142, "hallo" } );
            type_1 a;
            TEST_ASSERT( a.first != 142 );
            TEST_ASSERT( a.second != "hallo" );
            v.to( a );
            TEST_ASSERT( a.first == 142 );
            TEST_ASSERT( a.second == "hallo" );
            type_2 b;
            TEST_THROWS( v.to( b ) );
         }
         {
            const value v = value::array( { 142, "hallo", true } );
            type_2 a;
            TEST_ASSERT( std::get< 0 >( a ) != 142 );
            TEST_ASSERT( std::get< 1 >( a ) != "hallo" );
            TEST_ASSERT( std::get< 2 >( a ) != true );
            v.to( a );
            TEST_ASSERT( std::get< 0 >( a ) == 142 );
            TEST_ASSERT( std::get< 1 >( a ) == "hallo" );
            TEST_ASSERT( std::get< 2 >( a ) == true );
            type_1 b;
            TEST_THROWS( v.to( b ) );
         }
         {
            const value v = value::array( { 142, 143 } );
            type_1 a;
            TEST_THROWS( v.to( a ) );
            type_2 b;
            TEST_THROWS( v.to( b ) );
         }
         {
            const value v = value::array( { 142, "hallo", 144 } );
            type_2 a;
            TEST_THROWS( v.to( a ) );
            type_1 b;
            TEST_THROWS( v.to( b ) );
         }
         {
            const type_1 a = { 19, "ciao" };
            const value v = a;
            TEST_ASSERT( v.is_array() );
            TEST_ASSERT( v.unsafe_get_array().size() == 2 );
            TEST_ASSERT( v.unsafe_get_array()[ 0 ].is_integer() );
            TEST_ASSERT( v.unsafe_get_array()[ 0 ].as< int >() == 19 );
            TEST_ASSERT( v.unsafe_get_array()[ 1 ].is_string_type() );
            TEST_ASSERT( v.unsafe_get_array()[ 1 ].as< std::string >() == "ciao" );

            TEST_ASSERT( a == v );
            TEST_ASSERT( v == a );
            TEST_ASSERT( !( a != v ) );
            TEST_ASSERT( !( v != a ) );
            const type_1 b = { 20, "ciao" };
            const type_1 c = { 19, "hola" };
            TEST_ASSERT( b != c );
            TEST_ASSERT( c != b );
            TEST_ASSERT( a != b );
            TEST_ASSERT( b != a );
            TEST_ASSERT( a != c );
            TEST_ASSERT( c != a );
            TEST_ASSERT( !( b == c ) );
            TEST_ASSERT( !( c == b ) );
            TEST_ASSERT( !( a == b ) );
            TEST_ASSERT( !( b == a ) );
            TEST_ASSERT( !( a == c ) );
            TEST_ASSERT( !( c == a ) );

            const value w = produce::to_value( a );
            TEST_ASSERT( w == v );
            TEST_ASSERT( w == a );
            TEST_ASSERT( a == v );
         }
         {
            const type_2 a = { 20, "hola", false };
            const value v = a;
            TEST_ASSERT( v.is_array() );
            TEST_ASSERT( v.unsafe_get_array().size() == 3 );
            TEST_ASSERT( v.unsafe_get_array()[ 0 ].is_integer() );
            TEST_ASSERT( v.unsafe_get_array()[ 0 ].as< int >() == 20 );
            TEST_ASSERT( v.unsafe_get_array()[ 1 ].is_string_type() );
            TEST_ASSERT( v.unsafe_get_array()[ 1 ].as< std::string >() == "hola" );
            TEST_ASSERT( v.unsafe_get_array()[ 2 ].is_boolean() );
            TEST_ASSERT( v.unsafe_get_array()[ 2 ].as< bool >() == false );

            TEST_ASSERT( a == v );
            TEST_ASSERT( v == a );
            TEST_ASSERT( !( a != v ) );
            TEST_ASSERT( !( v != a ) );
            const type_2 b = { 19, "hola", false };
            const type_2 c = { 20, "hola", true };
            TEST_ASSERT( b != c );
            TEST_ASSERT( c != b );
            TEST_ASSERT( a != b );
            TEST_ASSERT( b != a );
            TEST_ASSERT( a != c );
            TEST_ASSERT( c != a );
            TEST_ASSERT( !( b == c ) );
            TEST_ASSERT( !( c == b ) );
            TEST_ASSERT( !( a == b ) );
            TEST_ASSERT( !( b == a ) );
            TEST_ASSERT( !( a == c ) );
            TEST_ASSERT( !( c == a ) );

            const value w = produce::to_value( a );
            TEST_ASSERT( w == v );
            TEST_ASSERT( w == a );
            TEST_ASSERT( a == v );
         }
         {
            parts_parser p( "[1,\"foo\"]", __FUNCTION__ );
            const auto a = consume< type_1 >( p );
            TEST_ASSERT( a.first == 1 );
            TEST_ASSERT( a.second == "foo" );
         }
         {
            parts_parser p( "1", __FUNCTION__ );
            TEST_THROWS( consume< type_1 >( p ) );
         }
         {
            parts_parser p( "[1]", __FUNCTION__ );
            TEST_THROWS( consume< type_1 >( p ) );
         }
         {
            parts_parser p( "[1,\"foo\",2]", __FUNCTION__ );
            TEST_THROWS( consume< type_1 >( p ) );
         }
         {
            parts_parser p( "\"foo\"", __FUNCTION__ );
            TEST_THROWS( consume< type_1 >( p ) );
         }
         {
            parts_parser p( "{}", __FUNCTION__ );
            TEST_THROWS( consume< type_1 >( p ) );
         }
      }

   }  // namespace json

}  // namespace tao

#include "../../test/json/main.hpp"
