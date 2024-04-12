#include "vulkan.hpp"

#include "glwindow.hpp"

#include <GLFW/glfw3.h>    //�g���@�\���擾���邽�߂ɕK�v
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

namespace
{
   const std::uint32_t            WIDTH  = 800;
   const std::uint32_t            HEIGHT = 600;
   const std::vector<const char*> validationlayers = { "VK_LAYER_KHRONOS_validation" };
#ifdef NDEBUG
   const bool enable_validationlayers = false;
#else
   const bool enable_validationlayers = true;
#endif         // NDEBUG

   VkResult    // VkDebugUtilsMessengerEXT���쐬����֐���T���o��
   create_debug_utils_messenger_ext( VkInstance                                instance,
                                     const VkDebugUtilsMessengerCreateInfoEXT* create_info_ptr,
                                     const VkAllocationCallbacks*              allocator_ptr,
                                     VkDebugUtilsMessengerEXT*                 debug_messenger_ptr )
   {
      auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" ) );
      if ( func != nullptr ) { return func( instance, create_info_ptr, allocator_ptr, debug_messenger_ptr ); }
      else { return VK_ERROR_EXTENSION_NOT_PRESENT; }
   }

   void    // VkDebugUtilsMessengerEXT���������֐���T���o��
   destroy_debug_utils_messenger_ext( VkInstance                   instance,
                                      VkDebugUtilsMessengerEXT     debug_messenger,
                                      const VkAllocationCallbacks* allocator_ptr )
   {
      auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT" ) );
      if ( func != nullptr ) { func( instance, debug_messenger, allocator_ptr ); }
   }

}    // namespace

namespace my_library
{
   // vulkan::core
   namespace vulkan
   {
      vulkan::vulkan()
        : vk_instance { VK_NULL_HANDLE }, vk_debug_messenger { VK_NULL_HANDLE }, vk_physical_device { VK_NULL_HANDLE }
      {}
      /*
       VkDebugUtilsMessageSeverityFlagBitsEXT :�@���b�Z�[�W�̏d�v�x
       VkDebugUtilsMessageTypeFlagsEXT : ���b�Z�[�W�̎��
       const VkDebugUtilsMessageCallbackDataEXT* : ���b�Z�[�W�̏ڍׂ��܂ލ\���́ApCallbackData���Q�ƁB
      */
      VKAPI_ATTR VkBool32 VKAPI_CALL
      vulkan::debag_callBack( VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                            VkDebugUtilsMessageTypeFlagsEXT             message_type,
                            const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
                            void*                                       user_data_ptr )
      {
         std::cerr << "validation layer:" << callback_data_ptr->pMessage << std::endl;
         return VK_FALSE;
      }

      vulkan*
      vulkan::create()
      {
         return new create_helper();
      }

