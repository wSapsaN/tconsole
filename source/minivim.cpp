#include "minivim.hpp"

Minivim::Minivim()
: filename("") {
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
    if (x == (mx-1) || line[x+1][y] == '\0' || line[x+1][y] == '~') {
      std::cout << "\a";  
      return 1;
    }
    move(++x, y);
    return 1;
  
  case KEY_UP:
    if (x == 0) {
      std::cout << "\a";  
      return 1;
    }
    move(--x, y);
    return 1;

  case KEY_LEFT:
    if (y == 0) {
      std::cout << "\a";  
      return 1;
    }
    move(x, --y);
    return 1;

  case KEY_RIGHT:
    if (y == (my-1) || line[x][y+1] == '\0' || line[x][y+1] == '~') {
      std::cout << "\a";
      return 1;
    }
    move(x, ++y);
    return 1;

  }

  return 0;
}

void Minivim::read_file()
{
  std::string tmp_l;
  tmp_l.resize(my);

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

  mvprintw(x, 0, "%s", line[x].c_str());
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
    
    size_t s = y;
    for (size_t i = 0; i < tmp.size(); i++, s++)
    {
      line[x][s-1] = tmp[i];
    }

    line[x].resize(line[x].size() - 1);

    // line[x][s-1] = ' ';
    line[x][line[x].size()] = ' ';

    // for (size_t i = (s-1); i < line[x].size(); i++)
    // {
    //   line[x][i] = ' ';
    // }
    

    mvprintw(x, 0, "%s", line[x].c_str());
    y--;
    move(x,y);

    return 1;
  } else if (bs) { return 1; }

  return 0;
}

void Minivim::outputfile()
{
  for (short int i = 0; i < (mx-1); i++)
  {
    mvprintw(i, 0, "%s", line[i].c_str());
  }
  
  move(x, y);
  // mvprintw(1,0, "%s", line[2].c_str());
}

void Minivim::create_space() {
  getmaxyx(stdscr, mx, my);
  char l[(my-1)] = "~";
  for (short int i = 0; i < (mx-1); i++)
  {
    line.push_back(l);
  }

  line[0][0] = ' ';
}

void Minivim::resize_space() {
  if ((short int)line.size() < mx)
  {
    char l[my-1] = "~";
    for (short int i = line.size(); i < mx; i++)
    {
      line.push_back(l);
    }
  }
}

void Minivim::run()
{
  clear();

  short int w = mx, h = my;
  
  if (filename != "")
  {
    read_file();
    outputfile();
  } else {
    create_space();
    outputfile();  
  }
  
  while (flag_exit)
  {
    getmaxyx(stdscr, mx, my);
    
    if ((mx != w || my != h) && filename != "")
    {
      read_file();
      // outputfile();
      w = mx, h = my;
      
      if (mx < x || my < y) move(mx, my);
    
    } else if (mx != w || my != h) {
      // create_space();
      resize_space();
      // outputfile();

      if (mx < x || my < y) {
        move(mx, my);
        x = mx, y = my;
      }

      w = mx, h = my;

    }
    
    mvprintw((mx-1), 0, "%d - %d || %d - %d", x, y, mx, my);
    
    refresh();
    move(x, y);
    
    int ch = getch();

    // if (cursor(ch) || purge(ch)) continue;

    if (cursor(ch)) continue;

    clear();
    outputfile();
    // insert(ch);
  }
}