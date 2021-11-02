
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>



struct Player
{
  int x, y;
};



int main (int argc, char *argv[])
{
  bool run = TRUE;
  int max_y = 0, max_x = 0;
  int k;
  
  
  initscr();
  // Global var `stdscr` is created by the call to `initscr()`
  getmaxyx(stdscr, max_y, max_x);
  noecho();
  curs_set(FALSE);
  
  struct Player player = {max_x/2, max_y - 3};
  bool player_a = false;
  bool player_n = false;
  bool player_t = false;
  
  while(run)
  {
    // Drawing base layer
    for( int y = 0; y < max_y; y++ )
    {
      for( int x = 0; x < max_x; x++ )
      {
        mvprintw(y, x, ".");
      }
    }
    
    // Drawing player
    mvprintw(player.y, player.x,   " \\()/ ");
    mvprintw(player.y+1, player.x, " -<>- ");
    mvprintw(player.y+2, player.x, " /( )\\");
    // Refresh drawing
    refresh();
    
    k = getch();
    
    if( k == 'Q' )
    {
      run = FALSE;
    }
    else if( k == 'a' )
    {
      player_a = true;
    }
    else if( k == 'n' )
    {
      if( player_a )
      {
        player_n = true;
      }
    }
    else if( k == 't' )
    {
      if( player_a && player_n )
      {
        player_t = true;
      }
    }


    if( player_a && player_n && player_t )
    {
      player.y -= 1;
      player_a = false;
      player_n = false;
      player_t = false;
    }
  }
  
  endwin();
  
  return 0;
}
