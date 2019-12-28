#include "pandorasdk.h"
//#include "gp2xmemfuncs.h"
#include "burner.h"
#include "config.h"
#include "snd.h"
#include <getopt.h>
#include "fba_player.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <math.h>

#include "font.h"

#define BLOCKSIZE 1024
#define SetTaken(Start, Size) TakenSize[(Start - 0x2000000) / BLOCKSIZE] = (Size - 1) / BLOCKSIZE + 1

//extern CFG_OPTIONS config_options;

extern char **environ;

extern CFG_OPTIONS config_options;
extern CFG_KEYMAP config_keymap;


//static int mem_fd = -1;
//void *UpperMem;
//int TakenSize[0x2000000 / BLOCKSIZE];
unsigned short *VideoBuffer = NULL;
static int screen_mode = 0;
//volatile static unsigned short *gp2xregs = NULL;
//unsigned long gp2x_physvram[4]={0,0,0,0};
//unsigned short *framebuffer[4]={0,0,0,0};


char LEFTDOWN,RIGHTDOWN,ADOWN,BDOWN,XDOWN,YDOWN,UPDOWN,DOWNDOWN,STARTDOWN,SELECTDOWN,LSDOWN,RSDOWN,QDOWN,VUDOWN,VDDOWN,PAUSEDOWN=0;
int kinput=0;

int WINDOW_WIDTH;
int WINDOW_HEIGHT;


static int currentframebuffer = 0;
//struct usbjoy *joys[4];
char joyCount = 0;

SDL_Joystick *joys[4];
const char* WINDOW_TITLE = "FBA";

SDL_Surface* myscreen;
SDL_Surface* framebuffer[4];
SDL_Surface* SDL_VideoBuffer;

/***** karin zhao -> virtual buttons surface *****/
#ifdef _HARMATTAN
#define USING_INFOBANNER
#endif
#define KZ_MIN(a, b) ((a) > (b) ? (b) : (a))

extern bool bPauseOn;

extern KZ_GetOpt_t harm_options;
char harm_CURRENT_PLAYER = 0; // 0, 1, 2, 3
char harm_CONTROL_TOGETHER = 0; // 0, 1, 2
char harm_SELECT = 0;

#define KZ_FBA_PATH ".fba"
#define KZ_STATE_PATH KZ_FBA_PATH "/states"
#define KZ_STATE_SUFFIX "_default_state"
#define KZ_STATE_EXTENSION ".fs"

#define KZ_ACTION_BUTTON 0
#define KZ_ACTION_SWITCH 1
#define KZ_ACTION_DISABLED -1

#ifndef _RESC_DIR
#ifdef _IN_QFBACAPEX
#define _RESC_DIR "/opt/qmlfbacapex4harmattan/fbalpha"
#else
#define _RESC_DIR "/usr/share/fba"
#endif
#endif

// width of virtual button
#define VBUTTONWIDTH 64
#define VBUTTONSPACING 20

#define VBS(x) (VBUTTONSPACING * (x))
#define VBW(x) (VBUTTONWIDTH * (x))

#define BUTTON_SPRITE_WIDTH 64

// screen width and height of MeeGo Harmattan devices(Nokia N9 and N950)
#define N950SCREENHEIGHT 480
#define N950SCREENWIDTH 854
//big circle 283x283 center point (142,142)
//x offset=50 y offset=30
#define CIRCLECENTERX 182
#define CIRCLECENTERY 308
#define JOYSTICKOFFSET 70
//small circle 139x139 center point (70,70)
//x offset=69 y offset=69
#define BIGCIRCLEWIDTH 283
#define SMALLCIRCLEWIDTH 139
#define MYARG 30 // 22.5

#define CIRCLEHALFLENGTH 185.0
#define CIRCLEHALFLENGTHWITHOUTLISTENER 40.0 // 25.0
#define TRANSLUCENTLEVEL 157

#define UPBUTTONX 192
#define DOWNBUTTONX 320
#define LEFTBUTTONX 128
#define RIGHTBUTTONX 256
#define FIRE1BUTTONX 384
#define FIRE2BUTTONX 448
#define FIRE3BUTTONX 448
#define FIRE4BUTTONX 0
#define FIRE5BUTTONX 64
#define FIRE6BUTTONX 384
#define STARTBUTTONX 320
#define COINBUTTONX 128
#define PAUSEBUTTONX 256
#define QUITBUTTONX 64

#define UPBUTTONRELESEDY 0
#define DOWNBUTTONRELESEDY 0
#define LEFTBUTTONRELESEDY 0
#define RIGHTBUTTONRELESEDY 0
#define FIRE1BUTTONRELESEDY 0
#define FIRE2BUTTONRELESEDY 0
#define FIRE3BUTTONRELESEDY 256
#define FIRE4BUTTONRELESEDY 128
#define FIRE5BUTTONRELESEDY 128
#define FIRE6BUTTONRELESEDY 256
#define STARTBUTTONRELESEDY 256
#define COINBUTTONRELESEDY 128
#define PAUSEBUTTONRELESEDY 256
#define QUITBUTTONRELESEDY 0

#define UPBUTTONPRESSEDY 64
#define DOWNBUTTONPRESSEDY 64
#define LEFTBUTTONPRESSEDY 64
#define RIGHTBUTTONPRESSEDY 64
#define FIRE1BUTTONPRESSEDY 64
#define FIRE2BUTTONPRESSEDY 64
#define FIRE3BUTTONPRESSEDY 320
#define FIRE4BUTTONPRESSEDY 192
#define FIRE5BUTTONPRESSEDY 192
#define FIRE6BUTTONPRESSEDY 320
#define STARTBUTTONPRESSEDY 320
#define COINBUTTONPRESSEDY 192
#define PAUSEBUTTONPRESSEDY 320
#define QUITBUTTONPRESSEDY 64

#define BIGCIRCLEX 0
#define BIGCIRCLEY 0
#define SMALLCIRCLEX 294
#define SMALLCIRCLEY 89

#define VB_SPRITE_X(x) (64 * (x))
#define VB_SPRITE_Y(x) (64 * (x))

#define PLAYER_SWITCH_BUTTON_X VB_SPRITE_X(5)
#define PLAYER_SWITCH_BUTTON_Y VB_SPRITE_Y(2)
#define PLAYER_SWITCH_BUTTON_PRESSED_X VB_SPRITE_X(7)
#define PLAYER_SWITCH_BUTTON_PRESSED_Y VB_SPRITE_Y(3)
#define CONTROL_SWITCH_BUTTON_X VB_SPRITE_X(3)
#define CONTROL_SWITCH_BUTTON_Y VB_SPRITE_Y(4)
#define CONTROL_SWITCH_BUTTON_PRESSED_X VB_SPRITE_X(1)
#define CONTROL_SWITCH_BUTTON_PRESSED_Y VB_SPRITE_Y(5)
#define STATE_LOAD_BUTTON_X VB_SPRITE_X(1)
#define STATE_LOAD_BUTTON_Y VB_SPRITE_Y(4)
#define STATE_LOAD_BUTTON_PRESSED_X VB_SPRITE_X(1)
#define STATE_LOAD_BUTTON_PRESSED_Y VB_SPRITE_Y(5)
#define STATE_SAVE_BUTTON_X VB_SPRITE_X(2)
#define STATE_SAVE_BUTTON_Y VB_SPRITE_Y(4)
#define STATE_SAVE_BUTTON_PRESSED_X VB_SPRITE_X(1)
#define STATE_SAVE_BUTTON_PRESSED_Y VB_SPRITE_Y(5)
#define SELECT_BUTTON_X VB_SPRITE_X(3)
#define SELECT_BUTTON_Y VB_SPRITE_Y(2)
#define SELECT_BUTTON_PRESSED_X VB_SPRITE_X(3)
#define SELECT_BUTTON_PRESSED_Y VB_SPRITE_Y(3)

#ifdef USING_INFOBANNER
#define KZ_MAX_INFO_BANNER_TEXT_LENGTH 512

#define KZ_INFOBANNER_STATUS_CLOSED 0 
#define KZ_INFOBANNER_STATUS_READY 1
#define KZ_INFOBANNER_STATUS_OPENED 2

typedef struct KZ_InfoBanner_s
{
	int timeout; // seconds
	char text[KZ_MAX_INFO_BANNER_TEXT_LENGTH + 1];

	int font_width, font_height; // font size
	int x, y; // coord
	int start_time; // time()
	int status; // visible
} KZ_InfoBanner_t;

static KZ_InfoBanner_t infobanner = {
	3, {0},
	
	8, 9,
	0, 0,
	0, KZ_INFOBANNER_STATUS_CLOSED
};

