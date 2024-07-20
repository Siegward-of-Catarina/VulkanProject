#pragma once
#include"common.hpp"

#include<optional>

namespace my_library::vulkan
{
   struct queuefamily_indices
   {
      std::optional<uint32_t> graphcsfamily;
      bool
      is_complete();
      queuefamily_indices
      find_queuefamily( const vk_physicaldevice& device );
   };
}    // namespace my_libraray_vulkan
