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

      class DebugUtilsMessenger
      {
      public:
         const vkm::UniqueDebugUtilsMessengerEXT& vkobj;
         void
         create( const UniqueInstance&                instance,
                 const vkm::DebugUtilsMessengerCreateInfoEXT& create_info,
                 const DispatchLoaderDynamic&         dld );
         DebugUtilsMessenger();
         ~DebugUtilsMessenger();

      private:
         vkm::UniqueDebugUtilsMessengerEXT _messenger;
      };
   }    // namespace vkm
}    // namespace my_library