extern int fwidth, fheight;
extern int *harm_PhysicalBufferWidth_ptr;
static void harm_show_info_banner(const char *text)
{
	infobanner.status = KZ_INFOBANNER_STATUS_READY;
	strncpy(infobanner.text, text, KZ_MAX_INFO_BANNER_TEXT_LENGTH);
	infobanner.y = 0;
	infobanner.x = *harm_PhysicalBufferWidth_ptr / 2 - infobanner.font_width * strlen(infobanner.text) / 2;
	infobanner.start_time = time(NULL);
}

static inline void harm_render_info_banner(void)
{
	if(infobanner.status == KZ_INFOBANNER_STATUS_CLOSED)
		return;

	if(infobanner.status == KZ_INFOBANNER_STATUS_READY)
	{
		infobanner.status = KZ_INFOBANNER_STATUS_OPENED;
		infobanner.start_time = time(NULL);
	}
	else// if(infobanner.status == KZ_INFOBANNER_STATUS_OPENED)
	{
		if(time(NULL) - infobanner.start_time > infobanner.timeout)
		{
			infobanner.status = KZ_INFOBANNER_STATUS_CLOSED;
			return;
		}
		//DrawString (infobanner.text, (uint16 *)SDL_VideoBuffer->pixels, infobanner.x, infobanner.y, fwidth);
		DrawString (infobanner.text, (unsigned short *) &VideoBuffer[0], infobanner.x, infobanner.y,*harm_PhysicalBufferWidth_ptr);
	}
}
#endif

enum PointInCircle{
	OutCircle=0,
	InCircleAndCenter,
	InCircleButCenter
};

typedef struct{
	Sint16 x1;
	Sint16 y1;
	Sint16 x2;
	Sint16 y2;
}KZ_Rect;

typedef struct KZ_Point_s
{
	Sint16 x, y;
} KZ_Point_t;

typedef struct KZ_Circle_s
{
	struct KZ_Point_s center;
	Uint16 radius;
	Uint16 inside_radius;
} KZ_Circle_t;

typedef void (* KZ_ActionEvent_f)(char *ptr, bool pressed);

// button action type
typedef enum KZ_Button_Type_e
{
	Up_Button = 0,
	Down_Button,
	Left_Button,
	Right_Button,

	Coin_Button,
	Start_Button,
	Fire1_Button,
	Fire2_Button,
	Fire3_Button,
	Fire4_Button,
	Fire5_Button,
	Fire6_Button,
	Pause_Button,
	Quit_Button,

	Select_Button,

	// specials
	StateLoadButton,
	StateSaveButton,

	PlayerChangeButton,
	ControlTogetherButton,

	Total_Button,
} KZ_Button_Type_t;

// virtual button
typedef struct KZ_VKB_s
{
	int action;
	int state;
	SDL_Rect button; // is screen position
	SDL_Rect sprite; // is image position
	SDL_Rect sprite_pressed; // is highlight image position
	KZ_Rect event; // is mouse event position
	char *ptr;
	char type;
} KZ_VKB_t;

typedef struct KZ_JoyStick_s
{
	SDL_Rect big_button; // is screen position
	SDL_Rect small_button; // is screen position
	SDL_Rect big_sprite; // is image position
	SDL_Rect small_sprite; // is image position
	KZ_Rect event; // is mouse event position
	struct KZ_Circle_s circle;
	char *ptr[4]; // up down, left, right
} KZ_JoyStick_t;

typedef enum KZ_Position_Baseline_Type_e
{
	Position_Baseline_Begin = 0,
	Position_Baseline_End,
	Position_Baseline_Middle_Front,
	Position_Baseline_Middle_Back,
} KZ_Position_Baseline_Type_t;

// range of mouse click
static KZ_VKB_t vkb[Total_Button];
static KZ_JoyStick_t joy;

// surface of virtual button
/*
	 static SDL_Surface *up_button=NULL;
	 static SDL_Surface *down_button=NULL;
	 static SDL_Surface *left_button=NULL;
	 static SDL_Surface *right_button=NULL;
	 */
/*
	 static SDL_Surface *fire1_button=NULL;
	 static SDL_Surface *fire2_button=NULL;
	 static SDL_Surface *fire3_button=NULL;
	 static SDL_Surface *fire4_button=NULL;
	 static SDL_Surface *fire5_button=NULL;
	 static SDL_Surface *fire6_button=NULL;
	 static SDL_Surface *start_button=NULL;
	 static SDL_Surface *coin_button=NULL;
	 static SDL_Surface *pause_button=NULL;
	 static SDL_Surface *quit_button=NULL;
	 static SDL_Surface *big_circle=NULL;
	 static SDL_Surface *small_circle=NULL;
	 */

static SDL_Surface *joystick_surface=NULL;
static SDL_Surface *buttons_surface=NULL;

//inline static bool checkClickedRange(Uint16 mx,Uint16 my,Sint16 x,Sint16 y,Uint16 w,Uint16 h);
//inline static bool checkClickedRange(Uint16 mx,Uint16 my,const SDL_Rect *rect);
inline static bool checkClickedRange(Uint16 mx,Uint16 my,const KZ_Rect *rect)
{
	return ((mx >= rect->x1 && my >= rect->y1) && (mx < rect->x2 && my < rect->y2));
}

inline static PointInCircle checkPointPostion(Uint16 x,Uint16 y, const KZ_Circle_t *circle)
{
	float a=(x- circle->center.x);
	float b=(y- circle->center.y);
	float c=sqrt(a*a+b*b);
	//printf("a=%f ,b=%f ,c=%f\n",a,b,c);
	if(c>circle->radius)
		return OutCircle;
	else if(c<circle->inside_radius)
		return InCircleAndCenter;
	else
		return InCircleButCenter;
}

inline static float mycast(Uint16 x,Uint16 y, const KZ_Point_t *point)
{
	int a=(x- point->x);
	int b=(y- point->y);
	/*
		 if(a==0 && b==0)
		 return -1.0;
		 */
	if(a==0 && b>0)
		return 270.0;
	else if(a==0 && b<0)
		return 90.0;
	else if(b==0 && a>0)
		return 0.0;
	else if(b==0 && a<0)
		return 180.0;
	float c=sqrt(a*a+b*b);
	//bxb+bxb/2bc
	float arg=acos(abs(a)/c)*(180/M_PI);
	//printf("%d, %f -> %f\n", a, b, arg);
	if(a>0 && b>0)
	{
		//printf("> %f\n",360.0-arg);
		return 360.0-arg;
	}
	if(a>0 && b<0)
	{
		//printf("< %f\n",arg);
		return arg;
	}
	if(a<0 && b>0)
	{
		//printf("< %f\n",180.0+arg);
		return 180.0+arg;
	}
	if(a<0 && b<0)
	{
		//printf("< %f\n",180.0-arg);
		return 180.0-arg;
	}
	/*
		 if(arg<90.0-MYARG)
		 DOWNDOWN=1;
		 if(arg>90.0+MYARG)
		 UPDOWN=1;
		 if(arg>MYARG && arg < 180.0-MYARG && x>CIRCLECENTERX)
		 RIGHTDOWN=1;
		 if(arg>MYARG && arg < 180.0-MYARG && x<CIRCLECENTERX)
		 LEFTDOWN=1;
		 */
}

