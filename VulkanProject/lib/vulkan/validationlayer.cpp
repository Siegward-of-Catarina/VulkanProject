#include "validationlayer.hpp"
namespace
{
   bool
   check_validationlayer_support( const std::vector<const char*>& validationlayers )
   {
      if ( validationlayers.empty() ) return false;

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
}    // namespace

namespace my_library::vulkan
{
   const std::vector<const char*>
   get_validationlayers()
   {
#ifdef NDEBUG
      const std::vector<const char*> validationlayers {};
#else
      const std::vector<const char*> validationlayers { "VK_LAYER_KHRONOS_validation" };

      if ( !check_validationlayer_support( validationlayers ) )
         throw std::runtime_error( "validation layers requested, but not available!" );
#endif
      return validationlayers;
   }
}    // namespace my_library::vulkan
