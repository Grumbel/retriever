#include <stdio.h>
#include "guy.h"
#include "room.h"
#include "objects.h"

void guys::set_aim(int x, int y)
{
  waiting = FALSE;
  standing = FALSE;
  
  if (current_object != KEINS) {
    x = room[actual_room].object[current_object].walk_to_x;
    y = room[actual_room].object[current_object].walk_to_y;
  }

  aim_x_pos = x;
  aim_y_pos = y;
  walking = TRUE;

  // this points the guy in the correct direction
  if (abs(aim_y_pos - y_pos) <= abs(aim_x_pos - x_pos)) {
    if (aim_x_pos < x_pos) {
      direction = LEFT;
    } else {
      direction = RIGHT;
    }
  } else {
    if (aim_y_pos > y_pos) {
      direction = DOWN;
    } else {
      direction = UP;
    }
  }
  
  walkto_x_pos = aim_x_pos;
  walkto_y_pos = aim_y_pos;
}

/* unused at the moment, should be  
 * there for a path routine */
void guys::get_walkto_pos()
{
  if (!path_walkable()) {
    //exit(EXIT_FAILURE);
  }
}

/* unused at the moment, should be  
 * there for a path routine */
bool guys::path_walkable()
{
  int x = x_pos;
  int y = y_pos;
  int y_s = 0;

  // line(screen, x, y, walkto_x_pos, walkto_y_pos, 100);

  while(x != walkto_x_pos || y != walkto_y_pos) {
    if (x < walkto_x_pos)
      ++x;
    if (x > walkto_x_pos)
      --x;
    if (y < walkto_y_pos)
      ++y_s;
    if (y > walkto_y_pos)
      --y_s;
    
    if (abs(y_s) > 1) {
      y += y_s / 2;
      y_s = 0;
    }
    //    putpixel(screen, x, y, rand()%155);
    //    rest(1);
    if (getpixel(map, x, y) == 0)
      return FALSE;
  }
  return TRUE;
}

void guys::set_pos(int x, int y)
{
  aim_x_pos = x;
  aim_y_pos = y;
  x_pos = x;
  y_pos = y;
  walking = FALSE;
}

int guys::get_z_pos()
{
  z_pos = getpixel(map, x_pos, y_pos);
  return z_pos;
}

/* lets the sprite walk to the aim position and stop it when its
 * at the right position or if it is on a zero z_pos */
void guys::goto_aim()
{
  temp_x_pos = x_pos;
  temp_y_pos = y_pos;

  if (walking) {
    if (x_pos < walkto_x_pos)
      ++x_pos;
    if (x_pos > walkto_x_pos)
      --x_pos;
    if (y_pos < walkto_y_pos)
      ++y_steps;
    if (y_pos > walkto_y_pos)
      --y_steps;

    if (abs(y_steps) > 1) {
      y_pos += y_steps / 2;
      y_steps = 0;
    }
  }

  if ((z_pos = getpixel(map, x_pos, y_pos)) == 0) {
    walking = FALSE;
    x_pos = aim_x_pos = walkto_x_pos = temp_x_pos;
    y_pos = aim_y_pos = walkto_y_pos = temp_y_pos;
    z_pos = getpixel(map, x_pos, y_pos);
    anim  = 0;
    steps = 0;
    if (walkto_object != KEINS)
      direction = room[actual_room].object[walkto_object].walkto_direction;
  }

  if (x_pos == aim_x_pos && y_pos == aim_y_pos) {
    walking = FALSE;
    anim  = 0;
    steps = 0;

    if (walkto_object != KEINS)
      direction = room[actual_room].object[walkto_object].walkto_direction;

    return;
  }

  if (x_pos == walkto_x_pos && y_pos == walkto_y_pos) {
    get_walkto_pos();
  }

  ++steps;

  if (steps > 4) {
    steps = 0;
    ++anim;
    if (anim >= SPRITE_FRAMES)
      anim = 0;
  }
}

void guys::stop()
{
  anim = 0;
  temp_x_pos = x_pos;
  temp_y_pos = y_pos;
}

