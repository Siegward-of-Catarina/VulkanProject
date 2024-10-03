#pragma once
#include "core_commonheaders.hpp"
#include "device.hpp"

namespace my_library
{
   namespace vkm
   {
      struct SwapChainInfo
      {
         class vkm::Queuefamily queuefamily;
         class SwapChainDetails swapchain_details;
         class UniqueSurface&   surface;
         std::pair<int, int>    glfw_fbuffer_size {};
      };
      class SwapChain
      {
      public:
         void
         create( const SwapChainInfo info );
         SwapChain();
         ~SwapChain();
      };
   }    // namespace vkm
}    // namespace my_library
