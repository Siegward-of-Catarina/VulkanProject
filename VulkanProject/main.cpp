
#include "hello_triangle_app.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{
   my_app::hello_triangle_app app;
   try
   {
      app.run();
   } catch ( const std::exception& e )
   {
      std::cerr << e.what() << std::endl;//cerr �W���G���[�o�͂ɑ΂���o�̓X�g���[��
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}
