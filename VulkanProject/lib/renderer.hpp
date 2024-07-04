#pragma once
#include "create_helper.hpp"

namespace my_library
{
   namespace vulkan
   {
      class vulkan;
   }
   namespace window
   {
      class glwindow;
   }
   class renderer
   {
   private:
      renderer();
      ~renderer();
      CREATE_HELPER;
      using create_helper = Impl<renderer>;

   private:
      vulkan::vulkan*   vulkan;

   public:
      static renderer*
      create( );

   public:
      void
      init();
      void
      release();
   };
}    // namespace my_library
