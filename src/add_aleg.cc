/***********************************/
/* This are some add on function   */
/* for Allegro. All this function, */
/* should be indepentend from the  */
/* game and could be reused in     */
/* other projects. Have fun!       */
/* author: Ingo Ruhnke             */
/* eMail:  grumbel@gmx.de          */
/***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>

#include "add_aleg.h"


/* Create a boarder around the text, the text must be white (255)
 * and on a clear screen (0), stupid and slow, but works ok */
void edit_text_screen(BITMAP *screen)
{
 int x, y;

 for (y=0; y < screen->h ; ++y)
     for (x=0; x < screen->w ; ++x) {
         if (getpixel(screen, x, y) == 255) {
            if (getpixel(screen, x-1, y) == 0)
               putpixel(screen, x-1, y, 1);
            if (getpixel(screen, x+1, y) == 0)
               putpixel(screen, x+1, y, 1);
            if (getpixel(screen, x, y+1) == 0)
               putpixel(screen, x, y+1, 1);
            if (getpixel(screen, x, y-1) == 0)
               putpixel(screen, x, y-1, 1);

            if (getpixel(screen, x-1, y-1) == 0)
               putpixel(screen, x-1, y-1, 1);
            if (getpixel(screen, x+1, y-1) == 0)
               putpixel(screen, x+1, y-1, 1);
            if (getpixel(screen, x-1, y+1) == 0)
               putpixel(screen, x-1, y+1, 1);
            if (getpixel(screen, x+1, y+1) == 0)
               putpixel(screen, x+1, y+1, 1);
         }
     }
}


/* converts three values to a rgb structure  */
RGB rgb_color(unsigned char r, unsigned char g, unsigned char b)
{
 RGB color;

 color.r = r;
 color.g = g;
 color.b = b;

 return color;
}

/* draws a pcx-file direct from file  */
void draw_pcx(BITMAP *scr, char *filename, int x, int y)
{
 BITMAP *pcx_file;
 RGB palette[256];

 if ((pcx_file = load_pcx(filename, palette)) == NULL) {
    allegro_exit();
    fprintf(stderr, "\"%s\" not found\n", filename);
    exit(1);
 }

 blit(pcx_file, scr, 0, 0, x, y, pcx_file->w, pcx_file->h);
 destroy_bitmap(pcx_file);
}

/* same as draw_pcx(), but it draws a sprite  */
void draw_pcx_sprite(BITMAP *scr, char *filename, int x, int y)
{
 BITMAP *pcx_file;
 RGB palette[256];
 if ((pcx_file = load_pcx(filename, palette)) == NULL) {
    allegro_exit();
    fprintf(stderr, "\"%s\" not found\n", filename);
    exit(1);
 }

 draw_sprite(scr, pcx_file, x, y);
 destroy_bitmap(pcx_file);
}

/* same as draw_pcx_sprite, but it draws a stretched sprite */
void draw_pcx_sprite_stretch
(BITMAP *scr, char *filename, int x, int y, int w, int h)
{
 BITMAP *pcx_file;
 RGB palette[256];
 if ((pcx_file = load_pcx(filename, palette)) == NULL) {
    allegro_exit();
    fprintf(stderr, "\"%s\" not found\n", filename);
    exit(1);
 }

 stretch_sprite(scr, pcx_file, x, y, w, h);
 destroy_bitmap(pcx_file);
}

/* Replacement for the alert function, it */
/* solves the error with the text back-   */
/* ground color change.                   */
int
_alert(char *s1, char *s2, char *s3, char *b1, char *b2, int c1, int c2)
{
 int return_value;

 return_value = alert(s1, s2, s3, b1, b2, c1, c2);
 text_mode(0);

 return return_value;
}


/* Only checks if the mouse is in the rectangle discribed */
/* by x1, y1 (top/left) and x2, y2 (bottem/right). Some   */
/* changes were made on 1.1.98                            */
bool mouse_between(int x1, int y1, int x2, int y2)
{
 if (mouse_x < x1 || mouse_x > x2 ||
     mouse_y < y1 || mouse_y > y2)
    return FALSE;
 return TRUE;
}


/* checks if the mouse button is pressed, solves the problem */
/* with the multible button presses, when the button is hold */
bool mouse_button1_pressed()
{
 static bool temp_mouse_b1;
 bool mouse_b1 = mouse_b & 1;

 if (mouse_b1 && mouse_b1 != temp_mouse_b1) {
   temp_mouse_b1 = mouse_b1;
   return TRUE;
 }
 temp_mouse_b1 = mouse_b1;
 return FALSE;
}

bool mouse_button2_pressed()
{
 static bool temp_mouse_b;
 bool mouse2_b = mouse_b & 2;

 if (mouse2_b && mouse2_b != temp_mouse_b) {
   temp_mouse_b = mouse2_b;
   return TRUE;
 }
 temp_mouse_b = mouse2_b;
 return FALSE;
}

bool mouse_button3_pressed()
{
 static bool temp_mouse_b;
 bool mouse3_b = mouse_b & 4;

 if (mouse3_b && mouse3_b != temp_mouse_b) {
   temp_mouse_b = mouse3_b;
   return TRUE;
 }
 temp_mouse_b = mouse3_b;
 return FALSE;
}

