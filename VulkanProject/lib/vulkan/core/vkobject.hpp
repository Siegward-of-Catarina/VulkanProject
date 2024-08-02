#pragma once
namespace my_library
{
   namespace vulkan
   {
      class vkobject
      {
      public:
         vkobject( const std::shared_ptr<class container>& c );
         virtual ~vkobject();
      protected:
         const std::shared_ptr<class container> ctr_lock();
      private:
         const std::weak_ptr<class container> _ctr;
      };

      using shared_container = std::shared_ptr<container>;
   }    // namespace vulkan
}    // namespace my_library
