#include "vulkan.hpp"

#include "../glwindow.hpp"
#include "container.hpp"
#include "core/instance.hpp"
#include "debugUtils.hpp"
#include "initializeInfo.hpp"
#include "validationLayer.hpp"

namespace
{
   const std::uint32_t WIDTH  = 800;
   const std::uint32_t HEIGHT = 600;
}    // namespace

namespace my_library
{
   // vulkan::core
   namespace vkm
   {
      void
      Vulkan::init( const vulkan::CreateInfo& create_info, const bool debug )
      {
         instance::CreateInfo instance_create_info;
         instance_create_info.app_name          = create_info.app_name;
         instance_create_info.debug_info        = debug_utl::populateCreateInfo();
         instance_create_info.extensions        = create_info.window->getRequiredInstanceExtensions( debug );
         instance_create_info.validation_layers = layer::getValidationLayers();

         _instance->init( instance_create_info, debug );
      }

      Vulkan::Vulkan() 
          : _container { std::make_shared<Container>() }
          , _instance { std::make_unique<Instance>(_container) } {}

      Vulkan::~Vulkan() {}
   }       // namespace vkm
}    // namespace my_library