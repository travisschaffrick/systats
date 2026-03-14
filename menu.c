#include <locale.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "sysinfo.h" // includes <ncurses.h>

#define ENTER 10
#define KEY_S 115

#define OFFSET 4
#define MAX 64

int 
main() {
  /*
   *   MENU
   */
  char *options[] = {"CPU Info", "CPU Temp", "Memory Info", "Load Avg. Info", "Uptime"};
  int settings = 0;

  setlocale(LC_ALL, "");
  initscr();
  keypad(stdscr, TRUE);  
  cbreak();
  noecho();
  curs_set(0);
  WINDOW* menu = newwin(0,0,0,0);

  size_t count = sizeof(options)/sizeof(options[0]);

  // user input
  int input = 0;
  int selected = 0;

  int running = 1;
  while (running) {

    while(input != 's') { refresh();
      wclear(menu);
      mvwprintw(menu, 1, 12, "Select Variables to Monitor");
      mvwprintw(menu, 2, 12, "Press 's' to start!");

      for (size_t i = OFFSET; i < count + OFFSET; i++) {
        // print selected status
        char box[4];
        if (settings & 1 << (i - OFFSET)) {
          strcpy(box, "[x]");
        }
        else {
          strcpy(box, "[ ]");
        }

        // build selected status + setting
        // print
        mvwprintw(menu, i, 15, "%s %s", box, options[i-OFFSET]);
      }

      if (selected >= 0 && selected < (int)count) {
        mvwchgat(menu, selected+OFFSET, 15, strnlen(options[selected], MAX) + 4, A_REVERSE, 1, NULL); 
      }

      // Print frame
      wrefresh(menu);

      //Get input
      input = getch();
      if (input == ERR) input = 0;

      if (input == KEY_UP) {
        if (selected == 0) {
          selected = (int)count - 1;
        }
        else {
          selected--;
        }
      }
      if (input == KEY_DOWN) {
        if (selected == (int)count - 1) {
          selected = 0;
        }
        else {
          selected++;
        } 
      }

      if (input == ENTER) {
        settings ^= (1 << selected);
      }
    }

    wclear(menu);

    WINDOW* loading = newwin(0,0,0,0);

    mvwprintw(loading, 1, 12, "Loading...");


    wrefresh(loading);

    delwin(loading);

    /*
     *   PROGRAM LOOP
     */
    WINDOW* display = newwin(0,0,0,0);
    nodelay(stdscr, TRUE);

    // init cpu
    struct cpu_usage prev = get_cpu_usage();

    while (1) {
      int quit = 0;

      for (int i = 0; i < 4; i++) {
        usleep(250000);
        int ch = getch();

        if (ch == 'q') {
          nodelay(stdscr, FALSE);
          input = 0;
          running = 0;
          quit = 1;
          break;
        }

        if (ch == 's') {
          nodelay(stdscr, FALSE);
          input = 0;
          quit = 1;
          break;
        }
      }

      if (quit) break;

      struct cpu_usage curr = get_cpu_usage();
      struct sysinfo sys;
      struct cpu_info cpu_info;

      float diff_total = curr.total_time - prev.total_time; 
      float diff_idle = curr.idle_time - prev.idle_time;

      cpu_info.diff_total = diff_total;
      cpu_info.diff_idle = diff_idle;
      cpu_info.temp = get_cpu_temp();

      sys.cpu_info = cpu_info;

      sys.mem_usage = get_mem_usage();
      sys.load_avg = get_la();
      sys.uptime = get_uptime();

      sys.settings = settings;

      wclear(display);                       // clear
      ncurses_selected(display, 1, 12, sys); // draw new frame 
      wrefresh(display);                     // show it

      // update cpu
      prev = curr;
    }
  }
  endwin();
  return 0;
}


