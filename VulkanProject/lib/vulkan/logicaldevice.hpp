#pragma once
#include "vk_object_types.hpp"

namespace my_library::vulkan
{
   class logicaldevice
   {
   private:

   public:
      void
      init( const vk_physicaldevice&        physicaldevice,
            const std::vector<const char*>& validationlayers,
            vk_dispatchloader_dynamic&      dld );
      void
      init( const vk_physicaldevice& physicaldevice, vk_dispatchloader_dynamic& dld );

      unq_vk_device&
      vk_obj();
      logicaldevice();
      ~logicaldevice();

   private:
      unq_vk_device _device;
   };
}    // namespace my_library::vulkan