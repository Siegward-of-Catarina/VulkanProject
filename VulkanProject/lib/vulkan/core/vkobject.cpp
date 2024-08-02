#include "vkobject.hpp"

#include "../../../pch.hpp"

namespace my_library
{
   namespace vulkan
   {
      vkobject::vkobject( const std::shared_ptr<class container>& c ) : _ctr { c } {}

      vkobject::~vkobject() {}

      const std::shared_ptr<class container>
      vkobject::ctr_lock()
      {
         return _ctr.lock();
      }
   }    // namespace vulkan
}    // namespace my_library
