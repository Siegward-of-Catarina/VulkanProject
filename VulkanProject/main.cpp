
#include "helloTriangleApp.hpp"
//#include "lib/shader/hlslCompiler.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#ifdef _CONSOLE
int
main(){
   my_app::HelloTriangleApp app;
   try
   {
      //my_library::shader::HLSLCompiler cv {};
      //cv.compileToSPIR_V( "./shaderSrc/test.hlsl", my_library::shader::Type::PIXEL, "./shaderSrc/spir_v/" );
      app.run();
   } catch ( const std::exception& e )
   {
      std::cerr << e.what() << std::endl;    // cerr 標準エラー出力に対する出力ストリーム
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}
#else
int
WinMain()
{
   my_app::HelloTriangleApp app;
   try
   {
      // my_library::shader::HLSLCompiler cv {};
      // cv.compileToSPIR_V( "./shaderSrc/test.hlsl", my_library::shader::Type::PIXEL, "./shaderSrc/spir_v/" );

      app.run();
   } catch ( const std::exception& e )
   {
      std::cerr << e.what() << std::endl;    // cerr 標準エラー出力に対する出力ストリーム
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}
#endif