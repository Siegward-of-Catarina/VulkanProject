#pragma once
#include "vk_object_types.hpp"
#include "utilities.hpp"

#include <optional>
namespace my_library::vulkan::queuefamily
{
   enum class types
   {
      GRAPHICS_QUEUE,
      PRESENT_QUEUE,
      COUNT
   };

   struct indices
   {
      std::optional<uint32_t> graphicsfamily;
      bool
      is_complete();

      std::optional<uint32_t>
      select( const types& type );
   };

   indices
   find_queuefamily( const vk_physicaldevice& device );

}    // namespace my_library::vulkan::queuefamily
