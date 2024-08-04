#include "swap_chain.hpp"

#include "../../../pch.hpp"
#include "../container.hpp"
#include "../swap_chain_support.hpp"

namespace my_library
{
   namespace vulkan
   {
      void
      swap_chain::init( const std::pair<int, int>& glfw_fbuffer_size )
      {
         if ( const shared_container c = ctr_lock() )
         {
            // スワップチェーンを作成するための詳細情報を取得する
            const swapchain_support::details& swp_detailes = swapchain_support::query_details(c->gpu, c->surface);

            vk_surface_format_khr             surf_format  = swp_detailes.choose_swap_surfaceformat();
            vk_present_mode_khr present_mode = swp_detailes.choose_swap_presentmode( vk_presentmode_khr::eMailbox );
            vk_extent_2d        extent       = swp_detailes.choose_swap_extent( glfw_fbuffer_size );

            // スワップチェーンに含める画像の数を決定
            // NOTE: 画像数を最小値に固執するとドライバが内部操作を完了するのを待つ可能性が出てくるので、
            // 最小値より1つ多い数を指定しておくといい
            uint32_t image_count = swp_detailes.capabilities.minImageCount + 1;
            // サポートされている最大個数が0でないかつ、image_count が最大数を超えていた場合は、最大数に丸める。
            if ( swp_detailes.capabilities.maxImageCount > 0 )
            {
               image_count = std::clamp( image_count, image_count, swp_detailes.capabilities.maxImageCount );
            }

            vk_swp_createinfo_khr createinfo(
              {},
              c->surface.get(),
              image_count,
              surf_format.format,
              surf_format.colorSpace,
              extent,
              1, /*imageArrayLayers これはvrアプリケーションの開発以外は基本的に１のようだ*/
              vk_img_usageflag_bits::eColorAttachment, /*画像に直接レンダリングする*/
              vk_sharing_mode::eExclusive,
              {},
              swp_detailes.capabilities
                .currentTransform, /*ディスプレイとFバッファの向きがことなる場合がある。変更が不要なら現在の向きを指定する。*/
              vk_comp_alphaflag_bits_khr::eOpaque, /*今のところアルファブレントは無視*/
              present_mode,
              VK_TRUE,                             /*clipped*/
              VK_NULL_HANDLE /*今のところnullptr*/ );

            uint32_t graphicsfamily = c->queuefamilies.graphicsfamily.value();
            uint32_t presentfamily  = c->queuefamilies.presentfamily.value();
            // グラフィックスとプレゼントファミリのindexがことなる場合は
            // イメージを複数のキューで共有するようにする
            if ( graphicsfamily != presentfamily )
            {
               createinfo.setImageSharingMode( vk_sharing_mode::eConcurrent );
               std::vector indices { graphicsfamily, presentfamily };
               createinfo.setQueueFamilyIndices( indices );
            }

         }
      }
      swap_chain::swap_chain( const shared_container& c ) : core_object { c } {}
      swap_chain::~swap_chain() {}
   }    // namespace vulkan
}    // namespace my_library
