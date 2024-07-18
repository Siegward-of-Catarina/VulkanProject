#pragma once

#include <memory>

struct GLFWwindow;

namespace my_library
{
   namespace vulkan
   {
      class vulkan;
   }

   class renderer
   {
   public:
      void
      init( const GLFWwindow* window );

      renderer();
      ~renderer();

   private:
      std::unique_ptr<vulkan::vulkan> _vulkan;
   };
}    // namespace my_library
