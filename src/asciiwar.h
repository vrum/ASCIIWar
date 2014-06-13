/*
* ASCIIWar
* Copyright (c) 2014 Frédéric Brachfogel
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Frédéric Brachfogel may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY FREDERIC BRACHFOGEL ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL FREDERIC BRACHFOGEL BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef ASCIIWAR_H
#define	ASCIIWAR_H

/*
 * std
 */

#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <cstring>
#include <cfloat>
#include <cassert>
#include <list>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <float.h>

#ifdef WIN32
#include <Windows.h>
#elif defined(__linux)
#include "linux.h"
#endif

using namespace std;

/*
 * misc
 */

typedef string str;
#define null NULL

/*
 * includes
 */

#include <pthread.h>
#include <libtcod.h>
#include <jansson.h>
#include <enet/enet.h>
#include <zlib.h>
#include <SDL/SDL.h>
#include <SDL_mixer.h>

/*
 * ASCIIWar
 */

#define RELEASE 										false
#define REPLAY 											false
#define RECORD 											false
#define FULLSCREEN    							true
#define THREADS											true

#define NORMAL_TIME_STEP 						10
#define SAME_UNIT_TYPE_SIZE 				40
#define MAX_GROUP_SELECTION 				10
#define MAX_TIME_STEP 							50
#define CMD_TURN_SHIFT 							2
#define STARTING_TURN_FRAME_COUNT 	1
#define MAX_TURN_FRAME_COUNT				20
#define MAX_AVER_FRAME_TIMES 				300
#define MAX_AVER_WAIT_TIMES 				1
#define MAX_MAX_PING_TIMES 					100	
#define MAX_FRAME_TIME 							(MAX_TURN_FRAME_COUNT*NORMAL_TIME_STEP)							
#define ZLIB_CHUNK									(1<<18) /* 256Kb */
#define MAX_CHAR  									64
#define SMALL_SIZE									32
#define AW_null 										-1
#define CHAR_SIZE	 									16
#define MAP_SIZE_X     							512
#define MAP_SIZE_Y     							512
#define COLOR(C) 										((((C).r)<<16) + (((C).g)<<8) + (((C).b)<<0))
#define AMBIENT_LIGHT 							50
#define AMBIENT_LIGHTMAP 						TCOD_color_multiply_scalar(TCOD_color_RGB(97, 157, 180), 1)
#define UNIT_AO_STRENGTH 						0.95f
#define DIR_LIGHTMAP_X 							8
#define DIR_LIGHTMAP_Y 							16
#define DIR_LIGHTMAP_COLOR 					TCOD_color_multiply_scalar(TCOD_color_RGB(255, 222, 0), 1)
#define MAP_SIZE 										(MAP_SIZE_X * MAP_SIZE_Y)
#define RANGE_SHIFT 								2
#define RANGE_MAP_SIZE_X 						(MAP_SIZE_X>>RANGE_SHIFT)
#define RANGE_MAP_SIZE_Y 						(MAP_SIZE_Y>>RANGE_SHIFT)
#define RANGE_MAP_SIZE 							(RANGE_MAP_SIZE_X * RANGE_MAP_SIZE_Y)
#define MINIMAP_CON_SIZE_X 					12
#define MINIMAP_CON_SIZE_Y 					12
#define MINIMAP_CON_START_X 				(CON_RES_X - MINIMAP_CON_SIZE_Y - 2)
#define MINIMAP_CON_START_Y 				(CON_RES_Y - MINIMAP_CON_SIZE_Y - 2)
#define MINIMAP_START_Y 						(MINIMAP_CON_START_Y*CHAR_SIZE)
#define MINIMAP_RATIO_X 						(MINIMAP_CON_SIZE_X*CHAR_SIZE)/MAP_SIZE_X
#define MINIMAP_RATIO_Y 						(MINIMAP_CON_SIZE_Y*CHAR_SIZE)/MAP_SIZE_Y
#define MINIMAP_INV_RATIO_X 				MAP_SIZE_X/(MINIMAP_CON_SIZE_X*CHAR_SIZE)
#define MINIMAP_INV_RATIO_Y 				MAP_SIZE_Y/(MINIMAP_CON_SIZE_Y*CHAR_SIZE)
#define HUD_HEIGHT 									9
#define ABILITIES_LIST_WIDTH 				16
#define ABILITIES_PER_LIST 					(HUD_HEIGHT-1)
#define START_ABILITIES_X 					(CON_RES_X-ABILITIES_LIST_WIDTH-MINIMAP_CON_SIZE_X-2-2)
#define HUD_START_Y   							(CON_RES_Y-HUD_HEIGHT)
#define HUD_RES_Y     							(CON_RES_Y)-(MAP_SIZE_Y)
#define LAGOMETER_SIZE 							MAX_AVER_FRAME_TIMES
#define LAGOMETER_START_Y 					((CON_RES_Y*CHAR_SIZE)-LAGOMETER_SIZE)
#define FLUID_DIFFUSION 						0.00001
#define FLUID_VISCOSITY 						0.0000001
#define FLUID_BOX_SIZE 							80
#define FLUID_BOX_SIZE_SQUARE 			(FLUID_BOX_SIZE*FLUID_BOX_SIZE)
#define TEAM_ID(PLAYER_ID) 					(player(GI_GetPlayerPtr(gi, (PLAYER_ID))).team_id)
#define SWAP(T, A, B)								{T tmp; tmp = A; A = B; B = tmp;}
#define AT(M, X, Y) 								(gi->M[(Y)*MAP_SIZE_X+(X)])
#define AT2(M, X, Y) 								(M[(Y)*MAP_SIZE_X+(X)])
#define AT_ENV(X, Y) 								(gi->env_map[(Y)*RANGE_MAP_SIZE_X+(X)])
#define AT_FL(M, X, Y) 							(fl->M[(Y)*FLUID_BOX_SIZE+(X)])
#define AT_FL2(M, X, Y) 						(M[(Y)*FLUID_BOX_SIZE+(X)])
#define ON_CON_X(X) 								(((X)-offx)/CHAR_SIZE)
#define ON_CON_Y(Y) 								(((Y)-offy)/CHAR_SIZE)
#define INSIDE_MAP(X, Y) \
 	((X) >= 0 && (X) < MAP_SIZE_X && (Y) >= 0 && (Y) < MAP_SIZE_Y)
#define INSIDE_SCREEN(X, Y) \
 	((X) >= 0 && (X) < resx && (Y) >= 0 && (Y) < resy \
 	&& gi->post_render_mask[CON_RES_X*ON_CON_Y(Y)+ON_CON_X(X)] != gi->frame_mark)
#define INSIDE_CON(X, Y) \
 		 ((X) >= 0 && (X) < CON_RES_X && (Y) >= 0 && (Y) < HUD_START_Y \
 	&& (((X) < MINIMAP_CON_START_X || (X) >= MINIMAP_CON_START_X+MINIMAP_CON_SIZE_X) \
 	|| ((Y) < MINIMAP_CON_START_Y || (Y) >= MINIMAP_CON_START_Y+MINIMAP_CON_SIZE_Y)))
#define INSIDE_HUD(X, Y) ((X) >= 0 && (X) < CON_RES_X && (Y) >= HUD_START_Y && (Y) < CON_RES_Y)
#define INSIDE_FULL_CON(X, Y) ((X) >= 0 && (X) < CON_RES_X && (Y) >= 0 && (Y) < CON_RES_Y)
#define INSIDE_MINI(X, Y) \
 	   ((X) >= MINIMAP_CON_START_X && (X) < MINIMAP_CON_START_X+MINIMAP_CON_SIZE_X \
 	&&  (Y) >= MINIMAP_CON_START_Y && (Y) < MINIMAP_CON_START_Y+MINIMAP_CON_SIZE_Y)
#define INSIDE_FLUID(X, Y) \
 	   ((X) >= fl->pos_x - FLUID_BOX_SIZE/2 && (X) < fl->pos_x + FLUID_BOX_SIZE/2 \
 	 && (Y) >= fl->pos_y - FLUID_BOX_SIZE/2 && (Y) < fl->pos_y + FLUID_BOX_SIZE/2)
#define INSIDE_FLUID2(X, Y) \
 	   ((X) >= 0 && (X) < FLUID_BOX_SIZE \
 	 && (Y) >= 0 && (Y) < FLUID_BOX_SIZE)
#define BOX_INSIDE_MAP(X1, Y1, X2, Y2, X3, Y3, X4, Y4) \
 		(INSIDE_MAP(X1, Y1) \
  || INSIDE_MAP(X2, Y2) \
  || INSIDE_MAP(X3, Y3) \
  || INSIDE_MAP(X4, Y4))
#define BOUND(BX, X) \
 	if((X) < 0) (X) = 0; \
  if((X) > (BX)) (X) = (BX);
#define IN_ORDER(X1, Y1, X2, Y2) \
  if((X1) > (X2)) SWAP(short, (X1), (X2)); \
  if((Y1) > (Y2)) SWAP(short, (Y1), (Y2));
#define BOUND_IN_ORDER(X1, Y1, X2, Y2) \
  BOUND(MAP_SIZE_X-1, X1) BOUND(MAP_SIZE_Y-1, Y1) \
  BOUND(MAP_SIZE_X-1, X2) BOUND(MAP_SIZE_Y-1, Y2) \
  IN_ORDER(X1, Y1, X2, Y2)
