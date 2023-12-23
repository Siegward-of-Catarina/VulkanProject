#pragma once
namespace core
{
   class app
   {
   protected:
      app() {};
      ~app() {};
      virtual void
      init() = 0;
      virtual void
      main_loop() = 0;
      virtual void
      cleanup() = 0;

   public:
      virtual void
      run();
   };
}    // namespace core
