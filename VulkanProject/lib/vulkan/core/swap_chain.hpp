#pragma once
#include "core_commonheaders.hpp"

namespace my_library{
   namespace vulkan{
      class swap_chain : public core_object
      {
      public:
         void
         init(const std::pair<int, int>& glfw_fbuffer_size);
         swap_chain( const shared_container& c );
         ~swap_chain();
      };
   }
}
