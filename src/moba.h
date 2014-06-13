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

#define MOBS_SPAWN_TIME             45000
#define PLAYER_COUNT_PER_TEAM       (1 + /* mobs */ 1)

#define START_HUD_INFO_X            25
#define START_HUD_INFO_X2           (START_HUD_INFO_X+17)
#define START_HUD_INFO_Y            (HUD_START_Y+2)
#define START_HUD_LEFT_INFO_X       2
#define HUD_INFO_SIZE               30

/* this determines the selection order */
enum AW_moba_unit_type_t {
  /* rat */
  AW_unit_type_r,
  /* skeleton */
  AW_unit_type_k,
  /* necromancer */
  AW_unit_type_N,
  /* mobs */
  AW_unit_type_m,
  /* tower */
  AW_unit_type_T,
  /* library */
  AW_unit_type_L,
  /* graveyard */
  AW_unit_type_G,
  /* castle */
  AW_unit_type_C,
  /* unit type count */
  AW_unit_type_count
};

enum AW_moba_cmd_generic_t {
  AW_moba_generic_learn_skill         = 1,
};

/*
 * players
 *  - this is unit user data
 */

#define MAX_MOBA_PLAYER         MAX_PLAYER
#define moba_player(ptr)        (moba_gi.moba_players[(ptr)])
typedef short AW_moba_player_ptr;

#define PLAYER_STARTING_GOLD        10000
#define PLAYER_STARTING_XP          500

#define LAWFUL_GOOD_LEVEL1_XP       500
#define CHAOTIC_GOOD_LEVEL1_XP      500
#define LAWFUL_NEUTRAL_LEVEL1_XP    500
#define CHAOTIC_NEUTRAL_LEVEL1_XP   500
#define LAWFUL_EVIL_LEVEL1_XP       500
#define CHAOTIC_EVIL_LEVEL1_XP      500

enum AW_moba_skills_t {
  AW_moba_skill_lawful_good_level1 = 1,
  AW_moba_skill_chaotic_good_level1,
  AW_moba_skill_lawful_neutral_level1,
  AW_moba_skill_chaotic_neutral_level1,
  AW_moba_skill_lawful_evil_level1,
  AW_moba_skill_chaotic_evil_level1,
};

