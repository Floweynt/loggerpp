# loggerpp
C++ logging system  
Simple configurable C++ logging system
# Features
Logging things!
```cpp
#include <logger.h>

int main()
{
  logger::get_instance().add_transport<cout_transporter>(logger::INFO, true);
  logging::logger::get_instance().info("Foo");
}
```
This is a pretty simple logging setup, and will print "Foo" to stdout. See [here](https://github.com/FloweyTheFlower420/loggerpp/wiki/Advanced-Usage) for details on what is going on.
