#pragma once

#include "vkObjectTypes.hpp"

namespace my_library
{
   namespace vkm
   {
      class DebugUtilsMessengerCreateInfo
      {
      public:
         DebugUtilsMessengerCreateInfo();
         DebugUtilsMessengerCreateInfo( const DebugUtilsMesseagSeverityFlagsEXT severityflag,
                                        const DebugUtilsMessageTypeFlagsEXT     messageflag );
         ~DebugUtilsMessengerCreateInfo();
         void
         setMessageSeverity( const DebugUtilsMesseagSeverityFlagsEXT severityflag );
         void
         setMessageType( const DebugUtilsMessageTypeFlagsEXT messageflag );

         const vkm::DebugUtilsMessengerCreateInfoEXT& vkobj;
      private:
         vkm::DebugUtilsMessengerCreateInfoEXT _create_info;
      };
   }    // namespace vkm
}    // namespace my_library