/* some funny animation, if the guy stands
 * for some time on the same place */
void guys::stand()
{
  if (!waiting) {
    waiting = TRUE;
    direction = FUN;
    standing_time = uhr;
    waiting_anim = (random() % 2) * 2;
  }

  if (waiting && ((uhr - standing_time) > (random()%50)+15))
    standing = TRUE;

  if (standing && uhr - standing_time > (waiting_anim?1:2)) {
    standing_time = uhr;

    if (++anim > (1 + waiting_anim)) {
      anim = waiting_anim;
    }
  
    direction = FUN;
    temp_x_pos = x_pos;
    temp_y_pos = y_pos;

    if (do_vsync)
      vsync();

    room[actual_room].draw();

    if (random()%10 == 1) {
      stop();
      direction = DOWN;
      waiting = FALSE;
      standing = FALSE;
      room[actual_room].draw();
    }
  }
}

/* paint the take anim, maybe this should
 * be in a own animation function */
void guys::take()
{
  int temp_direction = direction;
  show_mouse(NULL);

  direction = TAKE_ANIM;
  stop();
  put();
  rest(200);
  anim = 1;
  put();
  rest(200);
  anim = 0;
  put();
  rest(200);
  direction = temp_direction;

  show_mouse(screen);
}

void guys::put()
{
  remove();
  stretch_sprite(screen,
		 sprite[direction][anim],
		 x_pos - SPRITE_DIV_WIDTH * z_pos / 255,
		 y_pos - SPRITE_HEIGTH    * z_pos / 255,
		 SPRITE_WIDTH  * z_pos / 255,
		 SPRITE_HEIGTH * z_pos / 255);
}

void guys::put_scr(BITMAP *scr)
{
  get_temp_bitmap_scr(scr);
  stretch_sprite(scr,
		 sprite[direction][anim],
		 x_pos - SPRITE_DIV_WIDTH * z_pos / 255,
		 y_pos - SPRITE_HEIGTH    * z_pos / 255,
		 SPRITE_WIDTH  * z_pos / 255,
		 SPRITE_HEIGTH * z_pos / 255);
}

void guys::put_direct()
{
  stretch_sprite(screen,
		 sprite[direction][anim],
		 x_pos - SPRITE_DIV_WIDTH * z_pos / 255,
		 y_pos - SPRITE_HEIGTH    * z_pos / 255,
		 SPRITE_WIDTH  * z_pos / 255,
		 SPRITE_HEIGTH * z_pos / 255);
}

void guys::remove()
{
  // put the temp bitmap
  blit (temp,
	screen,
	0,
	0,
	temp_x_pos - SPRITE_DIV_WIDTH,
	temp_y_pos - SPRITE_HEIGTH,
	SPRITE_WIDTH,
	SPRITE_HEIGTH);

  // save the temp bitmap
  /*
    blit (screen,
       temp,
        x_pos - SPRITE_DIV_WIDTH,
        y_pos - SPRITE_HEIGTH,
	0,
	0,
	SPRITE_WIDTH,
	SPRITE_HEIGTH);
	*/
  get_temp_bitmap();
}

void guys::get_temp_bitmap()
{
  blit (screen,
	temp,
	x_pos - SPRITE_DIV_WIDTH,
	y_pos - SPRITE_HEIGTH,
	0,
	0,
	SPRITE_WIDTH,
	SPRITE_HEIGTH);
}

void guys::get_temp_bitmap_scr(BITMAP *scr)
{
  blit (scr,
	temp,
	x_pos - SPRITE_DIV_WIDTH,
	y_pos - SPRITE_HEIGTH,
	0,
	0,
	SPRITE_WIDTH,
	SPRITE_HEIGTH);
}


