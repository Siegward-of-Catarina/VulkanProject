#include "app.hpp"

namespace core
{
   void app::run()
   {
      init();
      main_loop();
      cleanup();
   }
}

