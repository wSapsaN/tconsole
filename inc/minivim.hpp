#pragma once

#include <string>
#include <ncurses.h>
#include <memory>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

#define ERROR_CODE   -1
#define SUCCSSE_CODE  0
#define TRANSFER_CODE 1

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
  void full_render(unsigned int start_postition = 0);

  int  insert(int&);
  bool purge(int&);

  // TODO: 
  // void save_buff();

  void create_space();
  void resize_space();

  void enter_event();
  void backspace_event(int);

  void render_line();
};