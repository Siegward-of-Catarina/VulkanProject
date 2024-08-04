#include "container.hpp"
namespace my_library
{
   namespace vkm
   {
      void
      Container::registerCtr( UniqueInstance& instance )
      {
         _instance = std::move( instance );
         _dld.init( *_instance );
      }
      void
      Container::registerCtr( PhysicalDevice& gpu )
      {
         _gpu = std::move( gpu );
      }
      void
      Container::registerCtr( UniqueSurface& surface )
      {
         _surface = std::move( surface );
      }
      Container::Container() : dld { _dld }, instance { _instance }, gpu { _gpu }, surface { _surface } { _dld.init(); }

      Container::~Container() {}
   }    // namespace vkm
}    // namespace my_library
