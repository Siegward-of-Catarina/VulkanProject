#pragma once
#include "App.h"

class HelloTriangleApp : public App
{
private:
   void init() override;
   void mainLoop() override;
   void cleanup() override;

   public:
   HelloTriangleApp();
   ~HelloTriangleApp();
};
