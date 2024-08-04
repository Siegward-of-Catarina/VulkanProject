#pragma once
#include "core_commonheaders.hpp"

namespace my_library::vkm
{
   class Instance
   {
   public:
      void
      init( const instance::CreateInfo& create_info, const bool debug );
      Instance( const shared_ptr_container& c );
      ~Instance();
   private:
      class Impl;
      std::unique_ptr<Impl> _pimpl;
   };

}    // namespace my_library::vulkan
