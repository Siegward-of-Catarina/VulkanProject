#include "renderer.hpp"

#include "glwindow.hpp"
#include "pch.hpp"
#include "vulkan/vkGraphics.hpp"

my_library::Renderer::Renderer( const std::unique_ptr<window::GLwindow>& gl_window,
                                const std::string&                       app_name,
                                const renderer::Platform&                platform )
  : _graphics {}
{
   switch ( platform )
   {
      case renderer::Platform::DirectX : break;
      case renderer::Platform ::Vulkan : _graphics = std::make_unique<vkm::VkGraphics>( gl_window, app_name ); break;
   }
}

my_library::Renderer::~Renderer() {}

void
my_library::Renderer::init()
{
   _graphics->init();
}

void
my_library::Renderer::beginPass()
{
   _graphics->beginPass();
}

void
my_library::Renderer::clearTarget( const float& r, const float& g, const float& b, const float& a )
{
   _graphics->clearTarget( r, g, b, a );
}

void
my_library::Renderer::clearTarget( glm::vec4 color )
{
   _graphics->clearTarget( color.r, color.g, color.b, color.a );
}

void
my_library::Renderer::clearTarget( const std::array<float, 4>& color )
{
   _graphics->clearTarget( color[ 0 ], color[ 1 ], color[ 2 ], color[ 3 ] );
}

void
my_library::Renderer::draw()
{
   _graphics->draw();
}

void
my_library::Renderer::endPass()
{
   _graphics->endPass();
}

void
my_library::Renderer::present()
{
   _graphics->present();
}

void
my_library::Renderer::waitIdle()
{
   _graphics->waitIdle();
}