#define BOUND_VIEWPORT \
  if(cl->viewport_x < 0) \
    cl->viewport_x = 0; \
  if(cl->viewport_x > MAP_SIZE_X-CON_RES_X) \
    cl->viewport_x = MAP_SIZE_X-CON_RES_X; \
  if(cl->viewport_y < 0) \
    cl->viewport_y = 0; \
  if(cl->viewport_y > MAP_SIZE_Y-HUD_START_Y) \
    cl->viewport_y = MAP_SIZE_Y-HUD_START_Y;
#define FOR_RECT(X1, X2, Y1, Y2) \
  for(short i = X1, _finished = 0; !_finished && i <= X2; i++) \
    for(short j = Y1; !_finished && j <= Y2; j++)
#define SOCLE(SIZE) \
  short _size = (SIZE)-1; \
  short _half = halfes[_size]; \
  short _a = 0-_half; \
  short _b = _size-_half; \
	FOR_RECT(_a, _b, _a, _b)
#define DO_TIMES(I) \
  for(int f = 0, _endi = (I); f < _endi; f++)
#define STR_EQ(S1, S2) strcmp((S1), (S2)) == 0
#define ON_SCREEN(X, Y) \
  (*(int*)((unsigned char*)surface->pixels + \
  	surface->pitch*(Y) + \
  	(X)*surface->format->BytesPerPixel))
#define ON_MINIMAP(X, Y) \
	ON_SCREEN((X)*MINIMAP_RATIO_X+MINIMAP_START_X, (Y)*MINIMAP_RATIO_Y+MINIMAP_START_Y)
#define ON_LAGOMETER(X, Y) \
	ON_SCREEN((X)+LAGOMETER_START_X, (Y)+LAGOMETER_START_Y)
#define ON_CLOSE_MAP(X, Y) close_map[(int)(Y)*MAP_SIZE_X+(X)]
#define ON_FOV_MAP(X, Y) gi->fov_maps[pl->team_id][RANGE_MAP_SIZE_X*(Y)+(X)]
#define START_BENCH \
	AW_time_t t = AW_GetTime();
#define END_BENCH \
	trace((AW_GetTime() - t));
#define SCREEN_UNIT_X(UN) 		(offx+((UN)->pos_x-cl->viewport_x-halfes[SIZE((UN))-1])*CHAR_SIZE)
#define SCREEN_UNIT_Y(UN) 		(offy+((UN)->pos_y-cl->viewport_y-halfes[SIZE((UN))-1])*CHAR_SIZE)
#define SCREEN_UNIT_SIZE(UN) 	(SIZE((UN))*CHAR_SIZE)
#define IS_COLOR(R, G, B) 		(c.r == (R) && c.g == (G) && c.b == (B))
#define SATURATE(X) 					MAX(0, MIN(1, (X)))
#if RELEASE
	#define MAIN_MUSIC "./data/Asgaror_39_s_Depths.ogg"
#else
	#define MAIN_MUSIC "none"
#endif
#define OPEN_SOUND(S) \
	Mix_LoadWAV_RW(SDL_RWFromFile((S), "rb"), 0)
#define START_SOUND(S, LOOP) \
	AW_sound_ptr s = SO_New(gi); \
  AW_sound_t *so = &sound(s); \
	so->channel = Mix_PlayChannel(-1, gi->S, (LOOP));
#define SLOW_START_SOUND(S, LOOP) \
	START_SOUND(S, LOOP) \
  Mix_Volume(so->channel, 0);

#define GOLD_PRICE(UN) 								(unit_dic[(UN)->unit_type].gold_price)
#define XP(UN) 												(unit_dic[(UN)->unit_type].xp)
#define BUILD_TIME(UN) 								(unit_dic[(UN)->unit_type].build_time)
#define MOVE_SPEED(UN) 								(unit_dic[(UN)->unit_type].move_speed)
#define ATTACK_SPEED(UN) 							(unit_dic[(UN)->unit_type].attack_speed)
#define ATTACK_DAMAGE(UN)  						(unit_dic[(UN)->unit_type].attack_damage)
#define MAX_HP(UN) 										(unit_dic[(UN)->unit_type].max_hp)
#define MAX_MANA(UN) 									(unit_dic[(UN)->unit_type].max_mana)
#define SIZE(UN) 											(unit_dic[(UN)->unit_type].size)
#define VISIBILITY_RANGE(UN) 					(unit_dic[(UN)->unit_type].visibility_range)
#define AGGRO_RANGE(UN) 							(unit_dic[(UN)->unit_type].aggro_range)
#define ATTACK_RANGE(UN) 							(unit_dic[(UN)->unit_type].attack_range)
#define CAN_BE_PUSHED(UN) 						(unit_dic[(UN)->unit_type].can_be_pushed)
#define ASCII(UN) 										(unit_dic[(UN)->unit_type].ascii)

#define MAX_PEER 										32
#define MAX_PLAYER 									8
#define MAX_TEAM 										2
#define MAX_PATH_LEN								(MAP_SIZE_X * 5)
#define MAX_BLOCKED_TIME						50
#define MAX_SPECIAL_BLOCKED_TIME 		250
// this one must be the greater time threshold
#define FOLLOW_COUNTER 							350
#define FOLLOW_R_SQUARED 						((SIZE(un)*3) * (SIZE(un)*3))
#define MAX_FORMATION_MOVE					(60*60)
#define MAX_DEADLOCK_TELEPORT 			(SIZE(un)*3)
#define MAX_ADVANCE_TURN 						10

#define BACKGROUND_COLOR 	TCOD_color_RGB(10, 10, 10)
#define SPEC_COLOR 				TCOD_color_RGB(20, 20, 20)
#define EMPTY_COLOR 			TCOD_color_RGB(94, 61, 40)//TCOD_color_RGB(116, 88, 62)//TCOD_color_RGB(94, 61, 40)
#define WALL_COLOR 				TCOD_color_RGB(59, 79, 117)//TCOD_color_RGB(76, 91, 118)//TCOD_color_RGB(158, 145, 126)//TCOD_color_RGB(76, 91, 118)//TCOD_dark_grey
#define WATER_COLOR 			TCOD_blue
#define WATER2_COLOR 			TCOD_darkest_blue
#define GRASS_COLOR 			TCOD_color_RGB(52, 150, 52)
#define BUSH1_COLOR 			TCOD_color_RGB(0, 142, 0)
#define BUSH2_COLOR 			TCOD_color_RGB(0, 130, 0)
#define TREE1_COLOR 			TCOD_color_RGB(0, 100, 0)
#define TREE2_COLOR 			TCOD_color_RGB(0, 150, 0)
#define TREE3_COLOR 			TCOD_color_RGB(0, 135, 0)
#define MUSH_COLOR 				TCOD_color_RGB(230, 10, 5)
#define TILE_COLOR 				TCOD_dark_grey
#define TILE2_COLOR 			TCOD_light_grey
#define TORCH_COLOR 			TCOD_amber
#define PLANK_COLOR 			TCOD_color_RGB(97, 58, 16)
#define PLANK2_COLOR 			TCOD_color_RGB(73, 37, 10)
#define BLUE 							TCOD_color_RGB(34, 70, 240)//TCOD_color_RGB(34, 90, 192)
#define RED 							TCOD_color_RGB(240, 29, 29)
#define BLOOD_COLOR 			TCOD_color_RGB(195, 25, 12)
#define BURN_TRACE_COLOR 	TCOD_color_RGB(45, 45, 45)
#define FLUID_COLOR 			TCOD_color_RGB(210, 210, 210)
#define LIGHTNING_COLOR 	TCOD_color_RGB(150, 200, 255)

#define	AW_EMPTY 		'.' 	
#define	AW_WALL  		'#'
#define	AW_WATER 		'~'	
#define AW_WATER2 	'~'	
#define	AW_GRASS 		','
#define	AW_BUSH1 		';'
#define	AW_BUSH2 		':'
#define	AW_TREE1 		0x5
#define	AW_TREE2 		0x6
#define	AW_TREE3 		0x14
#define AW_MUSH  		0x18
#define	AW_TILE  		0x16
#define	AW_TILE2 		0x4
#define	AW_TORCH 		'!'	
#define AW_PLANK 		'='	
#define AW_PLANK2 	0xbd	

struct AW_game_instance_t;

typedef short 					AW_ptr_t;
typedef int 						AW_id_t;
typedef int 						AW_time_t;
typedef str 						AW_net_ip_t;
typedef unsigned short 	AW_net_port_t;
typedef short AW_path_ptr;
#define path(p, w)									(gi->path_pool[p][w])
#define path2(p, w)									(gi->path_pool2[p][w])
#define path_point_ptr(id, index)		(gi->paths + (((id) * MAX_PATH_LEN + (index))<<1))

/*
 * buttons
 */

#define BTN_GLARE_SIZE 	4
#define MAX_BTN 				64

struct AW_btn_t {
	TCOD_color_t 		color;
	str 						s;
	short 					pos_x, 
									pos_y;
	char						shortcut;
	TCOD_keycode_t 	shortcut_special;
	bool  					highlight,
									entry;
	void						*user_data;
};

struct AW_btn_list_t {
	AW_btn_t 	btns[MAX_BTN];
	short 		btn_count;
	short 		pos_x,
						pos_y;
};

