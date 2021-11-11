
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>




/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
  struct timespec ts;
  int res;

  if (msec < 0)
  {
    errno = EINVAL;
    return -1;
  }

  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;

  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);

  return res;
}



struct Player
{
  int x, y;
};

struct Opponent
{
  int x, y;
};



bool run = TRUE;
struct Player player = {0, 0};
bool player_b = false;
bool player_u = false;
bool player_g = false;
bool player_wiggle = false;
struct Opponent opponent = {0, 0};

pthread_mutex_t lock;



void *keypressThread(void *vargp)
{
  int k;
  
  while(run)
  {
    // Key handler
    k = getch();
    
    if( k == 'Q' )
    {
      run = FALSE;
    }
    else if( k == 'b' )
    {
      pthread_mutex_lock(&lock);
      player_b = true;
      pthread_mutex_unlock(&lock);
    }
    else if( k == 'u' )
    {
      if( player_b )
      {
        pthread_mutex_lock(&lock);
        player_u = true;
        pthread_mutex_unlock(&lock);
      }
    }
    else if( k == 'g' )
    {
      if( player_b && player_u )
      {
        pthread_mutex_lock(&lock);
        player_g = true;
        pthread_mutex_unlock(&lock);
      }
    }
    
    // Player movement
    if( player_b && player_u && player_g )
    {
      pthread_mutex_lock(&lock);
      player.y -= 1;
      player_b = false;
      player_u = false;
      player_g = false;
      player_wiggle = !player_wiggle;
      pthread_mutex_unlock(&lock);
      
      // Re-drawing player
      mvprintw(player.y, player.x,   ".\\()/.");
      mvprintw(player.y+1, player.x, ".-<>-.");
      if( player_wiggle == true )
      {
        mvprintw(player.y+2, player.x, "./(_)\\");
      }
      else
      {
        mvprintw(player.y+2, player.x, "/(_)\\.");
      }
      mvprintw(player.y+3, player.x, "......");
      
      refresh();
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
  opponent.x = 2*max_x / 3;
  opponent.y = max_y - 3;
  
  
  pthread_t thread_id;
  pthread_create(&thread_id, NULL, keypressThread, NULL);
  //pthread_join(thread_id, NULL);
  if( pthread_mutex_init(&lock, NULL) != 0)
  {
    printf("mutex init has failed.\n");
    return 1;
  }
  
  
  // Drawing base layer
  for( int y = 0; y < max_y; y++ )
  {
   for( int x = 0; x < max_x; x++ )
    {
      mvprintw(y, x, ".");
    }
  }
  
  // Initial player drawing
  mvprintw(player.y, player.x,   ".\\()/.");
  mvprintw(player.y+1, player.x, ".-<>-.");
  if( player_wiggle == true )
  {
    mvprintw(player.y+2, player.x, "./(_)\\");
  }
  else
  {
    mvprintw(player.y+2, player.x, "/(_)\\.");
  }
  
  // Initial opponent drawing
  mvprintw(opponent.y, opponent.x,   "(=)");
  mvprintw(opponent.y+1, opponent.x, "( )");
  mvprintw(opponent.y+2, opponent.x, "(_)");

  
  // Refresh drawing
  refresh();
  
  
  // Opponent movement
  while(run)
  {
    msleep(1000);
    opponent.y -= 1;
    mvprintw(opponent.y, opponent.x,   "(=)");
    mvprintw(opponent.y+1, opponent.x, "( )");
    mvprintw(opponent.y+2, opponent.x, "(_)");
    mvprintw(opponent.y+3, opponent.x, "...");
    
    refresh();
  }
  
  pthread_mutex_destroy(&lock);
  endwin();
  
  return 0;
}
