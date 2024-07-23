#pragma once
#include "vk_object_types.hpp"
#include "queuefamily.hpp"
namespace my_library::vulkan
{
   class physicaldevice
   {
   private:
      bool
      is_device_suitable( const vk_physicaldevice&         device,
                          const unq_vk_surface&            surface,
                          const vk_dispatchloader_dynamic& dld );

   public:
      void
      pick_physical_device( const unq_vk_instance&           instance,
                            const unq_vk_surface&            surface,
                            const vk_dispatchloader_dynamic& dld );

      vk_physicaldevice&
      vk_obj();

      const uint32_t&
      valid_queuefamily_idx( const queuefamily::types& type );

      physicaldevice();
      ~physicaldevice();

   private:
      // これはvk_instanceが内部で破棄してくれるのでunique_ptrは使わない
      vk_physicaldevice    _physical_device;
      queuefamily::indices _indices;
   };
}    // namespace my_library::vulkan
