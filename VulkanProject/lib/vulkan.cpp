#include "vulkan.hpp"

#include <GLFW/glfw3.h>    //�g���@�\���擾���邽�߂ɕK�v
#include <algorithm>
#include <iostream>
#include <string>

namespace
{
   const std::uint32_t WIDTH  = 800;
   const std::uint32_t HEIGHT = 600;
}    // namespace

namespace lib::renderer
{

   vulkan::vulkan() : vk_instance { VK_NULL_HANDLE } {}
   void vulkan::create_instace()
   {
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

      VkInstanceCreateInfo create_info {};
#ifdef NDEBUG
      const bool enable_validationlayers = false;
#else
      const bool enable_validationlayers = true;
#endif    // NDEBUG
      // ���؃��C���[
      std::vector<const char*> validationlayers { "VK_LAYER_KHRONOS_validation" };

      // �C���X�^���X�쐬�̏ڍאݒ�
      create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      create_info.pApplicationInfo = &app_info;
      // mac os �ɑΉ����邽�߂��̃t���O������
      create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
      // ==[ �g���@�\ ]==========================
      auto extensions = get_required_extensions( enable_validationlayers );
      if ( !check_extension_support( extensions ) )
         throw std::runtime_error( "extensions requested, but not available!" );

      create_info.enabledExtensionCount   = static_cast<uint32_t>( extensions.size() );
      create_info.ppEnabledExtensionNames = extensions.data();
      // ==[ ���؃��C���[ ]================================
      if ( enable_validationlayers )
      {
         if ( !check_validationlayer_support( validationlayers ) )
            throw std::runtime_error( "validation layers requested, but not available!" );

         create_info.enabledLayerCount   = static_cast<uint32_t>( validationlayers.size() );
         create_info.ppEnabledLayerNames = validationlayers.data();
      }
      else { create_info.enabledLayerCount = 0; }
      //get_vkinstance_create_info( &app_info, &create_info );

      // �悤�₭����
      if ( vkCreateInstance( &create_info, nullptr, &vk_instance ) != VK_SUCCESS )
      {
         throw std::runtime_error( "failed to create instance!" );
      }
   }
   void vulkan::get_vkinstance_create_info( VkApplicationInfo* app_info, VkInstanceCreateInfo* create_info )
   {

#ifdef NDEBUG
      const bool enable_validationlayers = false;
#else
      const bool enable_validationlayers = true;
#endif    // NDEBUG
      // ���؃��C���[
      std::vector<const char*> validationlayers { "VK_LAYER_KHRONOS_validation" };

      // �C���X�^���X�쐬�̏ڍאݒ�
      create_info->sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      create_info->pApplicationInfo = app_info;
      // mac os �ɑΉ����邽�߂��̃t���O������
      create_info->flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
      // ==[ �g���@�\ ]==========================
      auto extensions = get_required_extensions( enable_validationlayers );
      if ( !check_extension_support( extensions ) )
         throw std::runtime_error( "extensions requested, but not available!" );

      create_info->enabledExtensionCount   = static_cast<uint32_t>( extensions.size() );
      create_info->ppEnabledExtensionNames = extensions.data();
      // ==[ ���؃��C���[ ]================================
      if ( enable_validationlayers )
      {
         if ( !check_validationlayer_support( validationlayers ) )
            throw std::runtime_error( "validation layers requested, but not available!" );

         create_info->enabledLayerCount   = static_cast<uint32_t>( validationlayers.size() );
         create_info->ppEnabledLayerNames = validationlayers.data();
      }
      else { create_info->enabledLayerCount = 0; }
   }

   std::vector<const char*> vulkan::get_required_extensions( const bool enable_validationlayers )
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

   bool vulkan::check_validationlayer_support( std::vector<const char*> validationlayers )
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
   bool vulkan::check_extension_support( std::vector<const char*> extensions )
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

   void vulkan::init() { create_instace(); }
   void vulkan::release()
   {
      vkDestroyInstance( vk_instance, nullptr );
      delete this;
   }
}    // namespace lib::renderer

namespace api::renderer
{
   vulkan* create() { return new lib::renderer::vulkan(); }
}    // namespace api::renderer