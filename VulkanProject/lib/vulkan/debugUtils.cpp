#include "debugUtils.hpp"

#include "../utilities.hpp"

namespace
{
   VKAPI_ATTR VkBool32 VKAPI_CALL
   debag_callBack( VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                   VkDebugUtilsMessageTypeFlagsEXT             message_type,
                   const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
                   void*                                       user_data_ptr )
   {
      std::cerr << "validation layer:" << callback_data_ptr->pMessage << std::endl;
      return VK_FALSE;
   }
}    // namespace
namespace my_library
{
   namespace vkm
   {
      DebugUtilsMessengerCreateInfo::DebugUtilsMessengerCreateInfo() : vkobj { _create_info }, _create_info {}
      {
         _create_info.setPfnUserCallback( debag_callBack );
      }
      DebugUtilsMessengerCreateInfo::DebugUtilsMessengerCreateInfo(
        const DebugUtilsMesseagSeverityFlagsEXT severityflag, const DebugUtilsMessageTypeFlagsEXT messageflag )
        : vkobj { _create_info }, _create_info {}
      {
         _create_info.setMessageSeverity( severityflag );
         _create_info.setMessageType( messageflag );
         _create_info.setPfnUserCallback( debag_callBack );
      }
      DebugUtilsMessengerCreateInfo::~DebugUtilsMessengerCreateInfo() {}

      void
      DebugUtilsMessengerCreateInfo::setMessageSeverity( const DebugUtilsMesseagSeverityFlagsEXT severityflag )
      {
         _create_info.setMessageSeverity( severityflag );
      }

      void
      DebugUtilsMessengerCreateInfo::setMessageType( const DebugUtilsMessageTypeFlagsEXT messageflag )
      {
         _create_info.setMessageType( messageflag );
      }

      void
      DebugUtilsMessenger::create( const UniqueInstance&                instance,
                                   const vkm::DebugUtilsMessengerCreateInfoEXT& create_info,
                                   const DispatchLoaderDynamic&         dld )
      {
         assert( instance );
         _messenger = instance->createDebugUtilsMessengerEXTUnique( create_info, nullptr, dld );
         if ( !_messenger ) { utl::runtimeError( "failed create debug_messenger." ); }
         utl::log( "create DebugUtilsMessenger succeeded." );
      }

      DebugUtilsMessenger::DebugUtilsMessenger() : vkobj { _messenger } {}
      DebugUtilsMessenger::~DebugUtilsMessenger() {}
   }    // namespace vkm
}    // namespace my_library
