#pragma once
namespace lib::base
{
   struct interface
   {
      virtual void
      release() = 0;

   protected:
      virtual ~interface() = default;
   };
}    // namespace lib::base
