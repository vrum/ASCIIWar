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

#include "asciiwar.h"

/*
 * mobs
 */

void MOBS_Init(int argc, char** argv) {
  for(int i = 0; i < MAX_MOBS; i++) {
    if(i != MAX_MOBS-1)
      mobs(i).fnext = i+1;
    else
      mobs(i).fnext = AW_null;
  }
  moba_gi.free_mobs_head = 0;
  moba_gi.mobs_head      = AW_null;
  trace("mobss initiated.");
}

AW_mobs_ptr MOBS_New() {
  if(moba_gi.free_mobs_head != AW_null) {
    AW_mobs_ptr r           = moba_gi.free_mobs_head;
    moba_gi.free_mobs_head  = mobs(moba_gi.free_mobs_head).fnext;
    mobs(r).mission         = 0;
    mobs(r).previous        = AW_null;
    mobs(r).next            = moba_gi.mobs_head;
    if(moba_gi.mobs_head != AW_null)
      mobs(moba_gi.mobs_head).previous = r;
    moba_gi.mobs_head = r;
    return r;
  }
  trace("No more mobs left.");
  return AW_null;
}

void MOBS_Free(AW_mobs_ptr l) {
  if(moba_gi.mobs_head == l)
    moba_gi.mobs_head = mobs(l).next;
  if(mobs(l).previous != AW_null)
    mobs(mobs(l).previous).next = mobs(l).next;
  if(mobs(l).next != AW_null)
    mobs(mobs(l).next).previous = mobs(l).previous;
  mobs(l).fnext = moba_gi.free_mobs_head;
  moba_gi.free_mobs_head = l;
}

void MOBS_FreeAll(AW_game_instance_t *gi) {
  while(moba_gi.mobs_head != AW_null)
    MOBS_Free(moba_gi.mobs_head);
  trace("mobss freed.");
}

/*
 * moba_player
 */

void MP_Init(int argc, char** argv) {
  for(int i = 0; i < MAX_MOBA_PLAYER; i++) {
    if(i != MAX_MOBA_PLAYER-1)
      moba_player(i).fnext = i+1;
    else
      moba_player(i).fnext = AW_null;
  }
  moba_gi.free_moba_player_head = 0;
  moba_gi.moba_player_head      = AW_null;
  trace("moba_players initiated.");
}

AW_moba_player_ptr MP_New() {
  if(moba_gi.free_moba_player_head != AW_null) {
    AW_moba_player_ptr r                  = moba_gi.free_moba_player_head;
    moba_gi.free_moba_player_head         = moba_player(moba_gi.free_moba_player_head).fnext;
    moba_player(r).gold                   = PLAYER_STARTING_GOLD;
    moba_player(r).xp                     = PLAYER_STARTING_XP;
    moba_player(r).previous               = AW_null;
    moba_player(r).next                   = moba_gi.moba_player_head;
    moba_player(r).has_lawful_good_level1     =
    moba_player(r).has_chaotic_good_level1    =
    moba_player(r).has_lawful_neutral_level1  =
    moba_player(r).has_chaotic_neutral_level1 =
    moba_player(r).has_lawful_evil_level1     =
    moba_player(r).has_chaotic_evil_level1    = false;
    if(moba_gi.moba_player_head != AW_null)
      moba_player(moba_gi.moba_player_head).previous = r;
    moba_gi.moba_player_head = r;
    return r;
  }
  trace("No more moba_player left.");
  return AW_null;
}

void MP_Free(AW_moba_player_ptr l) {
  if(moba_gi.moba_player_head == l)
    moba_gi.moba_player_head = moba_player(l).next;
  if(moba_player(l).previous != AW_null)
    moba_player(moba_player(l).previous).next = moba_player(l).next;
  if(moba_player(l).next != AW_null)
    moba_player(moba_player(l).next).previous = moba_player(l).previous;
  moba_player(l).fnext = moba_gi.free_moba_player_head;
  moba_gi.free_moba_player_head = l;
}

void MP_FreeAll(AW_game_instance_t *gi) {
  while(moba_gi.moba_player_head != AW_null)
    MP_Free(moba_gi.moba_player_head);
  trace("moba_players freed.");
}

/*
 * castle
 */

void CAS_Init(int argc, char** argv) {
  for(int i = 0; i < MAX_CASTLE; i++) {
    if(i != MAX_CASTLE-1)
      castle(i).fnext = i+1;
    else
      castle(i).fnext = AW_null;
  }
  moba_gi.free_castle_head = 0;
  moba_gi.castle_head      = AW_null;
  trace("castles initiated.");
}

AW_castle_ptr CAS_New() {
  if(moba_gi.free_castle_head != AW_null) {
    AW_castle_ptr r                = moba_gi.free_castle_head;
    moba_gi.free_castle_head       = castle(moba_gi.free_castle_head).fnext;
    castle(r).previous             = AW_null;
    castle(r).next                 = moba_gi.castle_head;
    if(moba_gi.castle_head != AW_null)
      castle(moba_gi.castle_head).previous = r;
    moba_gi.castle_head = r;
    return r;
  }
  trace("No more castle left.");
  return AW_null;
}

void CAS_Free(AW_castle_ptr l) {
  if(moba_gi.castle_head == l)
    moba_gi.castle_head = castle(l).next;
  if(castle(l).previous != AW_null)
    castle(castle(l).previous).next = castle(l).next;
  if(castle(l).next != AW_null)
    castle(castle(l).next).previous = castle(l).previous;
  castle(l).fnext = moba_gi.free_castle_head;
  moba_gi.free_castle_head = l;
}

void CAS_FreeAll(AW_game_instance_t *gi) {
  while(moba_gi.castle_head != AW_null)
    CAS_Free(moba_gi.castle_head);
  trace("castles freed.");
}

/*
 * graveyard
 */

void GRA_Init(int argc, char** argv) {
  for(int i = 0; i < MAX_GRAVEYARD; i++) {
    if(i != MAX_GRAVEYARD-1)
      graveyard(i).fnext = i+1;
    else
      graveyard(i).fnext = AW_null;
  }
  moba_gi.free_graveyard_head = 0;
  moba_gi.graveyard_head      = AW_null;
  trace("graveyards initiated.");
}

AW_graveyard_ptr GRA_New() {
  if(moba_gi.free_graveyard_head != AW_null) {
    AW_graveyard_ptr r                = moba_gi.free_graveyard_head;
    moba_gi.free_graveyard_head       = graveyard(moba_gi.free_graveyard_head).fnext;
    graveyard(r).previous             = AW_null;
    graveyard(r).next                 = moba_gi.graveyard_head;
    if(moba_gi.graveyard_head != AW_null)
      graveyard(moba_gi.graveyard_head).previous = r;
    moba_gi.graveyard_head = r;
    return r;
  }
  trace("No more graveyard left.");
  return AW_null;
}

void GRA_Free(AW_graveyard_ptr l) {
  if(moba_gi.graveyard_head == l)
    moba_gi.graveyard_head = graveyard(l).next;
  if(graveyard(l).previous != AW_null)
    graveyard(graveyard(l).previous).next = graveyard(l).next;
  if(graveyard(l).next != AW_null)
    graveyard(graveyard(l).next).previous = graveyard(l).previous;
  graveyard(l).fnext = moba_gi.free_graveyard_head;
  moba_gi.free_graveyard_head = l;
}

void GRA_FreeAll(AW_game_instance_t *gi) {
  while(moba_gi.graveyard_head != AW_null)
    GRA_Free(moba_gi.graveyard_head);
  trace("graveyards freed.");
}

/*
 * graveyard
 */

void LIB_Init(int argc, char** argv) {
  for(int i = 0; i < MAX_LIBRARY; i++) {
    if(i != MAX_LIBRARY-1)
      library(i).fnext = i+1;
    else
      library(i).fnext = AW_null;
  }
  moba_gi.free_library_head = 0;
  moba_gi.library_head      = AW_null;
  trace("librarys initiated.");
}

AW_library_ptr LIB_New() {
  if(moba_gi.free_library_head != AW_null) {
    AW_library_ptr r                = moba_gi.free_library_head;
    moba_gi.free_library_head       = library(moba_gi.free_library_head).fnext;
    library(r).previous             = AW_null;
    library(r).next                 = moba_gi.library_head;
    if(moba_gi.library_head != AW_null)
      library(moba_gi.library_head).previous = r;
    moba_gi.library_head = r;
    return r;
  }
  trace("No more library left.");
  return AW_null;
}