static void init_vkb(int width, int height)
{
#define _NAV_OVERFLOW 0 // 0.5
	struct KZ_VKB_Init_Data_s
	{
		Sint16 x, y, w, h;
		Sint16 ex, ey, ew, eh;
		Sint16 sx, sy, sw, sh;
		Sint16 sx_pressed, sy_pressed;
		//KZ_Position_Baseline_Type_t xb, yb;
		int xb, yb;
		char *b;
		char t;
	} vkb_init_data[][Total_Button] = {
		{
			// up down left right
			{
				VBW(1) + VBS(4), VBW(3) + VBS(5), VBUTTONWIDTH, VBUTTONWIDTH,
				VBS(2), VBW(3) + VBS(5), VBW(3) + VBS(4), VBUTTONWIDTH + VBS(1 - _NAV_OVERFLOW),
				UPBUTTONX, UPBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				UPBUTTONX, UPBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_End,
				&UPDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(1) + VBS(4), VBW(1) + VBS(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBS(2), VBW(1) + VBS(2 - _NAV_OVERFLOW), VBW(3) + VBS(4), VBUTTONWIDTH + VBS(1 - _NAV_OVERFLOW),
				DOWNBUTTONX, DOWNBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				DOWNBUTTONX, DOWNBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_End,
				&DOWNDOWN, KZ_ACTION_BUTTON
			},
			{
				VBS(2), VBW(2) + VBS(3), VBUTTONWIDTH, VBUTTONWIDTH,
				VBS(2), VBW(3) + VBS(5), VBUTTONWIDTH + VBS(1 - _NAV_OVERFLOW), VBW(3) + VBS(4),
				LEFTBUTTONX, LEFTBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				LEFTBUTTONX, LEFTBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_End,
				&LEFTDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(2) + VBS(6), VBW(2) + VBS(3), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(5 + _NAV_OVERFLOW), VBW(3) + VBS(5), VBUTTONWIDTH + VBS(1 - _NAV_OVERFLOW), VBW(3) + VBS(4),
				RIGHTBUTTONX, RIGHTBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				RIGHTBUTTONX, RIGHTBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_End,
				&RIGHTDOWN, KZ_ACTION_BUTTON
			},
			// start coin
			{
				VBW(1) + VBS(1), VBW(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1) + VBS(1), VBW(1), VBUTTONWIDTH, VBUTTONWIDTH,
				COINBUTTONX, COINBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				COINBUTTONX, COINBUTTONPRESSEDY,
				Position_Baseline_Middle_Front,
				Position_Baseline_End,
				&SELECTDOWN, KZ_ACTION_BUTTON
			},
			{
				VBS(1), VBW(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBS(1), VBW(1), VBUTTONWIDTH, VBUTTONWIDTH,
				STARTBUTTONX, STARTBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				STARTBUTTONX, STARTBUTTONPRESSEDY,
				Position_Baseline_Middle_Back,
				Position_Baseline_End,
				&STARTDOWN, KZ_ACTION_BUTTON
			},
			// A B | 1 2
			{
				VBW(2) + VBS(5), VBW(1) + VBS(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(5), VBW(1) + VBS(1 + 2.25), VBW(1) + VBS(2.25), VBW(1) + VBS(2.25),
				FIRE1BUTTONX, FIRE1BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE1BUTTONX, FIRE1BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&ADOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(1) + VBS(2), VBW(1) + VBS(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1) + VBS(2 + 2.25), VBW(1) + VBS(1 + 2.25), VBW(1) + VBS(2.25), VBW(1) + VBS(2.25),
				FIRE2BUTTONX, FIRE2BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE2BUTTONX, FIRE2BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&XDOWN, KZ_ACTION_BUTTON
			},
			// C, X | 3, 4
			{
				VBW(2) + VBS(5), VBW(2) + VBS(4), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(5), VBW(2) + VBS(4), VBW(1) + VBS(2.25), VBW(1) + VBS(2.25),
				FIRE3BUTTONX, FIRE3BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE3BUTTONX, FIRE3BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&BDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(1) + VBS(2), VBW(2) + VBS(4), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1) + VBS(2 + 2.25), VBW(2) + VBS(4), VBW(1) + VBS(2.25), VBW(1) + VBS(2.25),
				FIRE4BUTTONX, FIRE4BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE4BUTTONX, FIRE4BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&YDOWN, KZ_ACTION_BUTTON
			},
			// Y Z | 5 6
			{
				VBW(2) + VBS(5), VBW(3) + VBS(7), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(5), VBW(3) + VBS(7), VBUTTONWIDTH, VBUTTONWIDTH,
				FIRE5BUTTONX, FIRE5BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE5BUTTONX, FIRE5BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&LSDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(1) + VBS(2), VBW(3) + VBS(7), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1) + VBS(2), VBW(3) + VBS(7), VBUTTONWIDTH, VBUTTONWIDTH,
				FIRE6BUTTONX, FIRE6BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE6BUTTONX, FIRE6BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&RSDOWN, KZ_ACTION_BUTTON
			},
			// pause quit
			{
				0, 0, VBUTTONWIDTH, VBUTTONWIDTH,
				0, 0, VBUTTONWIDTH, VBUTTONWIDTH,
				PAUSEBUTTONX, PAUSEBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				PAUSEBUTTONX, PAUSEBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_Begin,
				&PAUSEDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(1), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				QUITBUTTONX, QUITBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				QUITBUTTONX, QUITBUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_Begin,
				&QDOWN, KZ_ACTION_BUTTON
			},
			{
#if 0
				VBW(0.5), VBW(2) + VBS(0), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(0.5), VBW(2) + VBS(0), VBUTTONWIDTH, VBUTTONWIDTH,
				SELECT_BUTTON_X, SELECT_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				SELECT_BUTTON_PRESSED_X, SELECT_BUTTON_PRESSED_Y,
				Position_Baseline_Middle_Front,
				Position_Baseline_End,
#endif
				VBW(1) + VBS(2), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1) + VBS(2), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				SELECT_BUTTON_X, SELECT_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				SELECT_BUTTON_PRESSED_X, SELECT_BUTTON_PRESSED_Y,
				Position_Baseline_Begin,
				Position_Baseline_Begin,
				&harm_SELECT, KZ_ACTION_BUTTON
			},
			// specials
			{
				VBW(3) + VBS(3), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(3) + VBS(3), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				STATE_LOAD_BUTTON_X, STATE_LOAD_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				STATE_LOAD_BUTTON_PRESSED_X, STATE_LOAD_BUTTON_PRESSED_Y,
				Position_Baseline_End,
				Position_Baseline_Begin,
				NULL, KZ_ACTION_BUTTON
			},
			{
				VBW(2) + VBS(2), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(2), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				STATE_SAVE_BUTTON_X, STATE_SAVE_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				STATE_SAVE_BUTTON_PRESSED_X, STATE_SAVE_BUTTON_PRESSED_Y,
				Position_Baseline_End,
				Position_Baseline_Begin,
				NULL, KZ_ACTION_BUTTON
			},
			{
				VBW(2) + VBS(3), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(3), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				PLAYER_SWITCH_BUTTON_X, PLAYER_SWITCH_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				PLAYER_SWITCH_BUTTON_PRESSED_X, PLAYER_SWITCH_BUTTON_PRESSED_Y,
				Position_Baseline_Begin,
				Position_Baseline_Begin,
				&harm_CURRENT_PLAYER, KZ_ACTION_SWITCH
			},
			{
				VBW(3) + VBS(4), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(3) + VBS(4), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				CONTROL_SWITCH_BUTTON_X, CONTROL_SWITCH_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				CONTROL_SWITCH_BUTTON_PRESSED_X, CONTROL_SWITCH_BUTTON_PRESSED_Y,
				Position_Baseline_Begin,
				Position_Baseline_Begin,
				&harm_CONTROL_TOGETHER, KZ_ACTION_SWITCH
			},
		}, // portrait

		{
			// up down left right
			{
				VBW(1) + VBS(4), VBW(3) + VBS(5), VBUTTONWIDTH, VBUTTONWIDTH,
				VBS(2), VBW(3) + VBS(5), VBW(3) + VBS(4), VBUTTONWIDTH + VBS(1 - _NAV_OVERFLOW),
				UPBUTTONX, UPBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				UPBUTTONX, UPBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_End,
				&UPDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(1) + VBS(4), VBW(1) + VBS(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBS(2), VBW(1) + VBS(2 - _NAV_OVERFLOW), VBW(3) + VBS(4), VBUTTONWIDTH + VBS(1 - _NAV_OVERFLOW),
				DOWNBUTTONX, DOWNBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				DOWNBUTTONX, DOWNBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_End,
				&DOWNDOWN, KZ_ACTION_BUTTON
			},
			{
				VBS(2), VBW(2) + VBS(3), VBUTTONWIDTH, VBUTTONWIDTH,
				VBS(2), VBW(3) + VBS(5), VBUTTONWIDTH + VBS(1 - _NAV_OVERFLOW), VBW(3) + VBS(4),
				LEFTBUTTONX, LEFTBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				LEFTBUTTONX, LEFTBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_End,
				&LEFTDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(2) + VBS(6), VBW(2) + VBS(3), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(5 + _NAV_OVERFLOW), VBW(3) + VBS(5), VBUTTONWIDTH + VBS(1 - _NAV_OVERFLOW), VBW(3) + VBS(4),
				RIGHTBUTTONX, RIGHTBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				RIGHTBUTTONX, RIGHTBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_End,
				&RIGHTDOWN, KZ_ACTION_BUTTON
			},
			// start coin
			{
				VBW(1) + VBS(1), VBW(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1) + VBS(1), VBW(1), VBUTTONWIDTH, VBUTTONWIDTH,
				COINBUTTONX, COINBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				COINBUTTONX, COINBUTTONPRESSEDY,
				Position_Baseline_Middle_Front,
				Position_Baseline_End,
				&SELECTDOWN, KZ_ACTION_BUTTON
			},
			{
				VBS(1), VBW(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBS(1), VBW(1), VBUTTONWIDTH, VBUTTONWIDTH,
				STARTBUTTONX, STARTBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				STARTBUTTONX, STARTBUTTONPRESSEDY,
				Position_Baseline_Middle_Back,
				Position_Baseline_End,
				&STARTDOWN, KZ_ACTION_BUTTON
			},
			// A B X | 1 2 5
			{
				VBW(3) + VBS(6.25), VBW(1) + VBS(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(3) + VBS(6.25), VBW(1) + VBS(3.25), VBW(1) + VBS(2.25), VBW(1) + VBS(2.25),
				FIRE1BUTTONX, FIRE1BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE1BUTTONX, FIRE1BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&ADOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(2) + VBS(3.25), VBW(1) + VBS(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(5.5), VBW(1) + VBS(3.25), VBW(1) + VBS(2.25), VBW(1) + VBS(2.25),
				FIRE2BUTTONX, FIRE2BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE2BUTTONX, FIRE2BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&XDOWN, KZ_ACTION_BUTTON
			},
			// C, Y, Z | 3, 4, 6
			{
				VBW(3) + VBS(6.25), VBW(2) + VBS(4), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(3) + VBS(6.25), VBW(2) + VBS(4), VBW(1) + VBS(2.25), VBW(1) + VBS(2.25),
				FIRE3BUTTONX, FIRE3BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE3BUTTONX, FIRE3BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&BDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(2) + VBS(3.25), VBW(2) + VBS(4), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(5.5), VBW(2) + VBS(4), VBW(1) + VBS(2.25), VBW(1) + VBS(2.25),
				FIRE4BUTTONX, FIRE4BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE4BUTTONX, FIRE4BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&YDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(1) + VBS(0.25), VBW(1) + VBS(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1) + VBS(0.25), VBW(1) + VBS(1), VBUTTONWIDTH, VBUTTONWIDTH,
				FIRE5BUTTONX, FIRE5BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE5BUTTONX, FIRE5BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&LSDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(1) + VBS(0.25), VBW(2) + VBS(4), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1) + VBS(0.25), VBW(2) + VBS(4), VBUTTONWIDTH, VBUTTONWIDTH,
				FIRE6BUTTONX, FIRE6BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE6BUTTONX, FIRE6BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&RSDOWN, KZ_ACTION_BUTTON
			},
			// pause quit
			{
				0, 0, VBUTTONWIDTH, VBUTTONWIDTH,
				0, 0, VBUTTONWIDTH, VBUTTONWIDTH,
				PAUSEBUTTONX, PAUSEBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				PAUSEBUTTONX, PAUSEBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_Begin,
				&PAUSEDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(1), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				QUITBUTTONX, QUITBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				QUITBUTTONX, QUITBUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_Begin,
				&QDOWN, KZ_ACTION_BUTTON
			},
			{
#if 0
				-VBW(0.5), VBW(2) + VBS(0), VBUTTONWIDTH, VBUTTONWIDTH,
				-VBW(0.5), VBW(2) + VBS(0), VBUTTONWIDTH, VBUTTONWIDTH,
				SELECT_BUTTON_X, SELECT_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				SELECT_BUTTON_PRESSED_X, SELECT_BUTTON_PRESSED_Y,
				Position_Baseline_Middle_Front,
				Position_Baseline_End,
#endif
				VBW(1) + VBS(2), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1) + VBS(2), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				SELECT_BUTTON_X, SELECT_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				SELECT_BUTTON_PRESSED_X, SELECT_BUTTON_PRESSED_Y,
				Position_Baseline_Begin,
				Position_Baseline_Begin,
				&harm_SELECT, KZ_ACTION_BUTTON
			},
			// specials
			{
				VBW(3) + VBS(3), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(3) + VBS(3), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				STATE_LOAD_BUTTON_X, STATE_LOAD_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				STATE_LOAD_BUTTON_PRESSED_X, STATE_LOAD_BUTTON_PRESSED_Y,
				Position_Baseline_End,
				Position_Baseline_Begin,
				NULL, KZ_ACTION_BUTTON
			},
			{
				VBW(2) + VBS(2), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(2), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				STATE_SAVE_BUTTON_X, STATE_SAVE_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				STATE_SAVE_BUTTON_PRESSED_X, STATE_SAVE_BUTTON_PRESSED_Y,
				Position_Baseline_End,
				Position_Baseline_Begin,
				NULL, KZ_ACTION_BUTTON
			},
			{
				VBW(2) + VBS(3), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(3), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				PLAYER_SWITCH_BUTTON_X, PLAYER_SWITCH_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				PLAYER_SWITCH_BUTTON_PRESSED_X, PLAYER_SWITCH_BUTTON_PRESSED_Y,
				Position_Baseline_Begin,
				Position_Baseline_Begin,
				&harm_CURRENT_PLAYER, KZ_ACTION_SWITCH
			},
			{
				VBW(3) + VBS(4), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(3) + VBS(4), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				CONTROL_SWITCH_BUTTON_X, CONTROL_SWITCH_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				CONTROL_SWITCH_BUTTON_PRESSED_X, CONTROL_SWITCH_BUTTON_PRESSED_Y,
				Position_Baseline_Begin,
				Position_Baseline_Begin,
				&harm_CONTROL_TOGETHER, KZ_ACTION_SWITCH
			},
		}, // landscape 2014

		{
			// up down left right
			{
				VBW(1) + VBS(4), VBW(3) + VBS(5), VBUTTONWIDTH, VBUTTONWIDTH,
				VBS(2), VBW(3) + VBS(5), VBW(3) + VBS(4), VBUTTONWIDTH + VBS(1 - _NAV_OVERFLOW),
				UPBUTTONX, UPBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				UPBUTTONX, UPBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_End,
				&UPDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(1) + VBS(4), VBW(1) + VBS(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBS(2), VBW(1) + VBS(2 - _NAV_OVERFLOW), VBW(3) + VBS(4), VBUTTONWIDTH + VBS(1 - _NAV_OVERFLOW),
				DOWNBUTTONX, DOWNBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				DOWNBUTTONX, DOWNBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_End,
				&DOWNDOWN, KZ_ACTION_BUTTON
			},
			{
				VBS(2), VBW(2) + VBS(3), VBUTTONWIDTH, VBUTTONWIDTH,
				VBS(2), VBW(3) + VBS(5), VBUTTONWIDTH + VBS(1 - _NAV_OVERFLOW), VBW(3) + VBS(4),
				LEFTBUTTONX, LEFTBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				LEFTBUTTONX, LEFTBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_End,
				&LEFTDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(2) + VBS(6), VBW(2) + VBS(3), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(5 + _NAV_OVERFLOW), VBW(3) + VBS(5), VBUTTONWIDTH + VBS(1 - _NAV_OVERFLOW), VBW(3) + VBS(4),
				RIGHTBUTTONX, RIGHTBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				RIGHTBUTTONX, RIGHTBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_End,
				&RIGHTDOWN, KZ_ACTION_BUTTON
			},
			// start coin
			{
				VBW(1) + VBS(1), VBW(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1) + VBS(1), VBW(1), VBUTTONWIDTH, VBUTTONWIDTH,
				COINBUTTONX, COINBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				COINBUTTONX, COINBUTTONPRESSEDY,
				Position_Baseline_Middle_Front,
				Position_Baseline_End,
				&SELECTDOWN, KZ_ACTION_BUTTON
			},
			{
				VBS(1), VBW(1), VBUTTONWIDTH, VBUTTONWIDTH,
				VBS(1), VBW(1), VBUTTONWIDTH, VBUTTONWIDTH,
				STARTBUTTONX, STARTBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				STARTBUTTONX, STARTBUTTONPRESSEDY,
				Position_Baseline_Middle_Back,
				Position_Baseline_End,
				&STARTDOWN, KZ_ACTION_BUTTON
			},
			// A B | 1 2
			{
				VBW(2) + VBS(5), VBW(1) + VBS(2), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(5), VBW(1) + VBS(2 + 2.25), VBW(1) + VBS(2.25), VBW(1) + VBS(2.25),
				FIRE1BUTTONX, FIRE1BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE1BUTTONX, FIRE1BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&ADOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(1) + VBS(2), VBW(1) + VBS(2), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1) + VBS(2 + 2.25), VBW(1) + VBS(2 + 2.25), VBW(1) + VBS(2.25), VBW(1) + VBS(2.25),
				FIRE2BUTTONX, FIRE2BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE2BUTTONX, FIRE2BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&XDOWN, KZ_ACTION_BUTTON
			},
			// C, X | 3, 4
			{
				VBW(2) + VBS(5), VBW(2) + VBS(5), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(5), VBW(2) + VBS(5), VBW(1) + VBS(2.25), VBW(1) + VBS(2.25),
				FIRE3BUTTONX, FIRE3BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE3BUTTONX, FIRE3BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&BDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(1) + VBS(2), VBW(2) + VBS(5), VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1) + VBS(2 + 2.25), VBW(2) + VBS(5), VBW(1) + VBS(2.25), VBW(1) + VBS(2.25),
				FIRE4BUTTONX, FIRE4BUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				FIRE4BUTTONX, FIRE4BUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_End,
				&YDOWN, KZ_ACTION_BUTTON
			},
			// Y Z | 5 6
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0,
				NULL, KZ_ACTION_DISABLED
			},
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0,
				NULL, KZ_ACTION_DISABLED
			},
			// pause quit
			{
				0, 0, VBUTTONWIDTH, VBUTTONWIDTH,
				0, 0, VBUTTONWIDTH, VBUTTONWIDTH,
				PAUSEBUTTONX, PAUSEBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				PAUSEBUTTONX, PAUSEBUTTONPRESSEDY,
				Position_Baseline_Begin,
				Position_Baseline_Begin,
				&PAUSEDOWN, KZ_ACTION_BUTTON
			},
			{
				VBW(1), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				QUITBUTTONX, QUITBUTTONRELESEDY, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				QUITBUTTONX, QUITBUTTONPRESSEDY,
				Position_Baseline_End,
				Position_Baseline_Begin,
				&QDOWN, KZ_ACTION_BUTTON
			},
			{
#if 0
				-VBW(0.5), VBW(2) + VBS(0), VBUTTONWIDTH, VBUTTONWIDTH,
				-VBW(0.5), VBW(2) + VBS(0), VBUTTONWIDTH, VBUTTONWIDTH,
				SELECT_BUTTON_X, SELECT_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				SELECT_BUTTON_PRESSED_X, SELECT_BUTTON_PRESSED_Y,
				Position_Baseline_Middle_Front,
				Position_Baseline_End,
#endif
				VBW(1) + VBS(2), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(1) + VBS(2), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				SELECT_BUTTON_X, SELECT_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				SELECT_BUTTON_PRESSED_X, SELECT_BUTTON_PRESSED_Y,
				Position_Baseline_Begin,
				Position_Baseline_Begin,
				&harm_SELECT, KZ_ACTION_BUTTON
			},
			// specials
			{
				VBW(3) + VBS(3), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(3) + VBS(3), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				STATE_LOAD_BUTTON_X, STATE_LOAD_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				STATE_LOAD_BUTTON_PRESSED_X, STATE_LOAD_BUTTON_PRESSED_Y,
				Position_Baseline_End,
				Position_Baseline_Begin,
				NULL, KZ_ACTION_BUTTON
			},
			{
				VBW(2) + VBS(2), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(2), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				STATE_SAVE_BUTTON_X, STATE_SAVE_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				STATE_SAVE_BUTTON_PRESSED_X, STATE_SAVE_BUTTON_PRESSED_Y,
				Position_Baseline_End,
				Position_Baseline_Begin,
				NULL, KZ_ACTION_BUTTON
			},
			{
				VBW(2) + VBS(3), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(2) + VBS(3), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				PLAYER_SWITCH_BUTTON_X, PLAYER_SWITCH_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				PLAYER_SWITCH_BUTTON_PRESSED_X, PLAYER_SWITCH_BUTTON_PRESSED_Y,
				Position_Baseline_Begin,
				Position_Baseline_Begin,
				&harm_CURRENT_PLAYER, KZ_ACTION_SWITCH
			},
			{
				VBW(3) + VBS(4), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				VBW(3) + VBS(4), 0, VBUTTONWIDTH, VBUTTONWIDTH,
				CONTROL_SWITCH_BUTTON_X, CONTROL_SWITCH_BUTTON_Y, BUTTON_SPRITE_WIDTH, BUTTON_SPRITE_WIDTH,
				CONTROL_SWITCH_BUTTON_PRESSED_X, CONTROL_SWITCH_BUTTON_PRESSED_Y,
				Position_Baseline_Begin,
				Position_Baseline_Begin,
				&harm_CONTROL_TOGETHER, KZ_ACTION_SWITCH
			},
		}, // only A B C D
	};

	KZ_VKB_t *v;
	struct KZ_VKB_Init_Data_s *d;
	int layout;
	int x, y, w, h, ex, ey, ew, eh;
