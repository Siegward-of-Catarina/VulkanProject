#pragma once
#include "vkObjectTypes.hpp"
namespace my_library
{
   namespace vkm
   {
      class VkObjContainer
      {
      public:
         void
         registerCtr( UniqueInstance instance );
         void
         registerCtr( PhysicalDevice& gpu );
         void
         registerCtr( UniqueSurface& surface );
         explicit VkObjContainer(const DispatchLoaderDynamic dispatchloaderdynamic);
         ~VkObjContainer();

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
