#pragma once
#include"../vkObjectTypes.hpp"

namespace my_library::vkm
{
   class Instance
   {
   public:
      void
      create( const std::string&                      app_name,
              const std::vector<const char*>&         extensions,
              const std::vector<const char*>&         validationlayers );
      const std::vector<device::Gpu>
      enumeratePhysicalDevices();

   public:
      const instance::UniqueHandle&        vk_instance;
      const DispatchLoaderDynamic& dld;
      Instance();
      ~Instance();

   private:
      instance::UniqueHandle                                    _instance;
      DispatchLoaderDynamic                             _dld;
   };

}    // namespace my_library::vkm
