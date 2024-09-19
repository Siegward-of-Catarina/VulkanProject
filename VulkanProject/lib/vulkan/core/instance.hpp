#pragma once
#include "../debugUtils.hpp"
#include "core_commonheaders.hpp"

namespace my_library::vkm
{
   namespace instance
   {
      struct CreateInfo
      {
         std::string                      app_name;
         DebugUtilsMessengerCreateInfoEXT debug_info;
         std::vector<const char*>         extensions;
      };
   }    // namespace instance

   class Instance
   {
   public:
      void
      create( const instance::CreateInfo& create_info, const bool debug );
      const std::vector<PhysicalDevice>
      enumeratePhysicalDevices();

   public:
      const UniqueInstance&        vkobj;
      const DispatchLoaderDynamic& dld;
      Instance();
      ~Instance();

   private:
      UniqueInstance                                    _instance;
      UniqueDebugUtilsMessengerEXT                      _dbg_messenger;
      DispatchLoaderDynamic                             _dld;
      std::vector<std::pair<PhysicalDevice, uint16_t> > _devices;
   };

}    // namespace my_library::vkm
