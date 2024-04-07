#pragma once
#include "../lib/interface.hpp"
namespace api::renderer
{
   struct vulkan : public lib::base::interface
   {
      virtual void
      init() = 0;
   };
   vulkan*
   create();
   // namespace vulkan

}    // namespace api::renderer