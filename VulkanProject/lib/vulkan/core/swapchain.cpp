#include "swapchain.hpp"

namespace my_library::vkm
{
   void
   SwapChain::create( const SwapChainInfo info )
   {
      auto swapchain_details = info.swapchain_details;
      auto surf_format       = swapchain_details.chooseSwapSurfaceFormat();
      auto present_mode      = swapchain_details.chooseSwapPresentMode( PresentModeKHR::eMailbox );
      auto extent            = swapchain_details.chooseSwapExtent( info.glfw_fbuffer_size );

      // スワップチェーンに含める画像の数を決定
      // NOTE: 画像数を最小値に固執するとドライバが内部操作を完了するのを待つ可能性が出てくるので、
      // 最小値より1つ多い数を指定しておくといい
      uint32_t image_count = swapchain_details.capabilities.minImageCount + 1;
      // サポートされている最大個数が0でないかつ、image_count が最大数を超えていた場合は、最大数に丸める。
      if ( swapchain_details.capabilities.maxImageCount > 0 )
      {
         image_count = std::clamp( image_count, image_count, swapchain_details.capabilities.maxImageCount );
      }

      SwapChainCreateInfo createinfo(
        {},
        info.surface.get(),
        image_count,
        surf_format.format,
        surf_format.colorSpace,
        extent,
        1, /*imageArrayLayers これはvrアプリケーションの開発以外は基本的に１のようだ*/
        ImageUsageFlagBits::eColorAttachment, /*画像に直接レンダリングする*/
        SharingMode::eExclusive,
        {},
        swapchain_details.capabilities
          .currentTransform, /*ディスプレイとFバッファの向きがことなる場合がある。変更が不要なら現在の向きを指定する。*/
        CompositeAlphaFlagBits::eOpaque, /*今のところアルファブレントは無視*/
        present_mode,
        VK_TRUE, /*clipped*/
        VK_NULL_HANDLE /*今のところnullptr*/ );

      const uint32_t graphicsfamily = info.queuefamily.graphicsfamily;
      const uint32_t presentfamily  = info.queuefamily.presentfamily;
      // グラフィックスとプレゼントファミリのindexがことなる場合は
      // イメージを複数のキューで共有するようにする
      if ( graphicsfamily != presentfamily )
      {
         createinfo.setImageSharingMode( SharingMode::eConcurrent );
         std::vector indices { graphicsfamily, presentfamily };
         createinfo.setQueueFamilyIndices( indices );
      }
   }
   SwapChain::SwapChain() {}
   SwapChain::~SwapChain() {}
}    // namespace my_library::vkm