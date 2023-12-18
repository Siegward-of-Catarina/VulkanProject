#include "hello_triangle_app.hpp"

#include "api/window.hpp"
#include "api/vulkan.hpp"

namespace my_app
{
   void hello_triangle_app::init() { window->init();
      renderer->init();
   }

   void hello_triangle_app::main_loop()
   {
      while ( !window->close() ) { window->pool_event(); }
   }

   void hello_triangle_app::cleanup() { window->release(); }

   hello_triangle_app::hello_triangle_app() : app {}, window { nullptr }, renderer { nullptr }
   {
      window = api::window::create( 800, 600, "Vulkan" );
      renderer = api::renderer::create();
   }

   hello_triangle_app::~hello_triangle_app() {}
}    // namespace my_app
