#pragma once

#include <string>
#include <ncurses.h>
#include <memory>
#include <vector>


class Minivim
{
public:
  Minivim(const std::string&);
  ~Minivim();
  void run();

protected:
  std::string filename;

  void update();
  void statusline();
  void input(int);

private:
  std::vector<std::string> lines;

  int x, y;
  char mode;
  std::string status;

};