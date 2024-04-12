#pragma once
#include <iostream>
#include <memory>

namespace my_library::helper
{
   template<class T>
   struct Impl : T
   {
      template<class... ARGS>
      explicit Impl( ARGS&&... args ) : T( std::forward<ARGS>( args )... ) {};
   };

}