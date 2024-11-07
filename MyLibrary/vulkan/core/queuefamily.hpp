#pragma once
#include"../vkObjectTypes.hpp"
namespace my_library::vkm
{
   class Queuefamily
   {
   public:
      static const uint32_t
      graphicsfamily();
      static const uint32_t
      presentfamily();
   public:
      const bool
      is_complete();

      explicit Queuefamily( const device::Gpu&        device,
                            const surface::UniqueHandle&         surface,
                            const DispatchLoaderDynamic& dld );
      Queuefamily();
      ~Queuefamily();
      Queuefamily&
      operator=( const Queuefamily& q );
   private:
   };
}
