#pragma once
#include <iostream>
#include <memory>
#define CREATE_HELPER                                                                                                  \
   template<class T>                                                                                                   \
   struct Impl : T                                                                                                     \
   {                                                                                                                   \
      template<class... ARGS>                                                                                          \
      explicit Impl( ARGS&&... args ) : T( std::forward<ARGS>( args )... ) {};                                         \
   }