      void
      vulkan::create_instace()
      {
         if ( enable_validationlayers
              && !check_validationlayer_support( validationlayers ) )    // �f�o�b�O���̂ݗL���j�X��
         {
            throw std::runtime_error( "validation layers requested, but not available!" );
         }

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

         // �C���X�^���X�쐬�̏ڍאݒ�
         VkInstanceCreateInfo create_info {};
         create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
         create_info.pApplicationInfo = &app_info;
         // mac os �ɑΉ����邽�߂��̃t���O������
         create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

         // ==[ �g���@�\ ]==========================
         std::vector<const char*> extensions { get_required_extensions( enable_validationlayers ) };
         if ( !check_extension_support( extensions ) )
         {
            throw std::runtime_error( "extensions requested, but not available!" );
         }
         create_info.enabledExtensionCount   = static_cast<uint32_t>( extensions.size() );
         create_info.ppEnabledExtensionNames = extensions.data();

         // ==[ ���؃��C���[ ]===========================
         // vkCreateInctance, vkDestroyInstance�̓����C�x���g�ɑΉ����邽�߂������debugMessenger��p�ӂ���B
         VkDebugUtilsMessengerCreateInfoEXT debug_create_info {};
         if ( enable_validationlayers )    // �f�o�b�O���̂ݗL���j�X��
         {
            create_info.enabledLayerCount   = static_cast<uint32_t>( validationlayers.size() );
            create_info.ppEnabledLayerNames = validationlayers.data();
            populate_debug_messenger_create_info( debug_create_info );
            // ������pNext��debug_create_info���w�肷�邱�Ƃ�create_info���g������f�o�b�O�ł���悤�ɂȂ�B
            create_info.pNext = reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>( &debug_create_info );
         }
         else
         {
            create_info.enabledLayerCount = 0;
            create_info.pNext             = nullptr;
         }

         // ==[ �悤�₭���� ]=============================
         if ( vkCreateInstance( &create_info, nullptr, &vk_instance ) != VK_SUCCESS )
         {
            throw std::runtime_error( "failed to create instance!" );
         }
      }
      void
      vulkan::setup_debug_messenger()
      {
         if ( !enable_validationlayers ) return;

         VkDebugUtilsMessengerCreateInfoEXT create_info {};
         populate_debug_messenger_create_info( create_info );

         if ( create_debug_utils_messenger_ext( vk_instance, &create_info, nullptr, &vk_debug_messenger )
              != VK_SUCCESS )
         {
            throw std::runtime_error( "failed to set up debug messenger!" );
         }
      }

      void
      vulkan::pick_up_physical_device()
      {
         // GPU�̌����܂��擾
         uint32_t device_count = 0;
         vkEnumeratePhysicalDevices( vk_instance, &device_count, nullptr );
         if ( device_count == 0 ) { throw std::runtime_error( "failed to find GPUs with Vulkan support!" ); }
         // GPU�̃��X�g���擾
         std::vector<VkPhysicalDevice> devices( device_count );
         vkEnumeratePhysicalDevices( vk_instance, &device_count, devices.data() );

         std::multimap<int, VkPhysicalDevice> candidates;

         // GPU�̃��X�g����K�؂ȃf�o�C�X���擾
         for ( const auto& device : devices )
         {
            int score = rate_device_suitability( device );
            candidates.insert( std::make_pair( score, device ) );

            if ( is_device_suitable( device ) )
            {
               vk_physical_device = device;
               break;
            }
         }
         // rbegin �����@-> �擪�@�̋t�C�e���[�^
         if ( candidates.rbegin()->first > 0 ) { vk_physical_device = candidates.rbegin()->second; }
         else { throw std::runtime_error( "failed to find a suitable GPU!" ); }
      }

      void
      vulkan::create_logical_device()
      {
         // �_���f�o�C�X���쐬���邽�߂ɃL���[�̏ڍׂ����߂�
         queue_family_indices indices =
           find_queue_familie( vk_physical_device );    // ���̓O���t�B�b�N�@�\��������L���[������΂���
         float queue_priority = 1.0f;    // �L���[�ɑ΂��ėD��x�����邽�߂̒l //����L���[��1�����A�K�v
         VkDeviceQueueCreateInfo queue_create_info {};
         queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
         queue_create_info.queueFamilyIndex = indices.graphics_family.value();
         queue_create_info.queueCount       = 1;
         queue_create_info.pQueuePriorities = &queue_priority;

         // �f�o�C�X�̋@�\�Z�b�g���w�肷��
         VkPhysicalDeviceFeatures device_features {};

         // �_���f�o�C�X�̏ڍׂ�ݒ�
         VkDeviceCreateInfo create_info {};
         create_info.sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
         create_info.pQueueCreateInfos     = &queue_create_info;
         create_info.queueCreateInfoCount  = queue_create_info.queueCount;
         create_info.pEnabledFeatures      = &device_features;
         create_info.enabledExtensionCount = 0;
         // �f�o�C�X�ŗL�̊g���@�\���Ή����Ă��邩�`�F�b�N����

         // �f�o�b�O���̂݌��؃��C���[��L����
         if ( enable_validationlayers )
         {
            create_info.enabledLayerCount   = static_cast<uint32_t>( validationlayers.size() );
            create_info.ppEnabledLayerNames = validationlayers.data();
         }
         else { create_info.enabledLayerCount = 0; }

         // �_���f�o�C�X���쐬
         if ( vkCreateDevice( vk_physical_device, &create_info, nullptr, &vk_device ) != VK_SUCCESS )
         {
            throw std::runtime_error( "faied to create logical device!" );
         }

         // �L���[���n���h���Ɏ擾
         vkGetDeviceQueue( vk_device, indices.graphics_family.value(), 0, &vk_graphics_queue );
      }