#if 0
	float per;

	per = KZ_MIN((float)height / N950SCREENHEIGHT, (float)width / N950SCREENWIDTH);
#endif
	layout = harm_options.layout >= (sizeof(vkb_init_data) / sizeof(vkb_init_data[0])) || harm_options.layout < 0 ? 0 : harm_options.layout;
	for(int i = 0; i < Total_Button; i++)
	{
		v = vkb + i;
		d = vkb_init_data[layout] + i;
		x = d->x;
		y = d->y;
		w = d->w;
		h = d->h;
		ex = d->ex;
		ey = d->ey;
		ew = d->ew;
		eh = d->eh;

		memset(v, 0, sizeof(KZ_VKB_t));

		v->action = i;
		v->state = 0;

		v->ptr = d->b;
		v->type = d->t;
		if(!v->type == KZ_ACTION_DISABLED)
			continue;

		switch(d->xb)
		{
			case Position_Baseline_Middle_Front:
				v->button.x = width / 2 - x;
				v->event.x1 = width / 2 - ex;
				break;
			case Position_Baseline_Middle_Back:
				v->button.x = width / 2 + x;
				v->event.x1 = width / 2 + ex;
				break;
			case Position_Baseline_End:
				v->button.x = width - x;
				v->event.x1 = width - ex;
				break;
			case Position_Baseline_Begin:
			default:
				v->button.x = x;
				v->event.x1 = ex;
				break;
		}

		switch(d->yb)
		{
			case Position_Baseline_Middle_Front:
				v->button.y = height / 2 - y;
				v->event.y1 = height / 2 - ey;
				break;
			case Position_Baseline_Middle_Back:
				v->button.y = height / 2 + y;
				v->event.y1 = height / 2 + ey;
				break;
			case Position_Baseline_End:
				v->button.y = height - y;
				v->event.y1 = height - ey;
				break;
			case Position_Baseline_Begin:
			default:
				v->button.y = y;
				v->event.y1 = ey;
				break;
		}

		v->button.w = w;
		v->button.h = h;

		v->event.x2 = v->event.x1 + ew;
		v->event.y2 = v->event.y1 + eh;

		v->sprite.x = d->sx;
		v->sprite.y = d->sy;
		v->sprite.w = d->sw;
		v->sprite.h = d->sh;

		v->sprite_pressed.x = d->sx_pressed;
		v->sprite_pressed.y = d->sy_pressed;
		v->sprite_pressed.w = d->sw;
		v->sprite_pressed.h = d->sh;
	}
