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
  short int x = 0, y = 0, my, mx;

  bool cursor(int&);
  
  void read_file();
  void outputfile();

  void insert(int&);
  bool purge(int&);

  void save_buff();

  void create_space();
  void resize_space();

  void render_line();

};