void LIB_Free(AW_library_ptr l) {
  if(moba_gi.library_head == l)
    moba_gi.library_head = library(l).next;
  if(library(l).previous != AW_null)
    library(library(l).previous).next = library(l).next;
  if(library(l).next != AW_null)
    library(library(l).next).previous = library(l).previous;
  library(l).fnext = moba_gi.free_library_head;
  moba_gi.free_library_head = l;
}

void LIB_FreeAll(AW_game_instance_t *gi) {
  while(moba_gi.library_head != AW_null)
    LIB_Free(moba_gi.library_head);
  trace("librarys freed.");
}

/*
 * game instance
 */

void MOBA_FreeAll(AW_game_instance_t *gi) {
  /* callbacls */
  gi->free_all_cb                     = null;
  gi->on_death_cb                        = null;
  gi->update_client_cb                = null;
  gi->get_abilities_count_cb          = null;
  gi->get_ability_name_cb             = null;
  gi->get_ability_shortcut_cb         = null;
  gi->trigger_ability_cb              = null;
  gi->hud_info_cb             = null;
  MP_FreeAll(gi);
  MOBS_FreeAll(gi);
  CAS_FreeAll(gi);
  GRA_FreeAll(gi);
  LIB_FreeAll(gi);
}

AW_client_ptr MOBA_GetMobsAIClient(AW_game_instance_t *gi) {
  AW_client_ptr c = gi->client_head;
  while(c != AW_null) {
    AW_client_t *cl = &client(c);
    if(cl->type == MOBS_AI_CLIENT)
      return c;
    c = cl->next;
  }
  return AW_null;
}

void MOBA_UpdateClient(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_player_ptr p = GI_GetPlayerPtr(gi, cl->player_id);
  AW_player_t *pl = &player(p);
  if(gi->game_time_step != 0
    && !pl->lose
    && !pl->win) {
    #if !REPLAY
    if(cl->type == MOBS_AI_CLIENT)
      MOBA_UpdateMobsAI(gi, c);
    #endif
  }
}

void MOBA_InitGame(AW_game_instance_t *gi, game_desc_t *gd) {
  /* init stuff */
  MP_Init(0, null);
  MOBS_Init(0, null);
  CAS_Init(0, null);
  GRA_Init(0, null);
  LIB_Init(0, null);
  /* create players*/
  bool team_captain = 
    game_desc.local_player_id % game_desc.player_count_per_team == 0;
  for(int j = 0; j < game_desc.team_count; j++)
    for(int i = 0; i < game_desc.player_count_per_team; i++) {
      int player_id = i + (j*game_desc.player_count_per_team);
      int team_id   = j;
      AW_player_ptr p   = PL_New(gi, MP_New());
      AW_player_t *pl   = &player(p);
      pl->id            = player_id;
      pl->team_id       = team_id;
      /* remote */
      if(j != game_desc.local_team_id) {
        AW_remote_cmd_store_ptr r = RS_New(gi, CS_New(gi));
        pl->cs = remote_cmd_store(r).cs;
        remote_cmd_store(r).player_id = player_id;
      } else 
      /* human */
      if(player_id == game_desc.local_player_id) {
        AW_client_ptr c = CL_New(gi, CS_New(gi));
        pl->cs = client(c).cs;
        client(c).type = HUMAN_CLIENT;
        client(c).player_id = player_id;
      } else 
      /* mobs ai */
      if(team_captain && i == game_desc.player_count_per_team-1) {
        AW_client_ptr c = CL_New(gi, CS_New(gi));
        pl->cs = client(c).cs;
        client(c).type = MOBS_AI_CLIENT;
        client(c).player_id = player_id;
        moba_gi.mobs_ai_clients[player_id].mobs_spawn_timer = MOBS_SPAWN_TIME;
        client(c).user_data = (AW_ptr_t)player_id;
      } else
        assert(false);
      #if REPLAY
      gi->max_turn = 0;
      FILE *file = fopen(str(str("./replay/") + i2a(pl->id)).c_str(), "r+b");
      trace(str(str("./replay/") + i2a(pl->id)).c_str());
      assert(file != null);
      while(!feof(file)) {
        AW_cmd_type_t type;
        fread(&type, sizeof(AW_cmd_type_t), 1, file);
        AW_id_t cmd_id;
        fread(&cmd_id, sizeof(cmd_id), 1, file);
        int count = sizeof(AW_cmd_type_t) + sizeof(AW_id_t);
        AW_cmd_ptr r        = CMD_New(gi, cmd_id);
        AW_cmd_t *cmd       = &cmd(r);
        switch(type) {
          case AW_cmd_type_unit_order: {
              AW_unit_order_packet_t data;
              data.type             = type;
              data.id               = cmd_id;
              fread((unsigned char*)&data + count, sizeof(data) - count, 1, file);
              cmd->type             = data.type;
              cmd->id               = data.id;
              cmd->turn             = data.turn;
              cmd->cmd_mask         = data.cmd_mask;
              cmd->r_squared        = data.r_squared;
              cmd->target_cx        = data.target_cx;
              cmd->target_cy        = data.target_cy;
              cmd->target           = data.target;
              cmd->target_player_id = data.target_player_id;
              cmd->target_cmd_id    = data.target_cmd_id;
              cmd->attack_here      = data.attack_here;
              cmd->push_back        = data.push_back;
              gi->max_turn          = MAX(gi->max_turn, cmd->turn);
              CS_InsertCmd(gi, pl->cs, r);
            }
            break;
          case AW_cmd_type_spawn_unit: {
              AW_spawn_unit_packet_t data;
              data.type       = type;
              data.id         = cmd_id;
              fread((unsigned char*)&data + count, sizeof(data) - count, 1, file);
              cmd->type       = data.type;
              cmd->id         = data.id;
              cmd->unit_type  = data.unit_type;
              cmd->turn       = data.turn;
              cmd->target_cx  = data.target_cx;
              cmd->target_cy  = data.target_cy;
              gi->max_turn    = MAX(gi->max_turn, cmd->turn);
              CS_InsertCmd(gi, pl->cs, r);
            }
            break;
          case AW_cmd_type_build_unit: {
              AW_build_unit_packet_t data;
              data.type               = type;
              data.id                 = cmd_id;
              fread((unsigned char*)&data + count, sizeof(data) - count, 1, file);
              cmd->turn               = data.turn;
              data.unit_type          = cmd->unit_type;
              data.target_cx          = cmd->target_cx;
              data.target_cy          = cmd->target_cy;
              data.player_id          = cmd->player_id;
              data.target_player_id   = cmd->target_player_id;
              data.start_it           = cmd->start_it;
              gi->max_turn    = MAX(gi->max_turn, cmd->turn);
              CS_InsertCmd(gi, pl->cs, r);
            }
            break;
          case AW_cmd_type_end_of_turn: {
              AW_end_of_turn_packet_t data;
              data.type               = type;
              data.id                 = cmd_id;
              fread((unsigned char*)&data + count, sizeof(data) - count, 1, file);
              cmd->type               = data.type;
              cmd->id                 = data.id;
              cmd->turn               = data.turn;
              cmd->turn_frame_count   = data.turn_frame_count;
              cmd->frame_time_step    = data.frame_time_step;
              cmd->hash               = 0;//data.hash;
              cmd->sent               = true;
              CS_InsertCmd(gi, pl->cs, r);
            }
            break;
        }
      }
      fclose(file);
      #else
      remove(str(str("./replay/") + i2a(pl->id)).c_str());
      //fclose(fopen(str(str("./replay/") + i2a(pl->id)).c_str(), "w+b"));
      #endif
    }
  /* callbacks */
  gi->free_all_cb                     = MOBA_FreeAll;
  gi->on_death_cb                     = MOBA_DeathCB;
  gi->update_client_cb                = MOBA_UpdateClient;
  gi->get_abilities_count_cb          = MOBA_GetAbilitiesCount;
  gi->get_ability_name_cb             = MOBA_GetAbilityName;
  gi->get_ability_shortcut_cb         = MOBA_GetAbilityShortcut;
  gi->trigger_ability_cb              = MOBA_TriggerAbility;
  gi->hud_info_cb                     = MOBA_HUDInfo;
  gi->on_spawn_unit_cb                = MOBA_OnSpawnUnit;
  gi->on_cancel_build_cb              = MOBA_OnCancelBuild;
  gi->on_generic_cmd_cb               = MOBA_GenericCmd;
}

