#include "vulkan.hpp"

#include "../utilities.hpp"
#include "./debugUtils.hpp"
#include "container.hpp"
#include "core/instance.hpp"
#include "core/surface.hpp"
#include "core/device.hpp"
#include "glwindowForVk.hpp"
#include "initializeInfo.hpp"
#include "validationLayer.hpp"
namespace
{
   namespace vkm = my_library::vkm;
   namespace utl = my_library::utl;
}    // namespace
namespace
{}    // namespace

namespace my_library
{
   // vulkan::core
   namespace vkm
   {
      void
      Vulkan::init( const vulkan::CreateInfo& create_info, const bool debug )
      {

         DebugUtilsMessengerCreateInfo debug_msg_create_info { create_info.debug_severity_flags,
                                                               create_info.debug_msg_type_flags };
         instance::CreateInfo          instance_create_info;
         instance_create_info.app_name   = create_info.app_name;
         instance_create_info.debug_info = debug_msg_create_info.vkobj;
         instance_create_info.extensions = create_info.window->getRequiredInstanceExtensions( debug );

         _instance->create( instance_create_info, debug );

         _surface = std::make_unique<Surface>( create_info.window );
         _surface->create( _instance->vkobj );

         _device = std::make_unique<Device>( _instance->enumeratePhysicalDevices(), _instance->dld );
         _device->pickUpSuitablePhysicalDevice(_surface->vkobj);
      }
      Vulkan::Vulkan()
        : _instance { std::make_unique<Instance>() }, _surface {}, _device {}
      {}
      Vulkan::~Vulkan() {}
   }    // namespace vkm
}    // namespace my_library