void backup_screen()
{
 static BITMAP *screen_buf = create_bitmap(screen->w, screen->h);
 static bool screen_saved = FALSE;

 show_mouse(NULL);

 if (!screen_saved) {
   blit(screen, screen_buf,0,0,0,0,screen->w,screen->h);
   screen_saved = TRUE;
 } else {
   blit(screen, screen,0,0,0,0,screen->h,screen->h);
   screen_saved = FALSE;
 }

 show_mouse(screen);
}

/* copy scr to tmp, to backup a screen  */
void _backup_scr(BITMAP **tmp)
{
 *tmp = create_bitmap(screen->w, screen->h);
 blit(screen, *tmp, 0, 0, 0, 0, screen->w, screen->h);
}

/* restores screen from tmp  */
void _restore_scr(BITMAP **tmp)
{
 blit(*tmp, screen, 0, 0, 0, 0, screen->w, screen->h);
 destroy_bitmap(*tmp);
}

void backup_scr(BITMAP **tmp)
{
 show_mouse(NULL);
 _backup_scr(tmp);
 show_mouse(screen);
}

void restore_scr(BITMAP **tmp)
{
 show_mouse(NULL);
 _restore_scr(tmp);
 show_mouse(screen);
}

void scroll(char **str, int lines, int speed)
{
 BITMAP *tmp;
 BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
 int y=0;
 int scrolling = SCREEN_H;
 char s[80];
 
 show_mouse(NULL);
 _backup_scr(&tmp);
 clear(screen);

 for(scrolling = SCREEN_H;
     (scrolling+(lines*10)) > 0
       && !keypressed();
     --scrolling) {
    clear(buffer);

    itoa(SCREEN_W, s, 10);
    textout(screen, font, s,100,100,255);
    textout(buffer, font, s,100,100,255);

    // here can be some color fading routine be placed
    for(y=0; y<lines; ++y)
       textout_centre(buffer,
		      font,
		      str[y],
		      buffer->w / 2,
		      (y * 10) + scrolling,
		      255
		      //MID(175, 255,
		      //  abs((10*y + scrolling+5)- screen->w/2) * 255 / (screen->w/2))
			  //  255-(int)((abs((y*10) + scrolling+5) -100)*2.55))
		      );

    vsync();
    blit(buffer, screen, 0, 0, 0, 0, screen->w, screen->h);
    rest(speed);
 }
 clear_keybuf();
 fade_out(64);
 _restore_scr(&tmp);
 show_mouse(screen);
}

void show_text(char **str, const int max_lines)
{
 BITMAP *tmp;
 BITMAP *buffer=create_bitmap(screen->w,screen->h);
 int last_line=0;
 int y;
 int c;
 int max_x = screen->w - 1;
 int max_y = screen->h - 1;

 
 show_mouse(NULL);
 _backup_scr(&tmp);

 do {
   clear(buffer);
   for(y=last_line; y-last_line<(max_y/10+1) && y<max_lines; ++y) {
      textout(buffer, font, str[y], 0, (y-last_line)*9, 255);
   }
   
   rect(buffer,
	max_x-8, y*max_y/ max_lines, max_x,
	last_line*max_y / max_lines,
	255);
   
   vsync();
   blit(buffer, screen, 0,0,0,0,screen->w,screen->h);
   c = readkey();

   if (c>>8 == KEY_UP && last_line > 0)
     --last_line;
   else if (c>>8 == KEY_DOWN && y < max_lines)
     ++last_line;

 } while(c>>8 != KEY_ESC);

 _restore_scr(&tmp);
 show_mouse(screen);
}

void make_gamma_pal(PALETTE srcpal, PALETTE destpal, float gamma)
{
 int i;

 for(i = 0; i < 256; i++) {
    destpal[i].r = (unsigned char)(MID(0, 63, srcpal[i].r * gamma));
    destpal[i].g = (unsigned char)(MID(0, 63, srcpal[i].g * gamma));
    destpal[i].b = (unsigned char)(MID(0, 63, srcpal[i].b * gamma));
 }
}


void input_line(char *buf, int x, int y, int color)
{
 BITMAP *tmp_scr;
 char c;
 int i;

 show_mouse(NULL);
 _backup_scr(&tmp_scr);
 rectfill(screen, 1, 190, 318, 198, 0);

 for(i = 0;  (c = readkey() & 0xff) != '\n' && c != '\r'; ++i) {
    if (c == '\b') {
      i -= 2;
      if (i < -1)
        i = -1;
      buf[i+1] = '_';
      buf[i+2] = '\0';

      rectfill(screen, 1, 190, 318, 198, 0);
      textout(screen, font, buf, x, y, color);
      continue;
    }
    if (i>35)
      i = 35;

    buf[i] = c;
    buf[i+1] = '_';
    buf[i+2] = '\0';

    textout(screen, font, buf, x, y, color);
 }
 buf[i] = '\0';
 _restore_scr(&tmp_scr);
 show_mouse(screen);
}

bool rect_col(int ax1, int ax2, int ay1, int ay2,
              int bx1, int bx2, int by1, int by2)
{
 if (ax1 > bx2)
   return 0;
 if (ax2 < bx1)
   return 0;
 if (ay1 > by2)
   return 0;
 if (ay2 < by1)
   return 0;
 return 1;
}
