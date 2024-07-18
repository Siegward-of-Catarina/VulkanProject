#include "hello_triangle_app.hpp"

#include "lib/glwindow.hpp"
#include "lib/renderer.hpp"

namespace my_app
{
   void
   hello_triangle_app::init()
   {
      _window->init();
      _renderer->init( _window->raw_glfwW_ptr() );
   }

   void
   hello_triangle_app::main_loop()
   {
      while ( !_window->close() ) { _window->pool_event(); }
   }

   void
   hello_triangle_app::cleanup()
   {
   }

   hello_triangle_app::hello_triangle_app()
     : app {}
     , _window { std::make_unique<my_library::window::glwindow>( 800, 600, "vulkan" ) }
     , _renderer { std::make_unique<my_library::renderer>() }
   {}

   hello_triangle_app::~hello_triangle_app() {}

}    // namespace my_app
