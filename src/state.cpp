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

if(GI_InState(gi, AW_state_game)) {
  /* debug */
  #if !RELEASE
  GI_CheckListIntegrity(gi);
  #endif
  /* listen master server */
  LISTEN_MASTER_SERVER
  /* ping */
  GI_Ping(gi, &e);
  /* check connection */
  GI_CheckConnectionToMasterServer(gi, &e, AW_state_game);
  /* time. */
  GI_UpdateTime(gi);
  /* the players before the cmds/inputs */
  GI_UpdatePlayers(gi);
  /* the balls. */
  GI_UpdateMiscs(gi);
  /* clients if any. */
  GI_UpdateClients(gi);
  /* remote stores. */
  GI_UpdateRemoteStores(gi);
  /* update turn */
  GI_UpdateTurn(gi);
} else {
  /* main menu */
  if(GI_InState(gi, AW_state_main_menu)) {
    if(gi->host) {
      LISTEN_MASTER_SERVER
      GI_CheckConnectionToMasterServer(gi, &e, AW_state_main_menu);
    }
    if(BTN_IsClicked(gi, &gi->multi_btns.btns[3])) /* quit */
      AW_Quit();
  }
  /* join */
  if(GI_InState(gi, AW_state_join)) {
    if(GI_IsKeyReleased(gi, TCODK_ESCAPE)) {
      AW_Quit();
    } else {
      LISTEN_MASTER_SERVER
      GI_CheckConnectionToMasterServer(gi, &e, AW_state_join);
      if(GI_JoinGameOnMasterServer(gi)) {
        GI_RemoveState(gi, AW_state_join);
        GI_AddState(gi, AW_state_waiting_players);
      }
    }
  }
  /* waiting for players */
  if(GI_InState(gi, AW_state_waiting_players)) {
    if(GI_IsKeyReleased(gi, TCODK_ESCAPE)) {
      AW_Quit();
    } else {
      BTN_Render(gi, &gi->waiting_for_players_btn); 
      if(GI_GameReady(gi)) {
        GI_InitGame(gi, &game_desc, MOBA_InitGame);
        GI_RemoveState(gi, AW_state_waiting_players);
        GI_AddState(gi, AW_state_ready);
      }
    }
  }
  /* ready */
  if(GI_InState(gi, AW_state_ready)) {
    if(GI_IsKeyReleased(gi, TCODK_ESCAPE)) {
      AW_Quit();
    } else {
      BTN_Render(gi, &gi->ready_btn);
      if(GI_PlayersConnected(gi)) {
        GI_StartGame(gi, MOBA_StartGame);
        GI_RemoveState(gi, AW_state_ready);
        GI_AddState(gi, AW_state_game);
      }
    }
  }
}