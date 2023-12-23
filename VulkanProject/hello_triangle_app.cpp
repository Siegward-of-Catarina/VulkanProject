#include "hello_triangle_app.hpp"

#include "api/vulkan.hpp"
#include "api/window.hpp"

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
      // window‚Ì‰ð•ú‚Íˆê”ÔÅŒã
      window->release();
   }

   hello_triangle_app::hello_triangle_app() : app {}, window { nullptr }, renderer { nullptr }
   {
      window   = api::window::create( 800, 600, "Vulkan" );
      renderer = api::renderer::create();
   }

   hello_triangle_app::~hello_triangle_app() {}
}    // namespace my_app
