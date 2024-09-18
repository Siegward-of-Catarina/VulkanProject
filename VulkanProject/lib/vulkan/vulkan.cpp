#include "vulkan.hpp"

#include "../utilities.hpp"
#include "./debugUtils.hpp"
#include "container.hpp"
#include "core/instance.hpp"
#include "core/surface.hpp"
#include "glwindowForVk.hpp"
#include "initializeInfo.hpp"
#include "validationLayer.hpp"
namespace
{
   namespace vkm = my_library::vkm;
   namespace utl = my_library::utl;

   vkm::DispatchLoaderDynamic        dld {};
   vkm::UniqueDebugUtilsMessengerEXT debug_messenger {};
   vkm::UniqueSurface                vk_surface {};
   vkm::PhysicalDevice               gpu {};
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
         auto devicesPair = _instance->enumeratePhysicalDevices();
         utl::log( devicesPair.size() );
         auto dev_pro = devicesPair[ 0 ].first.getProperties();

         switch ( dev_pro.deviceType )
         {
            case vk::PhysicalDeviceType::eCpu : utl::log( "device type is cpu" ); break;
            case vk::PhysicalDeviceType::eDiscreteGpu : utl::log( "device type is discrete" ); break;
            case vk::PhysicalDeviceType::eIntegratedGpu : utl::log( "device type is integratedGpu" ); break;
            case vk::PhysicalDeviceType::eOther : utl::log( "device type is other" ); break;
            case vk::PhysicalDeviceType::eVirtualGpu : utl::log( "device type is virtual" ); break;
         }
         _debug_messenger->create( _instance->vkobj, debug_msg_create_info.vkobj, _instance->dld );

         _surface = std::make_unique<Surface>( create_info.window );
         _surface->create( _instance->vkobj );
      }
      Vulkan::Vulkan()
        : _instance { std::make_unique<Instance>() }
        , _debug_messenger { std::make_unique<DebugUtilsMessenger>() }
        , _surface {}
      {}
      Vulkan::~Vulkan() {}
   }    // namespace vkm
}    // namespace my_library