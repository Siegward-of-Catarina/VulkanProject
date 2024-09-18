#pragma once
#include "../initializeInfo.hpp"
#include "../vkObjectTypes.hpp"
namespace my_library::vkm
{
   class VkObjContainer;
   using shared_ptr_container = std::shared_ptr<VkObjContainer>;
   using weak_ptr_container = std::weak_ptr<VkObjContainer>;
}