#undef _NAV_OVERFLOW
}

static void init_joy(int width, int height)
{
#if 0
	float per;

	per = KZ_MIN((float)height / N950SCREENHEIGHT, (float)width / N950SCREENWIDTH);
#endif

	joy.big_sprite = {BIGCIRCLEX,BIGCIRCLEY,BIGCIRCLEWIDTH,BIGCIRCLEWIDTH};
	joy.small_sprite = {SMALLCIRCLEX,SMALLCIRCLEY,SMALLCIRCLEWIDTH,SMALLCIRCLEWIDTH};
	joy.big_button = {VBS(1.5),height - (BIGCIRCLEWIDTH + VBS(1)),BIGCIRCLEWIDTH,BIGCIRCLEWIDTH};
	joy.circle = {
		{
			joy.big_button.x + joy.big_button.w / 2,
			joy.big_button.y + joy.big_button.h / 2
		},
		CIRCLEHALFLENGTH,
		CIRCLEHALFLENGTHWITHOUTLISTENER
	};
	joy.small_button = {joy.circle.center.x - SMALLCIRCLEWIDTH / 2, joy.circle.center.y - SMALLCIRCLEWIDTH / 2,SMALLCIRCLEWIDTH,SMALLCIRCLEWIDTH};
	joy.ptr[0] = &UPDOWN;
	joy.ptr[1] = &DOWNDOWN;
	joy.ptr[2] = &LEFTDOWN;
	joy.ptr[3] = &RIGHTDOWN;
}

static inline bool action_handler(KZ_VKB_t *v, bool pressed, void *data)
{
	if(v->type == KZ_ACTION_SWITCH)
	{
		if(pressed)
		{
			v->state ^= 1;
			if(v->ptr)
			{
				*((char *)(v->ptr)) = v->state;
				return true;
			}
		}
	}
	else
	{
		v->state = pressed ? 1 : 0;
		if(v->ptr)
		{
			*((char *)(v->ptr))=v->state;
			return true;
		}
	}

	if(v->action == StateLoadButton)
	{
		if(v->state)
		{
			if(harm_options.state_path)
			{
				BurnStateLoad(harm_options.state_path, 1, NULL);
#ifdef USING_INFOBANNER
				harm_show_info_banner("State loaded!");
#endif
			}
			return true;
		}
	}
	else if(v->action == StateSaveButton)
	{
		if(v->state)
		{
			if(harm_options.state_path)
			{
				BurnStateSave(harm_options.state_path, 1);
#ifdef USING_INFOBANNER
				harm_show_info_banner("State saved!");
#endif
			}
			return true;
		}
	}

	return false;
}

