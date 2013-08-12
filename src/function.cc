#include <stdio.h>
#include <string.h>
#include <allegro.h>

#include "prototyp.h"
#include "room.h"
#include "guy.h"
#include "objects.h"
#include "function.h"
#include "add_aleg.h"
#include "dos_func.h"

/* simple incomplete function for saveing savegames */
void save_game(char *file_name)
{
 FILE *out;

 out = fopen(file_name, "w");

 fprintf(out, "%d %d %d\n", guy1.x_pos, guy1.y_pos, actual_room);

 for(int x=0; x<15; ++x)
    fprintf(out, "%d %d\n", inventory[x][0], inventory[x][1]);

 fclose(out);
}

/* simple loading function for savegames */
void load_game(char *file_name)
{
 FILE *in;

 in = fopen(file_name, "r");

 fscanf(in, "%d %d %d", &guy1.x_pos, &guy1.y_pos, &actual_room);

 for(int x=0; x<15; ++x)
    fscanf(in, "%d %d\n", &inventory[x][0], &inventory[x][1]);

 show_mouse(NULL);
 room[actual_room].change_to();
 show_mouse(screen);

 fclose(in);
}

/* looks like a select function for the savegames,
 * but i am not hundert percent sure :( */
void load_status()
{
 BITMAP *tmp_scr;
 char   savegame_file[255] = {SAVEGAME_PATH};

 show_mouse(NULL);
 backup_scr(&tmp_scr);
 show_mouse(screen);

 if (file_select("Spielstand laden", savegame_file, NULL)) {
   if (!exists(savegame_file)) {
     _alert("Datei",
	    get_filename(savegame_file),
	    "nicht gefunden!",
	    "Ok", NULL, KEY_ENTER, NULL);
   } else {
     load_game(savegame_file);
     return;
   }
 }
 restore_scr(&tmp_scr);
}

/* same as load_status, but only for save_game ?? */
void save_status()
{
 BITMAP *tmp_scr;
 char   savegame_file[255] = {SAVEGAME_PATH};

 backup_scr(&tmp_scr);

 if (file_select("Spielstand speichern", savegame_file, NULL)) {
   if (exists(savegame_file)) {
     if (_alert("Die Datei",
		get_filename(savegame_file),
		"existiert, berschreiben?",
		"Ja", "Nein", 'j', 'n'))
       // save_game if yes else restore_scr
       save_game(savegame_file);
   } else {
     save_game(savegame_file);
   }
 }
 restore_scr(&tmp_scr);
}

/* phraser for the command line, when the game is running */
void phrase(char *str)
{
 if (!strncasecmp(str, "room", 4)) {
   actual_room = atoi(str+5);
   if (actual_room>NUMBER_OF_ROOMS)
     actual_room = NUMBER_OF_ROOMS;
   show_mouse(NULL);
   room[actual_room].change_to();
   show_mouse(screen);

 } else if (!strncasecmp(str, "gamma", 5)) {
	set_gamma(atof(str+6));
   set_config_float("graphics", "gamma", atof(str+6));

 } else if (!strcasecmp(str, "help")) {
   show_command_help();

 } else if (!strncasecmp(str, "digi_volume", 11)) {
   set_volume(atoi(str + 11), -1);
   set_config_int("music", "digi_volume", atoi(str + 11));

 } else if (!strncasecmp(str, "midi_volume", 11)) {
   set_volume(-1, atoi(str + 11));
   fprintf(stderr, "MIDI: %d\n", atoi(str + 11));
   set_config_int("music", "midi_volume", atoi(str + 11));

 }
}

/* self explaning, i think :) */
void show_command_help()
{
 BITMAP *tmp_scr;
 show_mouse(NULL);
 _backup_scr(&tmp_scr);

 clear(screen);
 text_mode(0);
 textout_centre(screen, font, "Command Help", 160, 1, 255);

 textout(screen, font, "gamma X - set gamma X", 10, 20, 255);
 textout(screen, font, "help    - show this help", 10, 30, 255);
 textout(screen, font, "room X  - jump to room number X", 10, 40, 255);
 textout(screen, font, "midi_volume X - 0<=X<=255", 10, 50, 255);
 textout(screen, font, "digi_volume X - 0<=X<=255", 10, 60, 255);
/* textout(screen, font, "F6  - Spielstand speichern (n impl)", 10, 70, 255);
 textout(screen, font, "F11 - About Screen (scrolling)", 10, 80, 255);
 textout(screen, font, "F12 - About Screen", 10, 90, 255);
*/
 textout_centre(screen, font,
		"[Taste druecken um fortzufahren]", 160, 190, 255);

 readkey();

 _restore_scr(&tmp_scr);
 show_mouse(screen);
}

/* show a little box with some
 * icons to select a action */
