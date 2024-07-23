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

      // GPU�̃��X�g����K�؂ȃf�o�C�X���擾
      for ( const auto& device : devices )
      {
         if ( is_device_suitable( device ) )
         {
            _physical_device = device;
            break;
         }
      }
      // rbegin �����@-> �擪�@�̋t�C�e���[�^
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
