#include "physicaldevice.hpp"

namespace my_library::vulkan
{
   bool
   physicaldevice::is_device_suitable( const vk_physicaldevice&         device,
                                       const unq_vk_surface&            surface,
                                       const vk_dispatchloader_dynamic& dld )
   {
      _indices = queuefamily::find_queuefamily( device, surface, dld );
      return _indices.is_complete();
   }
   void
   physicaldevice::pick_physical_device( const unq_vk_instance&           instance,
                                         const unq_vk_surface&            surface,
                                         const vk_dispatchloader_dynamic& dld )
   {
      std::vector<vk_physicaldevice> devices = instance->enumeratePhysicalDevices( dld );

      // GPUのリストから適切なデバイスを取得
      for ( const auto& device : devices )
      {
         if ( is_device_suitable( device, surface, dld ) )
         {
            _physical_device = device;
            break;
         }
      }

      if ( !_physical_device ) { throw std::runtime_error( "failed to find a suitable GPU!" ); }
   }
   vk_physicaldevice&
   physicaldevice::vk_obj()
   {
      return _physical_device;
   }
   const uint32_t&
   physicaldevice::valid_queuefamily_idx( const queuefamily::types& type )
   {
      assert( type < queuefamily::types::COUNT );
      return _indices.select( type ).value();
   }
   physicaldevice::physicaldevice() {}
   physicaldevice::~physicaldevice() {}
}    // namespace my_library::vulkan
