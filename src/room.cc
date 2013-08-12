#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "room.h"
#include "guy.h"
#include "objects.h"
#include "prototyp.h"
#include "function.h"
#include "dos_func.h"
#include "add_aleg.h"

void rooms::change_to()
{
  RGB palette[256];

  fade_out(5);

  destroy_bitmap(map);

  if ((map = load_pcx(map_filename, palette)) == NULL) {
    allegro_exit();
    out_of_memory();
    exit(EXIT_FAILURE);
  }
  show_mouse(NULL);
  draw_pcx(screen, bkgrnd_name, 0, 0);

  guy1.get_z_pos();

  guy1.stop();

  force_draw();

  show_mouse(NULL);
  guy1.get_temp_bitmap();

  draw();
  show_mouse(screen);

  guy1.walking = FALSE;
  fade_in(screen_pal, 5);
}

void rooms::redraw()
{
 BITMAP *tmp_scr=create_bitmap(320,200);

 guy1.stop();

 draw_pcx(tmp_scr, bkgrnd_name, 0, 0);

 for (int i=0; i<number_of_objects; ++i) {
    if (object[i].z_pos < guy1.z_pos ) {
       if (object[i].sprite_name[0] != '-' && object[i].status == 0)
         draw_rle_sprite(tmp_scr,
			 object[i].rle_sprite,
			 object[i].x_pos,
			 object[i].y_pos);
    }
 }

 guy1.put_scr(tmp_scr);

 // draws objects before the guy
 for (int i=0; i<number_of_objects; ++i) {
    if (object[i].z_pos >= guy1.z_pos ) {
       if (object[i].sprite_name[0] != '-' && object[i].status == 0)
         draw_rle_sprite(tmp_scr,
			 object[i].rle_sprite,
			 object[i].x_pos,
			 object[i].y_pos);
    }
 }
 vsync();
 restore_scr(&tmp_scr);
}

/* force to draw all object in the room */
void rooms::force_draw()
{
 show_mouse(NULL);

 for (int i=0; i<number_of_objects; ++i) {
    if (object[i].sprite_name[0] != '-' && object[i].status == 0) {      
      draw_rle_sprite(screen,
		      object[i].rle_sprite,
		      object[i].x_pos,
		      object[i].y_pos
		      );
    }
 }

 show_mouse(screen);
}

/* draws all objects in the room and the guy in the correct order
 * it don't draw the background, only objects */
void rooms::draw()
{
   int i;
   show_mouse(NULL);

   guy1.put();

   // draws objects before he guy
   for (i=0; i<number_of_objects; ++i) {
      if (object[i].z_pos >= guy1.z_pos ) {
         if (object[i].sprite_name[0] != '-' && object[i].status == 0) {

            /* checks if the object collide with a object, so it
             * needs to be redrawn */
            if (rect_col(object[i].x_pos,
			 object[i].x_pos + object[i].width,
			 object[i].y_pos,
			 object[i].y_pos + object[i].heigth,
                         guy1.x_pos - SPRITE_DIV_WIDTH,
			 guy1.x_pos + SPRITE_DIV_WIDTH,
			 guy1.y_pos- SPRITE_HEIGTH, guy1.y_pos))
	      {
		draw_rle_sprite(screen,
				object[i].rle_sprite,
				object[i].x_pos,
				object[i].y_pos);

            }
          }
      }
   }
   show_mouse(screen);
}

/* Prints out all data that is stored in this class, needs
 *  a normal text-mode. */
void rooms::print_data()
{
   int i;
   printf ("%s\n", bkgrnd_name);
   printf ("%s\n", map_filename);
   printf ("Number of Objects: %d\n", number_of_objects);
   for (i=0; i<number_of_objects; ++i)
      printf ("%2d. %12s = %20s = X:%3d Y:%3d W:%3d H:%3d Z:%3d\n"
	      "    Type: %d NewRoom: %d\n",
              i,
              object[i].sprite_name,
              object[i].name,
              object[i].x1,
              object[i].y1,
              object[i].x2,
              object[i].y2,
              object[i].z_pos,
              object[i].type,
              object[i].next.room
              );
} // void print_data()

