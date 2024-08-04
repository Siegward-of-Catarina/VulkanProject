#pragma once
#include "../initializeInfo.hpp"
#include "../vkObjectTypes.hpp"
namespace my_library::vkm
{
   class Container;
   using shared_ptr_container = std::shared_ptr<Container>;
   using weak_ptr_container = std::weak_ptr<Container>;
}