void MOBA_StartGame(AW_game_instance_t *gi) {
  MOBA_StartMobsAIClient(gi, MOBA_GetMobsAIClient(gi));
}

void MOBA_StartMobsAIClient(AW_game_instance_t *gi, AW_client_ptr c) {
  #if !REPLAY
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 25 : MAP_SIZE_X-1-25, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-25 : 25,
    AW_unit_type_C);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 40 : MAP_SIZE_X-1-40, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-40 : 40,
    AW_unit_type_G);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 55 : MAP_SIZE_X-1-55, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-55 : 55,
    AW_unit_type_L);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 75 : MAP_SIZE_X-1-75, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-75 : 75,
    AW_unit_type_T);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 75 : MAP_SIZE_X-1-75, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-32 : 32,
    AW_unit_type_T);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 33 : MAP_SIZE_X-1-33, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-75 : 75,
    AW_unit_type_T);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 112 : MAP_SIZE_X-1-112, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-112 : 112,
    AW_unit_type_T);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 169 : MAP_SIZE_X-1-169, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-169 : 169,
    AW_unit_type_T);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 150 : MAP_SIZE_X-1-150, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-32 : 32,
    AW_unit_type_T);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 33 : MAP_SIZE_X-1-33, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-150 : 150,
    AW_unit_type_T);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 225 : MAP_SIZE_X-1-225, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-225 : 225,
    AW_unit_type_T);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 255 : MAP_SIZE_X-1-255, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-32 : 32,
    AW_unit_type_T);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 33 : MAP_SIZE_X-1-33, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-255 : 255,
    AW_unit_type_T);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? (MAP_SIZE_X-150) : MAP_SIZE_X-1-(MAP_SIZE_X-150), 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-32 : 32,
    AW_unit_type_T);
  CL_CmdSpawnUnit(
    gi, 
    c, 
    game_desc.local_team_id == 0 ? 33 : MAP_SIZE_X-1-33, 
    game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-(MAP_SIZE_Y-150) : (MAP_SIZE_Y-150),
    AW_unit_type_T);
  #endif
}

void MOBA_UpdateMobsAI(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_player_ptr p = GI_GetPlayerPtr(gi, cl->player_id);
  AW_player_t *pl = &player(p);
  AW_mobs_ai_client_t *mobs_ai_clients = &moba_gi.mobs_ai_clients[(int)cl->user_data];
  /* mobs spawns */
  mobs_ai_clients->mobs_spawn_timer += gi->game_time_step;
  if(mobs_ai_clients->mobs_spawn_timer >= MOBS_SPAWN_TIME) {
    mobs_ai_clients->mobs_spawn_timer = 0;
    DO_TIMES(5) {
      {
        AW_mobs_ptr m = MOBS_New();
        if(m != AW_null) {
          AW_mobs_t *mo = &mobs(m);
          mo->lane = 1;
          CL_CmdSpawnUnit(
            gi, 
            c, 
            game_desc.local_team_id == 0 ? 35 : MAP_SIZE_X-1-35, 
            game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-35 : 35, 
            AW_unit_type_m,
            (AW_ptr_t)m);
        }
      }
      {
        AW_mobs_ptr m = MOBS_New();
        if(m != AW_null) {
          AW_mobs_t *mo = &mobs(m);
          mo->lane = 0;
          CL_CmdSpawnUnit(
            gi, 
            c, 
            game_desc.local_team_id == 0 ? 35 : MAP_SIZE_X-1-35, 
            game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-35 : 35, 
            AW_unit_type_m,
            (AW_ptr_t)m);
        }
      }
      {
        AW_mobs_ptr m = MOBS_New();
        if(m != AW_null) {
          AW_mobs_t *mo = &mobs(m);
          mo->lane = 2;
          CL_CmdSpawnUnit(
            gi, 
            c, 
            game_desc.local_team_id == 0 ? 35 : MAP_SIZE_X-1-35, 
            game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-35 : 35, 
            AW_unit_type_m,
            (AW_ptr_t)m);
        }
      }
    }
  }
  /* orders */
  AW_unit_ptr u;
  cl->attack_here = true;
  /* middle lane */
  u = pl->unit_head;
  while(u != AW_null) {
    AW_unit_t *un = &unit(u);
    AW_mobs_ptr m = un->user_data;
    AW_mobs_t *mo = &mobs(m);
    if(un->unit_type == AW_unit_type_m
    && mo->lane == MIDDLE_LANE
    && !CL_HasOrder(gi, c, u)) {
      CL_FreeUnitSelection(gi, c);
      CL_AddToSelection(gi, c, u);
      if(mo->mission <= 0)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id == 0 ? 75+6 : MAP_SIZE_X-1-75-6), 
          (game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-75-6 : 75+6),
          true,
          true,
          0,
          (10*10),
          0,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 1)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id == 0 ? 112+6 : MAP_SIZE_X-1-112-6), 
          (game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-112-6 : 112+6),
          true,
          true,
          0,
          (10*10),
          1,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 2)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id == 0 ? 169+6 : MAP_SIZE_X-1-169-6), 
          (game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-169-6 : 169+6),
          true,
          true,
          0,
          (10*10),
          2,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 3)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id == 0 ? 225+6 : MAP_SIZE_X-1-225-6), 
          (game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-225-6 : 225+6),
          true,
          true,
          0,
          (10*10),
          3,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      /* enemy side */
      if(mo->mission <= 4)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id != 0 ? 225-6 : MAP_SIZE_X-1-225+6), 
          (game_desc.local_team_id != 0 ? MAP_SIZE_Y-1-225+6 : 225-6),
          true,
          true,
          0,
          (10*10),
          4,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 5)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id != 0 ? 169-6 : MAP_SIZE_X-1-169+6), 
          (game_desc.local_team_id != 0 ? MAP_SIZE_Y-1-169+6 : 169-6),
          true,
          true,
          0,
          (10*10),
          5,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 6)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id != 0 ? 112-6 : MAP_SIZE_X-1-112+6), 
          (game_desc.local_team_id != 0 ? MAP_SIZE_Y-1-112+6 : 112-6),
          true,
          true,
          0,
          (10*10),
          6,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 7)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id != 0 ? 75-6 : MAP_SIZE_X-1-75+6), 
          (game_desc.local_team_id != 0 ? MAP_SIZE_Y-1-75+6 : 75-6),
          true,
          true,
          0,
          (10*10),
          7,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      CL_FreeUnitSelection(gi, c);
    }
    u = un->pnext;
  }
  
  /* left lane */
  u = pl->unit_head;
  while(u != AW_null) {
    AW_unit_t *un = &unit(u);
    AW_mobs_ptr m = un->user_data;
    AW_mobs_t *mo = &mobs(m);
    if(un->unit_type == AW_unit_type_m
    && mo->lane == LEFT_LANE
    && !CL_HasOrder(gi, c, u)) {
      CL_FreeUnitSelection(gi, c);
      CL_AddToSelection(gi, c, u);
      if(mo->mission <= 0)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id == 0 ? 33 : MAP_SIZE_X-1-33-1), 
          (game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-75-6 : 75+6),
          true,
          true,
          0,
          (10*10),
          0,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 1)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id == 0 ? 33 : MAP_SIZE_X-1-33-1), 
          (game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-150-6 : 150+6),
          true,
          true,
          0,
          (10*10),
          1,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 2)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id == 0 ? 33 : MAP_SIZE_X-1-33-1), 
          (game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-255-6 : 255+6),
          true,
          true,
          0,
          (10*10),
          2,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 3)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id == 0 ? 33 : MAP_SIZE_X-1-33-1), 
          (game_desc.local_team_id == 0 ? 150-6 : (MAP_SIZE_Y-1-150)+6),
          true,
          true,
          0,
          (10*10),
          3,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      // enemy side
      if(mo->mission <= 4)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id != 0 ? (MAP_SIZE_X-1-150)-6 : 150+6),
          (game_desc.local_team_id != 0 ? MAP_SIZE_Y-1-32 : 32),
          true,
          true,
          0,
          (10*10),
          4,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 5)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id != 0 ? 255-6 : MAP_SIZE_X-1-255+6),
          (game_desc.local_team_id != 0 ? MAP_SIZE_Y-1-32 : 32),
          true,
          true,
          0,
          (10*10),
          5,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 6)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id != 0 ? 150-6 : MAP_SIZE_X-1-150+6),
          (game_desc.local_team_id != 0 ? MAP_SIZE_Y-1-32 : 32),
          true,
          true,
          0,
          (10*10),
          6,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 7)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id != 0 ? 75-6 : MAP_SIZE_X-1-75+6),
          (game_desc.local_team_id != 0 ? MAP_SIZE_Y-1-32 : 32),
          true,
          true,
          0,
          (10*10),
          7,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      CL_FreeUnitSelection(gi, c);
    }
    u = un->pnext;
  }

  /* right lane */
  u = pl->unit_head;
  while(u != AW_null) {
    AW_unit_t *un = &unit(u);
    AW_mobs_ptr m = un->user_data;
    AW_mobs_t *mo = &mobs(m);
    if(un->unit_type == AW_unit_type_m
    && mo->lane == RIGHT_LANE
    && !CL_HasOrder(gi, c, u)) {
      CL_FreeUnitSelection(gi, c);
      CL_AddToSelection(gi, c, u);
      if(mo->mission <= 0)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id == 0 ? 75+6 : MAP_SIZE_X-1-75-6),
          (game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-32 : 32),
          true,
          true,
          0,
          (10*10),
          0,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 1)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id == 0 ? 150+6 : MAP_SIZE_X-1-150-6),
          (game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-32 : 32),
          true,
          true,
          0,
          (10*10),
          1,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 2)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id == 0 ? 255+6 : MAP_SIZE_X-1-255-6),
          (game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-32 : 32),
          true,
          true,
          0,
          (10*10),
          2,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 3)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id == 0 ? (MAP_SIZE_X-1-150)+6 : 150-6),
          (game_desc.local_team_id == 0 ? MAP_SIZE_Y-1-32 : 32),
          true,
          true,
          0,
          (10*10),
          3,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      // enemy side
      if(mo->mission <= 4)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id != 0 ? 33 : MAP_SIZE_X-1-33-1),
          (game_desc.local_team_id != 0 ? 150+6 : (MAP_SIZE_Y-1-150)-6),
          true,
          true,
          0,
          (10*10),
          4,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 5)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id != 0 ? 33 : MAP_SIZE_X-1-33-1),
          (game_desc.local_team_id != 0 ? MAP_SIZE_Y-1-255+6 : 255-6),
          true,
          true,
          0,
          (10*10),
          5,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 6)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id != 0 ? 33 : MAP_SIZE_X-1-33-1),
          (game_desc.local_team_id != 0 ? MAP_SIZE_Y-1-150+6 : 150-6),
          true,
          true,
          0,
          (10*10),
          6,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      if(mo->mission <= 7)
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (game_desc.local_team_id != 0 ? 33 : MAP_SIZE_X-1-33-1),
          (game_desc.local_team_id != 0 ? MAP_SIZE_Y-1-75+6 : 75-6),
          true,
          true,
          0,
          (10*10),
          7,
          MOBA_MobsOrderCompletedCB,
          MOBA_MobsOrderFailedCB);
      CL_FreeUnitSelection(gi, c);
    }
    u = un->pnext;
  }
  /* end */
  cl->attack_here = false;
}

