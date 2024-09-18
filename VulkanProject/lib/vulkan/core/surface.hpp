#pragma once
#include "core_commonheaders.hpp"
namespace my_library::window
{
   class GLwindowForVk;
}
namespace my_library::vkm
{
   class Surface
   {
   public:
      void
      create( const UniqueInstance& instance );

      const UniqueSurface& vkobj;
      Surface( const std::unique_ptr<my_library::window::GLwindowForVk>& window );
      ~Surface();
   private:
      const std::unique_ptr<my_library::window::GLwindowForVk>& _window;
      UniqueSurface                                             _surface;
   };
}    // namespace my_library::vulkan
