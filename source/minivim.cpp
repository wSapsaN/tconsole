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
    if (x == mx || line[x+1][y] == '\0' || line[x+1][y] == '~') {
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
  getmaxyx(stdscr, mx, my);
  mx--; my--;
  
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

  if (x == 0 && y == 0) {
    line[x][y++] = ch;
    move(x,y);

    return;
  }

  if (y == my)
  {
    y = 0;
    line[++x][y++] = ch;
    move(x,y);

    return;
  }
  

  line[x].push_back(ch);

  y++;
  move(x,y);

}

bool Minivim::purge(int &ch)
{

  bool bs = ch == KEY_BACKSPACE;
  if (bs)
  {

    std::string ltmp;
    ltmp.resize(0);
    if (line[x].size()) ltmp.resize(line[x].size() - 1);

    short int jump = 0;
    for (size_t i = 0; i < ltmp.size(); i++, jump++)
    {
      if (y == 0) break;
      if (jump == y && y != my) {        
        jump++;
      }

      ltmp[i] = line[x][jump];
    }
        
    // mvprintw(mx, my - 10, "%s", tmp.c_str());
    // move(x, y);
    if (!ltmp[0]) ltmp = "";
    line[x] = ltmp;

    clear();
    outputfile();

    // mvprintw(5, 0, "line: %s", line[x].c_str());
    // mvprintw(7, 0, "tmp: %s", ltmp.c_str());
    // mvprintw(6, 0, "line: %ld", line[x].size());
    // mvprintw(8, 0, "tmp: %ld", ltmp.size());
    if (y) move(x, --y);
    
    return 1;
  }

  return 0;
}

void Minivim::outputfile()
{
  for (short int i = 0; i < mx; i++)
  {
    mvprintw(i, 0, "%s", line[i].c_str());
  }
  
  move(x, y);
}

void Minivim::create_space() {
  getmaxyx(stdscr, mx, my);
  mx--; my--;

  char l[(my-1)] = "~";
  for (short int i = 0; i < mx; i++)
  {
    line.push_back(l);
  }

  line[0][0] = ' ';
}

void Minivim::resize_space() {
  // resize line of space to become more  

  if ((short int)line.size() < mx)
  {
    char l[my-1] = "~";
    for (short int i = line.size(); i < mx; i++)
    {
      line.push_back(l);
    }
  }
}

/* void Minivim::render_line()
{
  // clrtoeol();

  mvprintw(x, 0, "%s", line[x].c_str());
  move(x, y);

  refresh();
} */

void Minivim::run()
{
  // run function

  clear();
  
  // check filename
  if (filename != "") {
    read_file();
  } else create_space();
  
  outputfile();
  
  // w and h are needed to check the space
  short int w = mx, h = my;
  
  // main loop
  while (flag_exit) // works until the flag is zero
  {
    getmaxyx(stdscr, mx, my);
    mx--; my--;

    if ((mx != w || my != h) && filename != "")
    {
      read_file();
      outputfile();
      w = mx, h = my;
      
      if (mx < x || my < y) move(mx, my);
    
    } else if (mx != w || my != h) {
      resize_space();
      outputfile();

      if (mx < x || my < y) {
        move(mx, my);
        x = mx, y = my;
      }

      w = mx, h = my;
    }
    
    mvprintw(mx, 0, "x %d - y %d || %d - %d", x, y, mx, my);
    
    refresh();
    move(x, y);
    
    int ch = getch();

    if (cursor(ch) || purge(ch)) {
      // clear();
      // outputfile();
      continue;
    }

    // if (cursor(ch)) continue;
    
    insert(ch);

    // render_line();
    clear();
    outputfile();
  }
}



