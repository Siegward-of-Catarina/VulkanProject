#pragma once
#include "app.hpp"
namespace my_library
{
   class renderer;
}
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
      my_library::renderer* renderer;

   public:
      hello_triangle_app();
      ~hello_triangle_app();
   };

}    // namespace my_app
