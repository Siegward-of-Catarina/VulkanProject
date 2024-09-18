#include "extension.hpp"

#include "../utilities.hpp"

#include <GLFW/glfw3.h>    //�g���@�\���擾���邽�߂ɕK�v
#include <set>
namespace my_library::vkm::ext
{

   const bool
   checkExtensionSupport( const std::vector<const char*>&               extensions,
                          const my_library::vkm::DispatchLoaderDynamic& dld )
   {
      // �g���@�\�̏����擾
      std::vector<my_library::vkm::ExtensionProperties> available_extensions { vk::enumerateInstanceExtensionProperties(
        nullptr, dld ) };
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

   const bool
   check_device_ext_support( const PhysicalDevice& device )
   {
      const std::vector<my_library::vkm::ExtensionProperties> available_exts {
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