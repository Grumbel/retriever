#include <stdio.h>
#include <string.h>
#include <allegro.h>

#include "objects.h"
#include "dos_func.h"
#include "prototyp.h"
#include "room.h"
#include "add_aleg.h"
//#include <logalloc.h>

/* reads file and display it with scroll() */
void fscroll(char *filename, int speed)
{
 char **str;
 int lines;

 lines = fget_lines(filename);

 if (lines == -1) {
   perror(filename);
   allegro_exit();
   exit(1);
 }

 str = (char **)malloc(sizeof(char **) * (lines));

 if (str == NULL) {
   fprintf(stderr, "Out of memory, while loading \"%s\"!\n", filename);
   exit(1);
 }

 get_txt_file(filename, str);

 scroll(str, lines, speed);

 for(int i=0; i<lines; ++i) {
    free(str[i]);
 }
 
 free(str);
}

void show_file(char *filename)
{
 char **str;
 int lines;

 lines = fget_lines(filename);

 if (lines == -1) {
   fprintf(stderr, "File \"%s\" not Found\n", filename);
   allegro_exit();
   exit(1);
 }

 str = (char **)malloc(sizeof(char **) * lines);

 if (str == NULL) {
   fprintf(stderr, "Out of memory, while show_file(\"%s\")\n", filename);
   exit(1);
 }

 get_txt_file(filename, str);

 show_text(str, lines);

 for(int i=0; i<lines; ++i) {
    free(str[i]);
 }
 free(str);
}

void put_to_inv(int object, int act_room)
{
 int x;

 for(x=0; x<6; ++x)
    if (inventory[x][0] == KEINS)
      break;

 inventory[x][0] = object;
 inventory[x][1] = act_room;
}

void clear_inv()
{
 int x,y;

 for(x=0; x<6; x++)
   for(y=0; y<=2; y++)
     inventory[x][y] = KEINS;
}

void show_inv()
{
 int x, y;
 BITMAP *inv_temp_screen = create_bitmap(320,200);

 show_mouse(NULL);
 blit(screen, inv_temp_screen, 0,0, 0,0, 320,200);
 rectfill(screen, 50, 50, 280, 160, 1);

 for(y=50; y<160; y+=30)
    line(screen, 50, y, 280, y, 150);
 for(x=50; x<280; x+=30)
    line(screen, x, 50, x, 160, 150);

 for(x=0; x<6; ++x) {
   if (inventory[x][0] != KEINS) {
     draw_pcx_sprite_stretch(screen,
			     room[inventory[x][1]].object[inventory[x][0]].sprite_name,
			     (50 + (x * 30)),50,
			     room[inventory[x][1]].object[inventory[x][0]].rle_sprite->w > 30 ? 30 : room[inventory[x][1]].object[inventory[x][0]].rle_sprite->w,
			     room[inventory[x][1]].object[inventory[x][0]].rle_sprite->w > 30 ? 30 : room[inventory[x][1]].object[inventory[x][0]].rle_sprite->h);
   }
 }
 show_mouse(screen);

 while (!mouse_button3_pressed() &&
        !mouse_button2_pressed() &&
        !mouse_button1_pressed() &&
        !keypressed()
        );

 clear_keybuf();

 show_mouse(NULL);
 blit(inv_temp_screen, screen,  0,0, 0,0, 320,200);
 show_mouse(screen);

 destroy_bitmap(inv_temp_screen);
}

/* useless transparent bitmap to bitmap fade function :-) */
void fade2bitmap(BITMAP *scr2) {
 int x, y;
 int farbe1, farbe2;

 for (y=0; y<200; ++y)
    for (x=0; x<320; ++x) {
       farbe1 = _getpixel(screen, x, y);
       farbe2 = _getpixel(scr2, x, y);
       _putpixel(screen, x, y, ((farbe1 * 5) + (farbe2)) / 6);
    }
}

/* this function smooth a rectangle with the coordinats x1, y1, x2, y2
 * but it is very slow */
void smooth(BITMAP *scr, int x1, int y1, int x2, int y2)
{
 int x, y;
 int farbe;

 for (y=y1; y<=y2; ++y) {
    for (x=x1; x<=x2; ++x) {
       farbe = 0;
       farbe += _getpixel(scr, x-1, y);
       farbe += _getpixel(scr, x+1, y);
       farbe += _getpixel(scr, x, y+1);
       farbe += _getpixel(scr, x, y-1);
       farbe += _getpixel(scr, x, y);
       farbe /= 5;
       putpixel(scr, x, y, farbe);
    }
 }
}

void intro()
{
 // textout_centre(screen, font, "this game was done with", 160, 176, 255);
 // textout_centre(screen, font, allegro_id, 160, 190, 255);
 draw_pcx(screen, "graphics/poision.pcx", 0, 0);
 textout_centre(screen, font, "presents", 160, 180, 255);
 if (keypressed()) {
   clear_keybuf();
   return;
 }
 rest(2000);

 fade_out(2);
 if (keypressed()) {
   clear_keybuf();
   return;
 }

 draw_pcx(screen, "graphics/retriev.pcx", 0, 0);

 fade_in(screen_pal, 2);
 if (keypressed()) {
   clear_keybuf();
   return;
 }
 rest(1000);
 fade_out(2);
 clear(screen);
 fade_in(screen_pal,63);
 fscroll("story.txt", 75);
}

void init_mouse_sprite()
{
 static BITMAP *mouse_sprite;
 RGB palette[256];

 if ((mouse_sprite = load_pcx("graphics/mouse.pcx", palette)) == NULL) {
   allegro_exit();
   fprintf(stderr, "\"graphics/mouse.pcx\" not found\n");
   exit(1);
 }

 set_mouse_sprite(mouse_sprite);
 set_mouse_sprite_focus(7, 6);
 set_mouse_range(0,0, 319,199);
} // init_mouse_sprite(BITMAP *mouse_sprite)

void set_gamma(float gamma)
{
 make_gamma_pal(grey, screen_pal, gamma);
 set_palette(screen_pal);
}
