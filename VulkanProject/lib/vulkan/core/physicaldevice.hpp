#pragma once
#include "core_commonheaders.hpp"
namespace my_library
{
   namespace vulkan
   {
      namespace queuefamily
      {
         enum class types;
      }
   }    // namespace vulkan
}    // namespace my_library

namespace my_library::vulkan
{
   class physicaldevice : public core_object
   {
   private:
      bool
      is_device_suitable( const vk_physicaldevice& device );

   public:
      void
      pick_physical_device();
      physicaldevice( const shared_container& c );
      ~physicaldevice();
   };
}    // namespace my_library::vulkan