void MOBA_MobsOrderCompletedCB(AW_game_instance_t *gi, AW_unit_ptr u, AW_unit_order_ptr o) {
  AW_mobs_ptr m = unit(u).user_data;
  assert(mobs(m).mission==order(o).user_data);
  mobs(m).mission++;
}

void MOBA_MobsOrderFailedCB(AW_game_instance_t *gi, AW_unit_ptr u, AW_unit_order_ptr o) {
  AW_mobs_ptr m = unit(u).user_data;
}

void MOBA_DeathCB(AW_game_instance_t *gi, AW_player_ptr killer_p, AW_player_ptr unit_p, AW_unit_ptr u) {
  AW_unit_t *un = &unit(u);
  AW_player_t *pl = &player(unit_p);
  AW_moba_player_ptr mp = player(killer_p).user_data;
  assert(mp != AW_null);
  /* xp */
  AW_moba_player_t *killer_mpl = &moba_player(mp);
  AW_player_ptr ps[MAX_PLAYER];
  int ps_count = 0;
  FOR_RECT(-DEMI_XP_RADIUS, DEMI_XP_RADIUS, -DEMI_XP_RADIUS, DEMI_XP_RADIUS) {
    AW_unit_ptr u = AT(unit_map, un->pos_x+i, un->pos_y+j);
    if(u != AW_null) {
      AW_player_ptr p = 
        GI_GetPlayerPtr(gi, unit(u).player_id);
      bool already = false;
      DO_TIMES(ps_count)
        if(ps[f] == p) {
          already = true;
          break;
        }
      if(!already
      && p != unit_p
      && p != killer_p
      && player(p).team_id 
      == player(killer_p).team_id)
        ps[ps_count++] = p;
    }
  }
  DO_TIMES(ps_count) {
    AW_moba_player_ptr mp = player(ps[f]).user_data;
    assert(mp != AW_null);
    AW_moba_player_t *mpl = &moba_player(mp);
    mpl->xp += XP(un)/2;
    if(player(ps[f]).id == game_desc.local_player_id) {
      AW_floating_text_ptr ft = FT_New(gi);
      AW_floating_text_t *flt = &floating_text(ft);
      flt->pos_x = un->pos_x;
      flt->pos_y = un->pos_y;
      flt->s = "+" + i2a(XP(un)/2) + "xp";
      flt->c = TCOD_green;
    }
  }
  if(killer_p != unit_p) {
    killer_mpl->xp += XP(un);
    if(player(killer_p).id == game_desc.local_player_id) {
      AW_floating_text_ptr ft = FT_New(gi);
      AW_floating_text_t *flt = &floating_text(ft);
      flt->pos_x = un->pos_x;
      flt->pos_y = un->pos_y;
      flt->s = "+" + i2a(XP(un)) + "xp";
      flt->c = TCOD_green;
    }
  }
  /* miscs */
  switch(un->unit_type) {
    case AW_unit_type_C: {
        AW_build_explosion_ptr b = BE_New(gi);;
        AW_build_explosion_t *be = &build_explosion(b);
        be->pos_x = un->pos_x;
        be->pos_y = un->pos_y;
        AW_player_ptr p2 = gi->player_head;
        while(p2 != AW_null) {
          AW_player_t *pl2 = &player(p2);
          if(pl2->team_id == pl->team_id)
            pl2->lose = true;
          else
            pl2->win  = true;
          p2 = pl2->next;
        }
        if(unit(u).user_data != AW_null)
          CAS_Free((AW_castle_ptr)unit(u).user_data);
      } break;
    case AW_unit_type_m: {
        /* is this a remote mobs? */
        if(unit(u).user_data != AW_null)
          MOBS_Free((AW_mobs_ptr)unit(u).user_data);
      } break;
    case AW_unit_type_T: {
        AW_build_explosion_ptr b = BE_New(gi);;
        AW_build_explosion_t *be = &build_explosion(b);
        be->pos_x = un->pos_x;
        be->pos_y = un->pos_y;
      } break;
    case AW_unit_type_G: {
        AW_build_explosion_ptr b = BE_New(gi);;
        AW_build_explosion_t *be = &build_explosion(b);
        be->pos_x = un->pos_x;
        be->pos_y = un->pos_y;
      } break;
    case AW_unit_type_L: {
        AW_build_explosion_ptr b = BE_New(gi);;
        AW_build_explosion_t *be = &build_explosion(b);
        be->pos_x = un->pos_x;
        be->pos_y = un->pos_y;
      } break;
    default: {
        /* start the resurection */
        if(un->player_id == game_desc.local_player_id) {
          AW_player_ptr p2 = GI_GetPlayerPtr(gi, game_desc.local_player_id);
          AW_client_ptr c = GI_GetHumanClient(gi);
          AW_graveyard_ptr G = moba_gi.graveyard_head;
          while(G != AW_null
             && TEAM_ID(graveyard(G).player_id)
             != TEAM_ID(game_desc.local_player_id))
            G = graveyard(G).next;
          if(G != AW_null)
            CL_CmdBuildOrder(
              gi, 
              c, 
              unit(graveyard(G).u).cmd_id, 
              unit(graveyard(G).u).player_id,
              un->player_id,
              un->unit_type, 
              unit(graveyard(G).u).pos_x + (game_desc.local_team_id == 0 ? 2 : -2), 
              unit(graveyard(G).u).pos_y,
              true);
        }
      } break;
  }
}

