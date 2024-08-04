#pragma once
#include "core_commonheaders.hpp"
struct GLFWwindow;
namespace my_library::vulkan
{
   class surface
   {
   private:

   public:
      void
      init( GLFWwindow* window );
      const unq_vk_surface&
      vk_obj();
      surface( const shared_container& c );
      ~surface();
   };
}    // namespace my_library::vulkan
