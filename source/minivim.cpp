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
  bool state = 0;
  switch (ch)
  {
  case KEY_DOWN:
    state = 1;
    if (x == mx || line[x+1][y] == '\0' || line[x+1][y] == '~') {
      std::cout << "\a";  
      break;
    }
    move(++x, y);
    break;
  
  case KEY_UP:
    state = 1;
    if (x == 0) {
      std::cout << "\a";
      break;
    }
    move(--x, y);
    break;

  case KEY_LEFT:
    state = 1;
    if (y == 0) {
      std::cout << "\a";  
      break;
    }
    move(x, --y);
    break;

  case KEY_RIGHT:
    state = 1;
    if (y == (my-1) || line[x][y] == '\0' || line[x][y+1] == '~') {
      std::cout << "\a";
      break;
    }
    move(x, ++y);
    break;
  }

  return state;
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

  if (y == 0 && x == 0 && (line[x][y] == ' '))
  {
    line[x][y] = ch;

    render_line();
    move(x, ++y);
    
    return;
  }
  
  if ((short int)line[x].size() > y)
  {
    std::string head, tail, tmp;

    for (short int i = 0; i < y; i++) head += line[x][i];
    for (short int i = y; i < (short int)line[x].size(); i++) tail += line[x][i];

    if ((line[x].size() + 1) > my) {
      tmp = tail[tail.size()];
      tail.pop_back();

      if (x < mx && (line[x+1][0] == '~')) line.push_back(tmp);
      if (x == mx && ((short int)line.size() > x))
      {
        line.push_back(tmp);
        full_render(line.size()-(x-1));
      }

      // return code
    }

    head += ch;
    head += tail;

    line[x] = head;

    render_line();
    move(x, ++y);

    return;
  }

  line[x].push_back(ch);

  render_line();
  move(x, ++y);

  return;
}

bool Minivim::purge(int &ch)
{

  bool bs = ch == KEY_BACKSPACE;
  if (bs)
  {

    std::string ltmp = "";
    if (line[x].size()) ltmp.resize(line[x].size() - 1);

    short int jump = 0;
    for (size_t i = 0; i < ltmp.size(); i++, jump++)
    {
      if ((y+1) == 0) return 1;
      if (jump == (y-1) && (y-1) != (my-1)) jump++;

      ltmp[i] = line[x][jump];
    }

    if (!ltmp[0]) ltmp = "";
    line[x] = ltmp;

    if (y) move(x, --y);
    
    return 1;
  }

  return 0;
}

void Minivim::full_render(unsigned int start_postition = 0)
{
  clear();
  for (short int i = (0 + start_postition), lconsole = 0; i < mx; i++, lconsole++)
  {
    mvprintw(lconsole, 0, "%s", line[i].c_str());
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

void Minivim::render_line()
{
  std::string trash;
  for (short int i = 0; i < my; i++) trash += ' ';

  mvprintw(x, 0, "%s", trash.c_str());
  mvprintw(x, 0, "%s", line[x].c_str());
  move(x, y);

  refresh();
}

void Minivim::run()
{
  // run function

  clear();
  
  // check filename
  if (filename != "") {
    read_file();
  } else create_space();
  
  full_render();
  
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
      full_render();
      w = mx, h = my;
      
      if (mx < x || my < y) move(mx, my);
    
    } else if (mx != w || my != h) {
      resize_space();
      full_render();

      if (mx < x || my < y) {
        move(mx, my);
        x = mx, y = my;
      }

      w = mx, h = my;
    }
    
    mvprintw(mx, 0, "x %d - y %d || %d - %d\tsize: %ld   ", x, y, mx, my, line[x].size());
    
    refresh();
    move(x, y);
    
    int ch = getch();
    switch (ch)
    {
    case KEY_BACKSPACE:
      purge(ch);
      render_line();
      break;    
    default:
      if (cursor(ch)) break;
      insert(ch);
      break;
    }
  }
}