void guys::init()
{
  int x;
  int y;
  temp = create_bitmap(SPRITE_WIDTH, SPRITE_HEIGTH);
    
  // load sprite bitmaps
  sprite[RIGHT][0] = load_pcx("anim/mogli_01.pcx", pal);
  sprite[RIGHT][1] = load_pcx("anim/mogli_02.pcx", pal);
  sprite[RIGHT][2] = load_pcx("anim/mogli_03.pcx", pal);
  sprite[RIGHT][3] = load_pcx("anim/mogli_04.pcx", pal);
  sprite[RIGHT][4] = load_pcx("anim/mogli_01.pcx", pal);
  sprite[RIGHT][5] = load_pcx("anim/mogli_06.pcx", pal);
  sprite[RIGHT][6] = load_pcx("anim/mogli_07.pcx", pal);
  sprite[RIGHT][7] = load_pcx("anim/mogli_08.pcx", pal);

  sprite[UP][0] = load_pcx("anim/mogli_20.pcx", pal);
  sprite[UP][1] = load_pcx("anim/mogli_21.pcx", pal);
  sprite[UP][2] = load_pcx("anim/mogli_22.pcx", pal);
  sprite[UP][3] = load_pcx("anim/mogli_21.pcx", pal);
  sprite[UP][4] = load_pcx("anim/mogli_20.pcx", pal);
  sprite[UP][5] = load_pcx("anim/mogli_23.pcx", pal);
  sprite[UP][6] = load_pcx("anim/mogli_24.pcx", pal);
  sprite[UP][7] = load_pcx("anim/mogli_23.pcx", pal);

  sprite[DOWN][0] = load_pcx("anim/mogli_10.pcx", pal);
  sprite[DOWN][1] = load_pcx("anim/mogli_11.pcx", pal);
  sprite[DOWN][2] = load_pcx("anim/mogli_12.pcx", pal);
  sprite[DOWN][3] = load_pcx("anim/mogli_11.pcx", pal);
  sprite[DOWN][4] = load_pcx("anim/mogli_10.pcx", pal);
  sprite[DOWN][5] = load_pcx("anim/mogli_13.pcx", pal);
  sprite[DOWN][6] = load_pcx("anim/mogli_14.pcx", pal);
  sprite[DOWN][7] = load_pcx("anim/mogli_13.pcx", pal);

  sprite[FUN][0] = load_pcx("anim/mogli_30.pcx", pal);
  sprite[FUN][1] = load_pcx("anim/mogli_31.pcx", pal);
  sprite[FUN][2] = load_pcx("anim/mogli_32.pcx", pal);
  sprite[FUN][3] = load_pcx("anim/mogli_33.pcx", pal);
  sprite[FUN][4] = load_pcx("anim/mogli_30.pcx", pal);
  sprite[FUN][5] = load_pcx("anim/mogli_31.pcx", pal);
  sprite[FUN][6] = load_pcx("anim/mogli_32.pcx", pal);
  sprite[FUN][7] = load_pcx("anim/mogli_33.pcx", pal);

  sprite[TAKE_ANIM][0] = load_pcx("anim/mogli_40.pcx", pal);
  sprite[TAKE_ANIM][1] = load_pcx("anim/mogli_41.pcx", pal);

  //check if all sprites are loaded
  for(x=UP; x<=DOWN; ++x) {
    for(y=0; y<8; ++y) {
      if (sprite[x][y] == NULL) {
	allegro_exit();
	perror("Sprite not found.");
	exit(EXIT_FAILURE);
      }
    }
  }

  // create left version of the right sprite
  for (int i=0; i<SPRITE_FRAMES; ++i) {
    sprite[LEFT][i] = create_bitmap(26, 42);
    clear(sprite[LEFT][i]);
    draw_sprite_h_flip(sprite[LEFT][i], sprite[RIGHT][i], 0, 0);
  }
   
  blit (screen,
	temp,
	x_pos-SPRITE_DIV_WIDTH,
	y_pos-SPRITE_HEIGTH,
	0,
	0,
	SPRITE_WIDTH,
	SPRITE_HEIGTH);
  
   // tempor„t weil constructor nicht funkt?!
   x_pos = temp_x_pos = aim_y_pos = 120;
   y_pos = temp_y_pos = aim_x_pos = 150;
   walking = FALSE;
   anim = 0;
   steps= 0;
}

