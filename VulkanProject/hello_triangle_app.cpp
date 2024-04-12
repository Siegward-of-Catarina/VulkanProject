#include "hello_triangle_app.hpp"

#include "lib/renderer.hpp"

namespace my_app
{
   void
   hello_triangle_app::init()
   {
      renderer->init();
   }

   void
   hello_triangle_app::main_loop()
   {
      //while ( !window->close() ) { window->pool_event(); }
   }

   void
   hello_triangle_app::cleanup()
   {
      renderer->release();
   }

   hello_triangle_app::hello_triangle_app() : app {}, renderer { nullptr }
   {
      renderer = my_library::renderer::create( 800, 600, "vulkan" );
   }

   hello_triangle_app::~hello_triangle_app() {}
}    // namespace my_app
