#include "minivim.hpp"

void Minivim::downkey() {
  if (x != mx && line[x+1][0] != '~') {
    int len_str = strlen(line[x+1].c_str());
    if (!len_str) {
      y = 0;
    } else if (len_str < y) {
      y = len_str;
    }
    
    move(++x, y);
  }
}

void Minivim::upkey() {
  if (x) {
    int len_str = strlen(line[x-1].c_str());
    if (len_str) {
      if (len_str < y) y = len_str;
    } else {
      y = 0;
    }

    move(--x, y);
  }
}

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
    downkey();
    break;
  case KEY_UP:
    state = 1;
    upkey();    
    break;

  case KEY_LEFT:
    state = 1;
    if (y == 0) { break; }
    move(x, --y);
    break;

  case KEY_RIGHT:
    state = 1;
    if (y == (my-1) || line[x][y] == '\0' || line[x][y+1] == '~') {
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

int Minivim::insert(int &ch)
{
  if (y >= my) return 0;
  
  if (line[x][y]) {
    std::string tail = "";

    int len = strlen(line[x].c_str());
    for (int i = y, count = 0; i < len; i++, count++) {
      tail += line[x][y+count];
    }

    line[x][y] = ch;
    move(x, y++);
    
    for (int i = y, count = 0; i < len+1; i++, count++) {
      line[x][y+count] = tail[count];
    }
    
    clrtoeol();

    render_line();

    return SUCCSSE_CODE;
  }

  if (y == my-1)
  {
    x++;
    y = 0;
    move(x, y);

    line[x][y] = ch;

    render_line();
    y++;

    return SUCCSSE_CODE;
  }

  line[x][y] = ch;
  y++;
  line[x][y] = '\0';
  
  render_line();

  return SUCCSSE_CODE;
}

bool Minivim::purge(int &ch)
{
  if (ch == KEY_BACKSPACE && y)
  {
    if (line[x][y])
    {
      std::string tail = "";

      int len = strlen(line[x].c_str());
      for (int i = y, count = 0; i < len; i++, count++) {
        tail += line[x][y+count];
      }

      line[x][len-1] = '\0';

      int count = 0;
      for (int i = y; i < len; i++) {
        line[x][(y-1)+count] = tail[count];
        count++;
      }

      move(x, y);
      clrtoeol();

      y--;

      return 1;
    }
    
    y--;
    move(x, y);
    line[x][y] = '\0';

    clrtoeol();

    return 1;
  } else if (x && !y) {
    if (x != mx)
    {
      if (line[x+1][y] == '~' || x < mx) {
        line[x][0] = '~';
        render_line();
      }
    }
    
    x--;
    y = strlen(line[x].c_str());
    
    move(x, y);

    line[x][y] = '\0';
    clrtoeol();
    
    return 1;
  }
  
  return 0;
}

void Minivim::full_render(unsigned int start_postition) {
  
  clear();
  for (short int i = (0 + start_postition), lconsole = 0; i < mx; i++, lconsole++) {
    mvprintw(lconsole, 0, "%s", line[i].c_str());
    // if (line[i][0] != ' ' || !line[i][0])
    // {
    //   line[i][0] = '~';
    // }
    
  }
  
  move(x, y);
}

void Minivim::create_space() {
  getmaxyx(stdscr, mx, my);
  my--;

  line.resize(mx);
  for (short i = 0; i < mx; i++) {
    line[i].resize(my);
  }
  
  for (short int i = 0; i < mx; i++) {
    line[i][0] = '~';
    line[i][1] = '\0';
  }

  line[0][0] = '\0';

}

void Minivim::resize_space() {
  // resize line of space to become more  

  if ((short int)line.size() < mx) {
    for (short int i = line.size(); i < mx; i++) {
      line[i][0] = '~';
    }
  }
}

void Minivim::enter_event() {

  if (line[x+1].c_str() && line[x+1][0] != '~') {
    std::vector<std::string> tail;
    tail.resize(line.size());
    
    int count = 1;
    for (size_t i = x; i < line.size(); i++) {
      if (line[x+count][0] == '~') break;

      tail[count] = line[x + count];
      line[x + count] = '\0';

      count++;
    }
    
    x++;
    y = 0;

    count = 1;
    for (short i = x+5; i < mx; i++) {
      // line[x + count] = '\0';
      
      line[x + count] = tail[count];
      line[x] = "                      ";
      
      // mvprintw(x+1, 0,"%s", tail[count].c_str());
      // move(x,0);
      // mvprintw
      // clrtobot();

      mvprintw(x, 0, "%s", line[x].c_str());
      mvprintw(x+count, 0, "%s", tail[count].c_str());

      count++;
    }

    // count = 1;
    // for (size_t i = 20; i < 25; i++)
    // {
    //   mvprintw(i, 0, "TAIL: %s", tail[count++].c_str());
    // }
        


    // mvprintw(10, 20, "bASE: %s", line[x].c_str());
    
    // line[x+1] = '\0';
    // line[x+1] = tail[1];

    move(x, y);

    // full_render();
    
    return;
  }
  
  if (x >= (mx-1)) return;
  y = 0;
  ++x;
  
  line[x][y] = '\0';

  render_line();
  clrtoeol();

  return;
}

void Minivim::render_line()
{
  mvprintw(x, 0, "%s", line[x].c_str());
  
  refresh();
}

void Minivim::backspace_event(int ch)
{
  purge(ch);
  render_line();
}

void Minivim::tab_event()
{
  int tab_step = 4;
  for (int i = 0; i < tab_step; i++) {
    if (y <= (my-10)) {
      line[x][y+i] = ' ';
    } else {
      return;
    }
  }

  y += tab_step;
  line[x][y] = '\0';
  
  render_line();
}

void Minivim::run()
{
  // run function
  getmaxyx(stdscr, mx, my);
  mx--; my--;

  clear();
  
  // check filename
  if (filename != "") {
    read_file();
  } else create_space();
  
  full_render();
  
  // w and h are needed to check the space
  short int w = mx, h = my;
  x = 0, y = 0;
  
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
    
    mvprintw(mx, 0, "x %d - y %d || mx: %d - my: %d\tsize: %ld", x, 
      y, mx, my, strlen(line[x].c_str())
    );
    clrtoeol();
    
    refresh();
    move(x, y);
    
    int ch = getch();

    switch (ch)
    {
    case KEY_BACKSPACE:
      backspace_event(ch);
      break;
    case '\n':
      enter_event();
      break;
    case KEY_DOWN:
      downkey();
      break;
    case KEY_UP:
      upkey();
      break;
    case '\t':
      tab_event();
      break;
    default:
      if (cursor(ch)) break;
      insert(ch);
      break;
    }
  }
}