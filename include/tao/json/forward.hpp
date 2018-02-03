// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_FORWARD_HPP
#define TAOCPP_JSON_INCLUDE_FORWARD_HPP

namespace tao
{
   namespace json
   {
      namespace events
      {
         class virtual_base;

      }  // namespace events

      namespace internal
      {
         struct empty_base
         {
         };

         template< template< typename... > class Traits, typename Base >
         struct single;

         template< template< typename... > class Traits, typename Base >
         struct pair;

      }  // namespace internal

      template< typename T, typename = void >
      struct traits;

      template< template< typename... > class Traits, typename Base = internal::empty_base >
      class basic_value;

      using producer_t = void (*)( events::virtual_base&, const void* );

   }  // namespace json

}  // namespace tao

#endif