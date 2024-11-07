#pragma once
#include "../MyLibrary/app.hpp"
#include <memory>

namespace my_library
{
   class Renderer;
   namespace window
   {
      class GLwindow;
   }
}    // namespace my_library
namespace my_app
{
   class HelloTriangleApp : public core::App
   {
   private:
      void
      init() override;
      void
      mainLoop() override;
      void
      cleanup() override;

   public:
      HelloTriangleApp();
      ~HelloTriangleApp();

   private:
      std::unique_ptr<my_library::window::GLwindow> _window;
      std::unique_ptr<my_library::Renderer>         _renderer;
   };

}    // namespace my_app
