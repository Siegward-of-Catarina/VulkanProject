#include "logicaldevice.hpp"

#include "queuefamily.hpp"

#include <iostream>
namespace
{
   const std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
}
namespace my_library::vulkan
{
   void
   logicaldevice::init( const vk_physicaldevice&        physicaldevice,
                        const std::vector<const char*>& validationlayers,
                        vk_dispatchloader_dynamic&      dld )
   {
      queuefamily::indices       indices       = queuefamily::find_queuefamily( physicaldevice );
      float                      queuePriority = 1.f;
      vk_queue_createinfo        queue_createinfo( {}, indices.graphicsfamily.value(), 1, &queuePriority );
      vk_physicaldevice_features device_features {};
      vk_device_createinfo       createinfo( {}, queue_createinfo, {}, device_extensions, &device_features );
      createinfo.setPEnabledLayerNames( validationlayers );
      _device = physicaldevice.createDeviceUnique( createinfo, nullptr, dld );
      if ( !_device ) { throw std::runtime_error( "failed to create logical device!" ); }
   }
   void
   logicaldevice::init( const vk_physicaldevice& physicaldevice, vk_dispatchloader_dynamic& dld )
   {
      queuefamily::indices       indices       = queuefamily::find_queuefamily( physicaldevice );
      float                      queuePriority = 1.f;
      vk_queue_createinfo        queue_createinfo( {}, indices.graphicsfamily.value(), 1, &queuePriority );
      vk_physicaldevice_features device_features {};
      vk_device_createinfo       createinfo( {}, queue_createinfo, {}, device_extensions, &device_features );
      _device = physicaldevice.createDeviceUnique( createinfo, nullptr, dld );
      if ( !_device ) { throw std::runtime_error( "failed to create logical device!" ); }
   }

   unq_vk_device&
   logicaldevice::vk_obj()
   {
      return _device;
   }

   logicaldevice::logicaldevice() {}
   logicaldevice::~logicaldevice() {}
}    // namespace my_library::vulkan