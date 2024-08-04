#pragma once
#include "core_commonheaders.hpp"
namespace my_library
{
   namespace vulkan
   {
      namespace queuefamily
      {
         enum class types;
      }
   }    // namespace vulkan
}    // namespace my_library

namespace my_library::vulkan
{
   class logicaldevice : public core_object
   {
   public:
      void
      init();

      const vk_queue
      get_queue( const queuefamily ::types& type );

      logicaldevice( const shared_container& c );
      ~logicaldevice();

   private:
      unq_vk_device _device;
   };
}    // namespace my_library::vulkan