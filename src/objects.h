/* object numbers, they are not longer uses  */
#define KEINS            (-1)
#define SAVEGAME_PATH "sav/"

#define SCR_WIDTH  320
#define SCR_HEIGHT 200
#define NUMBER_OF_ROOMS 9

/* object types  */
#define DOOR     1
#define TAKEABLE 2
#define VIEWABLE 3


/* direction for the guy  */
#define RIGHT 1
#define LEFT  3
#define UP    0
#define DOWN  2
#define FUN   4 /* animation if the guy is standing long enough  */
#define TAKE_ANIM 5

/* events  */
#define NONE   0
#define USE    1
#define WATCH  2
#define TAKE   3
#define INV    4
#define WALKTO 5

#define out_of_memory() fprintf(stderr, "out of memory!\n");

// extern variables
extern BITMAP *map;
extern RGB grey[256];
extern RGB screen_pal[256];
extern class rooms room[NUMBER_OF_ROOMS];
extern class guys guy1;
extern int actual_room;
extern int inventory[15][2];
extern int current_object, last_object, walkto_object;
extern volatile int uhr;
extern bool do_vsync;
extern char retriever_version[];

