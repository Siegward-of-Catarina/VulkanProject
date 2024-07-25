#pragma once
#include "vk_object_types.hpp"
namespace my_library
{
   namespace vulkan
   {
      class physicaldevice;
   }    // namespace vulkan
}    // namespace my_library

namespace my_library::vulkan
{
   class logicaldevice
   {
   private:

   public:
      void
      init( const std::unique_ptr<physicaldevice>& physicaldevice,
            const std::vector<const char*>& validationlayers,
            const vk_dispatchloader_dynamic&      dld );
      void
      init( const std::unique_ptr<physicaldevice>& physicaldevice, const vk_dispatchloader_dynamic& dld );

      const vk_queue
      get_queue( const uint32_t& queuefamily_idx, const vk_dispatchloader_dynamic& dld );

      unq_vk_device&
      vk_obj();

      logicaldevice();
      ~logicaldevice();

   private:
      unq_vk_device _device;
   };
}    // namespace my_library::vulkan