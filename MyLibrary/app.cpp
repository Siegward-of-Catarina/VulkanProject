#include "app.hpp"

namespace core
{
   void App::run()
   {
      init();
      mainLoop();
      cleanup();
   }
}

