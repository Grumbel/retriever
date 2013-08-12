#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// structures
typedef struct objects {
 char  sprite_name[24];
 char  name[25];
 char  explanation[255];
 int   x1, y1, x2, y2;
 int   x_pos, y_pos, z_pos;
 int   width, heigth;
 int   type;
 int   status;
 int   new_room;
// the point where the guy is walking to if a object is the aim
 int   walk_to_x_pos, walk_to_y_pos;
} OBJECTS;

int getline(FILE *in, char *line)
{
 char c;
 int i;

 for (i=0; (c = fgetc(in)) != '\n' && c != EOF && c != '#'; ++i) {
     if (c == '/') {
        while ((c=fgetc(in)) != '\n');
        i = -1;
        continue;
     }
     line[i] = c;
 }
 line[i] = '\0';

 if (c == '#')
    return '#';
 if (c == EOF)
    return EOF;

 return 1;
}

int main()
{
 int i;
 FILE *in;
 char line[255];
 int c;
 int room = 0;
 OBJECTS object[15];
 int o_i = 0;
 char bg_name[50];
 char bg_map_name[50];

 in = fopen("retriev.rdf", "r");

// clrscr();

 while (getline(in, line) != '#');
 room = atoi(line);

 getline(in, bg_name);
 getline(in, bg_map_name);

 while(getline(in, line) != '#') {
   getline(in, object[o_i].sprite_name);
   getline(in, object[o_i].name);
   getline(in, object[o_i].explanation);
   getline(in, line);
     sscanf(line, "%d %d %d\n", &object[o_i].x_pos, &object[o_i].y_pos, &object[o_i].z_pos);
   getline(in, line);
     sscanf(line, "%d %d %d %d\n", &object[o_i].x1, &object[o_i].y1, &object[o_i].x2, &object[o_i].y2);
   getline(in, line);
     sscanf(line, "%d", &object[o_i].type);
   ++o_i;
 }

 printf("Room %d\n", room);
 printf("======\n");
 printf("Bkgrnd: %s\n", bg_name);
 printf("Map:    %s\n\n", bg_map_name);

 printf("Sprite: %s\n", object[0].sprite_name);
 printf("Name:   %s\n", object[0].name);
 printf("Expl.:  %s\n", object[0].explanation);
 printf("xPos:   %d\n", object[0].x_pos);
 printf("yPos:   %d\n", object[0].y_pos);
 printf("zPos:   %d\n", object[0].z_pos);
 printf("X1:     %d\n", object[0].x1);
 printf("Y1:     %d\n", object[0].y1);
 printf("X2:     %d\n", object[0].x2);
 printf("Y2:     %d\n", object[0].y2);
 printf("Type:   %d\n", object[0].type);

 fclose(in);
 exit(0);
}
