#pragma once

#include "../vkObjectTypes.hpp"

namespace my_library
{
   namespace vkm
   {
      class DebugMessenger
      {
      public:
         void
         create( const instance::UniqueHandle& instance, const DispatchLoaderDynamic& dld );
         void
         create( const instance::UniqueHandle&          instance,
                 const debug_utl_msgr::SeverityFlagsEXT severityflag,
                 const debug_utl_msgr::TypeFlagsEXT     messageflag,
                 const DispatchLoaderDynamic&           dld );
         DebugMessenger();
         ~DebugMessenger();

      private:
         debug_utl_msgr::UniqueHandle _messenger;
      };
   }    // namespace vkm
}    // namespace my_library
