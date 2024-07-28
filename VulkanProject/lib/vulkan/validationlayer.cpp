#include "validationlayer.hpp"
#include "vk_object_types.hpp"
namespace
{
   bool layerfound = false;

   bool
   check_validationlayer_support( const std::vector<const char*>& validationlayers )
   {

      if ( validationlayers.empty() ) return false;

      const std::vector<vk::LayerProperties> availablelayers { vk::enumerateInstanceLayerProperties() };
      bool                                   layerfound { false };
      for ( const char* layername : validationlayers )
      {
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
}    // namespace

namespace my_library::vulkan::layer
{
   const std::vector<const char*>
   get_validationlayers()
   {
#ifdef NDEBUG
      const std::vector<const char*> validationlayers {};
#else
      const std::vector<const char*> validationlayers { "VK_LAYER_KHRONOS_validation" };
      //一度だけチェックする
      static const bool valid { check_validationlayer_support( validationlayers ) };

      if ( !valid ) throw std::runtime_error( "validation layers requested, but not available!" );
#endif
      return validationlayers;
   }
}    // namespace my_library::vulkan
