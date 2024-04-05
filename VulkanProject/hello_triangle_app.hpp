#pragma once
#include "app.hpp"
namespace api
{
   namespace window
   {
      struct glwindow;
   }
   namespace renderer
   {
       struct vulkan;
   }

}    // namespace api
namespace my_app
{
   class hello_triangle_app : public core::app
   {
   private:
      void
      init() override;
      void
      main_loop() override;
      void
                             cleanup() override;
      api::window::glwindow* window;
      api::renderer::vulkan* renderer;

   public:
      hello_triangle_app();
      ~hello_triangle_app();
   };

}    // namespace my_app
