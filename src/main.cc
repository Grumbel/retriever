
/* Retriever - The Gateway 
 * =======================
 * Autor:      Ingo Ruhnke 
 * Beginn:  9. August 1997 
 * Beendet: ??????????????
 */                          

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
//#include <logalloc.h>
#include <allegro.h>

#include "objects.h"
#include "room.h"
#include "guy.h"
#include "function.h"
#include "prototyp.h"
#include "dos_func.h"
#include "add_aleg.h"

class guys guy1(120,150); //(120,150)
class rooms room[NUMBER_OF_ROOMS];
BITMAP *map;
RGB  grey[256];
RGB  screen_pal[256];
int  actual_room = 0;
int  inventory[15][2];
int  current_object, last_object, walkto_object;
bool do_vsync = TRUE;
char retriever_version[]={"V0.1Beta"};

volatile int uhr;

void uhr_laufen();

void uhr_laufen()
{
  ++uhr;
}
END_OF_FUNCTION(uhr_laufen);

int main (int argc, char *argv[])
{
  int    screen_w=320, screen_h=200;
  bool   exit_retriever = FALSE;
  RGB    palette_licht[256];
  int    event = 0;
  bool   show_intro = TRUE;
  bool   sound_on = TRUE;
  bool   do_event = FALSE;
  int    temp_mouse_x, temp_mouse_y;
  MIDI   *bg_music;
  SAMPLE *tuer;
  SAMPLE *gateway;
  int    show_room = -1;
  bool   argument_correct = TRUE;
  char   midi_file[255] = {"music/retriev.mid"};
  char   rdf_file[80] = {"retriev.rdf"};
  int    mouse_buttons;
  char   line_input[64];
  int    c;
  
  current_object = KEINS;
  walkto_object = KEINS;
  
  /* use of getopt, incomplete, see one screen down */
  opterr = 0;
  while ((c = getopt(argc, argv, "w:h:f:")) != -1) {
    switch(c)	{
      case 'w':
	screen_w = atoi(optarg);
	break;
      case 'h':
	screen_h = atoi(optarg);
	break;
      case 'f':
	strcpy(rdf_file, optarg);
	break;
	/* case '':  
	   break;*/
      case '?':
	fprintf(stderr, "Unknown option %c\n", c);
	break;
    }
  }
#if 0
  for (int i=1; i<argc; ++i) {

    if (stricmp(argv[i], "-nointro") == 0) {
      show_intro = FALSE;

    } else if (stricmp(argv[i], "-nosound") == 0) {
      sound_on = FALSE;

    } else if (stricmp(argv[i], "-?") == 0) {
      arg_help();

    } else if (stricmp(argv[i], "-midi") == 0) {
      strcpy(midi_file, argv[++i]);

    } else if (stricmp(argv[i], "-startroom") == 0) {
      actual_room = atoi(argv[++i]);

    } else if (stricmp(argv[i], "-showroom") == 0) {
      show_room = atoi(argv[++i]);

    } else if (!stricmp(argv[i], "-novsync")) {
      do_vsync = FALSE;

    } else if (!stricmp(argv[i], "-rdf")) {
      strcpy(rdf_file, argv[++i]);

    } else {
      fprintf(stderr, "Wrong argument: \"%s\"\n", argv[i]);
      argument_correct = FALSE;
      break;
    }
  }
#endif
  
  if (!argument_correct) {
    arg_help();
  } else {
    fprintf(stderr, "Arguments correct\n");
  }

  // time_t now;
  // time(&now);
  // printf("The current time is %s", asctime(localtime(&now)));

  for(int i = 0; i<NUMBER_OF_ROOMS; ++i)
    room[i].get_data(rdf_file, i);
  fprintf(stderr, "\"%s\" file read\n", rdf_file);

  if (show_room != -1) {
    printf("Raum %d:\n", show_room);
    printf("============\n");
    room[show_room].print_data();
    puts("============================================================\n");
    getchar();
  }

  // init grey pallete
  for (int i=0; i<256; ++i) {
    palette_licht[i].r = 63;
    palette_licht[i].g = 63;
    palette_licht[i].b = 63;
    screen_pal[i].r = grey[i].r = i/4;
    screen_pal[i].g = grey[i].g = i/4;
    screen_pal[i].b = grey[i].b = i/4;
  }

  /* Install all the allegro stuff */
  allegro_init();
  install_timer();
  install_keyboard();

  /* get config data from file  */

  if ((mouse_buttons = install_mouse()) == -1) {
    fprintf(stderr,"Mouse not found.\n");
    exit(1);
  } else {
    fprintf(stderr, "%d-button mouse found\n", mouse_buttons);
  }
  // set_mouse_speed(0, 500);
  temp_mouse_x = mouse_x;
  temp_mouse_y = mouse_y;

  if (sound_on) {
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) == -1) {
      fprintf (stderr,"Sound error\n%s\n", allegro_error);
      allegro_exit();
      exit(1);
    } else {
      if ((bg_music = load_midi(midi_file)) == NULL) {
	fprintf(stderr, "Midi File \"%s\" not found\n", midi_file);
	allegro_exit();
	exit(1);
      } {
	fprintf(stderr, "\"%s\" was loaded\n", midi_file);
      }
      play_midi(bg_music, 1);
      fprintf(stderr, "Sound initialisiert\n");
    }
  }

  fprintf(stderr, "init VGA(320x200), bye\n");
  if (set_gfx_mode(GFX_AUTODETECT, screen_w, screen_h, 0, 0) < 0) {
    printf ("VGA could not been init\n%s\n", allegro_error);
    allegro_exit();
    exit(0);
  }
  init_mouse_sprite();

  if (!(tuer = load_wav("music/tuer_.wav"))) {
    fprintf(stderr, "Sample nicht gefunden\n");
    exit(1);
  }

  if (!(gateway = load_wav("music/makelev.wav"))) {
    fprintf(stderr, "Sample nicht gefunden\n");
    exit(1);
  }


  LOCK_VARIABLE(uhr);
  LOCK_FUNCTION(uhr_laufen);

  install_int(uhr_laufen, 100);

  set_palette(screen_pal);

  set_config_file("retriev.cfg");
  set_gamma(get_config_float("graphic", "gamma", 1.0));
  set_volume(get_config_int("music", "digi_volume", 255),
	     get_config_int("music", "midi_volume", 255));

  if (show_intro)
    intro();

  guy1.init();

  room[actual_room].change_to();

  clear_inv();


  /*************************
   * here starts the games *
   *************************/
  while (!exit_retriever) {

    if (keypressed()) {
      switch (readkey() >> 8) {
	case KEY_ESC:
	  switch (_alert(NULL,
			 "Exit Retriever?",
			 NULL,
			 "Yes", "No", 'y', 'n')) {
	    case 1: // yes
	      exit_retriever = TRUE;
	      break;
	    case 2: // no
	      exit_retriever = FALSE;
	      break;
	  }
	  break;
	case KEY_F1:
	  show_help();
	  break;
	case KEY_F2:
	  backup_screen();
	  input_line(line_input, 2, 190, 255);
	  backup_screen();
	  phrase(line_input);
	  break;
	case KEY_F3:
	  show_command_help();
	  break;
	case KEY_F5:
	  load_status();
	  break;
	case KEY_F6:
	  save_status();
	  break;
	case KEY_F7:
	  show_file("story.txt");
	  break;
	case KEY_F11:
	  fscroll("credits.txt", 25);
	  fade_in(screen_pal, 2);
	  break;
	case KEY_F12:
	  show_about();
	  break;
	default:
	  show_help();
	  break;
      } // switch (readkey() >> 8)
    } // if (keypressed())

    //  here the guy beginns his walking and the main drawing routine
    if (guy1.walking) {
      // refresh_screen = TRUE;
      // 21:09
      guy1.goto_aim();

      if (do_vsync)
	vsync();

      room[actual_room].draw();
    } else {
      guy1.stand();
    } // if (guy1.walking || refresh_screen)

    last_object = current_object;
    current_object = get_object(actual_room);

    if (last_object != current_object) {
      if (current_object != KEINS) {
	textprintf(screen, font, 180,1,255,
		   "%-12s", room[actual_room].object[current_object].name);
      } else {
	textout(screen, font, "----             ", 180,1,255);
      }
    }

    if (temp_mouse_x != mouse_x || temp_mouse_y != mouse_y
        || last_object != current_object) {
      textprintf(screen, font, 1, 1, 155,
		 "%3d %3d %2d %3d %2d",
		 mouse_x, mouse_y, current_object, guy1.z_pos,actual_room);
      textprintf(screen, font, 1, 10, 155,
		 "%9s",
		 (getpixel(map, mouse_x, mouse_y) ? "walkable" : "wall"));
      // refresh_screen = TRUE;
    }

    temp_mouse_x = mouse_x;
    temp_mouse_y = mouse_y;

    /*    if (update_status_line) {
	  }*/

    if (mouse_button1_pressed()) {
      guy1.set_aim(mouse_x, mouse_y);
      walkto_object = current_object;

      if (room[actual_room].object[current_object].type == DOOR) {
        do_event = TRUE;
        event = WALKTO;
      } else {
        do_event = FALSE;
      }
    }

    if (mouse_button2_pressed()) {
      event = get_event();

      switch (event) {
	case USE: // use
	  guy1.set_aim(mouse_x, mouse_y);
          walkto_object = current_object;
          do_event = TRUE;
          break;
	case WATCH: // watch
	  guy1.set_aim(mouse_x, mouse_y);
          walkto_object = current_object;
          do_event = TRUE;
          break;
	case TAKE: // take
          guy1.set_aim(mouse_x, mouse_y);
          walkto_object = current_object;
          do_event = TRUE;
          break;
	case INV:
          show_inv();
          break;
      }
    }

    if (mouse_button3_pressed()) {
      show_inv();
    }

    if (do_event && !guy1.walking) {
      if (walkto_object != KEINS) {
	if (event == USE
	    && room[actual_room].object[walkto_object].type == VIEWABLE) {
	  view(room[actual_room].object[walkto_object].view);
	  do_event = FALSE;
	  event = 0;
	}

	if (event == WALKTO
	    && room[actual_room].object[walkto_object].type == DOOR) {
	  guy1.stop();
	  room[actual_room].draw();
	  //redraw_screen();
	  switch (room[actual_room].object[walkto_object].next.room) {
            case 1:
	      switch (actual_room) {
		case 0:
		  play_sample(tuer, 255,128,1000,0);
		  show_mouse(NULL);
		  draw_pcx(screen, "graphics/2tuer_3.pcx", 99, 87);
		  guy1.put_direct();
		  rest(200);

		  draw_pcx(screen, "graphics/2tuer_2.pcx", 99, 87);
		  guy1.put_direct();
		  rest(200);

		  guy1.set_pos(146, 146);
		  show_mouse(screen);
		  break;
		case 3:
		  fade_out(3);
		  guy1.set_pos(10, 160);
		  break;
	      }
	      break;
            case 0:
	      switch (actual_room) {
		case 1:
		  play_sample(tuer, 255,128,1000,0);
		  show_mouse(NULL);
		  draw_pcx(screen, "graphics/tuer_2.pcx", 118, 103);
		  guy1.put_direct();
		  rest(200);

		  draw_pcx(screen, "graphics/tuer_1.pcx", 118, 103);
		  guy1.put_direct();
		  rest(200);
		  guy1.set_pos(121, 117);
		  show_mouse(screen);
		  break;
		case 2:
		  play_sample(gateway, 255,128,1000,0);
		  fade_from(screen_pal, palette_licht, 3);
		  guy1.set_pos(207, 133);
		  break;
	      }
	      break;
            case 2:
	      play_sample(gateway, 255,128,1000,0);
	      fade_from(screen_pal, palette_licht, 3);
	      guy1.set_pos(220, 142);
	      break;
            default:
	      fade_out(3);
	      guy1.set_pos(room[actual_room].object[current_object].next.x_pos,
			   room[actual_room].object[current_object].next.y_pos);
	      break;
	  } // switch (room[actual_room].object[current_object].next.room)
	  actual_room = room[actual_room].object[walkto_object].next.room;

	  room[actual_room].change_to();
	} else if (event == USE) {
	  guy1.stop();
	  room[actual_room].draw();

	  char buf[100];
	  sprintf(buf,
		  "Ich weiá nicht den %s benutzen soll.",
		  room[actual_room].object[walkto_object].name);
	  print_text(buf);

	} else if (event == WATCH) {
	  if (room[actual_room].object[walkto_object].explanation[0]
	      != '\0') {
	    guy1.stop();
	    room[actual_room].draw();
	    print_text(room[actual_room].object[walkto_object].explanation);
	  }
	} else if (event == TAKE) {
          if (current_object != KEINS) {
	    if (room[actual_room].object[current_object].type == TAKEABLE) {
	      room[actual_room].object[current_object].status = 1;
	      guy1.take();
	      put_to_inv(current_object, actual_room);
	      room[actual_room].redraw();
	    } else {
	      char buf[100];
	      sprintf (buf, "Das %s kann ich nicht nehmen." ,
		       room[actual_room].object[current_object].name
                       );
	      print_text(buf);
	    }
          }
	}
	show_mouse(screen);
	do_event = FALSE;
	event = NONE;
	walkto_object = KEINS;
      }

    } // if (mouse_button1_pressed && !text_on_screen)
  } //  while (!keypressed())
  clear_keybuf();
  retriever_exit(0);
  return 0;
} // void main(void)
