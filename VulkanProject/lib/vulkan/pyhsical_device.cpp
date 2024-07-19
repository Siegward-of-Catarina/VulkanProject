#include "pyhsical_device.hpp"
namespace my_library::vulkan
{
   bool
   pyhsical_device::is_device_suitable( const vk::PhysicalDevice& device)
   {
      return false;
   }
   void
   pyhsical_device::pick_physical_device( UniqueInstance& instance, vk::DispatchLoaderDynamic& dld )
   {

       std::vector<vk::PhysicalDevice> devices = instance->enumeratePhysicalDevices(dld);

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
      if ( !_physical_device){ throw std::runtime_error( "failed to find a suitable GPU!" ); }
   }
   pyhsical_device::pyhsical_device() {}
   pyhsical_device::~pyhsical_device() {}
}

