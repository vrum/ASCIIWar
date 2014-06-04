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

// multi
gi->multi_btns.btn_count = 4;
gi->multi_btns.pos_x = CON_RES_X/2;
gi->multi_btns.btns[0].color = TCOD_white;
gi->multi_btns.btns[0].s = "(J)oin";
gi->multi_btns.btns[0].highlight = true;
gi->multi_btns.btns[0].entry = false;
gi->multi_btns.btns[0].shortcut = 'J';
gi->multi_btns.btns[0].shortcut_special = (TCOD_keycode_t)-1;
gi->multi_btns.btns[1].color = TCOD_white;
gi->multi_btns.btns[1].s = "(C)reate";
gi->multi_btns.btns[1].highlight = true;
gi->multi_btns.btns[1].entry = false;
gi->multi_btns.btns[1].shortcut = 'C';
gi->multi_btns.btns[1].shortcut_special = (TCOD_keycode_t)-1;
gi->multi_btns.btns[2].color = TCOD_white;
gi->multi_btns.btns[2].s = "";
gi->multi_btns.btns[2].highlight = false;
gi->multi_btns.btns[2].entry = false;
gi->multi_btns.btns[2].shortcut = -1;
gi->multi_btns.btns[2].shortcut_special = (TCOD_keycode_t)-1;
gi->multi_btns.btns[3].color = TCOD_white;
gi->multi_btns.btns[3].s = "(Q)uit";
gi->multi_btns.btns[3].highlight = true;
gi->multi_btns.btns[3].entry = false;
gi->multi_btns.btns[3].shortcut = 'Q';
gi->multi_btns.btns[3].shortcut_special = TCODK_ESCAPE;
BTN_CenterY(gi, &gi->multi_btns);

// create
gi->create_game_btns.btn_count = 2;
gi->create_game_btns.pos_x = CON_RES_X/2;
gi->create_game_btns.btns[0].color = TCOD_white;
gi->create_game_btns.btns[0].s = "Game Name:";
gi->create_game_btns.btns[0].highlight = false;
gi->create_game_btns.btns[0].entry = false;
gi->create_game_btns.btns[0].shortcut = -2;
gi->create_game_btns.btns[0].shortcut_special = (TCOD_keycode_t)-1;
gi->create_game_btns.btns[1].color = TCOD_white;
gi->create_game_btns.btns[1].s = "";
gi->create_game_btns.btns[1].highlight = false;
gi->create_game_btns.btns[1].entry = true;
gi->create_game_btns.btns[1].shortcut = 1;
gi->create_game_btns.btns[1].shortcut_special = TCODK_ENTER;
BTN_CenterY(gi, &gi->create_game_btns);

// refresh
gi->refresh_btn.color = TCOD_white;
gi->refresh_btn.s = "(R)efresh";
gi->refresh_btn.highlight = true;
gi->refresh_btn.entry = false;
gi->refresh_btn.shortcut = 'R';
gi->refresh_btn.shortcut_special = (TCOD_keycode_t)-1;
gi->refresh_btn.pos_x = CON_RES_X-15;
gi->refresh_btn.pos_y = CON_RES_Y-5;

// ready
gi->ready_btn.color = TCOD_white;
gi->ready_btn.s = "Ready...";
gi->ready_btn.highlight = false;
gi->ready_btn.entry = true;
gi->ready_btn.shortcut = -2;
gi->ready_btn.shortcut_special = (TCOD_keycode_t)-1;
BTN_CenterX(gi, &gi->ready_btn);
BTN_CenterY(gi, &gi->ready_btn);

// waiting for players
gi->waiting_for_players_btn.color = TCOD_white;
gi->waiting_for_players_btn.s = "Waiting for players...";
gi->waiting_for_players_btn.highlight = false;
gi->waiting_for_players_btn.entry = true;
gi->waiting_for_players_btn.shortcut = -2;
gi->waiting_for_players_btn.shortcut_special = (TCOD_keycode_t)-1;
BTN_CenterX(gi, &gi->waiting_for_players_btn);
BTN_CenterY(gi, &gi->waiting_for_players_btn);

// flash
gi->flash_btn.color = TCOD_red;
gi->flash_btn.s = "";
gi->flash_btn.highlight = false;
gi->flash_btn.entry = false;
gi->flash_btn.shortcut = -2;
gi->flash_btn.shortcut_special = (TCOD_keycode_t)-1;
BTN_CenterX(gi, &gi->flash_btn);
gi->flash_btn.pos_y = CON_RES_Y-3;