#include "extension.hpp"

#include "utilities.hpp"

#include <GLFW/glfw3.h>    //�g���@�\���擾���邽�߂ɕK�v
#include <set>
namespace
{
   const bool
   check_extension_support( const std::vector<const char*>&                      extensions,
                            const my_library::vulkan::vk_dispatchloader_dynamic& dld )
   {
      // �g���@�\�̏����擾
      std::vector<my_library::vulkan::vk_ext_properties> available_extensions {
         vk::enumerateInstanceExtensionProperties( nullptr, dld )
      };
      // �g���@�\���T�|�[�g����Ă��邩�ꗗ����`�F�b�N
      for ( const char* extension : extensions )
      {
         bool extension_found { false };
         for ( const auto& extension_property : available_extensions )
         {
            if ( strcmp( extension, extension_property.extensionName ) == 0 )
            {
               extension_found = true;
               break;
            }
         }
         if ( !extension_found ) return false;
      }
      return true;
   }
}    // namespace
namespace my_library::vulkan::ext
{
   const std::vector<const char*>
   get_required_extensions( const my_library::vulkan::vk_dispatchloader_dynamic& dld )
   {
      uint32_t     glfw_extension_count { 0 };
      const char** glfw_extensions;
      glfw_extensions = glfwGetRequiredInstanceExtensions( &glfw_extension_count );
      std::vector<const char*> extensions { glfw_extensions, glfw_extensions + glfw_extension_count };

#ifdef __APPLE__    // MacOs�ɑΉ����邽�ߓ����
      extensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );
#endif
#ifdef _DEBUG
      extensions.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
#endif

      if ( !check_extension_support( extensions, dld ) )
      {
         throw std::runtime_error( "extensions requested, but not available!" );
      }
      utl::log( "requested extensions is a suported." );
      return extensions;
   }

   const bool
   check_device_ext_support( const my_library::vulkan::vk_physicaldevice& device )
   {
      const std::vector<my_library::vulkan::vk_ext_properties> available_exts {
         device.enumerateDeviceExtensionProperties()
      };

      const auto&           dev_ext { get_device_exts() };
      std::set<std::string> required_exts { dev_ext.begin(), dev_ext.end() };
      // �v�����ꂽ�g���@�\�z�񂩂�L���Ȋg���@�\��������΍폜���Ă����B
      for ( const auto& ext : available_exts ) { required_exts.erase( ext.extensionName ); }

      my_library::utl::log( "requested device extensions is a suported." );

      // ���ׂėL���������ꍇ empty �ɂȂ�
      return required_exts.empty();
   }
   const std::vector<const char*>
   get_device_exts()
   {
      return std::vector<const char*> { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
   }
}    // namespace my_library::vulkan::ext