short MOBA_GetAbilitiesCount(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_type_t unit_type) {
  AW_client_t *cl = &client(c);
  return 0;
}

str MOBA_GetAbilityName(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_type_t unit_type, short i) {
  AW_client_t *cl = &client(c);
  switch(unit_type) {
    case AW_unit_type_C:
      switch(i) {
        case 0:
          return str("(G)oblin [" + i2a(unit_dic[AW_unit_type_g].gold_price) + "*]");
        case 1:
          return str("(O)rc [" + i2a(unit_dic[AW_unit_type_O].gold_price) + "*]");
      }
  }
  return "";
}

char MOBA_GetAbilityShortcut(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_type_t unit_type, short i) {
  AW_client_t *cl = &client(c);
  switch(unit_type) {
    case AW_unit_type_C:
      switch(i) {
        case 0:
          return 'g';
        case 1:
          return 'o';
      }
  }
  return 0;
}

void MOBA_TriggerAbility(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, short i) {
  AW_client_t *cl = &client(c);
  AW_moba_player_ptr mp = player(GI_GetPlayerPtr(gi, cl->player_id)).user_data;
  assert(mp != AW_null);
  AW_moba_player_t *mpl = &moba_player(mp);
  AW_unit_t *un = &unit(u);
  switch(un->unit_type) {
    /* castle */
    case AW_unit_type_C:
      switch(i) {
        case 0: {
            if(mpl->gold >= unit_dic[AW_unit_type_g].gold_price
            && BO_GetBuildOrderCount(gi, cl->player_id, c, u) < MAX_HUD_CENTRAL_INFO) {
              mpl->gold -= unit_dic[AW_unit_type_g].gold_price;
              START_SOUND(coin_sound, 0)
              CL_CmdBuildOrder(gi, c, un->cmd_id, un->player_id, cl->player_id,AW_unit_type_g, un->pos_x + (game_desc.local_team_id == 0 ? 4 : -4), un->pos_y,false);
            }
          } break;
        case 1: {
            if(mpl->gold >= unit_dic[AW_unit_type_O].gold_price
            && BO_GetBuildOrderCount(gi, cl->player_id, c, u) < MAX_HUD_CENTRAL_INFO) {
              mpl->gold -= unit_dic[AW_unit_type_O].gold_price;
              START_SOUND(coin_sound, 0)
              CL_CmdBuildOrder(gi, c, un->cmd_id, un->player_id,cl->player_id,AW_unit_type_O, un->pos_x + (game_desc.local_team_id == 0 ? 4 : -4), un->pos_y,false);
            }
          } break;
      }
  }
}

void MOBA_HUDInfo(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl     = &client(c);
  AW_moba_player_ptr mp = player(GI_GetPlayerPtr(gi, cl->player_id)).user_data;
  assert(mp != AW_null);
  AW_moba_player_t *mpl = &moba_player(mp);
  /* resource */
  TCOD_console_set_default_foreground(con, TCOD_gold);
  AW_PrintString("*", CON_RES_X-24, 0);
  TCOD_console_set_default_foreground(con, TCOD_white);
  AW_PrintString(i2a(mpl->gold), CON_RES_X-24+1, 0);
  TCOD_console_set_default_foreground(con, TCOD_green);
  AW_PrintString("XP", CON_RES_X-24+12, 0);
  TCOD_console_set_default_foreground(con, TCOD_white);
  AW_PrintString(i2a(mpl->xp), CON_RES_X-24+12+2, 0);
  /* multiple selection */
  int selected_units_count = CL_GetSelectedUnitsCount(gi, c);
  if(selected_units_count > 1)
    MOBA_HUDInfoMultipleSelection(gi, c, selected_units_count);
  else {
    /* one-unit selection */
    AW_unit_ptr u = cl->selected_units_head;
    if(u != AW_null) {
      AW_unit_t *un = &unit(u);
      AW_PrintString(MOBA_GetUnitName(gi, un->unit_type), START_HUD_INFO_X, HUD_START_Y);
      switch(un->unit_type) {
        /* castle */
        case AW_unit_type_C:
          MOBA_HUDInfoCastle(gi, c, u);
          break;
        /* graveyard */
        case AW_unit_type_G:
          MOBA_HUDInfoGraveyard(gi, c, u);
          break;
        /* library */
        case AW_unit_type_L:
          MOBA_HUDInfoLibrary(gi, c, u);
          break;
      }  
    }
  }
}

void MOBA_HUDInfoMultipleSelection(AW_game_instance_t *gi, AW_client_ptr c, int selected_units_count) {
  AW_client_t *cl     = &client(c);
  AW_moba_player_ptr mp = player(GI_GetPlayerPtr(gi, cl->player_id)).user_data;
  assert(mp != AW_null);
  AW_moba_player_t *mpl = &moba_player(mp);
  int idx[]           = {0, (27*2/3), 27};
  TCOD_color_t cols[] = {TCOD_red, TCOD_orange, TCOD_green};
  TCOD_color_t map[27+1];
  TCOD_color_gen_map(map, 3, cols, idx);
  /* render */
  AW_unit_ptr u = cl->selected_units_head;
  AW_PrintString("Selection: ", START_HUD_INFO_X, HUD_START_Y);
  moba_gi.central_hud_btns.btn_count = MIN(HUD_CENTRAL_INFO_COUNT, selected_units_count);
  /* pos_x is the center of the string */
  moba_gi.central_hud_btns.pos_x     = START_HUD_INFO_X;
  moba_gi.central_hud_btns.pos_y     = START_HUD_INFO_Y;
  int i = 0, j = 0, g = -1;
  while(u != AW_null
     && i < HUD_CENTRAL_INFO_COUNT) {
    AW_unit_t *un = &unit(u);
    short r = un->hp * 27 / MAX_HP(un);
    moba_gi.central_hud_btns.btns[i].color             = map[r];
    moba_gi.central_hud_btns.btns[i].s                 = 
      str((un->unit_type == cl->selected_sub_group ? "}" : " ") + MOBA_GetUnitName(gi, un->unit_type) +
      (un->unit_type == AW_unit_type_C && un->build_order_head[cl->player_id] != AW_null ? 
        str(" " + i2a(build_order(un->build_order_head[cl->player_id]).progress * 100 / unit_dic[build_order(un->build_order_head[cl->player_id]).unit_type].build_time) +
        "%%") : ""));
    moba_gi.central_hud_btns.btns[i].highlight         = true;
    moba_gi.central_hud_btns.btns[i].entry             = false;
    moba_gi.central_hud_btns.btns[i].shortcut          = -1;
    moba_gi.central_hud_btns.btns[i].shortcut_special  = (TCOD_keycode_t)-1;
    i++;
    u = un->snext[c];
  }
  BTN_Render(gi, &moba_gi.central_hud_btns, false, 1); 
  DO_TIMES(moba_gi.central_hud_btns.btn_count) {
    if(BTN_IsClicked(gi, &moba_gi.central_hud_btns.btns[f]))
      g = j;
    j++;
  }
  if(selected_units_count > HUD_CENTRAL_INFO_COUNT) {
    moba_gi.central_hud_btns.btn_count = MIN(HUD_CENTRAL_INFO_COUNT, selected_units_count-HUD_CENTRAL_INFO_COUNT);
    /* pos_x is the center of the string */
    moba_gi.central_hud_btns.pos_x     = START_HUD_INFO_X2;
    moba_gi.central_hud_btns.pos_y     = START_HUD_INFO_Y;
    i = 0;
    while(u != AW_null
       && i < HUD_CENTRAL_INFO_COUNT-1) {
      AW_unit_t *un = &unit(u);
      short r = un->hp * 27 / MAX_HP(un);
      moba_gi.central_hud_btns.btns[i].color             = map[r];
      moba_gi.central_hud_btns.btns[i].s                 = 
        str((un->unit_type == cl->selected_sub_group ? "}" : " ") + MOBA_GetUnitName(gi, un->unit_type));
      moba_gi.central_hud_btns.btns[i].highlight         = true;
      moba_gi.central_hud_btns.btns[i].entry             = false;
      moba_gi.central_hud_btns.btns[i].shortcut          = -1;
      moba_gi.central_hud_btns.btns[i].shortcut_special  = (TCOD_keycode_t)-1;
      i++;
      u = un->snext[c];
    }
    if(selected_units_count >= MAX_HUD_CENTRAL_INFO) {
      moba_gi.central_hud_btns.btns[i].color             = TCOD_white;
      moba_gi.central_hud_btns.btns[i].s                 = " ...";
      moba_gi.central_hud_btns.btns[i].highlight         = false;
      moba_gi.central_hud_btns.btns[i].entry             = false;
      moba_gi.central_hud_btns.btns[i].shortcut          = -1;
      moba_gi.central_hud_btns.btns[i].shortcut_special  = (TCOD_keycode_t)-1;
    }
    BTN_Render(gi, &moba_gi.central_hud_btns, false, 1);
      DO_TIMES(moba_gi.central_hud_btns.btn_count) {
      if(BTN_IsClicked(gi, &moba_gi.central_hud_btns.btns[f]))
        g = j;
      j++;
    }
  }
  /* clicks */
  if(g != -1) {
    j = 0;
    AW_unit_ptr u = cl->selected_units_head;
    while(j < g) {
      j++;
      u = unit(u).snext[c];
      assert(u != AW_null);
    }
    if(GI_IsKeyPressed2(gi, TCODK_SHIFT)) {
      CL_SubToSelection(gi, c, u);
    } else {
      CL_FreeUnitSelection(gi, c);
      CL_AddToSelection(gi, c, u);
    }
  }
}

