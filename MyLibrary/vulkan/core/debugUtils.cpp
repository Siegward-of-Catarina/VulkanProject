#include "debugUtils.hpp"

#include "../../pch.hpp"
#include "../../utilities.hpp"

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
      void
      DebugMessenger::create( const instance::UniqueHandle& instance, const DispatchLoaderDynamic& dld )
      {
         using Severity = debug_utl_msgr::SeverityFlagBitsEXT;
         using MsgType  = debug_utl_msgr::TypeFlagBitsEXT;
         debug_utl_msgr::CreateInfoEXT info( {},
                                             Severity::eWarning | Severity::eError | Severity::eVerbose,
                                             MsgType::eGeneral | MsgType::ePerformance | MsgType::eValidation,
                                             debag_callBack );
         _messenger = instance->createDebugUtilsMessengerEXTUnique( info, nullptr, dld );
         utl::runtimeError( _messenger->operator bool(), "failed to create debug util messenger!" );
         utl::log( "create debug util messenger succeeded." );
      }
      void
      DebugMessenger::create( const instance::UniqueHandle&          instance,
                              const debug_utl_msgr::SeverityFlagsEXT severityflag,
                              const debug_utl_msgr::TypeFlagsEXT     messageflag,
                              const DispatchLoaderDynamic&           dld )
      {
         debug_utl_msgr::CreateInfoEXT info( {}, severityflag, messageflag, debag_callBack );
         _messenger = instance->createDebugUtilsMessengerEXTUnique( info, nullptr, dld );
         utl::runtimeError( _messenger->operator bool(), "failed to create debug util messenger!" );
         utl::log( "create debug util messenger succeeded." );
      }
      DebugMessenger::DebugMessenger() : _messenger {} {}
      DebugMessenger::~DebugMessenger() {}
   }    // namespace vkm
}    // namespace my_library
