#include "instance.hpp"

#include <GLFW/glfw3.h>    //�g���@�\���擾���邽�߂ɕK�v
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
      create_info.sType             = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      create_info.pApplicationInfo  = &app_info;
      create_info.enabledLayerCount = 0;          // �������񂱂�Őݒ肵�Ă���
      create_info.pNext             = nullptr;    // �������񂱂�Őݒ肵�Ă���
      // mac os �ɑΉ����邽�߂��̃t���O������
      create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

      // ==[ �g���@�\ ]==========================
      extensions::register_for_create_info( create_info );

      // ==[ ���؃��C���[ ]===========================
      if ( !validationlayers.empty() ) debug_messenger->register_for_create_info( create_info, validationlayers );

      // ==[ �悤�₭���� ]=============================
      if ( vkCreateInstance( &create_info, nullptr, &vk_instance ) != VK_SUCCESS )
      {
         throw std::runtime_error( "failed to create instance!" );
      }
   }
}    // namespace my_library