void MOBA_HUDInfoCastle(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u) {
  AW_client_t *cl     = &client(c);
  AW_moba_player_ptr mp = player(GI_GetPlayerPtr(gi, cl->player_id)).user_data;
  assert(mp != AW_null);
  AW_moba_player_t *mpl = &moba_player(mp);
  AW_unit_t *un = &unit(u);
  int idx[]           = {0, (27*2/3), 27};
  TCOD_color_t cols[] = {TCOD_red, TCOD_orange, TCOD_green};
  TCOD_color_t map[27+1];
  TCOD_color_gen_map(map, 3, cols, idx);
  if(un->build_order_head[cl->player_id] != AW_null) {
    int i, j = 0, g = -1;
    moba_gi.central_hud_btns.btn_count = i = 0;
    moba_gi.central_hud_btns.pos_x     = START_HUD_INFO_X;
    moba_gi.central_hud_btns.pos_y     = START_HUD_INFO_Y;
    /* first build */
    AW_build_order_ptr b = un->build_order_head[cl->player_id];
    AW_build_order_t *bo = &build_order(b);
    int completion = bo->progress * 100 / unit_dic[bo->unit_type].build_time;
    moba_gi.central_hud_btns.btns[i].color             = map[completion*27/100];
    moba_gi.central_hud_btns.btns[i].s                 = 
      str(MOBA_GetUnitName(gi, bo->unit_type) + " " + i2a(completion) + "%%");
    moba_gi.central_hud_btns.btns[i].highlight         = true;
    moba_gi.central_hud_btns.btns[i].entry             = false;
    moba_gi.central_hud_btns.btns[i].shortcut          = -1;
    moba_gi.central_hud_btns.btns[i].shortcut_special  = (TCOD_keycode_t)-1;
    i++;
    moba_gi.central_hud_btns.btn_count++;
    b = bo->next;
    while(b != AW_null
       && i < HUD_CENTRAL_INFO_COUNT) {
      bo = &build_order(b);
      moba_gi.central_hud_btns.btns[i].color             = TCOD_white;
      moba_gi.central_hud_btns.btns[i].s                 = MOBA_GetUnitName(gi, bo->unit_type);
      moba_gi.central_hud_btns.btns[i].highlight         = true;
      moba_gi.central_hud_btns.btns[i].entry             = false;
      moba_gi.central_hud_btns.btns[i].shortcut          = -1;
      moba_gi.central_hud_btns.btns[i].shortcut_special  = (TCOD_keycode_t)-1;
      i++;
      moba_gi.central_hud_btns.btn_count++;
      b = bo->next;
    }
    BTN_Render(gi, &moba_gi.central_hud_btns, false, 1); 
    DO_TIMES(moba_gi.central_hud_btns.btn_count) {
      if(BTN_IsClicked(gi, &moba_gi.central_hud_btns.btns[f]))
        g = j;
      j++;
    }
    if(b != AW_null) {
      /* pos_x is the center of the string */
      moba_gi.central_hud_btns.pos_x     = START_HUD_INFO_X2;
      moba_gi.central_hud_btns.pos_y     = START_HUD_INFO_Y;
      moba_gi.central_hud_btns.btn_count = i = 0;
      while(b != AW_null
         && i < HUD_CENTRAL_INFO_COUNT) {
        bo = &build_order(b);
        moba_gi.central_hud_btns.btns[i].color             = TCOD_white;
        moba_gi.central_hud_btns.btns[i].s                 = MOBA_GetUnitName(gi, bo->unit_type);
        moba_gi.central_hud_btns.btns[i].highlight         = true;
        moba_gi.central_hud_btns.btns[i].entry             = false;
        moba_gi.central_hud_btns.btns[i].shortcut          = -1;
        moba_gi.central_hud_btns.btns[i].shortcut_special  = (TCOD_keycode_t)-1;
        i++;
        moba_gi.central_hud_btns.btn_count++;
        b = bo->next;
      }
      BTN_Render(gi, &moba_gi.central_hud_btns, false, 1); 
      DO_TIMES(moba_gi.central_hud_btns.btn_count) {
        if(BTN_IsClicked(gi, &moba_gi.central_hud_btns.btns[f]))
          g = j;
        j++;
      }
    }
    /* clicks */
    if(g != -1) {
      j = 0;
      b = un->build_order_head[cl->player_id];
      while(j < g) {
        j++;
        b = build_order(b).next;
        assert(b != AW_null);
      }
      bo = &build_order(b);
      CL_CmdCancelBuildOrder(gi, c, un->cmd_id, un->player_id, bo->unit_player_id, bo->unit_id);
    }
  }
  if(cl->window_opened
  && cl->hud_state == 1) {
    AW_PrintString("Enroll Units:", (cl->window_start_x+cl->window_end_x)/2-str("Enroll Units:").size()/2, cl->window_start_y+1);
    int i;
    moba_gi.central_hud_btns.btn_count = i = 0;
    moba_gi.central_hud_btns.pos_x     = (cl->window_start_x+cl->window_end_x)/2;
    moba_gi.central_hud_btns.pos_y     = cl->window_start_y+3;
    DISPLAY_CASTLE_UNIT(chaotic_evil_level1, "(G)oblin", AW_unit_type_g, 'G')
    DISPLAY_CASTLE_UNIT(chaotic_evil_level1, "(O)gre", AW_unit_type_O, 'O')
    BTN_Render(gi, &moba_gi.central_hud_btns, true, 1); 
    if(BTN_IsClicked(gi, &gi->close_btn)
    || GI_IsKeyReleased(gi, TCODK_ESCAPE)) {
      cl->window_opened = false;
      GI_StopPropagation(gi, TCODK_ESCAPE);
    }
    moba_gi.window_switch.color             = TCOD_white;
    moba_gi.window_switch.s                 = "Select Library (TAB)";
    moba_gi.window_switch.highlight         = true;
    moba_gi.window_switch.entry             = false;
    moba_gi.window_switch.shortcut          = -1;
    moba_gi.window_switch.shortcut_special  = TCODK_TAB;
    moba_gi.window_switch.pos_x             = (cl->window_start_x+cl->window_end_x)/2;
    moba_gi.window_switch.pos_y             = cl->window_end_y;
    BTN_CenterX(gi, &moba_gi.window_switch);
    BTN_Render(gi, &moba_gi.window_switch);
    if(BTN_IsClicked(gi, &moba_gi.window_switch)) {
      CL_FreeUnitSelection(gi, c);
      CL_AddToSelection(gi, c, MOBA_GetLibrary(gi, c));
      cl->window_opened = true;
      cl->hud_state = 1;
    }
  }
  /* open window? 
     must be after so the player don't click on a skill at the button-up */
  if(cl->hud_state == 0)
    MOBA_OpenWindow(gi, c, 45, 32);
}

