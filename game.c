
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <pthread.h>



struct Player
{
  int x, y;
};



bool run = TRUE;
struct Player player = {0, 0};
bool player_a = false;
bool player_n = false;
bool player_t = false;
bool player_wiggle = false;

pthread_mutex_t lock;



void *keypressThread(void *vargp)
{
  int k;
  
  while(run)
  {
    k = getch();
    
    if( k == 'Q' )
    {
      run = FALSE;
    }
    else if( k == 'a' )
    {
      pthread_mutex_lock(&lock);
      player_a = true;
      pthread_mutex_unlock(&lock);
    }
    else if( k == 'n' )
    {
      if( player_a )
      {
        pthread_mutex_lock(&lock);
        player_n = true;
        pthread_mutex_unlock(&lock);
      }
    }
    else if( k == 't' )
    {
      if( player_a && player_n )
      {
        pthread_mutex_lock(&lock);
        player_t = true;
        pthread_mutex_unlock(&lock);
      }
    }
  }
  
  return NULL;
}



int main (int argc, char *argv[])
{
  int max_y = 0, max_x = 0;
  
  
  initscr();
  // Global var `stdscr` is created by the call to `initscr()`
  getmaxyx(stdscr, max_y, max_x);
  noecho();
  curs_set(FALSE);
  player.x = max_x / 3;
  player.y = max_y - 3;
  
  
  pthread_t thread_id;
  pthread_create(&thread_id, NULL, keypressThread, NULL);
  //pthread_join(thread_id, NULL);
  if( pthread_mutex_init(&lock, NULL) != 0)
  {
    printf("mutex init has failed.\n");
    return 1;
  }
  
  
  while(run)
  {
    if( player_a && player_n && player_t )
    {
      player.y -= 1;
      player_a = false;
      player_n = false;
      player_t = false;
      player_wiggle = !player_wiggle;
    }
  
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
    if( player_wiggle == true )
    {
      mvprintw(player.y+2, player.x, " /( )\\");
    }
    else
    {
      mvprintw(player.y+2, player.x, "/( )\\ ");
    }
    
    // Refresh drawing
    refresh();
  }
  
  pthread_mutex_destroy(&lock);
  endwin();
  
  return 0;
}
