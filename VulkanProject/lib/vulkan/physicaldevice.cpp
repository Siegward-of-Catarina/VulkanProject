#include "physicaldevice.hpp"

#include "queuefamily.hpp"
namespace my_library::vulkan
{
   bool
   physicaldevice::is_device_suitable( const vk_physicaldevice& device )
   {
      queuefamily::indices indices { queuefamily::find_queuefamily( device ) };
      return indices.is_complete();
   }
   void
   physicaldevice::pick_physical_device( unq_vk_instance& instance, vk::DispatchLoaderDynamic& dld )
   {

      std::vector<vk::PhysicalDevice> devices = instance->enumeratePhysicalDevices( dld );

      // GPUのリストから適切なデバイスを取得
      for ( const auto& device : devices )
      {
         if ( is_device_suitable( device ) )
         {
            _physical_device = device;
            break;
         }
      }
      // rbegin 末尾　-> 先頭　の逆イテレータ
      if ( !_physical_device ) { throw std::runtime_error( "failed to find a suitable GPU!" ); }
   }
   vk_physicaldevice&
   physicaldevice::vk_obj()
   {
      return _physical_device;
   }
   physicaldevice::physicaldevice() {}
   physicaldevice::~physicaldevice() {}
}    // namespace my_library::vulkan
