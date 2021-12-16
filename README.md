# loggerpp
C++ logging system, header only (with overused inline specifiers!)
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
This is a pretty simple logging setup, and will print "Foo" to stdout. See [https://github.com/FloweyTheFlower420/loggerpp/wiki/Advanced-Usage](here) for details on what is going on.
