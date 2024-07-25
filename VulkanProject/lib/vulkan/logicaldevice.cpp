#include "logicaldevice.hpp"

#include "physicaldevice.hpp"
#include "queuefamily.hpp"

#include <iostream>
namespace
{
   const std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
}
namespace my_library::vulkan
{
   void
   logicaldevice::init( const std::unique_ptr<physicaldevice>& physicaldevice,
                        const std::vector<const char*>& validationlayers,
                        const vk_dispatchloader_dynamic&      dld )
   {
      uint32_t graphicsqueue = physicaldevice->valid_queuefamily_idx( queuefamily::types::GRAPHICS_QUEUE );

      float                      queuePriority = 1.f;
      vk_queue_createinfo queue_createinfo( {}, graphicsqueue, 1, &queuePriority );
      vk_physicaldevice_features device_features {};
      vk_device_createinfo       createinfo( {}, queue_createinfo, {}, device_extensions, &device_features );
      createinfo.setPEnabledLayerNames( validationlayers );

      _device = physicaldevice->vk_obj().createDeviceUnique( createinfo, nullptr, dld );

      if ( !_device ) { throw std::runtime_error( "failed to create logical device!" ); }
   }
   void
   logicaldevice::init( const std::unique_ptr<physicaldevice>& physicaldevice, const vk_dispatchloader_dynamic& dld )
   {
      uint32_t graphicsqueue = physicaldevice->valid_queuefamily_idx( queuefamily::types::GRAPHICS_QUEUE );

      float                      queuePriority = 1.f;
      vk_queue_createinfo queue_createinfo( {}, graphicsqueue, 1, &queuePriority );
      vk_physicaldevice_features device_features {};
      vk_device_createinfo       createinfo( {}, queue_createinfo, {}, device_extensions, &device_features );

      _device = physicaldevice->vk_obj().createDeviceUnique( createinfo, nullptr, dld );

      if ( !_device ) { throw std::runtime_error( "failed to create logical device!" ); }
   }

   const vk_queue
   logicaldevice::get_queue( const uint32_t& queuefamily_idx,
                             const vk_dispatchloader_dynamic& dld )
   {
      return _device->getQueue( queuefamily_idx, 0, dld );
   }

   unq_vk_device&
   logicaldevice::vk_obj()
   {
      return _device;
   }

   logicaldevice::logicaldevice() {}
   logicaldevice::~logicaldevice() {}
}    // namespace my_library::vulkan