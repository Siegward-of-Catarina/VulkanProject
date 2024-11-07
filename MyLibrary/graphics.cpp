#include "graphics.hpp"
#include "utilities.hpp"

const my_library::vkm::vkDevice
my_library::Graphics::getDevice()
{
   utl::runtimeError( "no override." );
   return vkm::vkDevice();
}

my_library::Graphics::Graphics() {}

my_library::Graphics::~Graphics() {}