void BTN_UpdateEntry		(AW_game_instance_t *gi, AW_btn_t *b);
void BTN_Render					(AW_game_instance_t *gi, AW_btn_t *b);
void BTN_Render					(AW_game_instance_t *gi, AW_btn_list_t *l, bool center_x=true, short step=2);
bool BTN_IsMouseOver		(AW_game_instance_t *gi, AW_btn_t *b);
bool BTN_IsClicked			(AW_game_instance_t *gi, AW_btn_t *b);
bool BTN_GonnaBeClicked	(AW_game_instance_t *gi, AW_btn_t *b);
void BTN_CenterX 				(AW_game_instance_t *gi, AW_btn_t *b, short axe=-1);
void BTN_CenterY 				(AW_game_instance_t *gi, AW_btn_t *b);
void BTN_CenterY 				(AW_game_instance_t *gi, AW_btn_list_t *l);

/*
 * unit order
 */

#define MAX_UNIT_ORDER 				30000
#define order(ptr)						(gi->orders[(ptr)])
typedef short AW_unit_order_ptr;
typedef short AW_unit_ptr;
typedef void (*AW_unit_order_cb_t)(AW_game_instance_t *gi, AW_unit_ptr u, AW_unit_order_ptr o);

enum AW_unit_order_type_t {
	AW_unit_order_none,
	AW_unit_order_move,
	AW_unit_order_attack,
	AW_unit_order_follow_target,
};

struct AW_unit_order_t {
	AW_unit_order_type_t 		order;
	AW_unit_ptr							target;
	AW_id_t 								target_player_id,
													target_cmd_id,
													cmd_mask,
													task_id;
	short 									target_cx,
													target_cy,
													click_cx,
													click_cy,
													chase_cx_proxy,
													chase_cy_proxy,
													path_len_proxy,
													last_target_cx,
													last_target_cy,
													r_squared,
													d_squared;
	AW_unit_order_ptr 			fnext,
													next;
	AW_time_t 							counter,
													time_stamp;												
	bool 										started,
													starting,
													started_proxy,
													done,
													with_counter,
													attack_here,
													precise_target,
													following_friend,
													generated;
	AW_ptr_t 								user_data;
	AW_unit_order_cb_t 			unit_order_completed_cb,
													unit_order_failed_cb;
};

void 									UO_Init					(AW_game_instance_t *gi, int argc, char** argv);
AW_unit_order_ptr 		UO_New					(AW_game_instance_t *gi);
AW_unit_order_ptr 		UO_PushFront 		(AW_game_instance_t *gi, AW_unit_ptr u);
AW_unit_order_ptr 		UO_PushBack			(AW_game_instance_t *gi, AW_unit_ptr u);
void 									UO_FreeFront		(AW_game_instance_t *gi, AW_unit_ptr u, bool complete=false);
void 									UO_FreeAll 			(AW_game_instance_t *gi, AW_unit_ptr u);
AW_unit_order_type_t 	UO_GetFrontType (AW_game_instance_t *gi, AW_unit_ptr u);
AW_unit_order_t* 			UO_GetFront 		(AW_game_instance_t *gi, AW_unit_ptr u);
bool 									UO_IsTarget			(AW_game_instance_t *gi, AW_unit_ptr u, AW_unit_ptr target, AW_unit_order_ptr o=AW_null);

/*
 * balls
 */

#define MAX_BALL 				512
#define BALL_SPEED 			15
#define ball(ptr)				(gi->balls[(ptr)])
typedef short AW_ball_ptr;
typedef short AW_player_ptr;

struct AW_ball_t {
	AW_player_ptr p;
	AW_time_t 		move;
	short 				pos_x,
								pos_y;
	short 				damage;
	AW_unit_ptr 	target;
	AW_id_t 			target_player_id,
								target_cmd_id;
	AW_ball_ptr		next,
								previous,
								fnext;
};

void 									BA_Init					(AW_game_instance_t *gi, int argc, char** argv);
AW_ball_ptr 					BA_New					(AW_game_instance_t *gi);
void 									BA_Free					(AW_game_instance_t *gi, AW_ball_ptr b);
void 									BA_FreeAll 			(AW_game_instance_t *gi, AW_ball_ptr b);
bool 									BA_Update 			(AW_game_instance_t *gi, AW_ball_ptr b);

/*
 * smokes
 */

#define MAX_SMOKE 					MAX_BALL
#define SMOKE_LIFE_SPAN 		500
#define SMOKE_MAX_RADIUS 		2
#define SMOKE_STRENGTH 			0.75f
#define smoke(ptr)					(gi->smokes[(ptr)])
typedef short AW_smoke_ptr;

struct AW_smoke_t {
	AW_time_t 		span;
	short 				pos_x,
								pos_y,
								d;
	AW_smoke_ptr	next,
								previous,
								fnext;
};

void 									SMOKE_Init				(AW_game_instance_t *gi, int argc, char** argv);
AW_smoke_ptr 					SMOKE_New					(AW_game_instance_t *gi);
void 									SMOKE_Free				(AW_game_instance_t *gi, AW_smoke_ptr b);
void 									SMOKE_FreeAll 		(AW_game_instance_t *gi, AW_smoke_ptr b);
bool 									SMOKE_Update 			(AW_game_instance_t *gi, AW_smoke_ptr b);

/*
 * blood
 */

#define MAX_BLOOD 					MAX_BALL
#define BLOOD_LIFE_SPAN 		10000
#define blood(ptr)					(gi->bloods[(ptr)])
typedef short AW_blood_ptr;

struct AW_blood_t {
	TCOD_color_t 	c;
	AW_time_t 		span;
	short 				pos_x,
								pos_y;
	AW_blood_ptr	next,
								previous,
								fnext;
};

void 									BLOOD_Init				(AW_game_instance_t *gi, int argc, char** argv);
AW_blood_ptr 					BLOOD_New					(AW_game_instance_t *gi);
void 									BLOOD_Free				(AW_game_instance_t *gi, AW_blood_ptr b);
void 									BLOOD_FreeAll 		(AW_game_instance_t *gi, AW_blood_ptr b);
bool 									BLOOD_Update 			(AW_game_instance_t *gi, AW_blood_ptr b);

/*
 * burn_trace
 */

#define MAX_BURN_TRACE 				8392
#define BURN_TRACE_LIFE_SPAN 	10000000
#define burn_trace(ptr)						(gi->burn_traces[(ptr)])
typedef short AW_burn_trace_ptr;

struct AW_burn_trace_t {
	TCOD_color_t 	c;
	AW_time_t 		span;
	float 				opacity;
	short 				pos_x,
								pos_y;
	AW_burn_trace_ptr	next,
								previous,
								fnext;
};

void 									BT_Init				(AW_game_instance_t *gi, int argc, char** argv);
AW_burn_trace_ptr 		BT_New				(AW_game_instance_t *gi);
void 									BT_Free				(AW_game_instance_t *gi, AW_burn_trace_ptr b);
void 									BT_FreeAll 		(AW_game_instance_t *gi, AW_burn_trace_ptr b);
bool 									BT_Update 		(AW_game_instance_t *gi, AW_burn_trace_ptr b);

/*
 * floating_text
 */

#define MAX_FLOATING_TEXT 					512
#define FLOATING_TEXT_LIFE_SPAN 		1000
#define floating_text(ptr)					(gi->floating_texts[(ptr)])
typedef short AW_floating_text_ptr;

struct AW_floating_text_t {
	TCOD_color_t 					c;
	AW_time_t 						span;
	str 									s;
	short 								pos_x,
												pos_y;
	AW_floating_text_ptr	next,
												previous,
												fnext;
};

void 									FT_Init				(AW_game_instance_t *gi, int argc, char** argv);
AW_floating_text_ptr 	FT_New				(AW_game_instance_t *gi);
void 									FT_Free				(AW_game_instance_t *gi, AW_floating_text_ptr ft);
void 									FT_FreeAll 		(AW_game_instance_t *gi, AW_floating_text_ptr ft);
bool 									FT_Update 		(AW_game_instance_t *gi, AW_floating_text_ptr ft);

/*
 * lights
 */

#define MAX_LIGHT 				256
#define light(ptr)				(gi->lights[(ptr)])
typedef short AW_light_ptr;

struct AW_light_t {
	TCOD_color_t 	c;
	short 				r,
								pos_x,
								pos_y;
 	float 				flickx;
	AW_light_ptr	next,
								previous,
								fnext;
};

void 									LI_Init					(AW_game_instance_t *gi, int argc, char** argv);
AW_light_ptr 					LI_New					(AW_game_instance_t *gi);
void 									LI_Free					(AW_game_instance_t *gi, AW_light_ptr l);
void 									LI_FreeAll 			(AW_game_instance_t *gi, AW_light_ptr l);
void 									LI_Update 			(AW_game_instance_t *gi);

/*
 * sounds
 */

#define MAX_SOUND 				256
#define sound(ptr)				(gi->sounds[(ptr)])
typedef short AW_sound_ptr;
typedef short AW_client_ptr;

struct AW_sound_t {
	int 					channel,
								volume;
	AW_unit_ptr 	u;
	short 				pos_x,
								pos_y;
	AW_sound_ptr	next,
								previous,
								fnext;
};

void 									SO_Init					(AW_game_instance_t *gi, int argc, char** argv);
AW_sound_ptr 					SO_New					(AW_game_instance_t *gi);
void 									SO_Free					(AW_game_instance_t *gi, AW_sound_ptr l);
void 									SO_FreeAll 			(AW_game_instance_t *gi, AW_sound_ptr l);
void 									SO_Update 			(AW_game_instance_t *gi, AW_client_ptr c);

