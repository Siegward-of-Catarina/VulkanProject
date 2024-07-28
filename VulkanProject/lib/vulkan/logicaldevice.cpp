#include "logicaldevice.hpp"

#include "physicaldevice.hpp"
#include "queuefamily.hpp"
#include "extension.hpp"
#include "utilities.hpp"
#include "validationlayer.hpp"

#include <set>
namespace my_library::vulkan
{
   void
   logicaldevice::init( const std::unique_ptr<physicaldevice>& physicaldevice, const vk_dispatchloader_dynamic& dld )
   {
      const uint32_t graphicsqueue { physicaldevice->valid_queuefamily_idx( queuefamily::types::GRAPHICS_QUEUE ) };
      const uint32_t presentqueue { physicaldevice->valid_queuefamily_idx( queuefamily::types::PRESENT_QUEUE ) };

      std::vector<vk_queue_createinfo> queue_createinfos;
      std::vector<uint32_t>            unq_queuefamilies { graphicsqueue };
      //graphicsqueue と　presentqueue が同じ場合 createDeviceUnique で例外が発生する。queuefamilyindex は一意である必要があるようだ.
      if ( graphicsqueue != presentqueue ) unq_queuefamilies.emplace_back( presentqueue );

      const float queue_priority { 1.f };

      for ( uint32_t queuefamily : unq_queuefamilies )
      {
         const vk_queue_createinfo queue_createinfo { {}, queuefamily, 1, &queue_priority };
         queue_createinfos.emplace_back( queue_createinfo );
      }

      const vk_physicaldevice_features device_features {};

      const auto& device_extensions { ext::get_device_exts() };

      vk_device_createinfo createinfo { {}, queue_createinfos, {}, device_extensions, &device_features };

      const auto validationlayers { layer::get_validationlayers() };
      if ( !validationlayers.empty() ) createinfo.setPEnabledLayerNames( validationlayers );

      _device = physicaldevice->vk_obj().createDeviceUnique( createinfo, nullptr, dld );

      if ( !_device ) { throw std::runtime_error( "failed to create logical device!" ); }

      utl::log( "create logical device & valid queue succeeded." );
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