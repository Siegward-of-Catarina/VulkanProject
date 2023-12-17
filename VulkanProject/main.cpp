
#include "hello_triangle_app.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{
   my_lib::HelloTriangleApp app;
   try
   {
      app.run();
   } catch ( const std::exception& e )
   {
      std::cerr << e.what() << std::endl;//cerr 標準エラー出力に対する出力ストリーム
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}
