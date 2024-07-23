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
#ifdef NDEBUG
   const std::vector<const char*> validationlayers;
#else
   const std::vector<const char*> validationlayers { "VK_LAYER_KHRONOS_validation" };
#endif    // NDEBUG

}    // namespace

namespace my_library
{
   // vulkan::core
   namespace vulkan
   {
      bool
      vulkan::check_validationlayer_support()
      {
         std::vector<vk::LayerProperties> availablelayers = vk::enumerateInstanceLayerProperties();

         for ( const char* layername : validationlayers )
         {
            bool layerfound = false;
            for ( const auto& layerproperties : availablelayers )
            {
               if ( strcmp( layername, layerproperties.layerName ) == 0 )
               {
                  layerfound = true;
                  break;
               }
            }
            if ( !layerfound ) { return false; }
         }
         return true;
      }

      void
      vulkan::init( GLFWwindow* window )
      {

         dld.init();

         if ( !validationlayers.empty() )
         {
            if ( !check_validationlayer_support() )
               throw std::runtime_error( "validation layers requested, but not available!" );

            _instance->init( "hello triangle", validationlayers, _vulkan_debug->messenger_create_info(), dld );
            _vulkan_debug->setup_messenger( _instance->vk_obj(), dld );
            _surface->init( _instance->vk_obj(), window );
            _physicaldevice->pick_physical_device( _instance->vk_obj(), _surface->vk_obj(), dld );
            _logicaldevice->init( _physicaldevice, validationlayers, dld );
         }
         else
         {
            _instance->init( "hello triangle", dld );
            _surface->init( _instance->vk_obj(), window );
            _physicaldevice->pick_physical_device( _instance->vk_obj(), _surface->vk_obj(), dld );
            _logicaldevice->init( _physicaldevice, dld );
         }

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