static inline bool handle_touch_event(Uint16 x, Uint16 y, bool pressed)
{
	bool r;
	KZ_VKB_t *v;
	int len;
	int start;

	len = bPauseOn ? Quit_Button + 1 : Total_Button;
	r = false;
	if(!bPauseOn && harm_options.joystick_mode != 2)
	{
		PointInCircle pos=checkPointPostion(x,y, &joy.circle);
		if(pos!=OutCircle)
		{
			if(pressed)
			{
				joy.small_button.x=x- joy.small_button.w / 2;
				joy.small_button.y=y- joy.small_button.h / 2;
			}
			else
			{
				joy.small_button.x=joy.circle.center.x- joy.small_button.w / 2;
				joy.small_button.y=joy.circle.center.y- joy.small_button.h / 2;
			}
			if(pos==InCircleButCenter)
			{
				if(pressed)
				{
					float arg=mycast(x,y, &joy.circle.center);
					/*
						 if(arg<0)
						 {
						 UPDOWN=0;
						 DOWNDOWN=0;
						 LEFTDOWN=0;
						 RIGHTDOWN=0;
						 }
						 */
					if(arg>MYARG && arg<180.0-MYARG)
						*(joy.ptr[0])=1;
					if(arg>180.0+MYARG && arg<360.0-MYARG)
						*(joy.ptr[1])=1;
					if(arg>90.0+MYARG && arg<270.0-MYARG)
						*(joy.ptr[2])=1;
					if((arg<90.0-MYARG && arg>=0) || (arg>270.0+MYARG))
						*(joy.ptr[3])=1;
				}
				else
				{
					*(joy.ptr[0])=0;
					*(joy.ptr[1])=0;
					*(joy.ptr[2])=0;
					*(joy.ptr[3])=0;
				}
			}

			r = true;
		}
	}

	start = harm_options.joystick_mode == 2 ? Up_Button : Coin_Button;
	for(int i = bPauseOn ? Pause_Button : start; i < len; i++)
	{
		if(!harm_options.show_2p)
		{
			if(i == ControlTogetherButton)
				continue;
		}
		v = vkb + i;
		if(v->type == KZ_ACTION_DISABLED)
			continue;

		if(checkClickedRange(x,y,&(v->event)))
		{
			r = action_handler(v, pressed, NULL);
		}
	}

	return r;
}

static inline bool handle_motion_event(Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel)
{
	bool r;
	bool last_in, now_in;
	bool pressed;
	KZ_VKB_t *v;
	int len;
	int start;

	Uint16 xoffset=x-xrel;
	Uint16 yoffset=y-yrel;
	//printf("x= %d,y= %d,xrel= %d,yrel= %d\n",event.motion.x,event.motion.y,event.motion.xrel,event.motion.yrel);
	r = false;
	len = bPauseOn ? Quit_Button + 1 : Total_Button;
	if(!bPauseOn && harm_options.joystick_mode != 2)
	{
		PointInCircle pos=checkPointPostion(xoffset,yoffset, &joy.circle);
		if(pos!=OutCircle)
		{
			if(pos==InCircleButCenter){
				*(joy.ptr[0])=0;
				*(joy.ptr[1])=0;
				*(joy.ptr[2])=0;
				*(joy.ptr[3])=0;
				/*
					 double arg=mycast(xoffset,yoffset);
					 if(arg<0)
					 {
					 UPDOWN=0;
					 DOWNDOWN=0;
					 LEFTDOWN=0;
					 RIGHTDOWN=0;
					 }
					 if(arg>MYARG && arg<180.0-MYARG)
					 UPDOWN=0;
					 if(arg>180.0+MYARG && arg<360.0-MYARG)
					 DOWNDOWN=0;
					 if(arg>90.0+MYARG && arg<270.0-MYARG)
					 LEFTDOWN=0;
					 if((arg<90.0-MYARG && arg>=0) || (arg>270.0+MYARG))
					 RIGHTDOWN=0;
					 */
			}

			{
				PointInCircle pos=checkPointPostion(x,y, &joy.circle);
				if(pos!=OutCircle)
				{
					joy.small_button.x=x- joy.small_button.w / 2;
					joy.small_button.y=y- joy.small_button.h / 2;
					if(pos==InCircleButCenter)
					{
						float arg=mycast(x,y, &joy.circle.center);
						/*
							 if(arg<0)
							 {
							 UPDOWN=0;
							 DOWNDOWN=0;
							 LEFTDOWN=0;
							 RIGHTDOWN=0;
							 }
							 */
						if(arg>MYARG && arg<180.0-MYARG)
							*(joy.ptr[0])=1;
						if(arg>180.0+MYARG && arg<360.0-MYARG)
							*(joy.ptr[1])=1;
						if(arg>90.0+MYARG && arg<270.0-MYARG)
							*(joy.ptr[2])=1;
						if((arg<90.0-MYARG && arg>=0) || (arg>270.0+MYARG))
							*(joy.ptr[3])=1;
					}
				}
				else
				{
					joy.small_button.x=joy.circle.center.x- joy.small_button.w / 2;
					joy.small_button.y=joy.circle.center.y- joy.small_button.h / 2;
					/*
						 UPDOWN=0;
						 DOWNDOWN=0;
						 LEFTDOWN=0;
						 RIGHTDOWN=0;
						 */
				}
			}

			r = true;
		}
	}

	/************ mouse motion with press and hold *********/

	start = harm_options.joystick_mode == 2 ? Up_Button : Coin_Button;
	for(int i = bPauseOn ? Pause_Button : start; i < len; i++)
	{
		if(!harm_options.show_2p)
		{
			if(i == ControlTogetherButton)
				continue;
		}
		v = vkb + i;
		if(!v->ptr)
			continue;
		if(v->type == KZ_ACTION_DISABLED)
			continue;

		last_in = checkClickedRange(xoffset,yoffset,&(v->event));
		now_in = checkClickedRange(x,y,&(v->event));
		if(last_in != now_in)
		{
			pressed = !last_in && now_in;
			r = action_handler(v, pressed, NULL);
		}
	}

	return r;
}

static inline void blit_button_sprite()
{
	KZ_VKB_t *v;
	SDL_Rect src_rect, dst_rect;
	bool pressed;
	int len;
	int start;

	len = bPauseOn ? Quit_Button + 1 : Total_Button;
	start = harm_options.joystick_mode == 2 ? Up_Button : Coin_Button;

	for(int i = bPauseOn ? Pause_Button : start; i < len; i++)
	{
		if(!harm_options.show_2p)
		{
			if(i == ControlTogetherButton)
				continue;
		}
		v = vkb + i;
		if(v->type == KZ_ACTION_DISABLED)
			continue;

		pressed = v->state == 1;
#define _TEST_RENDER_EVENT_RECT
#ifdef _TEST_RENDER_EVENT_RECT
		if(harm_options.debug && pressed)
		{
			SDL_PixelFormat *format = myscreen->format;
			Uint32 color = SDL_MapRGBA(format, i * 25, 255, 255 - i * 25, 128);
			SDL_Rect fill_rect = {
				v->event.x1,
				v->event.y1,
				v->event.x2 - v->event.x1,
				v->event.y2 - v->event.y1,
			};
			SDL_FillRect(myscreen, &fill_rect, color);
		}
#endif

		src_rect = pressed ? v->sprite_pressed : v->sprite;
		dst_rect = v->button;
		SDL_BlitSurface(buttons_surface,&src_rect,myscreen,&dst_rect);
		//SDL_SoftStretch(buttons_surface,&src_rect,myscreen,&dst_rect);
	}
}

static inline void blit_joystick_sprite()
{
	if(bPauseOn)
		return;

	if(harm_options.joystick_mode == 2)
		return;

	SDL_Rect rect = joy.big_button;
	SDL_BlitSurface(joystick_surface,&joy.big_sprite,myscreen,&rect);
	if(harm_options.joystick_mode != 1)
	{
		rect = joy.small_button;
		SDL_BlitSurface(joystick_surface,&joy.small_sprite,myscreen,&rect);
	}
}

static bool mkdirs(const char *path)
{
	char *p;
	char *i;
	int res;
	struct stat st;
	bool r;
	char *str;

	r = true;
	str = strdup(path);
	p = str;

	if(*p == '/')
		p++;

	while(p)
	{
		i = strchr(p, '/');
		if(!i)
			break;
		*i = '\0';
		//printf("--- %s +++ %s\n", str, p);
		res = stat(str, &st);
		if(res == 0)
		{
			if(!S_ISDIR(st.st_mode))
			{
				r = false;
				break;
			}
		}
		else
		{
			res = mkdir(str, 0755);
			//printf("mkdir %s %d\n", str, res);
			if(res != 0)
			{
				r = false;
				break;
			}
		}
		*i = '/';
		p = i + 1;
	}

	free(str);

	return r;
}

