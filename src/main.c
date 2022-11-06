#include <X11/Xlib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define I3IPC_IMPLEMENTATION
#include "i3ipc.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    exit(0);
  }

  int rev;
  Window win;
  Display *dis = XOpenDisplay(0);
  XGetInputFocus(dis, &win, &rev);

  if (fork() == 0) {
    execvp(argv[1], argv+1);
    exit(0);
  }

  i3ipc_subscribe_single(I3IPC_EVENT_WINDOW);

  int win_id = 0;

  while (true) {
    bool closed = false;

    I3ipc_event* ev_any = i3ipc_event_next(10);
    if (ev_any == NULL) continue;
    if (ev_any->type == I3IPC_EVENT_WINDOW) {
      I3ipc_event_window* ev = (I3ipc_event_window*)ev_any;
      switch (ev->change_enum) {
        case I3IPC_WINDOW_CHANGE_NEW:
          if (
              win_id == 0 && ev->container.window != win
              && (
                ev->container.floating_enum == I3IPC_NODE_FLOATING_AUTO_OFF ||
                ev->container.floating_enum == I3IPC_NODE_FLOATING_USER_OFF)
             ) {
            win_id = ev->container.window;
            XUnmapWindow(dis, win);
            XFlush(dis);
          }
          break;

        case I3IPC_WINDOW_CHANGE_CLOSE:
          if (win_id == ev->container.window) {
            win_id = 0;
            XMapWindow(dis, win);
            XFlush(dis);
            closed = true;
          }
          break;
      }
    }
    free(ev_any);
    if (closed) break;
  }
  XCloseDisplay(dis);

  int status;
  wait(&status);
  int ret = WEXITSTATUS(status);

  return ret;
}
