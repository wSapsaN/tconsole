#include "minivim.hpp"

Minivim::Minivim(const std::string& file)
: filename(file), x(0), y(0), mode('n'), status("NORMAL")
{
  lines.push_back("");

  if (file.empty()) filename = "untitled";

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

void Minivim::run()
{
  while (mode != 'q')
  {
    update();
    statusline();

    int c = getch();
    input(c);
  }
}

void Minivim::update()
{
  switch (mode)
  {
  case 'n':
    status = "NORMAL";
    break;
  case 'i':
    status = "INSERT";
    break;
  case 'q':
    break;
  }
}

void Minivim::statusline()
{
  attron(A_REVERSE);
  mvprintw(LINES - 1, 0, status.c_str());
  attroff(A_REVERSE);  
}

void Minivim::input(int c)
{
  switch (mode)
  {
  case 'n':
    
    switch (c)
    {
    case 'q':
      mode = 'q';
      break;
    
    case 'i':
      mode = 'i';
      break;
    case 'w':
      mode = 'w';
      break;
    }

    break;
  case 'i':
    
    switch (c)
    {
    case 27:
      mode = 'n';
      break;
    
    default:
      std::string s(1, c);
      lines.push_back(s);
      break;
    }

    break;
  }

  for (size_t i {}; i < lines.size(); i++)
  {
    mvprintw(0, i, lines[0].c_str());
  }
  

}