#include "extensions.hpp"

#include <GLFW/glfw3.h>    //�g���@�\���擾���邽�߂ɕK�v
#include <iostream>
#include <string>

namespace my_library
{
#ifdef NDEBUG
   void
   vulkan::extensions::register_for_create_info( VkInstanceCreateInfo&           create_info )
   {
      // ==[ �g���@�\ ]==========================
      uint32_t     glfw_extension_count = 0;
      const char** glfw_extensions;
      glfw_extensions = glfwGetRequiredInstanceExtensions( &glfw_extension_count );
      std::vector<const char*> extensions( glfw_extensions, glfw_extensions + glfw_extension_count );

#if defined( __APPLE__ )    // MacOs�ɑΉ����邽�ߓ����
      extensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );
#endif

      if ( !check_extension_support( extensions ) )
      {
         throw std::runtime_error( "extensions requested, but not available!" );
      }
      create_info.enabledExtensionCount   = static_cast<uint32_t>( extensions.size() );
      create_info.ppEnabledExtensionNames = extensions.data();
   }
#else
   void
   vulkan::extensions::register_for_create_info( VkInstanceCreateInfo&           create_info )
   {
      // ==[ �g���@�\ ]==========================
      uint32_t     glfw_extension_count = 0;
      const char** glfw_extensions;
      glfw_extensions = glfwGetRequiredInstanceExtensions( &glfw_extension_count );
      std::vector<const char*> extensions( glfw_extensions, glfw_extensions + glfw_extension_count );

#if defined( __APPLE__ )    // MacOs�ɑΉ����邽�ߓ����
      extensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );
#endif
      // �f�o�b�O���L��
      extensions.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );

      if ( !check_extension_support( extensions ) )
      {
         throw std::runtime_error( "extensions requested, but not available!" );
      }
      create_info.enabledExtensionCount   = static_cast<uint32_t>( extensions.size() );
      create_info.ppEnabledExtensionNames = extensions.data();
   }
#endif

   bool
   vulkan::extensions::check_extension_support( const std::vector<const char*>& extensions )
   {
      // �S�ẴT�|�[�g���Ă���g���@�\�̐��̂ݎ擾
      std::uint32_t extension_count = 0;
      vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, nullptr );

      // �g���@�\�̏����擾
      std::vector<VkExtensionProperties> available_extensions( extension_count );
      vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, available_extensions.data() );

      // �g���@�\�̖��O����ɂ܂Ƃ߂�

      // �g���@�\���T�|�[�g����Ă��邩�ꗗ����`�F�b�N
      for ( const char* extension : extensions )
      {
         bool extension_found = false;
         for ( const auto& extension_property : available_extensions )
         {
            if ( std::equal( std::string_view { extension_property.extensionName }.begin(),
                             std::string_view { extension_property.extensionName }.end(),
                             std::string_view { extension }.begin(),
                             std::string_view { extension }.end() ) )
            {
               std::cout << extension << " is support" << std::endl;
               extension_found = true;
               break;
            }
         }
         if ( !extension_found ) return false;
      }
      return true;
   }

}    // namespace my_library
