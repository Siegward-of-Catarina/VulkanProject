#pragma once
#include "vkObjectTypes.hpp"
namespace my_library
{
   namespace vkm
   {
      class Container
      {
      public:
         void
         registerCtr( UniqueInstance& instance );
         void
         registerCtr( PhysicalDevice& gpu );
         void
         registerCtr( UniqueSurface& surface );
         Container();
         ~Container();

      public:
         const DispatchLoaderDynamic& dld;
         const UniqueInstance&            instance;
         const UniqueSurface&            surface;
         const PhysicalDevice&            gpu;

      private:
         DispatchLoaderDynamic _dld;
         UniqueInstance            _instance;
         UniqueSurface            _surface;
         PhysicalDevice            _gpu;
      };
   }    // namespace vulkan
}    // namespace my_library
