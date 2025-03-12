#include "minivim.hpp"

Minivim::Minivim()
: filename("") {
  line.push_back("");

  initscr();
  noecho();
  cbreak();
  keypad(stdscr, true);

}

Minivim::Minivim(const std::string& file)
: filename(file), x(0), y(0)
{
  if (!std::filesystem::exists(filename)) {
    std::cout << "Can't open this file" << std::endl;
    exit(-1);
  }

  line.push_back("");
  
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, true);
}

Minivim::~Minivim()
{
  refresh();
  endwin();
}

void Minivim::cursor()
{
  int ch = getch();
  
  switch (ch)
  {
  case KEY_DOWN:
    if (x == mx) break;
    move(++x, y);
    break;
  
  case KEY_UP:
    if (x == 0) break;
    move(--x, y);
    break;

  case KEY_LEFT:
    // if (y == 0) break;
    move(x, --y);
    break;

  case KEY_RIGHT:
    // if (y == my) break;
    move(x, ++y);
    break;

  case 'q':
    flag_exit = 0;
    break;
  }
}

void Minivim::read_file()
{
  std::string tmp_l;
  std::ifstream file(filename);
  if (file.is_open() || filename != "")
  {
    while (getline(file, tmp_l))
    {
      line.push_back(tmp_l);
    }
  }
}

void Minivim::outputfile()
{
  for (size_t i = 1; i < line.size(); i++)
  {
    mvprintw(i-1, 0, line[i].c_str());
  }

  move(y, x);
}

void Minivim::run()
{
  read_file();
  outputfile();

  while (flag_exit)
  {
    getmaxyx(stdscr, my, mx);
    cursor();
  }
}