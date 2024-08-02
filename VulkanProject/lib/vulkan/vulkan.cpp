#include "vulkan.hpp"

#include "container.hpp"
#include "core/instance.hpp"
#include "logicaldevice.hpp"
#include "physicaldevice.hpp"
#include "queuefamily.hpp"
#include "core/surface.hpp"

#include <GLFW/glfw3.h>    //拡張機能を取得するために必要

namespace
{
   const std::uint32_t WIDTH  = 800;
   const std::uint32_t HEIGHT = 600;
}    // namespace

namespace my_library
{
   // vulkan::core
   namespace vulkan
   {
      void
      vulkan::init( GLFWwindow* window, const bool debug )
      {

         dld.init();

         _instance->init( "hello triangle", debug );

         //_surface->init( _container->instance, window );
         _physicaldevice->pick_physical_device( _container->instance, _surface->vk_obj(), dld );
         _logicaldevice->init( _physicaldevice, dld );

         _graphics_queue = _logicaldevice->get_queue(
           _physicaldevice->valid_queuefamily_idx( queuefamily::types::GRAPHICS_QUEUE ), dld );

         _present_queue = _logicaldevice->get_queue(
           _physicaldevice->valid_queuefamily_idx( queuefamily::types::PRESENT_QUEUE ), dld );
      }

      vulkan::vulkan()
        : _container { std::make_shared<container>() }
        , _instance { std::make_unique<instance>( _container ) }
        , _surface { std::make_unique<surface>() }
        , _physicaldevice { std::make_unique<physicaldevice>() }
        , _logicaldevice { std::make_unique<logicaldevice>() }
      {}

      vulkan::~vulkan() {}
   }    // namespace vulkan
}    // namespace my_library
