// #include "inc/minivim.hpp"
#include "minivim.hpp"

int main(int argc, char **argv)
{
  if (argc > 1)
  {
    auto minivim = std::make_shared<Minivim>(argv[1]);
    minivim->run();
  } else {
    auto minivim = std::make_shared<Minivim>();
    minivim->run();
  }

  return 0;
}