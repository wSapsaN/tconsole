#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <string>

#define PROD

using namespace std;

struct cursor_pos 
{
public:
  int x = 1;
  int y = 1;
};


enum
{
  KEY_UP    = 183,
  KEY_DOWN  = 184,
  KEY_LEFT  = 186,
  KEY_RIGHT = 185
};

int getch(void) {
  struct termios oldt,newt;
  int ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar() + getchar() + getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}

void change_position(cursor_pos& pos) {
  int ch = getch();
  switch (ch) {
    case KEY_UP:    pos.x--; break;
    case KEY_DOWN:  pos.x++; break;
    case KEY_LEFT:  pos.y--; break;
    case KEY_RIGHT: pos.y++; break;
    // default: puts("OTHER");
  }
}

int main(void) {
  
  cursor_pos pos;

  std::string b = "asgfasgksjdhfkjszbhdlkgjbfhlgjzdhbg";
  cout << b;


#ifdef PROD
  while (true)
  {
    cout << "\033[" << pos.x << ";" << pos.y << "H";
    change_position(pos);
  } 
#endif

  return 0;
}
