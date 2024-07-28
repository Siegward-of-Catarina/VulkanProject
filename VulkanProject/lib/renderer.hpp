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
      init( GLFWwindow* window, const bool debug = false );

      renderer();
      ~renderer();

   private:
      std::unique_ptr<vulkan::vulkan> _vulkan;
   };
}    // namespace my_library