/* this loads the room data for act_room from filename    
 * for a description of the rdf-fileformat see README.TXT */
int rooms::get_data(char *filename, int act_room)
{
  std::cout << "rooms::get_data: " << filename << std::endl;

    FILE *in;
    char line[256];
    int  x, i=0;
//  bool cont=TRUE;

    if ((in = fopen(filename, "r")) != NULL) {
       while (getline(in, line)) {
          if (first_char(line) == '#') {
             sscanf(line, "#%d\n", &x);

             if (x == act_room) {
               getline(in, bkgrnd_name);
               getline(in, map_filename);

               for(i=0; getline(in, object[i].name); ++i) {

                  if (first_char(object[i].name) == '#') {
                    break;
                  }

                  getline(in, object[i].sprite_name);

                  getline(in, line);
		  object[i].explanation = (char *) malloc((strlen(line)+1)
							  * sizeof(char));
		  if (object[i].explanation == NULL) {
                    allegro_exit();
                    perror("Not enough memory for object_explanation\n");
                    exit(EXIT_FAILURE);
                  }
                  strcpy(object[i].explanation, line);

                  getline(in, line);
                  if (sscanf (line, "%d %d %d\n",
                              &object[i].x_pos,
                              &object[i].y_pos,
                              &object[i].z_pos
                              ) != 3) {
                    allegro_exit();
                    perror("X_POS, Y_POS oder Z_POS nicht gefunden.");
                    exit(EXIT_FAILURE);
                  }

                  getline(in, line);
                  sscanf (line, "%d %d %d %d\n",
                          &object[i].x1, &object[i].y1,
                          &object[i].x2, &object[i].y2
                          );

                  getline(in, line);
                  sscanf (line, "%d %d %d\n",
                          &object[i].walk_to_x,
                          &object[i].walk_to_y,
                          &object[i].walkto_direction
                          );

                  getline(in, line);
                  sscanf (line, "%d", &object[i].type);

                  switch (object[i].type) {
                    case 0:
                    case 2:		      
		      break;
		      
                    case 1:
		      if (sscanf(line, "%d %d %d %d\n",
				 &object[i].type,
				 &object[i].next.room,
				 &object[i].next.x_pos,
				 &object[i].next.y_pos) != 4) {
			allegro_exit();
			fprintf(stderr,
				"In Line %d Error with next_room\n",i);
			exit(EXIT_FAILURE);
		      }
		      break;
                    case 3:
		      sscanf(line,
			     "%d %s\n",
			     &object[i].type, object[i].view);
		      break;
                    default:
		      allegro_exit();
		      fprintf(stderr,
			      "Unkown object type.\n"
			      "object: %2s type: %2d\n",
			      object[i].name, object[i].type);
		      exit(EXIT_FAILURE);
		      break;
                  }
               }
               number_of_objects = i;
               break;
             }
          }
       }
       fclose(in);

       BITMAP *temp_sprite;
       RGB temp_sprite_pal[256];

       for(i=0; i<number_of_objects; ++i) 
       {
	 if (object[i].sprite_name[0] != '-') 
         {
           std::cout << "loading: " << object[i].sprite_name << std::endl;
	   if (!(temp_sprite = load_pcx(object[i].sprite_name, temp_sprite_pal))) 
           {
	     allegro_exit();
	     perror(object[i].sprite_name);
	     exit(EXIT_FAILURE);
	   }
	   
	   object[i].heigth = temp_sprite->h;
	   object[i].width  = temp_sprite->w;
	   
	   object[i].rle_sprite = get_rle_sprite(temp_sprite);
	   
	   destroy_bitmap(temp_sprite);
	 }
       }
       
       return 1;
    }
    allegro_exit();
    perror(filename);
    exit(EXIT_FAILURE);
} // int get_data(char *filename, int act_room)
