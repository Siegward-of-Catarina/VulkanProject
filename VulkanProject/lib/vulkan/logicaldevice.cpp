#include "logicaldevice.hpp"

#include "physicaldevice.hpp"
#include "queuefamily.hpp"
#include "utilities.hpp"
#include "validationlayer.hpp"

#include <iostream>
namespace
{
   const std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
}
namespace my_library::vulkan
{
   void
   logicaldevice::init( const std::unique_ptr<physicaldevice>& physicaldevice, const vk_dispatchloader_dynamic& dld )
   {
      const uint32_t graphicsqueue = physicaldevice->valid_queuefamily_idx( queuefamily::types::GRAPHICS_QUEUE );

      const float                      queuePriority = 1.f;
      const vk_queue_createinfo        queue_createinfo( {}, graphicsqueue, 1, &queuePriority );
      const vk_physicaldevice_features device_features {};
      vk_device_createinfo             createinfo( {}, queue_createinfo, {}, device_extensions, &device_features );

      const auto validationlayers = get_validationlayers();
      if ( !validationlayers.empty() ) createinfo.setPEnabledLayerNames( validationlayers );

      _device = physicaldevice->vk_obj().createDeviceUnique( createinfo, nullptr, dld );

      if ( !_device ) { throw std::runtime_error( "failed to create logical device!" ); }
      utl::log( "create logical device succeeded." );
   }

   const vk_queue
   logicaldevice::get_queue( const uint32_t& queuefamily_idx, const vk_dispatchloader_dynamic& dld )
   {
      return _device->getQueue( queuefamily_idx, 0, dld );
   }

   const unq_vk_device&
   logicaldevice::vk_obj()
   {
      return _device;
   }

   logicaldevice::logicaldevice() {}
   logicaldevice::~logicaldevice() {}
}    // namespace my_library::vulkan