/*
 * fluids
 */

#define MAX_FLUID 				32
#define FLUID_MAX_COUNTER 10
#define FLUID_SPAN 				30000
#define fluid(ptr)				(gi->fluids[(ptr)])
typedef short AW_fluid_ptr;
typedef short AW_client_ptr;

struct AW_worker_fluid_param_t {
	AW_game_instance_t *gi;
	AW_fluid_ptr f;
	AW_time_t game_time_step;
};

struct AW_fluid_t {
	float 									fluid_s[FLUID_BOX_SIZE_SQUARE],
  												fluid_density[FLUID_BOX_SIZE_SQUARE],
  												fluid_final_density[FLUID_BOX_SIZE_SQUARE],
  												fluid_max_density[FLUID_BOX_SIZE_SQUARE],
  												fluid_Vx[FLUID_BOX_SIZE_SQUARE],
  												fluid_Vy[FLUID_BOX_SIZE_SQUARE],
  												fluid_Vx0[FLUID_BOX_SIZE_SQUARE],
  												fluid_Vy0[FLUID_BOX_SIZE_SQUARE],
  												diffusion;
	short 									counter,
													pos_x,
													pos_y;
	AW_time_t 							span;
	TCOD_color_t 						color;
	AW_fluid_ptr						next,
													previous,
													fnext;
	AW_worker_fluid_param_t worker_param;
	HANDLE 									handle;
	DWORD										id;
};

void 					FL_Init				(AW_game_instance_t *gi, int argc, char** argv);
AW_fluid_ptr 	FL_New				(AW_game_instance_t *gi);
void 					FL_Free				(AW_game_instance_t *gi, AW_fluid_ptr l);
void 					FL_FreeAll 		(AW_game_instance_t *gi, AW_fluid_ptr l);
void 					FL_Step 			(AW_game_instance_t *gi, AW_fluid_ptr l);
bool 					FL_Update 		(AW_game_instance_t *gi, AW_fluid_ptr l);
DWORD WINAPI 	FL_WorkerFun 	(void *param);
void 					FL_SetBnd 		(AW_game_instance_t *gi, int b, float *x);
void 					FL_LinSolve		(AW_game_instance_t *gi, int b, float *x, float *x0, float a, float c, int iter);
void 					FL_Diffuse		(AW_game_instance_t *gi, int b, float *x, float *x0, float diff, int iter);
void 					FL_Project		(AW_game_instance_t *gi, float *velocX, float *velocY, float *p, float *div, int iter);
void 					FL_Advect			(AW_game_instance_t *gi, int b, float *d, float *d0,  float *velocX, float *velocY);

/*
 * build_explosions
 */

#define MAX_BUILD_EXPLOSION 			MAX_FLUID
#define BUILD_EXPLOSION_SPAN 			500
#define build_explosion(ptr)			(gi->build_explosions[(ptr)])
typedef short AW_build_explosion_ptr;

struct AW_build_explosion_t {
	AW_time_t 							span;
	short 									state;
	short 									pos_x,
													pos_y,
													size;
	AW_build_explosion_ptr	next,
													previous,
													fnext;
};

void 										BE_Init					(AW_game_instance_t *gi, int argc, char** argv);
AW_build_explosion_ptr  BE_New					(AW_game_instance_t *gi);
void 										BE_Free					(AW_game_instance_t *gi, AW_build_explosion_ptr l);
void 										BE_FreeAll 			(AW_game_instance_t *gi, AW_build_explosion_ptr l);
bool 										BE_Update 			(AW_game_instance_t *gi, AW_build_explosion_ptr c);

/*
 * lightnings
 */

#define MAX_LIGHTNING 						256
#define LIGHTNING_STEP_COUNT 			6 						
#define MAX_LIGHTNING_POINT 			(32)
#define LIGHTNING_SPAN 						300
#define lightning(ptr)			(gi->lightnings[(ptr)])
typedef short AW_lightning_ptr;

struct AW_lightning_t {
	unsigned int 						seed;
	AW_time_t 							span;
	float 									opacity;
	short 									start_x,
													start_y,
													end_x,
													end_y,
													point_count,
													pos_x[MAX_LIGHTNING_POINT],
													pos_y[MAX_LIGHTNING_POINT];
	TCOD_color_t 						color;
	AW_lightning_ptr	next,
													previous,
													fnext;
};

void 										LG_Init					(AW_game_instance_t *gi, int argc, char** argv);
AW_lightning_ptr  			LG_New					(AW_game_instance_t *gi);
void 										LG_Free					(AW_game_instance_t *gi, AW_lightning_ptr l);
void 										LG_FreeAll 			(AW_game_instance_t *gi, AW_lightning_ptr l);
bool 										LG_Update 			(AW_game_instance_t *gi, AW_lightning_ptr c);
void 										LG_Compute 			(AW_game_instance_t *gi, AW_lightning_ptr l);
void 										LG_BuildPoints	(AW_game_instance_t *gi, AW_lightning_ptr l, int x0, int y0, int x1, int y1, int i, int j, int level);

/*
 * status_effects
 */

#define MAX_STATUS_EFFECT 			4096
#define status_effect(ptr)			(gi->status_effects[(ptr)])
#define STATUS_EFFECT_SPAN_INIT(S, P) case S: status_effect(r).span = P; break; 
#define INVOCATION_FATE_SPAN 		99999999
#define INVOCATION_FATE_RATE		1
typedef short AW_status_effect_ptr;

enum AW_status_effect_type_t {
	/* lose health points */
	AW_status_effect_invocation_fate,
};

struct AW_status_effect_t {
	AW_status_effect_type_t 	type;
	int 											amount;
	AW_time_t 								span;
	AW_status_effect_ptr			next,
														previous,
														fnext;
};

void 									ST_Init					(AW_game_instance_t *gi, int argc, char** argv);
AW_status_effect_ptr 	ST_New					(AW_game_instance_t *gi, AW_unit_ptr u);
void 									ST_Free					(AW_game_instance_t *gi, AW_unit_ptr u, AW_status_effect_ptr l);
void 									ST_FreeAll 			(AW_game_instance_t *gi, AW_unit_ptr u, AW_status_effect_ptr l);
void 									ST_Update 			(AW_game_instance_t *gi, AW_unit_ptr u);

/*
 * build order
 */

#define MAX_BUILD_ORDER     MAX_UNIT
#define build_order(ptr)    (gi->build_orders[(ptr)])
typedef short AW_build_order_ptr;
typedef short AW_unit_type_t;

struct AW_build_order_t {
  AW_unit_type_t        unit_type;
  short 								target_cx,
  											target_cy;
  AW_id_t 							unit_id,
  											unit_player_id;
  AW_time_t             progress;
  bool 									started;
  AW_build_order_ptr    previous,
  											next,
                        fnext;
};

void                  BO_Init     							(AW_game_instance_t *gi, int argc, char** argv);
AW_build_order_ptr    BO_New      							(AW_game_instance_t *gi);
void                  BO_Free										(AW_game_instance_t *gi, AW_build_order_ptr l);
void                  BO_FreeAll  							(AW_game_instance_t *gi, AW_unit_ptr u);
void              		BO_PushFrontBuildOrder   	(AW_game_instance_t *gi, AW_unit_ptr u, AW_build_order_ptr b);
void              		BO_PushBackBuildOrder    	(AW_game_instance_t *gi, AW_unit_ptr u, AW_build_order_ptr b);
short             		BO_GetBuildOrderCount    	(AW_game_instance_t *gi, AW_id_t player_id, AW_client_ptr c, AW_unit_ptr u);

/*
 * unit
 */

#define MAX_CLIENT						MAX_PLAYER
#define MAX_UNIT 							1024
#define MAX_UNIT_TYPE 				128
#define MAX_UNIT_SIZE					12
#define unit(ptr)							(gi->units[(ptr)])
typedef unsigned int AW_ascii_t;
typedef short AW_unit_type_t;

struct AW_unit_class_t {
	int 					gold_price,
								xp;
	AW_time_t 		build_time;
	float 				move_speed,
  							attack_speed,
  							attack_damage;
  int 					max_hp,
  							max_mana,
  							size, 	
  							visibility_range,
  							aggro_range,
  							attack_range;
  bool 					can_be_pushed;
  AW_ascii_t 		*ascii;
};

// todo: optimize for cache
struct AW_unit_t {
	AW_ptr_t 								user_data;
	TCOD_map_t 							visibility_map;
	AW_unit_type_t 					unit_type;
	AW_time_t 							move,
													attack,
													blocked_time,
													blink_acc;
	AW_unit_order_ptr 			order_head;
	AW_id_t 								player_id,
													team_id,
													cmd_id;
	AW_build_order_ptr  		build_order_head[MAX_PLAYER];
	short 									pos_x, 
													pos_y,
													virtual_hp,
													hp,
													mana,
													path_index,
													path_len,
													chase_cx,
													chase_cy,
													push_power;
	AW_status_effect_ptr 		status_effect_head;
	AW_unit_ptr							fnext,			/* free */
													previous,		/* in use */
													next,
													sprevious[MAX_CLIENT],	/* selected */
													snext[MAX_CLIENT],
													pprevious,	/* player */
													pnext,
													gsprevious[MAX_GROUP_SELECTION], /* group selection */
													gsnext[MAX_GROUP_SELECTION];
	/* owned by post render */
	short 									old_pos_x,
													old_pos_y;
};