void harm_set_romname(const char *romname)
{
	int length;
	char *p;
	char *path;

	if(harm_options.state_path)
		goto _Exit;

	harm_options.romname = strdup(romname);

	p = getenv("HOME");
	length = strlen(p);
	length += 1; // /
	length += strlen(KZ_STATE_PATH);
	length += 1; // /
	length += strlen(romname);
	length += strlen(KZ_STATE_SUFFIX);
	length += strlen(KZ_STATE_EXTENSION);
	length += 1;
	
	path = (char *)malloc(length);
	memset(path, 0, length);
	// sprintf(path, "%s/%s/%s%s%s", p, KZ_STATE_PATH, romname, KZ_STATE_SUFFIX, KZ_STATE_EXTENSION);
	strcat(path, p); p = path + strlen(p);
	p[0] = '/'; p += 1;
	strcat(p, KZ_STATE_PATH); p += strlen(KZ_STATE_PATH);
	p[0] = '/'; p += 1;
	strcat(p, romname); p += strlen(romname);
	strcat(p, KZ_STATE_SUFFIX); p += strlen(KZ_STATE_SUFFIX);
	strcat(p, KZ_STATE_EXTENSION); p += strlen(KZ_STATE_EXTENSION);
	p[0] = '\0';

	if(!mkdirs(path))
	{
		free(path);
		path = NULL;
	}

	if(path)
		harm_options.state_path = path;

_Exit:
	printf("[KZdebug]: %s -> state: %s\n", harm_options.romname, harm_options.state_path);
}

/***** karin zhao -> virtual buttons surface *****/

#define FBIO_WAITFORVSYNC _IOW('F', 0x20, __u32)
unsigned long fbdev;
int vb;

int scaleheight=N950SCREENHEIGHT;
int scalewidth=N950SCREENWIDTH;

void gp2x_initialize()
{

	BurnDrvGetFullSize(&WINDOW_WIDTH, &WINDOW_HEIGHT);
	if (((config_options.option_rotate==0) && (BurnDrvGetFlags() & BDF_ORIENTATION_VERTICAL)) || (config_options.option_rotate==2))
	{
		int t;
		t=WINDOW_HEIGHT;
		WINDOW_HEIGHT=WINDOW_WIDTH;
		WINDOW_WIDTH=t;
	}
	printf("dw:%d dh:%d\n",WINDOW_WIDTH,WINDOW_HEIGHT);
	if (config_options.option_rescale==0)
	{
		scaleheight=WINDOW_HEIGHT;
		scalewidth=WINDOW_WIDTH;
		if (scaleheight>N950SCREENHEIGHT) scaleheight=N950SCREENHEIGHT;
		if (scalewidth>N950SCREENWIDTH) scalewidth=N950SCREENWIDTH;
	}
	if (config_options.option_rescale==1)
	{
		scaleheight=WINDOW_HEIGHT*2;
		scalewidth=WINDOW_WIDTH*2;
		if (scaleheight>N950SCREENHEIGHT) scaleheight=N950SCREENHEIGHT;
		if (scalewidth>N950SCREENWIDTH) scalewidth=N950SCREENWIDTH;
	}
	if (config_options.option_rescale==2)
	{
		float xw,xh;
		xh=(float)N950SCREENHEIGHT/(float)WINDOW_HEIGHT;
		xw=(float)N950SCREENWIDTH/(float)WINDOW_WIDTH;
		if (xh>xw)
		{
			scalewidth=(int)((float)WINDOW_WIDTH*xw);
			scaleheight=(int)((float)WINDOW_HEIGHT*xw);
		}
		else
		{
			scalewidth=(int)((float)WINDOW_WIDTH*xh);
			scaleheight=(int)((float)WINDOW_HEIGHT*xh);

		}
		if (scaleheight>N950SCREENHEIGHT) scaleheight=N950SCREENHEIGHT;
		if (scalewidth>N950SCREENWIDTH) scalewidth=N950SCREENWIDTH;
	}
	if (config_options.option_rescale==3)
	{
		scaleheight=N950SCREENHEIGHT;
		scalewidth=N950SCREENWIDTH;
	}

	char scaling[64];
	sprintf(scaling,"SDL_OMAP_LAYER_SIZE=%dx%d",scalewidth,scaleheight);
	printf("scaling x:%d y:%d   %s\n",scalewidth,scaleheight,scaling);

	for (int i=0;environ[i];i++)
	{
		if (strstr(environ[i],"SDL_OMAP_LAYER_SIZE="))
		{
			strcpy(environ[i],scaling);
		}

	}


	printf("Setting screen to %d x %d\n",WINDOW_WIDTH,WINDOW_HEIGHT);
	if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_TIMER))<0)
	{
		printf("sdl failed to init\n");
	}			// Initialize SDL
	//    myscreen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 16, SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF);
	myscreen = SDL_SetVideoMode(scalewidth, scaleheight, 16, SDL_FULLSCREEN | SDL_SWSURFACE  | SDL_DOUBLEBUF);
	if(!myscreen)
	{
		printf("SDL_SetVideoMode screen not initialised.\n");								// debug output example for serial cable
	}
	else printf("SDL_SetVideoMode successful.\n",scalewidth,scaleheight);

	/*****karin zhao -> virtual buttons*******/

	// surface of virtual buttons image
	printf("KZdebug -> load virtual button image to surface......");
	SDL_Surface *surface;

	surface=IMG_Load(_RESC_DIR "/anna_buttons.png");
#if 0
	buttons_surface = SDL_ConvertSurface(surface, myscreen->format, myscreen->flags);
	SDL_FreeSurface(surface);
#else
	buttons_surface = surface;
#endif

	surface=IMG_Load(_RESC_DIR "/circle_joystick.png");
#if 0
	joystick_surface = SDL_ConvertSurface(surface, myscreen->format, myscreen->flags);
	SDL_FreeSurface(surface);
#else
	joystick_surface = surface;
#endif
	printf("done\n");

	// initial all sprite rectangles
	printf("KZdebug -> initial all sprite rectangles......");
	init_vkb(scalewidth, scaleheight);
	printf("done\n");

	/*
		 printf("KZdebug -> translucent surfaces......");
		 SDL_SetColorKey(buttons_surface,SDL_SRCCOLORKEY,SDL_MapRGB(buttons_surface -> format,0x99,0x99,0x99));
		 SDL_SetAlpha( buttons_surface, SDL_SRCALPHA | SDL_RLEACCEL, TRANSLUCENTLEVEL );
	//SDL_SetAlpha( joystick_surface, SDL_SRCALPHA | SDL_RLEACCEL, TRANSLUCENTLEVEL );
	printf("done\n");
	*/

	// define range of mouse click
	printf("KZdebug -> define mouse click range......");
	init_joy(scalewidth, scaleheight);
	printf("done\n");

	/*****karin zhao -> virtual buttons*******/

	SDL_ShowCursor(SDL_DISABLE);															// Disable mouse cursor on gp2x
	SDL_WM_SetCaption( WINDOW_TITLE, 0 );													// Sets the window title (not needed for gp2x)

	fbdev=open("/dev/fb0", O_RDONLY);

	joyCount=SDL_NumJoysticks();
	if (joyCount>5) joyCount=5;
	printf("%d Joystick(s) Found\n",joyCount);
	//if ((joyCount==1) && (strcmp(SDL_JoystickName(0),"gpio-keys")==0)) joyCount=0;
	if (joyCount>0)
	{

		for (int i=0;i<joyCount;i++)
		{
			printf("%s\t",SDL_JoystickName(i));
			joys[i] = SDL_JoystickOpen(i);
			printf("Hats %d\t",SDL_JoystickNumHats(joys[i]));
			printf("Buttons %d\t",SDL_JoystickNumButtons(joys[i]));
			printf("Axis %d\n",SDL_JoystickNumAxes(joys[i]));
			if (strcmp(SDL_JoystickName(i),"nub0")==0) joys[0]=SDL_JoystickOpen(i);
			if (strcmp(SDL_JoystickName(i),"nub1")==0) joys[1]=SDL_JoystickOpen(i);
		}
		//if (joyCount>1) joys[0]=SDL_JoystickOpen(1);
		//if (joyCount>2) joys[1]=SDL_JoystickOpen(2);
	}
	VideoBuffer=(unsigned short*)malloc((WINDOW_WIDTH*2) * WINDOW_HEIGHT);
	SDL_VideoBuffer=SDL_CreateRGBSurfaceFrom(VideoBuffer,WINDOW_WIDTH*2,WINDOW_HEIGHT,16,WINDOW_WIDTH*2,0xF800,0x7E0,0x1F,0x0);
	SDL_LockSurface(SDL_VideoBuffer);
	gp2x_video_flip();
}

void gp2x_terminate(char *frontend)
{
	struct stat info;
	SDL_Quit();
	if( (lstat(frontend, &info) == 0) && S_ISREG(info.st_mode) )
	{
		char path[256];
		char *p;
		strcpy(path, frontend);
		p = strrchr(path, '/');
		if(p == NULL) p = strrchr(path, '\\');
		if(p != NULL)
		{
			*p = '\0';
			chdir(path);
		}
		execl(frontend, frontend, NULL);
	}

}

