#include "container.hpp"
namespace my_library
{
   namespace vkm
   {
      void
      VkObjContainer::registerCtr( UniqueInstance instance )
      {
         _instance = std::move( instance );
         _dld.init( *_instance );
      }
      void
      VkObjContainer::registerCtr( PhysicalDevice& gpu )
      {
         _gpu = std::move( gpu );
      }
      void
      VkObjContainer::registerCtr( UniqueSurface& surface )
      {
         _surface = std::move( surface );
      }
      VkObjContainer::VkObjContainer( DispatchLoaderDynamic dispatchloaderdynamic )
        : _dld { dispatchloaderdynamic }, dld { _dld }, instance { _instance }, gpu { _gpu }, surface { _surface }
      {
         _dld.init();
      }

      VkObjContainer::~VkObjContainer() {}
   }    // namespace vkm
}    // namespace my_library