struct AW_get_unit_here_t {
	AW_unit_ptr result[MAX_UNIT_SIZE*MAX_UNIT_SIZE];
	short 			count;
};

void 									UN_Init										(AW_game_instance_t *gi, int argc, char** argv);
AW_unit_ptr 					UN_New 										(AW_game_instance_t *gi);
void 									UN_Free 									(AW_game_instance_t *gi, AW_unit_ptr u);
void 									UN_FreeAll 								(AW_game_instance_t *gi);
bool 									UN_CanBeHere 							(AW_game_instance_t *gi, AW_unit_ptr not_this_one, short size, short pos_x, short pos_y);
void 									UN_GetUnitHere 						(AW_get_unit_here_t *data, AW_game_instance_t *gi, AW_unit_ptr not_this_one, short size, short pos_x, short pos_y);
void 									UN_PutInMap								(AW_game_instance_t *gi, AW_unit_ptr u);
void 									UN_RemoveFromMap					(AW_game_instance_t *gi, AW_unit_ptr u);
bool 									UN_HasOrder								(AW_game_instance_t *gi, AW_unit_ptr u, AW_id_t cmd_mask);
int 									UN_GetDist 								(AW_game_instance_t *gi, AW_unit_ptr u, int x, int y);
int 									UN_GetDist 								(AW_game_instance_t *gi, AW_unit_ptr u, AW_unit_ptr u2);
int 									UN_GetLight 							(AW_game_instance_t *gi, AW_unit_ptr u, short pos_x, short pos_y);

/*
 * cmd
 */

#define MAX_CMD 			30000
#define cmd(ptr)			(gi->cmds[(ptr)])
typedef short 				AW_cmd_ptr;

enum AW_cmd_type_t {
	AW_cmd_type_none 						= 0,
	AW_cmd_type_unit_order 			= 1,
	AW_cmd_type_spawn_unit 			= 2,
	AW_cmd_type_build_unit 			= 3,
	AW_cmd_type_generic 				= 4,
	AW_cmd_type_end_of_turn   	= 5,
	AW_cmd_type_join_game 			= 6,
	AW_cmd_type_seed_game 			= 7,
	AW_cmd_type_leech_game 			= 8,
	AW_cmd_type_ping 						= 9,
};

#pragma pack(push, 1)
struct AW_unit_order_packet_t {
	AW_cmd_type_t 				type;
  AW_id_t 							id;
  AW_unit_order_type_t 	order;
  AW_id_t								cmd_mask;
  int 									turn;
  short 			 					r_squared,
  											target_cx,
  											target_cy,
  											click_cx,
  											click_cy;
  AW_id_t 							target,
  											target_player_id,
												target_cmd_id;
	AW_time_t 						time_stamp;
	bool 									attack_here,
												push_back;
};
struct AW_spawn_unit_packet_t {
	AW_cmd_type_t 				type;
	AW_id_t 							id,
												cmd_mask;
	AW_unit_type_t 				unit_type;
  int 									turn;
  short 								target_cx,
  											target_cy;
};
struct AW_build_unit_packet_t {
	AW_cmd_type_t 	type;
	AW_id_t 				id, /* the unit which build */
									cmd_mask, /* the unit to build */
									player_id,
									target_player_id;
	int 						turn;
	AW_unit_type_t 	unit_type;
	short 					target_cx,
									target_cy;
	bool 						start_it,
									cancel_it;
};
struct AW_generic_packet_t {
	AW_cmd_type_t 				type;
	AW_id_t 							id,
												player_id,
												cmd_type,
												cmd_mask;
	AW_unit_type_t 				unit_type;
  int 									turn;
  short 								target_cx,
  											target_cy;
};
struct AW_end_of_turn_packet_t {
	AW_cmd_type_t type;
  AW_id_t 			id;
  int 					turn;
  short 				turn_frame_count;
  AW_time_t 		frame_time_step;
  unsigned int 	hash;
};
struct AW_fill_game_list_packet_t {
	AW_cmd_type_t type;
};
struct AW_join_game_packet_t {
	AW_cmd_type_t type;
	int 			 		player_count_per_team;
	int 					team_count;
	char 					game_name[MAX_CHAR];
};
struct AW_seed_game_packet_t {
	AW_cmd_type_t type;
	char 					game_name[MAX_CHAR];
	int 			 		player_id;
};
struct AW_leech_game_packet_t {
	AW_cmd_type_t type;
	char 					game_name[MAX_CHAR];
	int 			 		player_id;
};
struct AW_ping_packet_t {
	AW_cmd_type_t type;
	AW_time_t 		time;
};
#pragma pack(pop)

struct AW_cmd_t {
	AW_time_t 							frame_time_step;
	AW_ptr_t 								user_data;
	int 										turn;
	AW_cmd_type_t   				type;
	AW_unit_type_t 					unit_type;
	AW_id_t 								id,
													cmd_type,
													cmd_mask;
	AW_cmd_ptr 							fnext,
													previous,
													next,
													cmd_next;
	short 									target_cx, 
													target_cy,
													click_cx, 
													click_cy,
													r_squared,
													turn_frame_count;
	AW_id_t 								target,
													target_player_id,
													target_cmd_id,
													player_id;
	AW_time_t 							time_stamp;
	bool 										sent,
													attack_here,
													push_back,
													start_it,
													cancel_it;
	unsigned int 						hash;
	AW_unit_order_cb_t 			unit_order_completed_cb,
													unit_order_failed_cb;
};

void 							CMD_Init				(AW_game_instance_t *gi, int argc, char** argv);
AW_cmd_ptr 				CMD_New 				(AW_game_instance_t *gi, AW_id_t id);
void 							CMD_Free 				(AW_game_instance_t *gi, AW_cmd_ptr r);
void 							CMD_FreeAll 		(AW_game_instance_t *gi);

/*
 * cmd store
 */

#define SHORT_CONNECTION_TIMEOUT 	25000
#define cmd_store(ptr)						(gi->cmd_stores[(ptr)])
typedef short AW_cmd_store_ptr;

struct AW_cmd_store_t {
	AW_cmd_store_ptr 	fnext,
										previous,
										next;
	AW_cmd_ptr 				cmd_head;
};

void 								CS_Init						(AW_game_instance_t *gi, int argc, char** argv);
AW_cmd_store_ptr 		CS_New 						(AW_game_instance_t *gi);
void 								CS_Free 					(AW_game_instance_t *gi, AW_cmd_store_ptr cs);
void 								CS_FreeAll 				(AW_game_instance_t *gi);
void 								CS_ClearCmdStore 	(AW_game_instance_t *gi, AW_cmd_store_ptr cs);
void 								CS_InsertCmd 			(AW_game_instance_t *gi, AW_cmd_store_ptr r, AW_cmd_ptr c);

/*
 * remote cmd store
 */

#define remote_cmd_store(ptr)						(gi->remote_cmd_stores[(ptr)])
typedef short AW_remote_cmd_store_ptr;

struct AW_remote_cmd_store_t {
	AW_cmd_store_ptr 					cs;
	AW_id_t 									player_id;
	AW_remote_cmd_store_ptr 	fnext,
														previous,
														next;
	ENetHost 									*host;
	ENetPeer									*peer;
	ENetAddress 							address;
	int 											ping;
	bool 											connected;
};

void 												RS_Init								(AW_game_instance_t *gi, int argc, char** argv);
AW_remote_cmd_store_ptr 		RS_New 								(AW_game_instance_t *gi, AW_cmd_store_ptr cmd_store);
void 												RS_Free 							(AW_game_instance_t *gi, AW_remote_cmd_store_ptr r);
void 												RS_FreeAll 						(AW_game_instance_t *gi);
void 												RS_Connect 						(AW_game_instance_t *gi, AW_remote_cmd_store_ptr r, const str &game_name);
void 												RS_WaitForConnection	(AW_game_instance_t *gi, AW_remote_cmd_store_ptr r);
void 												RS_Update 						(AW_game_instance_t *gi, AW_remote_cmd_store_ptr r);
void 												RS_Leech							(AW_game_instance_t *gi, AW_remote_cmd_store_ptr r);

/*
 * client
 */

#define client(ptr)									(gi->clients[(ptr)])
#define HUD_STATE_NO_WINDOW 		 		123456
#define HUD_STATE_OPTION_WINDOW 		12345

typedef short AW_client_ptr;

#define HUMAN_CLIENT 			0

struct AW_client_t {
	int 							type;
	short 						viewport_x,
										viewport_y;
	AW_cmd_store_ptr 	cs;
	AW_id_t 					player_id,
										cmd_id;
	AW_client_ptr 		fnext,
										previous,
										next;
	/* human */
	int 							hud_state;
	AW_unit_type_t 		selected_sub_group,
										gs_sub_group[MAX_GROUP_SELECTION];
	AW_unit_ptr 			selected_units_head,
										gs_head[MAX_GROUP_SELECTION];
	bool 							selection_started,
										pointing_minimap;
	int 							mp_lb_x, mp_lb_y,
										mr_lb_x, mr_lb_y;
	bool 							ability_here,
										window_opened;
	short 						ability_id;
	int 							window_start_x,
										window_start_y,
										window_size_x,
										window_size_y,
										window_end_x,
										window_end_y;
	short 						selected_gs;
	ENetHost 					*host;
	ENetPeer					*peer;
	ENetAddress 			address,
										seeding_address;
	bool 							connected;
	AW_ptr_t 					user_data;
};

