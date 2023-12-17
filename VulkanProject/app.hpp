#pragma once
class App
{
protected:
   App() {};
   ~App() {};
   virtual void init()     = 0;
   virtual void main_loop() = 0;
   virtual void cleanup()  = 0;

public:
   virtual void run();
};
