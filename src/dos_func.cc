#include <stdio.h>
#include <allegro.h>
#include <ctype.h>

#include "objects.h"
#include "function.h"
#include "add_aleg.h"

// old conio.h stuff, disabled to make it work in Linux
#define textattr(x)
#define textcolor(x)

#define COMMENT_CHAR ';'

int fget_lines(const char *filename)
{
 FILE *in;
 int lines;

 in = fopen(filename, "rt");

 if (in == NULL)
   return -1;

 lines = 0;
 while(!feof(in)) {
   if (fgetc(in) == '\n')
     ++lines;
 }
 fclose(in);
// fprintf(stderr, "number of lines: %d\n", lines);
 return lines;
}

int get_txt_file(const char *filename, char **str)
{
 FILE *in;
 char buffer[105];
 int line;

 in = fopen(filename, "rt");

 if (in == NULL)
   return -1;

 line = 0;

 while(fgets(buffer,100,in)) {
    str[line] = (char *)malloc(sizeof(char) * (strlen(buffer)+1));
    strcpy(str[line], buffer);
    ++line;
 }

 fclose(in);
 return line;
}

void show_help()
{
 show_mouse(NULL);
 BITMAP *tmp_scr;
 _backup_scr(&tmp_scr);

 clear(screen);
 set_palette(screen_pal);

 textout_centre(screen, font, "Tastatur Hilfe", 160, 1, 255);

 textout(screen, font, "ESC - Spiel beenden", 10, 20, 255);
 textout(screen, font, "F1  - anzeigen der Hilfe", 10, 30, 255);
 textout(screen, font, "F2  - Command-Line aufrufen", 10, 40, 255);
 textout(screen, font, "F3  - Command-Line Help", 10, 50, 255);
 textout(screen, font, "F5  - Spielstand landen    (n impl)", 10, 60, 255);
 textout(screen, font, "F6  - Spielstand speichern (n impl)", 10, 70, 255);
 textout(screen, font, "F11 - About Screen (scrolling)", 10, 80, 255);
 textout(screen, font, "F12 - About Screen", 10, 90, 255);

 textout_centre(screen, font,
		"[Taste druecken um fortzufahren]", 160, 190, 255);

 readkey();

 _restore_scr(&tmp_scr);
 show_mouse(screen);
}

void show_about()
{
 show_mouse(NULL);
 BITMAP *about_temp_screen = create_bitmap(320,200);
 blit(screen, about_temp_screen, 0,0, 0,0, 320,200);

 clear(screen);

 textout_centre(screen, font, "About", 160, 1, 255);

 textout_centre(screen, font, "Idee, Grafik, Programmierung", 160, 20, 255);
 textout_centre(screen, font, "Ingo Ruhnke", 160, 30, 200);
 textout_centre(screen, font, "Musik", 160, 50, 255);
 textout_centre(screen, font, "Till Hellweg", 160, 60, 200);
 textout_centre(screen, font, "Dank an", 160, 80, 255);
 textout_centre(screen, font, "Shawn Hargreaves fÅr Allegro", 160, 90, 200);
 textout_centre(screen, font, "und", 160, 100, 200);
 textout_centre(screen, font, "DJ Delorie fÅr DJGPP", 160, 110, 200);
 textout_centre(screen, font, "weitere Infos unter", 160, 130, 255);
 textout_centre(screen, font,
		"http://home.pages.de/~retriever/", 160, 140, 200);
 textout_centre(screen, font,
		"[Taste druecken um fortzufahren]", 160, 190, 255);

 readkey();

 blit(about_temp_screen, screen, 0,0, 0,0, 320,200);
 destroy_bitmap(about_temp_screen);
 show_mouse(screen);
}

void retriever_exit(int error_code)
{
 allegro_exit();
 printf("Thank you for playing ");
 textattr(143);
 printf("Retriever ");
 textcolor(11);
 printf("%s\n\r\n\r", retriever_version);
 textcolor(15);
 printf("a game by Ingo Ruhnke\n\r");
 textcolor(7);
 printf("email: grumbel@gmx.de\n\r");
 textcolor(8);
 printf("www: http://home.pages.de/~retriever/\n\r");
 exit(error_code);
}

void arg_help()
{
 printf
     ("Retriever %s - Command-Line-Help", retriever_version);
 puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
 puts("-?                 zeigt diesen Hilfetext an");
 puts("-nosound           Sound ausschalten");
 puts("-nointro           Intro wird uebersprungen");
 puts("-showroomX         zeigt Informationen Åber den Raum X");
 puts("-midi [Dateiname]  abspielen eines beliebigen Midi-Files");
 puts("-novsync           schaltet vsync aus, schnellerer Spielablauf");
 puts("-rdf [Dateiname]   zubenuzendes RDF-File (default: RETRIEV.RDF)");
 puts("");
 puts("Kommentare und Vorschlaege an:");
 puts("  grumbel@gmx.de");
 puts("");
 puts("Retriever gibt es unter:");
 puts("  http://home.pages.de/~retriever/");
 puts("");
 puts("weitere Infos gibts in der README.TXT Datei\n");
 
 exit(0);
}

/* finds the first char in the a line  */
char first_char(const char *line)
{
 int i;
 for(i=0; !isgraph(line[i]) && line[i] != '\0'; ++i);

 return line[i];
}

bool is_empty(char *line)
{
 for (int i=0; line[i] != '\0'; ++i) {
    if (isgraph(line[i]))
      return 0;
 }

 return 1;
}

/* Reads a line from a file, it ignores a line with a ';' at
   the beginning (this feature is very buggy!!:-)  */
int getline(FILE *in, char *line)
{
 int c;
 int i;

 do {
   for(i=0; (c = fgetc(in)) != '\n' && c != EOF; ++i) {
      line[i] = c;
   }

   line[i] = '\0';

   for(i=0; line[i] != '\0'; ++i) {
      if (line[i] == COMMENT_CHAR) {
        line[i] = '\0';
        break;
      }
   }

   if (c == EOF) {
     return 0;
   }

 } while(is_empty(line));

 return 1;
}


