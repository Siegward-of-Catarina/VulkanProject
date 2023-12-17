#include "app.hpp"

void App::run()
{
   init();
   main_loop();
   cleanup();
}
