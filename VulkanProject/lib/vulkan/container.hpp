#pragma once
#include "vk_object_types.hpp"
namespace my_library
{
   namespace vulkan
   {
      class container
      {
      public:
         void
         register_ctr( unq_vk_instance& ins );
         void
         register_ctr( unq_vk_instance&& ins );

         void
         register_ctr( const vk_physicaldevice& dev );

         container();
         ~container();

      public:
         const vk_dispatchloader_dynamic& dld;
         const unq_vk_instance&           instance;
         const unq_vk_surface&            surface;
         const vk_physicaldevice&         gpu;

      private:
         vk_dispatchloader_dynamic _dld;
         unq_vk_instance           _instance;
         unq_vk_surface            _surface;
         vk_physicaldevice         _gpu;
      };

      using shared_container = std::shared_ptr<container>;
   }    // namespace vulkan
}    // namespace my_library