int get_event()
{
 int x, y;
 int current_event;
 BITMAP *temp = create_bitmap(31, 31);

 x = mouse_x;
 y = mouse_y;

 if (x-15 <= 0)
    x = 16;
 if (x+15 >= 319)
    x = 319 - 16;
 if (y - 16 <= 0)
    y = 16;
 if (y + 15 >= 199)
    y = 199 - 16;

 show_mouse(NULL);
 blit (screen, temp, x - 15, y - 15, 0, 0, 31, 31);
 draw_pcx_sprite(screen, "graphics/events.pcx", x - 15, y - 15);
 show_mouse(screen);

 /* wait until mouse button is released */
 while(mouse_b & 2);

 if (mouse_x < x && mouse_y < y) {
   current_event = USE;
 } else if (mouse_x > x && mouse_y < y) {
   current_event = WATCH;
 } else if (mouse_x < x && mouse_y > y) {
   current_event = TAKE;
 } else if (mouse_x > x && mouse_y > y) {
   current_event = INV;
 } else {
   current_event = NONE;
 }

 show_mouse(NULL);
 blit (temp, screen, 0, 0, x - 15, y - 15, 31, 31);
 show_mouse(screen);

 destroy_bitmap(temp);

 position_mouse(x, y);

 return current_event;
} // int get_event()

/* prints out text on the screen with line breaks every 25 characters.
 * Line breaks were only done if no word is broken, else the line break
 * will be before the 25 characters */
void print_text(const char *orginal_str)
{
 int  x_pos_rechts, x_pos_links, i=0;
 int  x_pos = 160, y_pos;
 char *str;
 BITMAP *tmp_scr;
 BITMAP *text_buf = create_bitmap(320, 200);
 int string_lenght;
 int temp_uhr;

 string_lenght = strlen(orginal_str);

 str = (char *)malloc(sizeof(char)*string_lenght + 1);
 if (str == NULL) {
   fprintf(stderr, "Out of memory\n");
   allegro_exit();
   exit(1);
 }
 strcpy(str, orginal_str);

 show_mouse(NULL);
 _backup_scr(&tmp_scr);

 clear(text_buf);

 y_pos = 32;
 x_pos_rechts = x_pos + (split_strlen(str)*4);
 x_pos_links  = x_pos - (split_strlen(str)*4);

 if (x_pos_links < 2)
    x_pos = split_strlen(str)*4+2;
 if (x_pos_rechts > SCR_WIDTH - 1)
    x_pos = 319 - split_strlen(str)*4 ;

 i = 0;
 while (i < string_lenght) {
       split_str(str+i);

       textout_centre(text_buf, font, str + i, x_pos, y_pos, 255);
       y_pos += 10;
       i += strlen(str+i)+1;
 }
 // gives the text a black border
 edit_text_screen(text_buf);

 draw_sprite(screen, text_buf,0,0);

 temp_uhr = uhr;
 while(!mouse_b && !keypressed() && uhr < temp_uhr+30) {
    // timer must be placed here
 }
 while(mouse_b);
 clear_keybuf();

 destroy_bitmap(text_buf);
 _restore_scr(&tmp_scr);
 show_mouse(screen);
} // print_text(char *str, int x_pos, int y_pos)

/* Returns the number of the object the mouse is above, is no
 * object is there the return value will be KEINS (-1) */
int get_object(int actual_room)
{
 for (int i=0; i < room[actual_room].number_of_objects; ++i) {
     if (mouse_between (room[actual_room].object[i].x1-1,
                        room[actual_room].object[i].y1-1,
                        room[actual_room].object[i].x2+1,
                        room[actual_room].object[i].y2+1
                       ) && room[actual_room].object[i].status == 0)
     return i;
 }
 return KEINS;
} // int get_object(int room)

void view(char *str)
{
 BITMAP *temp=create_bitmap(320,200);
 show_mouse(NULL);
 blit(screen, temp, 0,0,0,0,320,200);
 draw_pcx(screen, str, 0, 0);
 show_mouse(screen);

 while(!mouse_b);
 while(mouse_b);

 show_mouse(NULL);
 blit(temp, screen, 0,0,0,0,320,200);
 show_mouse(screen);
 destroy_bitmap(temp);
}


/* Splits the string in lines which are smaller than 26 characters
   puts a '\0' on the end of the line */
void split_str(char *str)
{
 int i=0, temp_i;

 while (TRUE) {
       temp_i = i;
       ++i;

       if ((str[i] == ' ' && i > 25)|| str[i] == '\0' || str[i] == '\n')
          break;
 }
 if (i>25) {
    i = temp_i;
 }

 str[i+1] = '\0';
}

/* Returns the lenght of a string if it would be if split_str()
   is used. Must be used _before_ split_str() */
int split_strlen(char *str)
{
 int i=0, temp_i, i_len=0;

 while (str[i] != '\0') {
       temp_i = i;
       if (str[i] == '\n')
          break;
       if ((str[i] == ' ') && i > 25) {
          if (i>25)
             i = temp_i;
          if (i > i_len)
             i_len = i;
          str += i;
          i=0;
          continue;
       }
       ++i;
 }
 return i_len;
}
