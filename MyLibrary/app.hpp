#pragma once
namespace core
{
   class App
   {
   protected:
      App() {};
      ~App() {};
      virtual void
      init() = 0;
      virtual void
      mainLoop() = 0;
      virtual void
      cleanup() = 0;

   public:
      virtual void
      run();
   };
}    // namespace core
