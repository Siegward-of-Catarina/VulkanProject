#include "vulkan.hpp"

#include "instance.hpp"
#include "logicaldevice.hpp"
#include "physicaldevice.hpp"
#include "queuefamily.hpp"
#include "surface.hpp"
#include "vulkan_debug.hpp"

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

         if ( debug )
         {
            _instance->init( "hello triangle", _vulkan_debug->messenger_create_info(), dld );
            _vulkan_debug->setup_messenger( _instance->vk_obj(), dld );
         }
         else { _instance->init( "hello triangle", {}, dld ); }

         _surface->init( _instance->vk_obj(), window );
         _physicaldevice->pick_physical_device( _instance->vk_obj(), _surface->vk_obj(), dld );
         _logicaldevice->init( _physicaldevice, dld );

         _graphics_queue = _logicaldevice->get_queue(
           _physicaldevice->valid_queuefamily_idx( queuefamily::types::GRAPHICS_QUEUE ), dld );

         _present_queue = _logicaldevice->get_queue(
           _physicaldevice->valid_queuefamily_idx( queuefamily::types::PRESENT_QUEUE ), dld );
      }

      vulkan::vulkan()
        : _instance { std::make_unique<instance>() }
        , _vulkan_debug { std::make_unique<vulkan_debug>() }
        , _surface { std::make_unique<surface>() }
        , _physicaldevice { std::make_unique<physicaldevice>() }
        , _logicaldevice { std::make_unique<logicaldevice>() }
      {}

      vulkan::~vulkan() {}
   }    // namespace vulkan
}    // namespace my_library
