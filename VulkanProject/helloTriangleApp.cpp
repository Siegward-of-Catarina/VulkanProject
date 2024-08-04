#include "helloTriangleApp.hpp"

#include "lib/glwindow.hpp"
#include "lib/renderer.hpp"

namespace
{
#ifndef NDEBUG
   constexpr bool debug = true;
#else
   constexpr bool debug = false;
#endif
}    // namespace
namespace my_app
{
   void
   HelloTriangleApp::init()
   {
      _window->init();
      _renderer->init( _window, debug );
   }

   void
   HelloTriangleApp::mainLoop()
   {
      while ( !_window->close() ) { _window->poolEvent(); }
   }

   void
   HelloTriangleApp::cleanup()
   {}

   HelloTriangleApp::HelloTriangleApp()
     : App {}
     , _window { std::make_unique<my_library::window::GLwindow>( 800, 600, "vulkan" ) }
     , _renderer { std::make_unique<my_library::Renderer>() }
   {}

   HelloTriangleApp::~HelloTriangleApp() {}

}    // namespace my_app
