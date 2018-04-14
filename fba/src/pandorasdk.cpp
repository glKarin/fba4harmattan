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
// width of virtual button
#define VBUTTONWIDTH 64

// range of mouse click
static SDL_Rect v_coin_button,v_start_button,v_fire1_button,v_fire2_button,v_fire3_button,v_fire4_button,v_fire5_button,v_fire6_button,v_pause_button,v_quit_button;
static SDL_Rect big_circle_rectangle,small_circle_rectangle;

typedef struct{
	Sint16 x1;
	Sint16 y1;
	Sint16 x2;
	Sint16 y2;
}KZ_Rect;

static KZ_Rect kz_coin,kz_start,kz_fire1,kz_fire2,kz_fire3,kz_fire4,kz_fire5,kz_fire6,kz_pause,kz_quit;
inline static bool checkClickedRange(Uint16 mx,Uint16 my,const KZ_Rect *rect)
{
	return ((mx >= rect->x1 && my >= rect->y1) && (mx < rect->x2 && my < rect->y2));
}

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
#define MYARG 22.5

#define CIRCLEHALFLENGTH 185.0
#define CIRCLEHALFLENGTHWITHOUTLISTENER 25.0
//#define TRANSLUCENTLEVEL 157

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

SDL_Rect fire1_sprite, fire2_sprite, fire3_sprite, fire4_sprite, fire5_sprite, fire6_sprite, start_sprite, coin_sprite, pause_sprite, quit_sprite, big_circle_sprite, small_circle_sprite;

enum PointInCircle{
	OutCircle=0,
	InCircleAndCenter,
	InCircleButCenter
};

inline static PointInCircle checkPointPostion(Uint16 x,Uint16 y)
{
	double a=(double)(x- CIRCLECENTERX);
	double b=(double)(y- CIRCLECENTERY);
	double c=sqrt(a*a+b*b);
	//printf("a=%f ,b=%f ,c=%f\n",a,b,c);
	if(c>CIRCLEHALFLENGTH)
		return OutCircle;
	else if(c<CIRCLEHALFLENGTHWITHOUTLISTENER)
		return InCircleAndCenter;
	else
		return InCircleButCenter;
}

