#include <allegro.h>

/* the width and heigth of  
 * the bitmap of guy1       */
#define SPRITE_FRAMES 8
#define SPRITE_WIDTH 26
#define SPRITE_DIV_WIDTH 13
#define SPRITE_HEIGTH 43


/* This class hold all data which belong to a guy, it has also
 * the functions which were needed to put and remove the sprite
 */
class guys
{
public:
 RGB  pal[256];   // palette of the guy sprite (useless, because b/w)
 int  x_pos;      // current position
 int  y_pos;      // points between his feeds
 int  z_pos;      // z_pos of guy, is needed for object order
 int  y_steps;    // usless i think ?!
 int  temp_x_pos; // last position before moving
 int  temp_y_pos; // last position before moving
 int  aim_x_pos;  // position where the guy is walking to
 int  aim_y_pos;  // position where the guy is walking to
 int  walkto_x_pos;
 int  walkto_y_pos;
 int  anim;       // current guy pcx which is drawn
 int  steps;      // useless i think ?!
 bool walking;    // tells if the guy is walking or standing at the moment
 bool standing;   // the time
 bool waiting;
 char waiting_anim;
 int standing_timer;
 int              // how long the guy stands on the same position,
   standing_time; // needed for some fun animation, but not used, yet !!!!
 char direction;

 guys(int x, int y) {
    x_pos = temp_x_pos = aim_y_pos = x;
    y_pos = temp_y_pos = aim_x_pos = y;
    walking = 0;
    anim = 0;
    steps= 0;
 }

 // trash :)
 bool near_aim(int tol);

 void init();
 void set_aim(int x, int y);
 void set_pos(int x, int y);
 int  get_z_pos();
 void goto_aim();
 void stop();
 void put();
 void put_scr(BITMAP *scr);
 void take();
 void put_direct();
 void remove();
 void get_temp_bitmap();
 void guys::get_temp_bitmap_scr(BITMAP *scr);
 void stand();
 void get_walkto_pos();
 bool path_walkable();

private:
 BITMAP *sprite[6][8];
 BITMAP *temp;
}; /* class guys */

