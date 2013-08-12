#include <allegro.h>

// structures
typedef struct new_room {
 int room;  // this number shows the next room
 int x_pos; // startposition in the new_room
 int y_pos;
} NEW_ROOM;

typedef struct objects {
 char  sprite_name[80];     // name of the pcx_file of the object
 char  name[80];            // the name which is printed in the status line
 char  *explanation;        // this is printed out, if the object is whated
 int   x1, y1, x2, y2;      // rectangle to check which object is focused
 int   x_pos, y_pos, z_pos; // x and y position of th graphics, z pos for 3D
 int   width, heigth;       // object width and heigth, is this used ?!?
 int   walk_to_x;           // the objects position specified as a
 int   walk_to_y;           // simple point to walk to
 int   walkto_direction;    // direction to look at the object
 int   type;                // the object type, watch rdf.doc
 int   status;              // status of object (1 there, 0 gone)
 NEW_ROOM next;
 char  view[80];            // pcx_file of object in 320x200
 RLE_SPRITE *rle_sprite;   // Sprite of the object in memory
} OBJECTS;


/*
 * This class gives all infos about the data which belong to a room.
 * Rooms can be loaded with get_data(char *filename, int actual_room)
 * from a file  (*.rdf - Retriever Data File)and contain up to 15 rooms
 */
class rooms {
public:
 char   bkgrnd_name[24];
 char   map_filename[24];
 OBJECTS object[15];
 int    number_of_objects;

 /* draws a room if it changed, but it draws only the backgrounds */
 void change_to();

 /* draws all objects in the room and the guy in the correct order
    it don't draw the background, only objects */
 void draw();

 /* draws all objects in the room, but don't cares about the guy */
 void force_draw();

 void redraw();

 /* Prints out all data that is stored in this class, needs
    a normal text-mode. */
 void print_data();
 int get_data(char *filename, int actual_room);
};// class rooms

extern class guys guy1; //(120,150)
