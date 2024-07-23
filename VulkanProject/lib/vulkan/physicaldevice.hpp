#pragma once
#include "vk_object_types.hpp"
namespace my_library::vulkan
{
   class physicaldevice
   {
   private:
      bool
      is_device_suitable( const vk_physicaldevice& device );

   public:
      void
      pick_physical_device( unq_vk_instance& instance, vk::DispatchLoaderDynamic& dld );
      
      vk_physicaldevice&
      vk_obj();

      physicaldevice();
      ~physicaldevice();

   private:
      // これはvk_instanceが内部で破棄してくれるのでunique_ptrは使わない
      vk_physicaldevice _physical_device;
   };
}    // namespace my_library::vulkan
