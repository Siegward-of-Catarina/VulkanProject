#pragma once
#include "vk_object_types.hpp"
namespace my_library::vulkan
{
   class physicaldevice;
   class logicaldevice;
   namespace queuefamily
   {
      enum class types;
   }
}    // namespace my_library::vulkan
namespace my_library::vulkan
{
   class device
   {
   private:

   public:
      void
      init( unq_vk_instance&                instance,
            const std::vector<const char*>& validationlayers,
            vk_dispatchloader_dynamic&      dld );
      void
      init( unq_vk_instance& instance, vk_dispatchloader_dynamic& dld );
      const vk_queue&
      get_queue( const queuefamily::types&, vk_dispatchloader_dynamic& dld );
      device();
      ~device();

   private:
      std::unique_ptr<physicaldevice> _physicaldevice;
      std::unique_ptr<logicaldevice>  _logicaldevice;
   };
}    // namespace my_library::vulkan