inline static double mycast(Uint16 x,Uint16 y)
{
	double a=(double)(x- CIRCLECENTERX);
	double b=(double)(y- CIRCLECENTERY);
	/*
		 if(a==0 && b==0)
		 return -1.0;
		 */
	if(a==0 && b>0)
		return 270.0;
	if(a==0 && b<0)
		return 90.0;
	if(b==0 && a>0)
		return 0.0;
	if(b==0 && a<0)
		return 180.0;
	double c=sqrt(a*a+b*b);
	//bxb+bxb/2bc
	double arg=acos(abs(a)/c)*(180/M_PI);
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

static SDL_Surface *joystick_surface=NULL;
static SDL_Surface *buttons_surface=NULL;

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
	printf("Karin debug -> load virtual button and joystick image to surface......");
	buttons_surface=IMG_Load("/usr/share/fba/anna_buttons.png");
	joystick_surface=IMG_Load("/usr/share/fba/circle_joystick.png");
	printf("done\n");

	// initial all sprite rectangles
	printf("Karin debug -> initial all sprite rectangles......");
	fire1_sprite={FIRE1BUTTONX,FIRE1BUTTONRELESEDY,VBUTTONWIDTH,VBUTTONWIDTH};
	fire2_sprite={FIRE2BUTTONX,FIRE2BUTTONRELESEDY,VBUTTONWIDTH,VBUTTONWIDTH};
	fire3_sprite={FIRE3BUTTONX,FIRE3BUTTONRELESEDY,VBUTTONWIDTH,VBUTTONWIDTH};
	fire4_sprite={FIRE4BUTTONX,FIRE4BUTTONRELESEDY,VBUTTONWIDTH,VBUTTONWIDTH};
	fire5_sprite={FIRE5BUTTONX,FIRE5BUTTONRELESEDY,VBUTTONWIDTH,VBUTTONWIDTH};
	fire6_sprite={FIRE6BUTTONX,FIRE6BUTTONRELESEDY,VBUTTONWIDTH,VBUTTONWIDTH};
	start_sprite={STARTBUTTONX,STARTBUTTONRELESEDY,VBUTTONWIDTH,VBUTTONWIDTH};
	coin_sprite={COINBUTTONX,COINBUTTONRELESEDY,VBUTTONWIDTH,VBUTTONWIDTH};
	pause_sprite={PAUSEBUTTONX,PAUSEBUTTONRELESEDY,VBUTTONWIDTH,VBUTTONWIDTH};
	quit_sprite={QUITBUTTONX,QUITBUTTONRELESEDY,VBUTTONWIDTH,VBUTTONWIDTH};
	big_circle_sprite={BIGCIRCLEX,BIGCIRCLEY,BIGCIRCLEWIDTH,BIGCIRCLEWIDTH};
	small_circle_sprite={SMALLCIRCLEX,SMALLCIRCLEY,SMALLCIRCLEWIDTH,SMALLCIRCLEWIDTH};
	printf("done\n");

	/*
	printf("KZdebug -> translucent surfaces......");
	SDL_SetColorKey(buttons_surface,SDL_SRCCOLORKEY,SDL_MapRGB(buttons_surface -> format,0x99,0x99,0x99));
	SDL_SetAlpha( buttons_surface, SDL_SRCALPHA | SDL_RLEACCEL, TRANSLUCENTLEVEL );
	//SDL_SetAlpha( joystick_surface, SDL_SRCALPHA | SDL_RLEACCEL, TRANSLUCENTLEVEL );
	printf("done\n");
	*/

	// define range of mouse click
	printf("Karin debug -> define mouse click range......");
	v_coin_button={scalewidth/2-VBUTTONWIDTH-20,scaleheight-VBUTTONWIDTH,VBUTTONWIDTH,VBUTTONWIDTH};
	v_start_button={scalewidth/2+20,scaleheight-VBUTTONWIDTH,VBUTTONWIDTH,VBUTTONWIDTH};
	v_fire1_button={scalewidth-VBUTTONWIDTH*3-45*2-15,scaleheight-VBUTTONWIDTH-40,VBUTTONWIDTH,VBUTTONWIDTH};
	v_fire2_button={scalewidth-VBUTTONWIDTH*2-45-15,scaleheight-VBUTTONWIDTH-40,VBUTTONWIDTH,VBUTTONWIDTH};
	v_fire5_button={scalewidth-VBUTTONWIDTH-15,scaleheight-VBUTTONWIDTH-40,VBUTTONWIDTH,VBUTTONWIDTH};
	v_fire3_button={scalewidth-VBUTTONWIDTH*3-45*2-15,scaleheight-VBUTTONWIDTH*2-40-45,VBUTTONWIDTH,VBUTTONWIDTH};
	v_fire4_button={scalewidth-VBUTTONWIDTH*2-45-15,scaleheight-VBUTTONWIDTH*2-40-45,VBUTTONWIDTH,VBUTTONWIDTH};
	v_fire6_button={scalewidth-VBUTTONWIDTH-15,scaleheight-VBUTTONWIDTH*2-40-45,VBUTTONWIDTH,VBUTTONWIDTH};
	v_pause_button={0,0,VBUTTONWIDTH,VBUTTONWIDTH};
	v_quit_button={scalewidth-VBUTTONWIDTH,0,VBUTTONWIDTH,VBUTTONWIDTH};
	big_circle_rectangle={40,scaleheight-30-BIGCIRCLEWIDTH,BIGCIRCLEWIDTH,BIGCIRCLEWIDTH};
	small_circle_rectangle={CIRCLECENTERX- JOYSTICKOFFSET,CIRCLECENTERY- JOYSTICKOFFSET,SMALLCIRCLEWIDTH,SMALLCIRCLEWIDTH};

	kz_coin={v_coin_button.x,v_coin_button.y,v_coin_button.x+v_coin_button.w,v_coin_button.y+v_coin_button.h};
	kz_start={v_start_button.x,v_start_button.y,v_start_button.x+v_start_button.w,v_start_button.y+v_start_button.h};
	kz_pause={v_pause_button.x,v_pause_button.y,v_pause_button.x+v_pause_button.w,v_pause_button.y+v_pause_button.h};
	kz_quit={v_quit_button.x,v_quit_button.y,v_quit_button.x+v_quit_button.w,v_quit_button.y+v_quit_button.h};
	kz_fire1={v_fire1_button.x,v_fire1_button.y- 45,v_fire1_button.x+v_fire1_button.w+45,v_fire1_button.y+v_fire1_button.h};
	kz_fire2={v_fire2_button.x- 45,v_fire2_button.y- 45,v_fire2_button.x+v_fire2_button.w,v_fire2_button.y+v_fire2_button.h};
	kz_fire3={v_fire3_button.x,v_fire3_button.y,v_fire3_button.x+v_fire3_button.w+45,v_fire3_button.y+v_fire3_button.h+45};
	kz_fire4={v_fire4_button.x- 45,v_fire4_button.y,v_fire4_button.x+v_fire4_button.w,v_fire4_button.y+v_fire4_button.h+45};
	kz_fire5={v_fire5_button.x,v_fire5_button.y,v_fire5_button.x+v_fire5_button.w,v_fire5_button.y+v_fire5_button.h};
	kz_fire6={v_fire6_button.x,v_fire6_button.y,v_fire6_button.x+v_fire6_button.w,v_fire6_button.y+v_fire6_button.h};
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
			PointInCircle pos=checkPointPostion(event.button.x,event.button.y);
			if(pos!=OutCircle)
			{
				small_circle_rectangle.x=CIRCLECENTERX- JOYSTICKOFFSET;
				small_circle_rectangle.y=CIRCLECENTERY- JOYSTICKOFFSET;
				if(pos==InCircleButCenter){
					UPDOWN=0;
					DOWNDOWN=0;
					LEFTDOWN=0;
					RIGHTDOWN=0;
				}
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_fire1))
			{
				ADOWN=0;
				fire1_sprite.y=FIRE1BUTTONRELESEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_fire4))
			{
				YDOWN=0;
				fire4_sprite.y=FIRE4BUTTONRELESEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_fire2))
			{
				XDOWN=0;
				fire2_sprite.y=FIRE2BUTTONRELESEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_fire3))
			{
				BDOWN=0;
				fire3_sprite.y=FIRE3BUTTONRELESEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_fire5))
			{
				LSDOWN=0;
				fire5_sprite.y=FIRE5BUTTONRELESEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_fire6))
			{
				RSDOWN=0;
				fire6_sprite.y=FIRE6BUTTONRELESEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_coin))
			{
				SELECTDOWN=0;
				coin_sprite.y=COINBUTTONRELESEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_start))
			{
				STARTDOWN=0;
				start_sprite.y=STARTBUTTONRELESEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_quit))
			{
				QDOWN=0;
				quit_sprite.y=QUITBUTTONRELESEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_pause))
			{
				PAUSEDOWN=0;
				pause_sprite.y=PAUSEBUTTONRELESEDY;
			}

		}

		if (event.type== SDL_MOUSEBUTTONDOWN) 
		{		
			PointInCircle pos=checkPointPostion(event.button.x,event.button.y);
			if(pos!=OutCircle)
			{
				small_circle_rectangle.x=event.button.x- JOYSTICKOFFSET;
				small_circle_rectangle.y=event.button.y- JOYSTICKOFFSET;
				if(pos==InCircleButCenter)
				{
					double arg=mycast(event.button.x,event.button.y);
					if(arg>MYARG && arg<180.0-MYARG)
						UPDOWN=1;
					if(arg>180.0+MYARG && arg<360.0-MYARG)
						DOWNDOWN=1;
					if(arg>90.0+MYARG && arg<270.0-MYARG)
						LEFTDOWN=1;
					if((arg<90.0-MYARG && arg>=0) || (arg>270.0+MYARG))
						RIGHTDOWN=1;
				}
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_fire1))
			{
				//kinput=config_keymap.fire1;
				ADOWN=1;
				fire1_sprite.y=FIRE1BUTTONPRESSEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_fire4))
			{
				//kinput=config_keymap.fire4;
				YDOWN=1;
				fire4_sprite.y=FIRE4BUTTONPRESSEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_fire2))
			{
				//kinput=config_keymap.fire2;
				XDOWN=1;
				fire2_sprite.y=FIRE2BUTTONPRESSEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_fire3))
			{
				//kinput=config_keymap.fire3;
				BDOWN=1;
				fire3_sprite.y=FIRE3BUTTONPRESSEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_fire5))
			{
				//kinput=config_keymap.fire5;
				LSDOWN=1;
				fire5_sprite.y=FIRE5BUTTONPRESSEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_fire6))
			{
				//kinput=config_keymap.fire6;
				RSDOWN=1;
				fire6_sprite.y=FIRE6BUTTONPRESSEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_coin))
			{
				//kinput=config_keymap.coin1;
				SELECTDOWN=1;
				coin_sprite.y=COINBUTTONPRESSEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_start))
			{
				//kinput=config_keymap.start1;
				STARTDOWN=1;
				start_sprite.y=STARTBUTTONPRESSEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_quit))
			{
				//kinput=config_keymap.quit;
				QDOWN=1;
				quit_sprite.y=QUITBUTTONPRESSEDY;
			}

			if(checkClickedRange(event.button.x,event.button.y,&kz_pause))
			{
				//kinput=config_keymap.pause;
				PAUSEDOWN=1;
				pause_sprite.y=PAUSEBUTTONPRESSEDY;
			}

		}

		bool lastInCircle=false;
		if(event.type==SDL_MOUSEMOTION && event.motion.state==SDL_PRESSED)
		{
			Uint16 xoffset=event.motion.x-event.motion.xrel;
			Uint16 yoffset=event.motion.y-event.motion.yrel;
			//printf("x= %d,y= %d,xrel= %d,yrel= %d\n",event.motion.x,event.motion.y,event.motion.xrel,event.motion.yrel);
			PointInCircle pos=checkPointPostion(xoffset,yoffset);
			if(pos!=OutCircle)
			{
				lastInCircle=true;
				if(pos==InCircleButCenter){
					UPDOWN=0;
					DOWNDOWN=0;
					LEFTDOWN=0;
					RIGHTDOWN=0;
					/*
						 double arg=mycast(xoffset,yoffset);
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
			}

			if(checkClickedRange(xoffset,yoffset,&kz_fire1))
			{
				ADOWN=0;
				fire1_sprite.y=FIRE1BUTTONRELESEDY;
			}

			if(checkClickedRange(xoffset,yoffset,&kz_fire4))
			{
				YDOWN=0;
				fire4_sprite.y=FIRE4BUTTONRELESEDY;
			}

			if(checkClickedRange(xoffset,yoffset,&kz_fire2))
			{
				XDOWN=0;
				fire2_sprite.y=FIRE2BUTTONRELESEDY;
			}

			if(checkClickedRange(xoffset,yoffset,&kz_fire3))
			{
				BDOWN=0;
				fire3_sprite.y=FIRE3BUTTONRELESEDY;
			}

			if(checkClickedRange(xoffset,yoffset,&kz_fire5))
			{
				LSDOWN=0;
				fire5_sprite.y=FIRE5BUTTONRELESEDY;
			}

			if(checkClickedRange(xoffset,yoffset,&kz_fire6))
			{
				RSDOWN=0;
				fire6_sprite.y=FIRE6BUTTONRELESEDY;
			}

			if(checkClickedRange(xoffset,yoffset,&kz_coin))
			{
				SELECTDOWN=0;
				coin_sprite.y=COINBUTTONRELESEDY;
			}

			if(checkClickedRange(xoffset,yoffset,&kz_start))
			{
				STARTDOWN=0;
				start_sprite.y=STARTBUTTONRELESEDY;
			}

			if(checkClickedRange(xoffset,yoffset,&kz_quit))
			{
				QDOWN=0;
				quit_sprite.y=QUITBUTTONRELESEDY;
			}

			if(checkClickedRange(xoffset,yoffset,&kz_pause))
			{
				PAUSEDOWN=0;
				pause_sprite.y=PAUSEBUTTONRELESEDY;
			}

			/************ mouse motion with press and hold *********/

			if(lastInCircle)
			{
				PointInCircle pos=checkPointPostion(event.motion.x,event.motion.y);
				if(pos!=OutCircle)
				{
					small_circle_rectangle.x=event.motion.x- JOYSTICKOFFSET;
					small_circle_rectangle.y=event.motion.y- JOYSTICKOFFSET;
					if(pos==InCircleButCenter)
					{
						double arg=mycast(event.motion.x,event.motion.y);
						if(arg>MYARG && arg<180.0-MYARG)
							UPDOWN=1;
						if(arg>180.0+MYARG && arg<360.0-MYARG)
							DOWNDOWN=1;
						if(arg>90.0+MYARG && arg<270.0-MYARG)
							LEFTDOWN=1;
						if((arg<90.0-MYARG && arg>=0) || (arg>270.0+MYARG))
							RIGHTDOWN=1;
					}
				}
				else
				{
					small_circle_rectangle.x=CIRCLECENTERX- JOYSTICKOFFSET;
					small_circle_rectangle.y=CIRCLECENTERY- JOYSTICKOFFSET;
					/*
						 UPDOWN=0;
						 DOWNDOWN=0;
						 LEFTDOWN=0;
						 RIGHTDOWN=0;
						 */
				}
			}

			if(checkClickedRange(event.motion.x,event.motion.y,&kz_fire1))
			{
				//kinput=config_keymap.fire1;
				ADOWN=1;
				fire1_sprite.y=FIRE1BUTTONPRESSEDY;
			}

			if(checkClickedRange(event.motion.x,event.motion.y,&kz_fire4))
			{
				//kinput=config_keymap.fire4;
				YDOWN=1;
				fire4_sprite.y=FIRE4BUTTONPRESSEDY;
			}

			if(checkClickedRange(event.motion.x,event.motion.y,&kz_fire2))
			{
				//kinput=config_keymap.fire2;
				XDOWN=1;
				fire2_sprite.y=FIRE2BUTTONPRESSEDY;
			}

			if(checkClickedRange(event.motion.x,event.motion.y,&kz_fire3))
			{
				//kinput=config_keymap.fire3;
				BDOWN=1;
				fire3_sprite.y=FIRE3BUTTONPRESSEDY;
			}

			if(checkClickedRange(event.motion.x,event.motion.y,&kz_fire5))
			{
				//kinput=config_keymap.fire5;
				LSDOWN=1;
				fire5_sprite.y=FIRE5BUTTONPRESSEDY;
			}

			if(checkClickedRange(event.motion.x,event.motion.y,&kz_fire6))
			{
				//kinput=config_keymap.fire6;
				RSDOWN=1;
				fire6_sprite.y=FIRE6BUTTONPRESSEDY;
			}

			if(checkClickedRange(event.motion.x,event.motion.y,&kz_coin))
			{
				//kinput=config_keymap.coin1;
				SELECTDOWN=1;
				coin_sprite.y=COINBUTTONPRESSEDY;
			}

			if(checkClickedRange(event.motion.x,event.motion.y,&kz_start))
			{
				//kinput=config_keymap.start1;
				STARTDOWN=1;
				start_sprite.y=STARTBUTTONPRESSEDY;
			}

			if(checkClickedRange(event.motion.x,event.motion.y,&kz_quit))
			{
				//kinput=config_keymap.quit;
				QDOWN=1;
				quit_sprite.y=QUITBUTTONPRESSEDY;
			}

			if(checkClickedRange(event.motion.x,event.motion.y,&kz_pause))
			{
				//kinput=config_keymap.pause;
				PAUSEDOWN=1;
				pause_sprite.y=PAUSEBUTTONPRESSEDY;
			}

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
	drawSprite(SDL_VideoBuffer,myscreen,0,0,0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
	if (nBurnFPS>5900) ioctl(fbdev,FBIO_WAITFORVSYNC,&vb); //use vblank if running at 60 hz
	/***** flip all virtual buttons surface *****/
	SDL_BlitSurface(buttons_surface,&fire1_sprite,myscreen,&v_fire1_button);
	SDL_BlitSurface(buttons_surface,&fire2_sprite,myscreen,&v_fire2_button);
	SDL_BlitSurface(buttons_surface,&fire3_sprite,myscreen,&v_fire3_button);
	SDL_BlitSurface(buttons_surface,&fire4_sprite,myscreen,&v_fire4_button);
	SDL_BlitSurface(buttons_surface,&fire5_sprite,myscreen,&v_fire5_button);
	SDL_BlitSurface(buttons_surface,&fire6_sprite,myscreen,&v_fire6_button);
	SDL_BlitSurface(buttons_surface,&start_sprite,myscreen,&v_start_button);
	SDL_BlitSurface(buttons_surface,&coin_sprite,myscreen,&v_coin_button);
	SDL_BlitSurface(buttons_surface,&pause_sprite,myscreen,&v_pause_button);
	SDL_BlitSurface(buttons_surface,&quit_sprite,myscreen,&v_quit_button);
	SDL_BlitSurface(joystick_surface,&big_circle_sprite,myscreen,&big_circle_rectangle);
	SDL_BlitSurface(joystick_surface,&small_circle_sprite,myscreen,&small_circle_rectangle);
	/***** flip all virtual buttons surface *****/
	SDL_Flip(myscreen);
	SDL_LockSurface(SDL_VideoBuffer);
}

void freeVirtualButtons()
{
	/***** free all virtual buttons surface *****/ 
	printf("Karin debug -> free image surfaces......");
	if(buttons_surface!= NULL) SDL_FreeSurface(buttons_surface);
	if(joystick_surface!= NULL) SDL_FreeSurface(joystick_surface);
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
