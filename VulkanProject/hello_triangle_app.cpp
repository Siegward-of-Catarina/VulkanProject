#include "hello_triangle_app.hpp"

#include "lib/glwindow.hpp"
#include "lib/renderer.hpp"

namespace my_app
{
   void
   hello_triangle_app::init()
   {
      window->init();
      renderer->init();
   }

   void
   hello_triangle_app::main_loop()
   {
      while ( !window->close() ) { window->pool_event(); }
   }

   void
   hello_triangle_app::cleanup()
   {
      renderer->release();
      window->release();
   }

   hello_triangle_app::hello_triangle_app()
     : app {}
     , window { my_library::window::glwindow::create( 800, 600, "vulkan" ) }
     , renderer { my_library::renderer::create() }
   {}

   hello_triangle_app::~hello_triangle_app() {}
}    // namespace my_app