void 						CL_Init 											(AW_game_instance_t *gi, int argc, char** argv);
AW_client_ptr 	CL_New												(AW_game_instance_t *gi, AW_cmd_store_ptr cs);
void 						CL_Free 											(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_FreeAll 										(AW_game_instance_t *gi);
void 						CL_Connect 										(AW_game_instance_t *gi, AW_client_ptr c, const str &game_name);
void 						CL_WaitForConnection					(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_Update											(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_Seed 											(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_EndOfTurn									(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_UpdateInputs								(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_UpdateHUD									(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderPointer							(AW_game_instance_t *gi, AW_client_ptr c, short x, short y, bool and_unit);
void 						CL_RenderCursor								(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderSelection						(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderSelectedUnitTargets	(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderSmokes								(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderBlood								(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderBurnTrace						(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderFloatingText					(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderEnvMap								(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderUnits								(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderBloom 								(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderBalls								(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderFluid								(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderLightning						(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderWinLose							(AW_game_instance_t *gi, AW_client_ptr c);
void          	CL_RenderWindow             	(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderFps									(AW_game_instance_t *gi);
void 						CL_RenderPing									(AW_game_instance_t *gi);
void 						CL_RenderStats								(AW_game_instance_t *gi);
void 						CL_ClearCmdStore							(AW_game_instance_t *gi, AW_client_ptr c);
bool 						CL_IsUnitSelected 						(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, short gs=-1);
short 					CL_GetSelectedUnitsCount 			(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_SelectUnits								(AW_game_instance_t *gi, AW_client_ptr c, int s_cx, int s_cy, int e_cx, int e_cy, AW_unit_type_t type);
bool 						CL_IsThereUnitsHere						(AW_game_instance_t *gi, AW_client_ptr c, int s_cx, int s_cy, int e_cx, int e_cy);
void 						CL_AddToSelection							(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u);
void 						CL_SubToSelection							(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, short gs=-1);
void 						CL_FreeUnitSelection					(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_AddToGroupSelection				(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, int gs);
void 						CL_CopySelectionTo 						(AW_game_instance_t *gi, AW_client_ptr c, int gs);
void 						CL_RestoreGroupSelection			(AW_game_instance_t *gi, AW_client_ptr c, int gs);
void 						CL_SelectNextSubGroup 				(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_CmdUnitOrderOnSelection		(AW_game_instance_t *gi, AW_client_ptr c, int to_x, int to_y, bool push_back, bool attack_here, bool ground_only=false, AW_id_t cmd_mask=-1, int r_squared=-1, AW_ptr_t user_data=AW_null, AW_unit_order_cb_t unit_order_completed_cb=null, AW_unit_order_cb_t unit_order_failed_cb=null);
void 						CL_CmdUnitOrder 							(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, int to_x, int to_y, bool push_back, bool attack_here, bool ground_only=false, AW_id_t cmd_mask=-1, int r_squared=-1, AW_ptr_t user_data=AW_null, AW_unit_order_cb_t unit_order_completed_cb=null, AW_unit_order_cb_t unit_order_failed_cb=null);
void 						CL_CmdSpawnUnit								(AW_game_instance_t *gi, AW_client_ptr c, AW_id_t from_u, int to_x, int to_y, AW_unit_type_t unit_type, AW_ptr_t user_data=AW_null);
void 						CL_CmdBuildOrder 							(AW_game_instance_t *gi, AW_client_ptr c, AW_id_t builder_id, AW_id_t builder_player_id, AW_id_t unit_player_id, AW_unit_type_t unit_type, short target_cx, short target_cy, bool start_it);
void 						CL_CmdCancelBuildOrder 				(AW_game_instance_t *gi, AW_client_ptr c, AW_id_t builder_id, AW_id_t builder_player_id, AW_id_t unit_player_id, AW_id_t unit_id);
void 						CL_CmdGeneric 								(AW_game_instance_t *gi, AW_client_ptr c, AW_id_t cmd_type, AW_id_t id, AW_id_t player_id, AW_id_t cmd_mask, AW_unit_type_t unit_type, short target_cx, short target_cy);
bool 						CL_HasOrder 									(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u);
void 						CL_FocusOnUnits								(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_UpdateWindowCloseButton		(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_OpenOptionWindow						(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_UpdateOptionWindow					(AW_game_instance_t *gi, AW_client_ptr c);
void 						CL_RenderRect									(void *sdl_surface, int thickness, int color, int x, int y, int w, int h);
void 						CL_PostRender 								(void *sdl_surface);

/*
 * player
 */

#define player(ptr)													(gi->players[(ptr)])
#define MAX_WORKER_TASK	 										(MAX_UNIT/MAX_PLAYER)
#define MAX_WORKER 													4
#define TASK_COST_PER_FRAME									(100*15)
#define HEAVY_PATH_COST 										20 		
#define path_point_worker_ptr(index)				(ta->path + (index<<1))			
#define worker(W) 													(gi->workers[p][(W)])		
#define ON_HEAVY_PATH_MAP(X, Y) 						(wo->heavy_path_map[MAP_SIZE_X*(Y)+(X)])
typedef short AW_player_ptr;
typedef short AW_worker_ptr;

struct AW_worker_task_t {
	AW_unit_ptr 				u;
	AW_id_t 						id,
											*task_id_proxy;
	short 							pos_x,
											pos_y,
											target_cx,
											target_cy,
											size,
											chase_cx,
											chase_cy,
											path_len,
											*chase_cx_proxy,
											*chase_cy_proxy,
											*path_len_proxy;
	bool 								*started_proxy;
	short 							path[MAX_PATH_LEN * 2];
};

struct AW_path_callback_t {
	AW_game_instance_t *gi;
	AW_player_ptr 			p;
	AW_worker_ptr 			w;
	short 							size;
	int 								start_mark,
											mark;
	bool 								follow_mode,
											can_follow,
											do_first_pass;
};

struct AW_worker_t {
	AW_game_instance_t *gi;
	AW_player_ptr 			p;
	AW_worker_ptr 			w;
	HANDLE 							handle;
	DWORD								id;
	int 								task_count,
											task_cost,
											frame_count,
											frame_counter;
	bool 								free;
	AW_path_callback_t 	callback_data;
	bool 								heavy_path_map[MAP_SIZE];
};

struct AW_player_t {
	map<AW_id_t, AW_unit_ptr> 	cmd_to_mem_unit_map;
	AW_id_t 										id,
															team_id;
	AW_cmd_store_ptr 						cs;
	TCOD_map_t 									tcod_map;
	AW_unit_ptr 								unit_head;
	AW_player_ptr								fnext,
															previous,
															next;
	int 												worker_count;
	AW_worker_ptr 							free_worker;
	bool 												win,
															lose;
	AW_ptr_t 										user_data;
};

void 								PL_Init 											(AW_game_instance_t *gi);
AW_player_ptr 			PL_New 												(AW_game_instance_t *gi, AW_ptr_t user_data=AW_null);
void 								PL_Free 											(AW_game_instance_t *gi, AW_player_ptr p);
void 								PL_FreeAll 										(AW_game_instance_t *gi);
bool 								PL_NextTurn 									(AW_game_instance_t *gi, AW_player_ptr p);
int 								PL_IsInFov 										(AW_game_instance_t *gi, AW_player_ptr p, short pos_x, short pos_y);
bool 								PL_IsInFov 										(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u);
int 								PL_GetLight 									(AW_game_instance_t *gi, AW_player_ptr p, short pos_x, short pos_y);
void 								PL_UpdateUnits								(AW_game_instance_t *gi, AW_player_ptr p);
bool 								PL_UpdateFollowTarget 				(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u);
bool 								PL_StartMoveOrder							(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u);
bool 								PL_MoveAndAttack							(AW_game_instance_t *gi, AW_player_ptr p);
void 								PL_ProcessCmdStore						(AW_game_instance_t *gi, AW_player_ptr p);
AW_unit_ptr 				PL_SpawnUnit									(AW_game_instance_t *gi, AW_player_ptr p, AW_id_t cmd_id, short x, short y, AW_unit_type_t unit_type, AW_ptr_t user_data=AW_null);
void 								PL_FindSpawnPoint							(AW_game_instance_t *gi, AW_player_ptr p, short size, short *to_x, short *to_y);
void 								PL_KillUnit										(AW_game_instance_t *gi, AW_player_ptr killer_p, AW_player_ptr unit_p, AW_unit_ptr u);
void 								PL_KillAll										(AW_game_instance_t *gi, AW_player_ptr p);
bool 								PL_IsUnitAlive 								(AW_game_instance_t *gi, AW_player_ptr p, AW_id_t cmd_id);
float 							PL_PathCallback								(int xFrom, int yFrom, int xTo, int yTo, void *user_data);
float 							PL_PathCallback2							(int xFrom, int yFrom, int xTo, int yTo, void *user_data);
bool 								PL_MoveUnit										(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u);
bool 								PL_UnitAttack									(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u);
bool 								PL_Walkable 									(AW_game_instance_t *gi, AW_player_ptr p, AW_worker_ptr w, short size, short pos_x, short pos_y);
bool 								PL_LightWalkable 							(AW_game_instance_t *gi, AW_player_ptr p, AW_worker_ptr w, short size, short pos_x, short pos_y);
void 								PL_PathfinderScanMap					(AW_game_instance_t *gi, short size, short pos_x, short pos_y, int *close_mark);
void 								PL_FindNearestWalkablePoint		(AW_game_instance_t *gi, AW_player_ptr p, AW_worker_ptr w, short size, short pos_x, short pos_y, short *to_x, short *to_y);
bool 								PL_GenFollowEnemy							(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, AW_id_t cmd_mask, AW_time_t time_stamp, bool following_friend);
bool 								PL_GenAttackEnemy							(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, AW_id_t cmd_mask, AW_time_t time_stamp, bool precise_target, bool following_friend);
bool 								PL_CheckFollowFriend 					(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, short level);
AW_unit_ptr 				PL_GetFirstEnemyInRange				(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, int range);
AW_unit_ptr 				PL_GetFirstEnemyInRange				(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, int range, short pos_x, short pos_y);
bool 								PL_PushUnit										(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, AW_time_t time_stamp, short x, short y, short from_x, short from_y);
bool 								PL_PushUnitRecursive					(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, AW_unit_ptr target, AW_time_t time_stamp, short size, short x, short y);
void 								PL_InitFreeWorker 						(AW_game_instance_t *gi, AW_player_ptr p);
void 								PL_StartWorker								(AW_game_instance_t *gi, AW_player_ptr p);
void 								PL_CloseWorker 								(AW_game_instance_t *gi, AW_player_ptr p, AW_worker_ptr w);
DWORD WINAPI  			PL_WorkerFun 									(void *params);

/*
 * game instance
 */

#define 		MASTER_SERVER_PORT 					30000
#define 		MASTER_SERVER_PING_INTERVAL 125
#define 		LISTEN_MASTER_SERVER \
 	ENetEvent e; \
 	GI_ListenMasterServer(gi, &e);

struct game_desc_t {
	str 							master_server;
	str  							game_name;
	short 						player_count_per_team,
										team_count;
	AW_id_t 					local_player_id,
										local_team_id;					 
};

typedef void 	(*AW_free_all_cb_t)(AW_game_instance_t *gi);
typedef void 	(*AW_init_game_cb_t)(AW_game_instance_t *gi, game_desc_t *gd);
typedef void 	(*AW_start_game_cb_t)(AW_game_instance_t *gi);
typedef void 	(*AW_update_client_cb_t)(AW_game_instance_t *gi, AW_client_ptr c);
typedef short (*AW_get_abilities_count_cb_t)(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_type_t unit_type);
typedef str 	(*AW_get_ability_name_cb_t)(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, short i);
typedef char 	(*AW_get_ability_shortcut_cb_t)(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, short i);
typedef bool 	(*AW_trigger_ability_cb_t)(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, short i, short cx, short cy);
typedef void  (*AW_hud_info_cb_t)(AW_game_instance_t *gi, AW_client_ptr c);
typedef void 	(*AW_on_death_cb_t)(AW_game_instance_t *, AW_player_ptr, AW_player_ptr, AW_unit_ptr);
typedef void 	(*AW_on_spawn_unit_cb_t)(AW_game_instance_t *gi, AW_unit_ptr from_u, AW_unit_ptr u);
typedef void 	(*AW_on_cancel_build_cb_t)(AW_game_instance_t *gi, AW_build_order_ptr b);
typedef void 	(*AW_on_generic_cmd_cb_t)(AW_game_instance_t *gi, AW_id_t cmd_type, AW_id_t id, AW_id_t player_id, AW_id_t cmd_mask, AW_unit_type_t unit_type, short target_cx, short target_cy);
typedef void  (*AW_on_unit_order_cb_t)(AW_game_instance_t *gi, AW_unit_ptr u, AW_unit_ptr target, AW_id_t id, AW_id_t player_id, AW_id_t cmd_mask, AW_unit_type_t unit_type, short target_cx, short target_cy);
typedef void  (*AW_draw_unit_target_cb_t)(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u);
typedef void  (*AW_draw_pointer_cb_t)(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, AW_unit_type_t type, int ability_id, short cx, short cy);
typedef void  (*AW_draw_cursor_cb_t)(SDL_Surface *surface, AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, AW_unit_type_t type, int ability_id, int offx, int offy);

enum AW_peer_message_type_t {
	AW_peer_message_cmd_pack = 1
};

enum AW_state_t {
	AW_state_main_menu 						= 1<<1,
	AW_state_game 		 						= 1<<2,
	AW_state_join 		 						= 1<<3,
	AW_state_waiting_players 		 	= 1<<4,
	AW_state_ready 		 						= 1<<5,
};

// todo: split this for cache locallity 
struct AW_game_instance_t {
	ENetAddress											master_server_addr;
	str 														name;
	short 													env_map[RANGE_MAP_SIZE],
																	env_art_map[MAP_SIZE];
	TCOD_color_t 										static_light_map[MAP_SIZE],
																	dyn_light_map[MAP_SIZE],
																	env_color[MAP_SIZE],
																	final_color[MAP_SIZE],
																	*glow_map,
																	*glow_map2;
	bool 														shadow_map[MAP_SIZE];
	int 														*post_render_mask;
	float 													smoke_map[MAP_SIZE];
	float 													normal_map[MAP_SIZE][3];
	float 													dir_light[3];
	AW_unit_ptr 										unit_map[MAP_SIZE];
	AW_ball_ptr 										ball_map[MAP_SIZE];
	int 														fov_maps[MAX_TEAM][RANGE_MAP_SIZE];
	bool 														heavy_path_map[MAP_SIZE];
	TCOD_path_t  										path_pool[MAX_PLAYER][MAX_WORKER],
																	path_pool2[MAX_PLAYER][MAX_WORKER];
	short 													paths[MAX_UNIT * MAX_PATH_LEN * 2];
	AW_time_t 											game_time,
																	frame_time,
																	last_t,
																	game_time_step,
																	frame_time_step,
																	game_time_acc,
																	wait_time,
																	aver_frame_times[MAX_AVER_FRAME_TIMES],
																	aver_wait_times[MAX_AVER_WAIT_TIMES],
																	aver_frame_time,
																	aver_wait_time,
																	ping_acc,
																	turn_frame_count_dec_acc;
	int 														aver_frame_times_counter,
																	aver_wait_times_counter,
																	max_ping_times_counter;
	AW_unit_order_t 								orders[MAX_UNIT_ORDER];
	AW_unit_order_ptr 							free_unit_order_head;
	AW_ball_t 											balls[MAX_BALL];
	AW_ball_ptr 										free_ball_head;
	AW_ball_ptr 										ball_head;
	AW_smoke_t 											smokes[MAX_SMOKE];
	AW_smoke_ptr 										free_smoke_head;
	AW_smoke_ptr 										smoke_head;
	AW_blood_t 											bloods[MAX_BLOOD];
	AW_blood_ptr 										free_blood_head;
	AW_blood_ptr 										blood_head;
	AW_burn_trace_t 								burn_traces[MAX_BURN_TRACE];
	AW_burn_trace_ptr 							free_burn_trace_head;
	AW_burn_trace_ptr 							burn_trace_head;
	AW_floating_text_t 							floating_texts[MAX_FLOATING_TEXT];
	AW_floating_text_ptr 						free_floating_text_head;
	AW_floating_text_ptr 						floating_text_head;
	AW_light_t 											lights[MAX_LIGHT];
	AW_light_ptr 										free_light_head;
	AW_light_ptr 										light_head;
	TCOD_noise_t 										torch_noise,
																	bump_noise;
	TCOD_map_t 											tcod_light_map;
	AW_sound_t 											sounds[MAX_SOUND];
	AW_sound_ptr 										free_sound_head;
	AW_sound_ptr 										sound_head;
	AW_fluid_t 											fluids[MAX_FLUID];
	AW_fluid_ptr 										free_fluid_head;
	AW_fluid_ptr 										fluid_head;
	AW_build_explosion_t 						build_explosions[MAX_BUILD_EXPLOSION];
	AW_build_explosion_ptr 					free_build_explosion_head;
	AW_build_explosion_ptr 					build_explosion_head;
	AW_lightning_t 									lightnings[MAX_LIGHTNING];
	AW_lightning_ptr 								free_lightning_head;
	AW_lightning_ptr 								lightning_head;
	AW_status_effect_t 							status_effects[MAX_STATUS_EFFECT];
	AW_status_effect_ptr 						free_status_effect_head;
	AW_unit_t 											units[MAX_UNIT];
	AW_unit_ptr 										free_unit_head;
	AW_unit_ptr 										unit_head;
	AW_cmd_store_t 									cmd_stores[MAX_PLAYER];
	AW_cmd_store_ptr 								free_cmd_store_head;
	AW_cmd_store_ptr 								cmd_store_head;
	AW_remote_cmd_store_t 					remote_cmd_stores[MAX_PLAYER];
	AW_remote_cmd_store_ptr 				free_remote_cmd_store_head;
	AW_remote_cmd_store_ptr 				remote_cmd_store_head;
	AW_player_t 										players[MAX_PLAYER];
	AW_worker_t 										workers[MAX_PLAYER][MAX_WORKER];
	AW_worker_task_t 								worker_tasks[MAX_PLAYER][MAX_WORKER][MAX_WORKER_TASK];
	AW_player_ptr 									free_player_head;
	AW_player_ptr 									player_head;
	AW_cmd_t 												cmds[MAX_CMD];
	AW_cmd_ptr 											free_cmd_head;
	AW_cmd_ptr 											cmd_head;
	AW_client_t 										clients[MAX_CLIENT];
	AW_client_ptr 									free_client_head;
	AW_client_ptr 									client_head;
	AW_build_order_t      					build_orders[MAX_BUILD_ORDER];
  AW_build_order_ptr    					free_build_order_head;
	int 														turn, 
																	ping,
																	max_ping,
																	pings[MAX_MAX_PING_TIMES];
	int 														frame_mark,
							 										close_map[MAX_UNIT_SIZE][MAP_SIZE],
							 										close_map2[MAP_SIZE],
							 										close_map3[MAX_PLAYER][MAX_WORKER][MAX_UNIT_SIZE][MAP_SIZE],
							 										close_map4[MAX_PLAYER][MAX_WORKER][RANGE_MAP_SIZE],
							 										task_mark,
							 										close_map_mark,
							 										close_map_mark3[MAX_PLAYER][MAX_WORKER],
							 										close_map_mark4[MAX_PLAYER][MAX_WORKER],
			 														square_x[8],
			 														square_y[8];		 												
	short 													turn_frame_count,
																	turn_acc;
	TCOD_key_t     									keys[SMALL_SIZE];
	int 														key_counter;
	TCOD_mouse_t   									mouse;
	bool 														lbtn_down,
																	lbtn_up,
																	rbtn_down,
																	rbtn_up;
	unsigned int 										state,
																	add_state[SMALL_SIZE],
																	add_state_count,
																	remove_state[SMALL_SIZE],
																	remove_state_count;
	ENetHost 												*host;
	ENetPeer												*peer;
	ENetAddress 										address;
	bool 														connected,
																	minimap_cleared,
																	desync;
	TCOD_noise_t 										fog_noise; 
	TCOD_image_t 										fog_mask;
	float 													fog_scroll_x,
																	fog_scroll_y;	
		
	#if REPLAY		
	int 														max_turn;
	#endif		
	AW_time_t 											connection_acc;
	#if !RELEASE		
	bool 														show_all,
																	show_lagometer;
	#endif		
  bool 														render_bloom,
  																render_spec;
  /* callbacks */
  AW_free_all_cb_t 								free_all_cb; 				
  AW_update_client_cb_t 					update_client_cb;
  AW_on_death_cb_t 								on_death_cb;			
  AW_get_abilities_count_cb_t 		get_abilities_count_cb;
  AW_get_ability_name_cb_t 				get_ability_name_cb;
  AW_get_ability_shortcut_cb_t 		get_ability_shortcut_cb;
  AW_trigger_ability_cb_t 				trigger_ability_cb;
  AW_hud_info_cb_t hud_info_cb;
  AW_on_spawn_unit_cb_t 					on_spawn_unit_cb;
  AW_on_cancel_build_cb_t 				on_cancel_build_cb;
  AW_on_generic_cmd_cb_t 					on_generic_cmd_cb;
  AW_on_unit_order_cb_t 					on_unit_order_cb;
  AW_draw_unit_target_cb_t 				draw_unit_target_cb;
  AW_draw_pointer_cb_t 						draw_pointer_cb;
  AW_draw_cursor_cb_t 						draw_cursor_cb;
	/* ui */
	AW_btn_t 												refresh_btn,
																	ready_btn,
																	waiting_for_players_btn,
																	flash_btn,
																	close_btn;
	AW_btn_list_t 									multi_btns,
																	join_game_btns,
																	abilities_btns,
																	central_hud_btns;
	/* audio */		
	Mix_Music 											*main_music;
	Mix_Chunk 											*coin_sound,
																	*torch_sound,
																	*foot_sound;
};

void 							GI_Init														(AW_game_instance_t *gi, int argc, char** argv);
void 							GI_Free														(AW_game_instance_t *gi);
void 							GI_Close 													(AW_game_instance_t *gi);
void 							GI_InitEnvMap											(AW_game_instance_t *gi);
void 							GI_InitGame												(AW_game_instance_t *gi, game_desc_t *gd, AW_init_game_cb_t init_game_cb);
void 							GI_StartGame											(AW_game_instance_t *gi, AW_start_game_cb_t start_game_cb);
void 							GI_StartHumanClient								(AW_game_instance_t *gi, AW_client_ptr c);
void 							GI_UpdateTime											(AW_game_instance_t *gi);
void 							GI_UpdateTurn 										(AW_game_instance_t *gi);
void 							GI_Update													(AW_game_instance_t *gi);
void 							GI_UpdatePlayers 									(AW_game_instance_t *gi);
void 							GI_UpdateMiscs 										(AW_game_instance_t *gi);
void 							GI_UpdateMaps											(AW_game_instance_t *gi);
void 							GI_UpdateClients 									(AW_game_instance_t *gi);
void 							GI_UpdateRemoteStores 						(AW_game_instance_t *gi);
void 							GI_EndOfTurn 											(AW_game_instance_t *gi);
void 							GI_UpdateAverFrameTime 						(AW_game_instance_t *gi, AW_time_t frame_time);
void 							GI_UpdateAverWaitTime 						(AW_game_instance_t *gi, AW_time_t wait_time);
void 							GI_UpdateMaxPing 									(AW_game_instance_t *gi);
void 							GI_UpdateInputs 									(AW_game_instance_t *gi);
bool 							GI_IsKeyReleased 									(AW_game_instance_t *gi, TCOD_keycode_t k, char c = -1, int ctrl=-1);
bool 							GI_IsKeyPressed 									(AW_game_instance_t *gi, TCOD_keycode_t k, char c = -1, int ctrl=-1);
bool 							GI_IsKeyPressed2 									(AW_game_instance_t *gi, TCOD_keycode_t k);
void 							GI_StopKeyPropagation							(AW_game_instance_t *gi, TCOD_keycode_t k, char c = -1);
void 							GI_StopClicksPropagation					(AW_game_instance_t *gi);
bool 							GI_InState 												(AW_game_instance_t *gi, AW_state_t st);
void 							GI_AddState												(AW_game_instance_t *gi, AW_state_t st);
void 							GI_RemoveState										(AW_game_instance_t *gi, AW_state_t st);
void 							GI_UpdateState 										(AW_game_instance_t *gi);
void 							GI_ClearConsole										(AW_game_instance_t *gi);
void 							GI_ConnectToMasterServer					(AW_game_instance_t *gi);
void 							GI_DisconnectFromMasterServer			(AW_game_instance_t *gi);
void 							GI_KillHost												(AW_game_instance_t *gi);
bool 							GI_ListenMasterServer 						(AW_game_instance_t *gi, ENetEvent *e);
bool 							GI_CheckConnectionToMasterServer 	(AW_game_instance_t *gi, ENetEvent *e, AW_state_t current_state);
bool 							GI_JoinGameOnMasterServer					(AW_game_instance_t *gi);
bool 							GI_GameReady											(AW_game_instance_t *gi);
bool 							GI_PlayersConnected								(AW_game_instance_t *gi);
void 							GI_Ping 													(AW_game_instance_t *gi, ENetEvent *e);
void 							GI_SuccessFlash 									(AW_game_instance_t *gi, const str &msg);
void 							GI_FailureFlash 									(AW_game_instance_t *gi, const str &msg);
AW_player_ptr 		GI_GetPlayerPtr 									(AW_game_instance_t *gi, AW_id_t player_id);
AW_client_ptr 		GI_GetHumanClient 								(AW_game_instance_t *gi);
void 							GI_CheckListIntegrity 						(AW_game_instance_t *gi);

extern int  												argc;
extern char 												**argv;
extern bool 												run;
extern int  												resx, resy;
extern str  												bin_dir;

extern AW_game_instance_t 					client_gi;

extern LARGE_INTEGER 								pfrequency;
extern float 												ref_time;

extern TCOD_console_t  							con;
extern unsigned char 								zlib_in[ZLIB_CHUNK],
                    								zlib_out[ZLIB_CHUNK];

extern AW_game_instance_t 					*renderer_gi;

extern int 													CON_RES_X,
																		CON_RES_Y,
																		MINIMAP_START_X,
																		LAGOMETER_START_X;
#if THREADS
extern CRITICAL_SECTION      				log_crit;
#endif					
extern short 												halfes[MAX_UNIT_SIZE];

/*
 * MOBA
 */

#include "moba.h"

/*
 *  parsers
 */

#include "parsers.h"

/*
 * aw 
 */

void 				AW_Quit();
void 				AW_SetBinDir();
AW_time_t 	AW_GetTime(); 
int 				AW_Rand(unsigned int *seedp);
void 				AW_ZErr(int ret);

#define get16bits(d) (*((unsigned short *) (d)))
unsigned int Aw_Hash(unsigned char * data, int len);

void  			AW_PrintString(const str &s, int x, int y);
void  			AW_PrintString(int i, int x, int y);
void  			AW_PrintString(float f, int x, int y);
void 				AW_PrintDebug(const str &s);
void 				AW_PrintDebug(int i);
void 				AW_PrintDebug(float f);
			
str   			f2a(double f);
str   			i2a(int i);
str   			get_date();
void  			log(const char *msg);
void  			exit();
void  			trace(const str &msg);
void 				trace(int i);
void 				trace(float f);

#ifdef DEBUG
	#define debug_trace(s)  trace(s)
#else
	#define debug_trace(s)
#endif

#endif	/* ASCIIWAR_H */