void MOBA_HUDInfoGraveyard(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u) {
  AW_client_t *cl     = &client(c);
  AW_moba_player_ptr mp = player(GI_GetPlayerPtr(gi, cl->player_id)).user_data;
  assert(mp != AW_null);
  AW_moba_player_t *mpl = &moba_player(mp);
  AW_unit_t *un = &unit(u);
  int idx[]           = {0, (27*2/3), 27};
  TCOD_color_t cols[] = {TCOD_red, TCOD_orange, TCOD_green};
  TCOD_color_t map[27+1];
  TCOD_color_gen_map(map, 3, cols, idx);
  AW_build_order_ptr b = un->build_order_head[cl->player_id];
  int f = 0;
  while(b != AW_null
     && f < HUD_CENTRAL_INFO_COUNT) {
    AW_build_order_t *bo = &build_order(b);
    int completion = bo->progress * 100 / unit_dic[bo->unit_type].build_time;
    TCOD_console_set_default_foreground(con, map[completion*27/100]);
    AW_PrintString(
      str(MOBA_GetUnitName(gi, bo->unit_type) + " " + i2a(completion) + "%%"), 
      START_HUD_INFO_X, 
      HUD_START_Y+f+2);
    f++;
    b = bo->next;
  }
  if(b != AW_null) {
    f = 0;
    while(b != AW_null
       && f < HUD_CENTRAL_INFO_COUNT-1) {
      AW_build_order_t *bo = &build_order(b);
      int completion = bo->progress * 100 / unit_dic[bo->unit_type].build_time;
      TCOD_console_set_default_foreground(con, map[completion*27/100]);
      AW_PrintString(
        str(MOBA_GetUnitName(gi, bo->unit_type) + " " + i2a(completion) + "%%"), 
        START_HUD_INFO_X2, 
        HUD_START_Y+f+2);
      f++;
      b = bo->next;
    }
    if(b != AW_null) {
      TCOD_console_set_default_foreground(con, TCOD_white);
      AW_PrintString("...", 45, HUD_START_Y+f+2);
    }
  }
}

void MOBA_HUDInfoLibrary(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u) {
  AW_client_t *cl       = &client(c);
  /* render window */
  if(cl->window_opened
  && cl->hud_state == 1) {
    AW_moba_player_ptr mp = player(GI_GetPlayerPtr(gi, cl->player_id)).user_data;
    assert(mp != AW_null);
    AW_moba_player_t *mpl = &moba_player(mp);
    AW_unit_t *un         = &unit(u);
    int idx[]             = {0, (27*2/3), 27};
    TCOD_color_t cols[]   = {TCOD_red, TCOD_orange, TCOD_green};
    TCOD_color_t map[27+1];
    TCOD_color_gen_map(map, 3, cols, idx);
    AW_PrintString("Unlock abilities:", (cl->window_start_x+cl->window_end_x)/2-str("Unlock abilities:").size()/2, cl->window_start_y+1);
    int i = 0;
    moba_gi.central_hud_btns.btn_count = i = 0;
    moba_gi.central_hud_btns.pos_x     = (cl->window_start_x+cl->window_end_x)/2;
    moba_gi.central_hud_btns.pos_y     = cl->window_start_y+3;
    DISPLAY_LIBRARY_SKILL(lawful_good_level1, "(G) Lawful Good Units Level 1", LAWFUL_GOOD_LEVEL1_XP, 'G', "Lawful Good Units Level 1 unlocks new types of units.")
    DISPLAY_LIBRARY_SKILL(chaotic_good_level1, "(H) Chaotic Good Units Level 1", CHAOTIC_GOOD_LEVEL1_XP, 'H', "Chaotic Good Units Level 1 unlocks new types of units.")
    DISPLAY_LIBRARY_SKILL(lawful_neutral_level1, "(N) Lawful Neutral Units Level 1", LAWFUL_NEUTRAL_LEVEL1_XP, 'N', "Lawful Neutral Units Level 1 unlocks new types of units.")
    DISPLAY_LIBRARY_SKILL(chaotic_neutral_level1, "(M) Chaotic Neutral Units Level 1", CHAOTIC_NEUTRAL_LEVEL1_XP, 'M', "Chaotic Neutral Units Level 1 unlocks new types of units.")
    DISPLAY_LIBRARY_SKILL(lawful_evil_level1, "(E) Lawful Evil Units Level 1", LAWFUL_EVIL_LEVEL1_XP, 'E', "Lawful Evil Units Level 1 unlocks new types of units.")
    DISPLAY_LIBRARY_SKILL(chaotic_evil_level1, "(R) Chaotic Evil Units Level 1", CHAOTIC_EVIL_LEVEL1_XP, 'R', "Chaotic Evil Units Level 1 unlocks new types of units.")
    BTN_Render(gi, &moba_gi.central_hud_btns, true, 1); 
    if(BTN_IsClicked(gi, &gi->close_btn)
    || GI_IsKeyReleased(gi, TCODK_ESCAPE)) {
      cl->window_opened = false;
      GI_StopPropagation(gi, TCODK_ESCAPE);
    }
    moba_gi.window_switch.color             = TCOD_white;
    moba_gi.window_switch.s                 = "Select Castle (TAB)";
    moba_gi.window_switch.highlight         = true;
    moba_gi.window_switch.entry             = false;
    moba_gi.window_switch.shortcut          = -1;
    moba_gi.window_switch.shortcut_special  = TCODK_TAB;
    moba_gi.window_switch.pos_x             = (cl->window_start_x+cl->window_end_x)/2;
    moba_gi.window_switch.pos_y             = cl->window_end_y;
    BTN_CenterX(gi, &moba_gi.window_switch);
    BTN_Render(gi, &moba_gi.window_switch);
    if(BTN_IsClicked(gi, &moba_gi.window_switch)) {
      CL_FreeUnitSelection(gi, c);
      CL_AddToSelection(gi, c, MOBA_GetCastle(gi, c));
      cl->window_opened = true;
      cl->hud_state = 1;
    }
  }
  /* open window? 
     must be after so the player don't click on a skill at the button-up */
  if(cl->hud_state == 0)
    MOBA_OpenWindow(gi, c, 60, 32);
}

str MOBA_GetUnitName(AW_game_instance_t *gi, AW_unit_type_t unit_type) {
  switch(unit_type) {
    case AW_unit_type_g:
      return "Goblin   ";
    case AW_unit_type_O:
      return "Orc      ";
    case AW_unit_type_m:
      return "Mob      ";
    case AW_unit_type_C:
      return "Castle  ";
    case AW_unit_type_G:
      return "Graveyard";
    case AW_unit_type_T:
      return "Tower    ";
    case AW_unit_type_L:
      return "Library  ";
  }
  assert(false);
  return "";
}

void MOBA_OnSpawnUnit(AW_game_instance_t *gi, AW_unit_ptr u) {
  AW_unit_t *un = &unit(u);
  if(TEAM_ID(un->player_id) == game_desc.local_team_id)
    switch(un->unit_type) {
      case AW_unit_type_C: {
          AW_castle_ptr C = CAS_New();
          assert(C != AW_null);
          castle(C).u = u;
          castle(C).player_id = un->player_id;
          un->user_data = (AW_ptr_t)C;
        } break;
      case AW_unit_type_G: {
          AW_graveyard_ptr G = GRA_New();
          assert(G != AW_null);
          graveyard(G).u = u;
          graveyard(G).player_id = un->player_id;
          un->user_data = (AW_ptr_t)G;
        } break;
      case AW_unit_type_L: {
          AW_library_ptr L = LIB_New();
          assert(L != AW_null);
          library(L).u = u;
          library(L).player_id = un->player_id;
          un->user_data = (AW_ptr_t)L;
        } break;
    }
}

void MOBA_OnCancelBuild(AW_game_instance_t *gi, AW_build_order_ptr b) {
  AW_build_order_t *bo = &build_order(b);
  AW_moba_player_ptr mp = player(GI_GetPlayerPtr(gi, bo->unit_player_id)).user_data;
  assert(mp != AW_null);
  AW_moba_player_t *mpl = &moba_player(mp);
  mpl->gold += unit_dic[bo->unit_type].gold_price;
}

