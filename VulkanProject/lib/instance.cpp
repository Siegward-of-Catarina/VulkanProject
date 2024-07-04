#include "instance.hpp"

#include <GLFW/glfw3.h>    //�g���@�\���擾���邽�߂ɕK�v
#include <iostream>
#include <string>
namespace
{
   const std::vector<const char*> validationlayers = { "VK_LAYER_KHRONOS_validation" };
#ifdef NDEBUG
   const bool enable_validationlayers = false;
#else
   const bool enable_validationlayers = true;
#endif    // NDEBUG
}
namespace my_library
{
   vulkan::instance*
   vulkan::instance::create()
   {
      return new create_helper();
   }
   vulkan::instance::instance() {}
   std::vector<const char*>
   vulkan::instance::get_required_extensions( const bool enable_validationlayers )
   {
      uint32_t     glfw_extension_count = 0;
      const char** glfw_extensions;
      glfw_extensions = glfwGetRequiredInstanceExtensions( &glfw_extension_count );
      std::vector<const char*> extensions( glfw_extensions, glfw_extensions + glfw_extension_count );
      // MacOs�ɑΉ����邽�ߓ����
      extensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );
      // �f�o�b�O���L��
      if ( enable_validationlayers ) { extensions.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ); }
      return extensions;
   }

   bool
   vulkan::instance::check_extension_support( const std::vector<const char*>& extensions )
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

   bool
   vulkan::instance::check_validationlayer_support( const std::vector<const char*>& validationlayers )
   {
      // �g�p�\�ȃ��C���[�����擾
      std::uint32_t layer_count;
      vkEnumerateInstanceLayerProperties( &layer_count, nullptr );
      // �g�p�\���C���[�̏����擾
      std::vector<VkLayerProperties> availablelayers( layer_count );
      vkEnumerateInstanceLayerProperties( &layer_count, availablelayers.data() );
      // �g�p���������C���[���S�ăT�|�[�g����Ă��邩�m�F
      for ( const char* layer_name : validationlayers )
      {
         bool layer_found = false;
         for ( const auto& layer_propertys : availablelayers )
         {
            if ( std::equal( std::string_view { layer_propertys.layerName }.begin(),
                             std::string_view { layer_propertys.layerName }.end(),
                             std::string_view { layer_name }.begin(),
                             std::string_view { layer_name }.end() ) )
            {
               layer_found = true;
               break;
            }
         }
         if ( !layer_found ) return false;
      }
      return true;
   }
}    // namespace my_library