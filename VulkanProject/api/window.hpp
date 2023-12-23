#pragma once
#include "../lib/interface.hpp"

#include <iostream>

namespace api
{
   namespace window
   {
      struct glwindow : public lib::base::interface
      {
         virtual void
         init() = 0;
         virtual int
         close() = 0;
         virtual void
         pool_event() = 0;

      protected:
         virtual ~glwindow() = default;
      };

      glwindow*
      create( const std::uint32_t width, const std::uint32_t height, const char* title );
   }    // namespace window
}    // namespace api