int get_pc_keyboard()
{
	int pckeydata=0;
	kinput=0;
	SDL_Event event;
	while( SDL_PollEvent( &event ) )
	{
		if (event.type== SDL_MOUSEBUTTONUP)
		{		
			handle_touch_event(event.button.x, event.button.y, false);
		}

		else if (event.type== SDL_MOUSEBUTTONDOWN) 
		{
			handle_touch_event(event.button.x, event.button.y, true);
		}

		else if(event.type==SDL_MOUSEMOTION && event.motion.state==SDL_PRESSED)
		{
			handle_motion_event(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
		}

		if (event.type== SDL_KEYUP)
		{																	// PC buttons

			if (event.key.keysym.sym==config_keymap.up)
				UPDOWN=0;

			if (event.key.keysym.sym==config_keymap.left)
				LEFTDOWN=0;

			if (event.key.keysym.sym==config_keymap.right)
				RIGHTDOWN=0;

			if (event.key.keysym.sym==config_keymap.fire1)
				ADOWN=0;

			if (event.key.keysym.sym==config_keymap.fire4)
				YDOWN=0;

			if (event.key.keysym.sym==config_keymap.fire2)
				XDOWN=0;

			if (event.key.keysym.sym==config_keymap.fire3)
				BDOWN=0;

			if (event.key.keysym.sym==config_keymap.fire5)
				LSDOWN=0;

			if (event.key.keysym.sym==config_keymap.down)
				DOWNDOWN=0;

			if (event.key.keysym.sym==config_keymap.fire6)
				RSDOWN=0;

			if (event.key.keysym.sym==config_keymap.coin1)
				SELECTDOWN=0;

			if (event.key.keysym.sym==config_keymap.start1)
				STARTDOWN=0;

			if (event.key.keysym.sym==config_keymap.quit)
				QDOWN=0;

			if (event.key.keysym.sym==config_keymap.pause)
				PAUSEDOWN=0;
		}

		if (event.type== SDL_KEYDOWN)
		{																	// PC buttons
			kinput=event.key.keysym.scancode;
			//                      printf("key=%d\n",event.key.keysym.sym);

			if (event.key.keysym.sym==config_keymap.up)
				UPDOWN=1;

			if (event.key.keysym.sym==config_keymap.left)
				LEFTDOWN=1;

			if (event.key.keysym.sym==config_keymap.right)
				RIGHTDOWN=1;

			if (event.key.keysym.sym==config_keymap.fire1)
				ADOWN=1;

			if (event.key.keysym.sym==config_keymap.fire4)
				YDOWN=1;

			if (event.key.keysym.sym==config_keymap.fire2)
				XDOWN=1;

			if (event.key.keysym.sym==config_keymap.fire3)
				BDOWN=1;

			if (event.key.keysym.sym==config_keymap.fire5)
				LSDOWN=1;

			if (event.key.keysym.sym==config_keymap.down)
				DOWNDOWN=1;

			if (event.key.keysym.sym==config_keymap.fire6)
				RSDOWN=1;


			if (event.key.keysym.sym==config_keymap.coin1)
				SELECTDOWN=1;

			if (event.key.keysym.sym==config_keymap.start1)
				STARTDOWN=1;

			if (event.key.keysym.sym==config_keymap.quit)
				QDOWN=1;

			if (event.key.keysym.sym==config_keymap.pause)
				PAUSEDOWN=1;


		}
		/* move here test */
		if (kinput)
		{
			int conv=0;
			//printf("keycode: %d\n",kinput);
			if (kinput==32) {kinput=1;conv=1;}
			if (kinput==46) {kinput=38;conv=1;}
			if (kinput==33) {kinput=39;conv=1;}
			if (kinput==13) {kinput=40;conv=1;}
			if ((kinput>=97) && (kinput<=122)) {kinput-=95;conv=1;}
			if ((kinput>=48) && (kinput<=57)) {kinput-=20;conv=1;}
			if (conv==0) kinput=0;
		}
		/* move here test */

	}


	if (UPDOWN) pckeydata|=MY_UP;
	if (LEFTDOWN) pckeydata|=MY_LEFT;
	if (RIGHTDOWN) pckeydata|=MY_RIGHT;
	if (DOWNDOWN) pckeydata|=MY_DOWN;
	if (ADOWN) pckeydata|=MY_BUTT_A;
	if (YDOWN) pckeydata|=MY_BUTT_Y;
	if (XDOWN) pckeydata|=MY_BUTT_X;
	if (BDOWN) pckeydata|=MY_BUTT_B;
	if (LSDOWN) pckeydata|=MY_BUTT_SL;
	if (RSDOWN) pckeydata|=MY_BUTT_SR;
	if (STARTDOWN) pckeydata|=MY_START;
	if (SELECTDOWN) pckeydata|=MY_SELECT;
	if (PAUSEDOWN) pckeydata|=MY_PAUSE;
	if (QDOWN)
	{
		pckeydata|=MY_QT;
	}
#ifdef _HARMATTAN
	if(harm_SELECT) pckeydata |= KZ_SELECT;
#endif
	return pckeydata;
}

unsigned long gp2x_joystick_read(void)
{
	int value=get_pc_keyboard();
	return value;
}

void drawSprite(SDL_Surface* imageSurface, SDL_Surface* screenSurface, int srcX, int srcY, int dstX, int dstY, int width, int height)
{
	SDL_Rect srcRect;
	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = width;
	srcRect.h = height;

	SDL_Rect dstRect;
	dstRect.x = dstX;
	dstRect.y = dstY;
	dstRect.w = scalewidth;
	dstRect.h = scaleheight;
	SDL_SoftStretch(imageSurface, &srcRect, screenSurface, &dstRect);
	//	SDL_BlitSurface(imageSurface, &srcRect, screenSurface, &dstRect);
}



void gp2x_clear_framebuffers()
{
	memset(VideoBuffer,0,WINDOW_HEIGHT*WINDOW_WIDTH*2);
}

void gp2x_video_flip()
{
	SDL_UnlockSurface(SDL_VideoBuffer);
#ifdef USING_INFOBANNER
	harm_render_info_banner();
#endif
	drawSprite(SDL_VideoBuffer,myscreen,0,0,0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
	if (nBurnFPS>5900) ioctl(fbdev,FBIO_WAITFORVSYNC,&vb); //use vblank if running at 60 hz
	/***** flip all virtual buttons surface *****/
	blit_button_sprite();
	blit_joystick_sprite();
	/***** flip all virtual buttons surface *****/
	SDL_Flip(myscreen);
	SDL_LockSurface(SDL_VideoBuffer);
}

void freeVirtualButtons()
{
	/***** free all virtual buttons surface *****/ 
	printf("KZdebug -> free image surfaces......");
	if(buttons_surface!= NULL) SDL_FreeSurface(buttons_surface);
	if(joystick_surface != NULL) SDL_FreeSurface(joystick_surface);
	printf("done\n");
	/***** free all virtual buttons surface *****/ 
}

//void * UpperMalloc(size_t size)
//{
//printf("using upper malloc\n");
/*int i = 0;
ReDo:
for (; TakenSize[i]; i += TakenSize[i]);
if (i >= 0x2000000 / BLOCKSIZE) {
printf("UpperMalloc out of mem!");
return NULL;
}
int BSize = (size - 1) / BLOCKSIZE + 1;
for(int j = 1; j < BSize; j++) {
if (TakenSize[i + j]) {
i += j;
goto ReDo; //OMG Goto, kill me.
}
}

TakenSize[i] = BSize;
void* mem = ((char*)UpperMem) + i * BLOCKSIZE;
//  gp2x_memset(mem, 0, size);
*/
/*void * mem=(char*)malloc(size);
	if (mem==NULL) printf("mem alloc of %d bytes failed\n",size);
	return mem;
	}/*

//Releases UpperMalloced memory
//void UpperFree(void* mem)
{
/* int i = (((int)mem) - ((int)UpperMem));
if (i < 0 || i >= 0x2000000) {
fprintf(stderr, "UpperFree of not UpperMalloced mem: %p\n", mem);
} else {
if (i % BLOCKSIZE)
fprintf(stderr, "delete error: %p\n", mem);
TakenSize[i / BLOCKSIZE] = 0;
}*/
//  free(mem);
//}

//Returns the size of a UpperMalloced block.
/*int GetUpperSize(void* mem)
	{
	int i = (((int)mem) - ((int)UpperMem));
	if (i < 0 || i >= 0x2000000) {
	fprintf(stderr, "GetUpperSize of not UpperMalloced mem: %p\n", mem);
	return -1;
	}
	return TakenSize[i / BLOCKSIZE] * BLOCKSIZE;
	}

*/
