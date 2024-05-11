#include "common.h"

// #define AST_FPS 20
static bool down(void);
static bool up(void);
static display_t draw(void);
static bool modeset(void);
static void  show_ast(void);
static u8 ast_fps = 20;

void watchface_ast()
{
    printf("watchface_ast\r\n");
	display_setDrawFunc(draw);
	buttons_setFuncs(up, menu_select, down);
	buttons_setFuncs_add4(modeset);
	animation_start(NULL, ANIM_MOVE_ON);
	watchface_save(watchface_ast);
}
static bool modeset()
{
    ast_fps +=10;

    if(ast_fps>60)
    {
        ast_fps = 10;
    }
    //printf("ast_fps=%d\r\n",ast_fps);
	return true;
}

static bool down()
{
    display_f  fun = get_other_watchface(watchface_ast, 1);
	display_set(fun);
	display_load();
	return true;
}

static bool up()
{
    display_f  fun = get_other_watchface(watchface_ast, 0);
	display_set(fun);
	display_load();

	return true;
}

static display_t draw()
{
    show_ast();					
    watchface_drawDate();
	watchface_ticker();
    show_battery_mode_1();
							
	return DISPLAY_DONE;
}

static  u8 ast_Tick(void)
{
	static u8  iAng = 1;
	
    iAng++;
    if(iAng == 10) iAng = 0;
	
	return iAng;
	// printf("iAng=%d\r\n",iAng);
}

void  show_ast(void)
{
    static int lastDraw; // Time of last draw
    int ast_flash_time = 1000/ast_fps;
    static int ast_pos = 0;
	int now = millis();
    int ca = now - lastDraw;

	if((ca >= ast_flash_time)|| (ca <= 0 && ca+1000 >=  ast_flash_time))
    {
		lastDraw = now;
        ast_pos = ast_Tick();
	}

    draw_bitmap(5, 5, astronaut_bmp[ast_pos], 52, 48, NOINVERT, 0);
}
