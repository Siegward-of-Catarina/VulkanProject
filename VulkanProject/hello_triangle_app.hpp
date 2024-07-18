#pragma once
#include "app.hpp"

#include <memory>
namespace my_library
{
   class renderer;
   namespace window
   {
      class glwindow;
   }
}    // namespace my_library
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

   public:
      hello_triangle_app();
      ~hello_triangle_app();

   private:
      std::unique_ptr<my_library::window::glwindow> _window;
      std::unique_ptr<my_library::renderer>         _renderer;
   };

}    // namespace my_app
