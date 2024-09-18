#include "helloTriangleApp.hpp"

#include "lib/enumExtensions.hpp"
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

      using DebugSeverityFlagBits    = my_library::renderer::DebugSeverityFlagBits;
      using DebugMessageTypeFlagBits = my_library::renderer::DebugMessageTypeFlagBits;
      using DebugSeverityFlags       = my_library::enumExtensions::BitFlag<DebugSeverityFlagBits>;
      using DebugMessageTypeFlags    = my_library::enumExtensions::BitFlag<DebugMessageTypeFlagBits>;

      DebugSeverityFlags debug_severity_flags {
         {DebugSeverityFlagBits::Error, DebugSeverityFlagBits::Warning, DebugSeverityFlagBits::Verbose}
      };
      DebugMessageTypeFlags debug_msg_flags {
         {DebugMessageTypeFlagBits::General,
          DebugMessageTypeFlagBits::Performance,
          DebugMessageTypeFlagBits::Validation}
      };
      my_library::renderer::CreateInfo create_info { .gl_window { _window },
                                                     .app_name { "vulkan tutorial" },
                                                     .enable_debug { debug },
                                                     .debug_severity_flags { debug_severity_flags },
                                                     .debug_msg_type_flags { debug_msg_flags } };
      _renderer->init( create_info );
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
