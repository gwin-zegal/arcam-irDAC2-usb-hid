#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>
#include <mpd/message.h>

#include <assert.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


static int
handle_error(struct mpd_connection *c)
{
  assert(mpd_connection_get_error(c) != MPD_ERROR_SUCCESS);

  fprintf(stderr, "%s\n", mpd_connection_get_error_message(c));
  mpd_connection_free(c);
  return EXIT_FAILURE;
}


int main()
{
  int fd;
  struct input_event ev;
  const char *DEVICE = "/dev/input/by-id/usb-ARCAM_irDAC_II-event-if03";
  struct mpd_connection *conn;

  // Connection do USB HID
  fd = open(DEVICE, O_RDONLY);

  // Connection to mpd client on port 6600
  conn = mpd_connection_new(NULL, 6600, 30000);
  
  if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
    return handle_error(conn);
  
  while (1)
    {
      read(fd, &ev, sizeof(struct input_event));

      if(ev.type == 1){
	//printf("key %i state %i\n", ev.code, ev.value);
	if(ev.value == 0){
	  // printf(" : [key %i]\n ", ev.code);
	  if(ev.code == 119){
	    mpd_run_toggle_pause(conn);
	  }
	  else if(ev.code == 163){
	    mpd_run_next(conn);
	  }
	  else if(ev.code == 165){
	    mpd_run_previous(conn);
	  } 
	  else if(ev.code == 166){
	    mpd_run_stop(conn);
	  }	  
	  else if(ev.code == 207){
	    mpd_run_play(conn);
	  }
	}
      }
    }
}
