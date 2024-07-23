#pragma once
#include "vk_object_types.hpp"
struct GLFWwindow;
namespace my_library::vulkan
{
   class surface
   {
   private:

   public:
      void
      init(const unq_vk_instance& instance,
            GLFWwindow* window);
      unq_vk_surface&
      vk_obj();
      surface();
      ~surface();
   private:
      unq_vk_surface _surface;
   };
}
