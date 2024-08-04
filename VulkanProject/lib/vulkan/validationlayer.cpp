#include "validationLayer.hpp"
#include "vkObjectTypes.hpp"
namespace
{
   bool layerfound = false;
   bool
   checkValidationLayerSupport( const std::vector<const char*>& validationlayers )
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

namespace my_library::vkm::layer
{
   const std::vector<const char*>
   getValidationLayers()
   {
#ifdef NDEBUG
      const std::vector<const char*> validationlayers {};
#else
      const std::vector<const char*> validationlayers { "VK_LAYER_KHRONOS_validation" };
      //��x�����`�F�b�N����
      static const bool valid { checkValidationLayerSupport( validationlayers ) };

      if ( !valid ) throw std::runtime_error( "validation layers requested, but not available!" );
#endif
      return validationlayers;
   }
}    // namespace my_library::vulkan
