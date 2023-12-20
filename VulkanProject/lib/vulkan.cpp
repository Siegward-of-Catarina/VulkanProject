#include "vulkan.hpp"

#include <GLFW/glfw3.h>    //�g���@�\���擾���邽�߂ɕK�v
#include <algorithm>
#include <string>
#include <vector>

namespace
{
   const std::uint32_t WIDTH  = 800;
   const std::uint32_t HEIGHT = 600;
}    // namespace


namespace lib::renderer
{
   void vulkan::create_instace()
   {
       //���؃��C���[
      const std::vector<const char*> validationlayers { "VK_LAYER_KHRONOS_validation" };
      #ifdef NDEBUG
      const bool enable_validationlayers = false;
      #else
      const bool enable_validationlayers = true;
      #endif



      VkApplicationInfo app_info {};
      // �A�v���P�[�V�����̏ڍאݒ�
       {
         app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
         app_info.pApplicationName   = "Hello Triangle!";
         app_info.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
         app_info.pEngineName        = "No Engine";
         app_info.engineVersion      = VK_MAKE_VERSION( 1, 0, 0 );
         app_info.apiVersion         = VK_API_VERSION_1_0;
       }

      // �E�B���h�E�V�X�e���̊g���@�\���擾
      std::uint32_t glfw_extension_count = 0;
      const char**  glfw_extensions;
      glfw_extensions = glfwGetRequiredInstanceExtensions( &glfw_extension_count );

      // �C���X�^���X�쐬�̏ڍאݒ�
      VkInstanceCreateInfo create_info {};
      {
         create_info.sType             = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
         create_info.pApplicationInfo  = &app_info;
         create_info.enabledLayerCount = 0;    // ���͋�
                                               // create_info.ppEnabledLayerNames

         // OS�ɂ���Ċg���@�\�̒ǉ���������̂ŕ���
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
      }
      
      // �g���@�\���T�|�[�g����Ă��邩�m�F
      check_extension_support( glfw_extensions, glfw_extension_count );

      // �悤�₭����
      if ( vkCreateInstance( &create_info, nullptr, &vk_instance ) != VK_SUCCESS )
      {
         throw std::runtime_error( "failed to create instance!" );
      }

   }

   bool vulkan::check_validationlayer_support( std::vector<const char*, std::allocator<const char*> > validationlayers )
   { 
       //�g�p�\�ȃ��C���[�����擾
       std::uint32_t layer_count;
      vkEnumerateInstanceLayerProperties( &layer_count, nullptr );
      //�g�p�\���C���[�̏����擾
       std::vector<VkLayerProperties> availablelayers( layer_count );
      vkEnumerateInstanceLayerProperties( &layer_count, availablelayers.data() );
      //�g�p���������C���[���S�ăT�|�[�g����Ă��邩�m�F
      for ( const char* layerName : validationlayers ) {}
      return false;
   }

   void vulkan::check_extension_support( const char** extensions, const std::uint32_t count )
   {
      // �S�ẴT�|�[�g���Ă���g���@�\�̐��̂ݎ擾
      std::uint32_t extension_count = 0;
      vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, nullptr );

      // �g���@�\�̏����擾
      std::vector<VkExtensionProperties> supported_extensions( extension_count );
      vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, supported_extensions.data() );

      // �g���@�\�̖��O����ɂ܂Ƃ߂�
      std::string supported_extension_names;
      for ( const auto& extension : supported_extensions )
      {
         supported_extension_names += extension.extensionName;
         supported_extension_names += "\n";
      }

      // �g���@�\���T�|�[�g����Ă��邩�ꗗ����`�F�b�N
      for ( std::uint32_t i = 0; i < count; i++ )
      {
         if ( supported_extension_names.find( extensions[ i ] ) != std::string::npos )
            std::cout << extensions[ i ] << " is supported" << std::endl;
         else throw std::runtime_error( std::string { extensions[ i ] } + std::string { " is not supported" } );
      }
   }
   void vulkan::init() { create_instace(); }
   void vulkan::release()
   {
      vkDestroyInstance( vk_instance, nullptr );
      delete this;
   }
   vulkan::vulkan() : vk_instance { VK_NULL_HANDLE } {}
}    // namespace lib::renderer


namespace api::renderer
{
   vulkan* create() { return new lib::renderer::vulkan(); }
}    // namespace api::renderer