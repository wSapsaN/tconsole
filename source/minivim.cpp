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


bool Minivim::cursor(int &ch)
{
  switch (ch)
  {
  case KEY_DOWN:
    if (x == mx) return 1;
    move(++x, y);
    return 1;
  
  case KEY_UP:
    if (x == 0) return 1;
    move(--x, y);
    return 1;

  case KEY_LEFT:
    if (y == 0) return 1;
    move(x, --y);
    return 1;

  case KEY_RIGHT:
    if (y == my) return 1;
    move(x, ++y);
    return 1;

  case 'q':
    flag_exit = 0;
    return 1;
  }

  return 0;
}

void Minivim::read_file()
{
  std::string tmp_l;
  std::ifstream file(filename);
  if (file.is_open())
  {
    while (getline(file, tmp_l))
    {
      line.push_back(tmp_l);
    }
  }
}

void Minivim::insert(int &ch)
{
  std::string tmp;
  for (size_t i = y; i < line[x].size(); i++)
  {
    tmp += line[x][i];
  }
  

  line[x][y] = ch;
  line.resize(line.size() + 1);

  for (size_t i = 0, s = y; i < tmp.size(); i++, s++)
  {
    line[x][s+1] = tmp[i];
  }
  

  mvprintw(x, y, &line[x][y]);
  y++;
  move(x,y);

}

bool Minivim::purge(int &ch)
{
  bool bs = ch == KEY_BACKSPACE;
  if (bs && (y > 0 && y < my))
  {
    std::string tmp;
    for (size_t i = y; i < line[x].size(); i++)
    {
      tmp += line[x][i];
    }
    
    for (size_t i = 0, s = y; i < tmp.size(); i++, s++)
    {
      line[x][s-1] = tmp[i];
    }

    mvprintw(0, 0, line[x].c_str());
    y--;
    move(x,y);

    return 1;
  } else if (bs) { return 1; }

  return 0;
}

void Minivim::outputfile()
{
  for (size_t i = 0; i < line.size(); i++)
  {
    mvprintw(i, 0, line[i].c_str());
  }

  move(y, x);
}

void Minivim::run()
{
  if (filename != "")
  {
    read_file();
    outputfile();
  }

  while (flag_exit)
  {
    getmaxyx(stdscr, mx, my);

    int ch = getch();

    if (cursor(ch) || purge(ch)) continue;
    
    insert(ch);
  }
}