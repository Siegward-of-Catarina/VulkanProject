#pragma once
#include "create_helper.hpp"

namespace my_library
{
   namespace window
   {
      class glwindow;
   }
   namespace vulkan
   {
      class vulkan;
   }
   class renderer
   {
   private:
      renderer() = delete;
      renderer( const std::uint32_t width, const std::uint32_t height, const char* title );
      ~renderer();
      using create_helper = helper::Impl<renderer>;
      friend create_helper;

   private:
      window::glwindow* window;
      vulkan::vulkan*   vulkan;

   public:
      static renderer*
      create( const std::uint32_t width, const std::uint32_t height, const char* title );

   public:
      void
      init();
      void
      release();
   };
}    // namespace my_library
