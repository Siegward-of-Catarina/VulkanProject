
#include "device.hpp"

#include "logicaldevice.hpp"
#include "physicaldevice.hpp"
#include "queuefamily.hpp"
namespace my_library::vulkan
{
   void
   device::init( unq_vk_instance&                instance,
                 const std::vector<const char*>& validationlayers,
                 vk_dispatchloader_dynamic&      dld )
   {
      _physicaldevice->pick_physical_device( instance, dld );
      _logicaldevice->init( _physicaldevice->vk_obj(), validationlayers, dld );
   }
   void
   device::init( unq_vk_instance& instance, vk_dispatchloader_dynamic& dld )
   {
      _physicaldevice->pick_physical_device( instance, dld );
      _logicaldevice->init( _physicaldevice->vk_obj(), dld );
   }

   const vk_queue&
   device::get_queue( const queuefamily::types& type, vk_dispatchloader_dynamic& dld )
   {
      assert( type < queuefamily::types::COUNT );
      queuefamily::indices indices      = queuefamily::find_queuefamily( _physicaldevice->vk_obj() );
      uint32_t             select_index = indices.select( type ).value();
      return _logicaldevice->vk_obj()->getQueue( select_index, 0, dld );
   }

   device::device()
     : _physicaldevice { std::make_unique<physicaldevice>() }, _logicaldevice { std::make_unique<logicaldevice>() }
   {}

   device::~device() {}
}    // namespace my_library::vulkan
