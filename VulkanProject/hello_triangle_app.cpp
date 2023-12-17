#include "hello_triangle_app.hpp"

#include "api/window.hpp"

namespace my_lib
{
   void HelloTriangleApp::init() { window->init(); }

   void HelloTriangleApp::main_loop()
   {
      while ( !window->close() ) { window->pool_event(); }
   }

   void HelloTriangleApp::cleanup() { window->release(); }

   HelloTriangleApp::HelloTriangleApp() : App {}, window {nullptr } { 
        window = api::window::create( 800, 600, "Vulkan" );
   }

   HelloTriangleApp::~HelloTriangleApp() {}
}