void MOBA_GenericCmd(AW_game_instance_t *gi, AW_id_t id, AW_id_t player_id, AW_id_t cmd_mask, AW_unit_type_t unit_type, short target_cx, short target_cy) {
  switch(id) {
    case AW_moba_generic_learn_skill: {
      AW_moba_player_ptr mp = player(GI_GetPlayerPtr(gi, player_id)).user_data;
      assert(mp != AW_null);
      AW_moba_player_t *mpl = &moba_player(mp);
      switch(cmd_mask) {
        GENERIC_LIBRARY_SKILL(lawful_good_level1, LAWFUL_GOOD_LEVEL1_XP)
        GENERIC_LIBRARY_SKILL(chaotic_good_level1, CHAOTIC_GOOD_LEVEL1_XP)
        GENERIC_LIBRARY_SKILL(lawful_neutral_level1, LAWFUL_NEUTRAL_LEVEL1_XP)
        GENERIC_LIBRARY_SKILL(chaotic_neutral_level1, CHAOTIC_NEUTRAL_LEVEL1_XP)
        GENERIC_LIBRARY_SKILL(lawful_evil_level1, LAWFUL_EVIL_LEVEL1_XP)
        GENERIC_LIBRARY_SKILL(chaotic_evil_level1, CHAOTIC_EVIL_LEVEL1_XP)
      }
    } break;
  }
}

AW_unit_ptr MOBA_GetCastle(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_castle_ptr ca = moba_gi.castle_head;
  while(ca != AW_null) {
    if(TEAM_ID(castle(ca).player_id) == TEAM_ID(cl->player_id))
      return castle(ca).u;
    ca = castle(ca).next;
  }
  return AW_null;
}

AW_unit_ptr MOBA_GetLibrary(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_library_ptr ca = moba_gi.library_head;
  while(ca != AW_null) {
    if(TEAM_ID(library(ca).player_id) == TEAM_ID(cl->player_id))
      return library(ca).u;
    ca = library(ca).next;
  }
  return AW_null;
}

void MOBA_OpenWindow(AW_game_instance_t *gi, AW_client_ptr c, int size_x, int size_y) {
  AW_client_t *cl       = &client(c);
  cl->window_opened     = true;
  cl->hud_state         = 1;
  cl->window_size_x     = size_x;
  cl->window_size_y     = size_y;
  cl->window_start_x    = (CON_RES_X-cl->window_size_x)/2;
  cl->window_start_y    = ((HUD_START_Y-cl->window_size_y)/2);
  cl->window_end_x      = (cl->window_start_x+cl->window_size_x);
  cl->window_end_y      = (cl->window_start_y+cl->window_size_y); 
  CL_UpdateWindowCloseButton(gi, c);
  CL_RenderWindow(gi, c);
}

/*
 *
 */

AW_moba_game_instance_t moba_gi;

// units.
AW_ascii_t              little_g[1]   = {'g'},
                        big_O[2*2]    = {0x00 + 0xE0 + 0x8, 0x01 + 0xE0 + 0x8,
                                         0x10 + 0xE0 + 0x8, 0x11 + 0xE0 + 0x8},
                        little_m[2*2] = {0x00 + 0xE0 + 0xB, 0x01 + 0xE0 + 0xB,
                                         0x10 + 0xE0 + 0xB, 0x11 + 0xE0 + 0xB},
                        big_C[8*8]    = {0x00 + 0x80, 0x01 + 0x80, 0x02 + 0x80, 0x03 + 0x80, 0x04 + 0x80, 0x05 + 0x80, 0x06 + 0x80, 0x07 + 0x80,
                                         0x10 + 0x80, 0x11 + 0x80, 0x12 + 0x80, 0x13 + 0x80, 0x14 + 0x80, 0x15 + 0x80, 0x16 + 0x80, 0x17 + 0x80,
                                         0x20 + 0x80, 0x21 + 0x80, 0x22 + 0x80, 0x23 + 0x80, 0x24 + 0x80, 0x25 + 0x80, 0x26 + 0x80, 0x27 + 0x80,
                                         0x30 + 0x80, 0x31 + 0x80, 0x32 + 0x80, 0x33 + 0x80, 0x34 + 0x80, 0x35 + 0x80, 0x36 + 0x80, 0x37 + 0x80,
                                         0x40 + 0x80, 0x41 + 0x80, 0x42 + 0x80, 0x43 + 0x80, 0x44 + 0x80, 0x45 + 0x80, 0x46 + 0x80, 0x47 + 0x80,
                                         0x50 + 0x80, 0x51 + 0x80, 0x52 + 0x80, 0x53 + 0x80, 0x54 + 0x80, 0x55 + 0x80, 0x56 + 0x80, 0x57 + 0x80,
                                         0x60 + 0x80, 0x61 + 0x80, 0x62 + 0x80, 0x63 + 0x80, 0x64 + 0x80, 0x65 + 0x80, 0x66 + 0x80, 0x67 + 0x80,
                                         0x70 + 0x80, 0x71 + 0x80, 0x72 + 0x80, 0x73 + 0x80, 0x74 + 0x80, 0x75 + 0x80, 0x76 + 0x80, 0x77 + 0x80},
                        big_T[5*5]   = {0x00 + 0x90 + 0x8, 0x01 + 0x90 + 0x8, 0x02 + 0x90 + 0x8, 0x03 + 0x90 + 0x8, 0x04 + 0x90 + 0x8,
                                        0x10 + 0x90 + 0x8, 0x11 + 0x90 + 0x8, 0x12 + 0x90 + 0x8, 0x13 + 0x90 + 0x8, 0x14 + 0x90 + 0x8,
                                        0x20 + 0x90 + 0x8, 0x21 + 0x90 + 0x8, 0x22 + 0x90 + 0x8, 0x23 + 0x90 + 0x8, 0x24 + 0x90 + 0x8,
                                        0x30 + 0x90 + 0x8, 0x31 + 0x90 + 0x8, 0x32 + 0x90 + 0x8, 0x33 + 0x90 + 0x8, 0x34 + 0x90 + 0x8,
                                        0x40 + 0x90 + 0x8, 0x41 + 0x90 + 0x8, 0x42 + 0x90 + 0x8, 0x43 + 0x90 + 0x8, 0x44 + 0x90 + 0x8},
                        big_G[4*4]   = {0x00 + 0x100, 0x01 + 0x100, 0x02 + 0x100, 0x03 + 0x100,
                                        0x10 + 0x100, 0x11 + 0x100, 0x12 + 0x100, 0x13 + 0x100,
                                        0x20 + 0x100, 0x21 + 0x100, 0x22 + 0x100, 0x23 + 0x100,
                                        0x30 + 0x100, 0x31 + 0x100, 0x32 + 0x100, 0x33 + 0x100},
                        big_L[6*6]   = {0x00 + 0x104, 0x01 + 0x104, 0x02 + 0x104, 0x03 + 0x104, 0x04 + 0x104, 0x05 + 0x104,
                                        0x10 + 0x104, 0x11 + 0x104, 0x12 + 0x104, 0x13 + 0x104, 0x14 + 0x104, 0x15 + 0x104,
                                        0x20 + 0x104, 0x21 + 0x104, 0x22 + 0x104, 0x23 + 0x104, 0x24 + 0x104, 0x25 + 0x104,
                                        0x30 + 0x104, 0x31 + 0x104, 0x32 + 0x104, 0x33 + 0x104, 0x34 + 0x104, 0x35 + 0x104,
                                        0x40 + 0x104, 0x41 + 0x104, 0x42 + 0x104, 0x43 + 0x104, 0x44 + 0x104, 0x45 + 0x104,
                                        0x50 + 0x104, 0x51 + 0x104, 0x52 + 0x104, 0x53 + 0x104, 0x54 + 0x104, 0x55 + 0x104}; 
AW_unit_class_t         unit_dic[MAX_UNIT_TYPE] =
 {{50,    10, 10000, 10, 1,  3,    50,    0, 1, 40,   8*8,   5*5,  true, little_g},
  {100,   10, 10000, 10, 1,  3,    50,  100, 2, 40,   8*8,   5*5,  true, big_O},
  {25,    10, 10000, 8, 1,   3,    50,    0, 2, 40,   8*8,   3*3,  true, little_m},
  {800,   10, 10000, 0, 2,  10,  1500,    0, 5, 60, 25*25, 25*25, false, big_T},
  {1500,  10, 10000, 0, 1,   0,  3500,    0, 6, 80,   8*8,    -1, false, big_L},
  {1500,  10, 10000, 0, 1,   0,  3500,    0, 4, 80,   8*8,    -1, false, big_G},
  {5000,  10, 10000, 0, 1,   0,  2250,    0, 8, 80,   8*8,    -1, false, big_C}};
  

/*
 * end of file
 */  