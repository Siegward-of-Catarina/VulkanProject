#include "helloTriangleApp.hpp"

#include "../MyLibrary/enumExtensions.hpp"
#include "../MyLibrary/glwindow.hpp"
#include "../MyLibrary/renderer.hpp"
#include "../MyLibrary/graphics.hpp"

namespace my_app
{
   void
   HelloTriangleApp::init()
   {
      _window->init();
      _renderer->init();
   }

   void
   HelloTriangleApp::mainLoop()
   {
      while ( !_window->close() )
      {
         _window->poolEvent();
         _renderer->beginPass();
         _renderer->clearTarget();
         _renderer->draw();
         _renderer->endPass();
         _renderer->present();
      }
      _renderer->waitIdle();
   }

   void
   HelloTriangleApp::cleanup()
   {}

   HelloTriangleApp::HelloTriangleApp()
     : App {}
     , _window { std::make_unique<my_library::window::GLwindow>( 800, 600, "vulkan" ) }
     , _renderer { std::make_unique<my_library::Renderer>( _window, "vulkan tutorial", my_library::renderer::Platform::Vulkan ) }
   {}

   HelloTriangleApp::~HelloTriangleApp() {}

}    // namespace my_app