#define DISPLAY_LIBRARY_SKILL(VAR, S, XP, SHORTCUT, INFO) \
  { \
    if(!mpl->has_##VAR) { \
      moba_gi.central_hud_btns.btns[i].color             = TCOD_white; \
      moba_gi.central_hud_btns.btns[i].s                 = str(S) + " [" + i2a(XP) + "XP]"; \
      moba_gi.central_hud_btns.btns[i].highlight         = true; \
      moba_gi.central_hud_btns.btns[i].entry             = false; \
      moba_gi.central_hud_btns.btns[i].shortcut          = SHORTCUT; \
      moba_gi.central_hud_btns.btns[i].shortcut_special  = (TCOD_keycode_t)-1; \
      moba_gi.central_hud_btns.btns[i].pos_x             = moba_gi.central_hud_btns.pos_x; \
      moba_gi.central_hud_btns.btns[i].pos_y             = moba_gi.central_hud_btns.pos_y + i; \
      BTN_CenterX(gi, &moba_gi.central_hud_btns.btns[i], moba_gi.central_hud_btns.btns[i].pos_x); \
      if(BTN_IsMouseOver(gi, &moba_gi.central_hud_btns.btns[i])) \
        TCOD_console_print_rect(con, START_HUD_INFO_X, START_HUD_INFO_Y, HUD_INFO_SIZE, HUD_HEIGHT-2, INFO); \
      if(BTN_IsClicked(gi, &moba_gi.central_hud_btns.btns[i]) \
      && mpl->xp >= XP) \
        CL_CmdGeneric(gi, c, AW_moba_generic_learn_skill, 0, cl->player_id, AW_moba_skill_##VAR, (AW_unit_type_t)AW_null, -1, -1); \
      moba_gi.central_hud_btns.btn_count++; \
      i++; \
    } \
  }

#define DISPLAY_CASTLE_UNIT(VAR, S, UNIT, SHORTCUT) \
  { \
    if(mpl->has_##VAR) { \
      moba_gi.central_hud_btns.btns[i].color             = TCOD_white; \
      moba_gi.central_hud_btns.btns[i].s                 = str(S) + " [" + i2a(unit_dic[UNIT].gold_price) + "*]"; \
      moba_gi.central_hud_btns.btns[i].highlight         = true; \
      moba_gi.central_hud_btns.btns[i].entry             = false; \
      moba_gi.central_hud_btns.btns[i].shortcut          = SHORTCUT; \
      moba_gi.central_hud_btns.btns[i].shortcut_special  = (TCOD_keycode_t)-1; \
      moba_gi.central_hud_btns.btns[i].pos_x             = moba_gi.central_hud_btns.pos_x; \
      moba_gi.central_hud_btns.btns[i].pos_y             = moba_gi.central_hud_btns.pos_y + i; \
      BTN_CenterX(gi, &moba_gi.central_hud_btns.btns[i], moba_gi.central_hud_btns.btns[i].pos_x); \
      if(BTN_IsClicked(gi, &moba_gi.central_hud_btns.btns[i]) \
      && mpl->gold >= unit_dic[UNIT].gold_price \
      && BO_GetBuildOrderCount(gi, cl->player_id, c, u) < MAX_HUD_CENTRAL_INFO) { \
        mpl->gold -= unit_dic[UNIT].gold_price; \
        START_SOUND(coin_sound, 0) \
        CL_CmdBuildOrder(gi, c, un->cmd_id, un->player_id, cl->player_id, UNIT, un->pos_x + (game_desc.local_team_id == 0 ? 4 : -4), un->pos_y, false); \
      } \
      moba_gi.central_hud_btns.btn_count++; \
      i++; \
    } \
  }

#define GENERIC_LIBRARY_SKILL(VAR, XP) \
  case AW_moba_skill_##VAR: \
    mpl->has_##VAR = true; \
    assert(mpl->xp >= XP); \
    mpl->xp -= XP; \
    break;

#define ABILITY_HERE \
    if(cx == -1) { \
    cl->ability_here = true; \
    cl->ability_id = i; \
  } else

#define TRIGGER_ATTACK \
  if(cx == -1) { \
    cl->ability_here = true; \
    cl->ability_id = i; \
  } else \
    CL_CmdUnitOrderOnSelection(gi, c, cx, cy, GI_IsKeyPressed2(gi, TCODK_SHIFT), true); \
  return false;

#define DRAW_ATTACK_POINTER \
  if(u != AW_null) { \
    AW_unit_t *un = &unit(u); \
    AW_id_t team_id = TEAM_ID(un->player_id); \
    SOCLE(SIZE(un)) { \
      short cx = un->pos_x+i-cl->viewport_x, \
            cy = un->pos_y+j-cl->viewport_y; \
      if(INSIDE_CON(cx, cy)) \
        TCOD_console_set_char_background( \
          con, \
          cx, \
          cy, \
          TCOD_dark_red,  \
          TCOD_BKGND_SET); \
    } \
  } else \
    TCOD_console_set_char_background(con, cx, cy, TCOD_red, TCOD_BKGND_SET);

struct AW_moba_player_t {
  int                 gold,
                      xp;
  AW_moba_player_ptr  next,
                      previous,
                      fnext;
  bool                has_lawful_good_level1,
                      has_chaotic_good_level1,
                      has_lawful_neutral_level1,
                      has_chaotic_neutral_level1,
                      has_lawful_evil_level1,
                      has_chaotic_evil_level1;
};

void                  MP_Init     (int argc, char** argv);
AW_moba_player_ptr    MP_New      ();
void                  MP_Free     (AW_moba_player_ptr l);
void                  MP_FreeAll  (AW_moba_player_ptr l);

/*
 * mobs
 *  - this is unit user data
 */

#define MAX_MOBS         MAX_UNIT
#define mobs(ptr)        (moba_gi.mobs[(ptr)])
typedef short AW_mobs_ptr;

struct AW_mobs_t {
  short         lane,
                mission;
  AW_mobs_ptr   next,
                previous,
                fnext;
};

void          MOBS_Init     (int argc, char** argv);
AW_mobs_ptr   MOBS_New      ();
void          MOBS_Free     (AW_mobs_ptr l);
void          MOBS_FreeAll  (AW_mobs_ptr l);

/*
 * castles
 *  - this is unit user data
 */

#define MAX_CASTLE                 32
#define castle(ptr)                (moba_gi.castles[(ptr)])
typedef short AW_castle_ptr;

struct AW_castle_t {
  AW_unit_ptr       u;
  AW_unit_ptr       rally_target;
  short             rally_cx,
                    rally_cy;
  AW_id_t           player_id;
  AW_castle_ptr     next,
                    previous,
                    fnext;
};

void              CAS_Init                  (int argc, char** argv);
AW_castle_ptr     CAS_New                   ();
void              CAS_Free                  (AW_castle_ptr l);
void              CAS_FreeAll               (AW_castle_ptr l);

/*
 * graveyards
 *  - this is unit user data
 */

#define MAX_GRAVEYARD                 32
#define graveyard(ptr)                (moba_gi.graveyards[(ptr)])
typedef short AW_graveyard_ptr;

struct AW_graveyard_t {
  AW_unit_ptr         u;
  AW_id_t             player_id;
  AW_graveyard_ptr    next,
                      previous,
                      fnext;
};

void              GRA_Init                  (int argc, char** argv);
AW_graveyard_ptr  GRA_New                   ();
void              GRA_Free                  (AW_graveyard_ptr l);
void              GRA_FreeAll               (AW_graveyard_ptr l);

/*
 * librarys
 *  - this is unit user data
 */

#define MAX_LIBRARY                 32
#define library(ptr)              (moba_gi.librarys[(ptr)])
typedef short AW_library_ptr;

struct AW_library_t {
  AW_unit_ptr         u;
  AW_id_t             player_id;
  AW_library_ptr      next,
                      previous,
                      fnext;
};

void              LIB_Init                  (int argc, char** argv);
AW_library_ptr    LIB_New                   ();
void              LIB_Free                  (AW_library_ptr l);
void              LIB_FreeAll               (AW_library_ptr l);

/*
 * client user_data
 */

struct AW_mobs_ai_client_t {
  AW_time_t mobs_spawn_timer;
};

/*
 * game instance
 */
#define RIGHT_LANE              0
#define MIDDLE_LANE             1
#define LEFT_LANE               2
/* 0 is human */
#define MOBS_AI_CLIENT          1
#define HUD_CENTRAL_INFO_COUNT  (HUD_HEIGHT-3)
#define MAX_HUD_CENTRAL_INFO    (HUD_CENTRAL_INFO_COUNT*2)
#define DEMI_XP_RADIUS          15

struct AW_moba_game_instance_t {
  AW_mobs_ai_client_t   mobs_ai_clients[MAX_CLIENT];
  AW_mobs_t             mobs[MAX_MOBS];
  AW_mobs_ptr           free_mobs_head;
  AW_mobs_ptr           mobs_head;
  AW_moba_player_t      moba_players[MAX_MOBA_PLAYER];
  AW_moba_player_ptr    free_moba_player_head;
  AW_moba_player_ptr    moba_player_head;
  AW_castle_t           castles[MAX_CASTLE];
  AW_castle_ptr         free_castle_head;
  AW_castle_ptr         castle_head;
  AW_graveyard_t        graveyards[MAX_GRAVEYARD];
  AW_graveyard_ptr      free_graveyard_head;
  AW_graveyard_ptr      graveyard_head;
  AW_library_t          librarys[MAX_LIBRARY];
  AW_library_ptr        free_library_head;
  AW_library_ptr        library_head;
  AW_btn_list_t         central_hud_btns;
  AW_btn_t              window_switch;
};

void          MOBA_FreeAll                  (AW_game_instance_t *gi);
AW_client_ptr MOBA_GetMobsAIClient          (AW_game_instance_t *gi);
void          MOBA_InitGame                 (AW_game_instance_t *gi, game_desc_t *gd);
void          MOBA_StartGame                (AW_game_instance_t *gi);
void          MOBA_StartMobsAIClient        (AW_game_instance_t *gi, AW_client_ptr c);
void          MOBA_UpdateClient             (AW_game_instance_t *gi, AW_client_ptr c);
void          MOBA_UpdateMobsAI             (AW_game_instance_t *gi, AW_client_ptr c);
void          MOBA_MobsOrderCompletedCB     (AW_game_instance_t *gi, AW_unit_ptr u, AW_unit_order_ptr o);
void          MOBA_MobsOrderFailedCB        (AW_game_instance_t *gi, AW_unit_ptr u, AW_unit_order_ptr o);
void          MOBA_DeathCB                  (AW_game_instance_t *gi, AW_player_ptr killer_p, AW_player_ptr unit_p, AW_unit_ptr u);
short         MOBA_GetAbilitiesCount        (AW_game_instance_t *gi, AW_client_ptr c, AW_unit_type_t unit_type);
str           MOBA_GetAbilityName           (AW_game_instance_t *gi, AW_client_ptr c, AW_unit_type_t unit_type, short i);
char          MOBA_GetAbilityShortcut       (AW_game_instance_t *gi, AW_client_ptr c, AW_unit_type_t unit_type, short i);
bool          MOBA_TriggerAbility           (AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, short i, short cx, short cy);
void          MOBA_HUDInfo                  (AW_game_instance_t *gi, AW_client_ptr c);
void          MOBA_HUDInfoMultipleSelection (AW_game_instance_t *gi, AW_client_ptr c, int selected_units_count);
void          MOBA_HUDInfoCastle            (AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u);
void          MOBA_HUDInfoGraveyard         (AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u);
void          MOBA_HUDInfoLibrary           (AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u);
str           MOBA_GetUnitName              (AW_game_instance_t *gi, AW_unit_type_t unit_type);
void          MOBA_OnSpawnUnit              (AW_game_instance_t *gi, AW_unit_ptr from_u, AW_unit_ptr u);
void          MOBA_OnCancelBuild            (AW_game_instance_t *gi, AW_build_order_ptr b);
void          MOBA_GenericCmd               (AW_game_instance_t *gi, AW_id_t cmd_type, AW_id_t id, AW_id_t player_id, AW_id_t cmd_mask, AW_unit_type_t unit_type, short target_cx, short target_cy);
void          MOBA_OnUnitOrder              (AW_game_instance_t *gi, AW_unit_ptr u, AW_unit_ptr target, AW_id_t id, AW_id_t player_id, AW_id_t cmd_mask, AW_unit_type_t unit_type, short target_cx, short target_cy);
AW_unit_ptr   MOBA_GetCastle                (AW_game_instance_t *gi, AW_client_ptr c);
AW_unit_ptr   MOBA_GetLibrary               (AW_game_instance_t *gi, AW_client_ptr c);
void          MOBA_OpenWindow               (AW_game_instance_t *gi, AW_client_ptr c, int size_x, int size_y);
void          MOBA_DrawUnitTarget           (AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u);
void          MOBA_DrawPointer              (AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, AW_unit_type_t unit_type, int ability_id, short cx, short cy);
void          MOBA_DrawCursor               (SDL_Surface *surface, AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, AW_unit_type_t unit_type, int ability_id, int offx, int offy);
void          MOBA_DrawAttackCursor         (SDL_Surface *surface, AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, int offx, int offy);

extern AW_moba_game_instance_t moba_gi;

extern AW_unit_class_t              unit_dic[MAX_UNIT_TYPE];

/*
 * end of file
 */    