#include "instance.hpp"

#include <GLFW/glfw3.h>    //拡張機能を取得するために必要
#include <iostream>
#include <string>
namespace my_library
{
   vulkan::instance*
   vulkan::instance::create()
   {
      return new create_helper();
   }
   vulkan::instance::instance() : vk_instance {VK_NULL_HANDLE}
   {
      VkApplicationInfo app_info {};
      // アプリケーションの詳細設定
      {
         app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
         app_info.pApplicationName   = "Hello Triangle!";
         app_info.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
         app_info.pEngineName        = "No Engine";
         app_info.engineVersion      = VK_MAKE_VERSION( 1, 0, 0 );
         app_info.apiVersion         = VK_API_VERSION_1_0;
      }

      // インスタンス作成の詳細設定
      VkInstanceCreateInfo create_info {};
      create_info.sType             = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      create_info.pApplicationInfo  = &app_info;
      create_info.enabledLayerCount = 0;          // いったんこれで設定しておく
      create_info.pNext             = nullptr;    // いったんこれで設定しておく
      // mac os に対応するためこのフラグがいる
      create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

      // ==[ 拡張機能 ]==========================
      extensions::register_for_create_info( create_info );

      // ==[ 検証レイヤー ]===========================
      if ( !validationlayers.empty() ) debug_messenger->register_for_create_info( create_info, validationlayers );

      // ==[ ようやく生成 ]=============================
      if ( vkCreateInstance( &create_info, nullptr, &vk_instance ) != VK_SUCCESS )
      {
         throw std::runtime_error( "failed to create instance!" );
      }
   }
}    // namespace my_library