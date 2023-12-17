#pragma once
#include "app.hpp"
namespace api::window
{
   struct glwindow;
}
namespace my_lib
{
   class HelloTriangleApp : public App
   {
   private:
      void      init() override;
      void      main_loop() override;
      void      cleanup() override;
      api::window::glwindow* window;

   public:
      HelloTriangleApp();
      ~HelloTriangleApp();
   };

}