      std::vector<const char*>
      vulkan::get_required_extensions( const bool enable_validationlayers )
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
      vulkan::check_validationlayer_support( const std::vector<const char*>& validationlayers )
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
      bool
      vulkan::check_extension_support( const std::vector<const char*>& extensions )
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
      vulkan::is_device_suitable( VkPhysicalDevice device )
      {
         queue_family_indices indices = find_queue_familie( device );

         return indices.isComplete();
      }
      int
      vulkan::rate_device_suitability( VkPhysicalDevice device )
      {
         int score = 0;
         // �f�o�C�X�̊�{�����擾
         VkPhysicalDeviceProperties device_properties;
         vkGetPhysicalDeviceProperties( device, &device_properties );
         // �f�o�C�X�̃I�v�V���������擾
         VkPhysicalDeviceFeatures device_features;
         vkGetPhysicalDeviceFeatures( device, &device_features );

         // �ʂ�GPU���ς܂�Ă��邩�ǂ���(CPU�����O���t�B�b�N�łȂ�)
         if ( device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ) { score += 1000; }

         // �ő�摜�T�C�Y
         score += device_properties.limits.maxImageDimension2D;

         // �W�I���g���V�F�[�_�[�ւ̑Ή��͕K�{�Ƃ���
         if ( !device_features.geometryShader ) { return 0; }
         if ( !is_device_suitable( device ) ) { return 0; }

         return score;
      }
      queue_family_indices
      vulkan::find_queue_familie( VkPhysicalDevice device )
      {
         queue_family_indices indices;

         // �L���[�t�@�~���[�̃��X�g���擾
         uint32_t family_count = 0;
         vkGetPhysicalDeviceQueueFamilyProperties( device, &family_count, nullptr );

         std::vector<VkQueueFamilyProperties> queue_families( family_count );
         vkGetPhysicalDeviceQueueFamilyProperties( device, &family_count, queue_families.data() );

         int i = 0;
         for ( const auto& queue_family : queue_families )
         {
            if ( queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT ) { indices.graphics_family = i; }
            if ( indices.isComplete() ) { break; }
            i++;
         }
         return indices;
      }
      void
      vulkan::populate_debug_messenger_create_info( VkDebugUtilsMessengerCreateInfoEXT& create_info )
      {
         create_info                 = {};
         create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
         create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT    // ���ڍׂȏ��
                                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT    // �x��
                                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;     // �G���[
         create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    // �������񂷂ׂĂ̎�ނ�\��
                                 | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
         create_info.pfnUserCallback = debag_callBack;
         create_info.pUserData       = nullptr;
      }
      void
      vulkan::init( const my_library::window::glwindow& window )
      {
         create_instace();
         setup_debug_messenger();
         pick_up_physical_device();
      }
      void
      vulkan::release()
      {
         //vkDestroyDevice( vk_device, nullptr );    // �_���f�o�C�X
         if ( enable_validationlayers )            // �f�o�b�O���b�Z���W���[
         {
            destroy_debug_utils_messenger_ext( vk_instance, vk_debug_messenger, nullptr );
         }
         vkDestroyInstance( vk_instance, nullptr );    // Vulkan�C���X�^���X
         delete this;                                  // �f�X�g���N�^
      }
   }                                                   // namespace vulkan
   // vulkan::queue_family
   namespace vulkan
   {
      bool
      queue_family_indices::isComplete()
      {
         return graphics_family.has_value();
      }
   }    // namespace vulkan

}    // namespace lib::renderer

