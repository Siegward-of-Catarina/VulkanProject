#include "vulkan.hpp"

#include <GLFW/glfw3.h>    //拡張機能を取得するために必要
#include <algorithm>
#include <string>
#include <vector>
namespace lib::renderer
{
   void vulkan::create_instace()
   {
      VkApplicationInfo app_info {};
      app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      app_info.pApplicationName   = "Hello Triangle!";
      app_info.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
      app_info.pEngineName        = "No Engine";
      app_info.engineVersion      = VK_MAKE_VERSION( 1, 0, 0 );
      app_info.apiVersion         = VK_API_VERSION_1_0;

      // ウィンドウシステムの拡張機能を取得
      std::uint32_t glfw_extension_count = 0;
      const char**  glfw_extensions;
      glfw_extensions = glfwGetRequiredInstanceExtensions( &glfw_extension_count );
      VkInstanceCreateInfo create_info {};
      create_info.sType             = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      create_info.pApplicationInfo  = &app_info;
      create_info.enabledLayerCount = 0;    // 今は空
                                            // create_info.ppEnabledLayerNames
#if defined( _WIN32 )
      create_info.enabledExtensionCount   = glfw_extension_count;
      create_info.ppEnabledExtensionNames = glfw_extensions;
#elif defined( __APPLE__ )    // macos
      std::vector<const char*> required_extensions;
      for ( std::uint32_t i = 0; i < glfw_extension_count; i++ )
      {
         required_extensions.emplace_back( glfw_extensions[ i ] );
      }

      required_extensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );

      create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

      create_info.enabledExtensionCount   = static_cast<uint32_t>( required_extensions.size() );
      create_info.ppEnabledExtensionNames = required_extensions.data();
#endif

      check_vk_extension_supported( glfw_extensions, glfw_extension_count );
      // ようやく生成
      if ( vkCreateInstance( &create_info, nullptr, &vk_instance ) != VK_SUCCESS )
      {
         throw std::runtime_error( "failed to create instance!" );
      }
   }
   void vulkan::check_vk_extension_supported( const char** extensions, const std::uint32_t count )
   {
      std::uint32_t
        // 全てのサポートしている拡張機能の数のみ取得
        extension_count = 0;
      vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, nullptr );

      // 拡張機能の情報を取得
      std::vector<VkExtensionProperties> supported_extensions( extension_count );
      vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, supported_extensions.data() );

      std::string res;
      for ( const auto& extension : supported_extensions )
      {
         res += extension.extensionName;
         res += "\n";
      }

      for ( std::uint32_t i = 0; i < count; i++ )
      {
         // std::cout << extensions[ i ] << std::endl;
         if ( res.find( extensions[ i ] ) != std::string::npos ) 
         { 
             std::cout << extensions[ i ] << " is supported" << std::endl; 
         }
         else { std::cout << extensions[ i ] << " is not supported" << std::endl;
         }
      }
   }
   void vulkan::init() { create_instace(); }
   void vulkan::release() {}
   vulkan::vulkan() : vk_instance {VK_NULL_HANDLE}
   {

   }
}    // namespace lib::renderer

namespace api::renderer
{
   vulkan* create() { return new lib::renderer::vulkan(); }
}    // namespace api::renderer