#include "queuefamily.hpp"

#include "../../pch.hpp"
#include "../../utilities.hpp"

#include <optional>
namespace
{
   // suitable queufamilies
   std::optional<uint32_t> _graphicsfamily;
   std::optional<uint32_t> _presentfamily;
}    // namespace
namespace my_library::vkm
{
   const uint32_t
   Queuefamily::graphicsfamily()
   {
      if ( !_graphicsfamily.has_value() )
      {
         utl::runtimeError( "_graphicsfamily not has value" );
         return 0;
      }
      return _graphicsfamily.value();
   }
   const uint32_t
   Queuefamily::presentfamily()
   {
      if ( !_presentfamily.has_value() )
      {
         utl::runtimeError( "_presentfamily not has value" );
         return 0;
      }
      return _presentfamily.value();
   }
   const bool
   Queuefamily::is_complete()
   {
      return _graphicsfamily.has_value() && _presentfamily.has_value();
   }
   Queuefamily::Queuefamily( const device::Gpu&           device,
                             const surface::UniqueHandle& surface,
                             const DispatchLoaderDynamic& dld )
   {
      const std::vector<vk::QueueFamilyProperties> queuefamilyes { device.getQueueFamilyProperties() };

      uint32_t i { 0 };
      for ( const auto& queuefamily : queuefamilyes )
      {
         // デバイスがグラフィック関連のコマンドに対応しているか検索
         if ( queuefamily.queueFlags & vk::QueueFlagBits::eGraphics ) _graphicsfamily = i;

         const VkBool32 present_support { device.getSurfaceSupportKHR( i, surface.get(), dld ) };
         if ( present_support ) { _presentfamily = i; }

         // 見つかり次第終了
         if ( is_complete() ) break;
         ++i;
      }
      if ( !is_complete() ) utl::runtimeError( "find queuefamily successed." );
   }
   Queuefamily::Queuefamily() {}
   Queuefamily::~Queuefamily() {}
   Queuefamily&
   Queuefamily::operator=( const Queuefamily& q )
   {
      return *this;
   }
}    // namespace my_library::vkm