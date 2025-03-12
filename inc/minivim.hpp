#pragma once

#include <string>
#include <ncurses.h>
#include <memory>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

class Minivim
{
public:
  Minivim(const std::string&);
  Minivim();
  ~Minivim();
  
  void run();

protected:
  std::string filename;

private:
  std::vector<std::string> line;

  bool flag_exit = 1;
  int x = 0, y = 0, my, mx;

  void cursor();
  void read_file();
  void outputfile();

};