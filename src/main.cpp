/* 
 * File:   main.cpp
 * Author: Fr3dBrach
 *
 */

#include "asciiwar.h"

/*
 * 
 */
int main(int _argc, char **_argv) {
	run = true;
  argc = _argc;
  argv = _argv;
  InitializeCriticalSectionAndSpinCount(&log_crit, 4000);
  AW_SetBinDir();
  fclose(fopen((bin_dir + "log.txt").c_str(), "w+"));
  log((str("Binary directory: ") + bin_dir + "\n").c_str());
  atexit(exit);
  
  TCOD_sys_get_current_resolution(&resx, &resy);
  #if FULLSCREEN
	  CON_RES_X = floor(resx/CHAR_SIZE);
	  CON_RES_Y = floor(resy/CHAR_SIZE);
	#else
	  CON_RES_X = 70;
	  CON_RES_Y = 40;
	#endif
	TCOD_console_set_custom_font("./data/terminal.png", TCOD_FONT_LAYOUT_ASCII_INROW, 16, 32);
  TCOD_console_init_root(CON_RES_X, CON_RES_Y, "ASCIIWar", false, TCOD_RENDERER_SDL);
  con = TCOD_console_new(CON_RES_X, CON_RES_Y);
  #if FULLSCREEN
    TCOD_sys_force_fullscreen_resolution(resx, resy);   
    TCOD_console_set_fullscreen(true);
  #endif
  TCOD_sys_get_current_resolution(&resx, &resy);
  TCOD_mouse_show_cursor(true);
  TCOD_sys_set_fps(1000/NORMAL_TIME_STEP);
  #if FULLSCREEN
	  int offx, offy;
	  TCOD_sys_get_fullscreen_offsets(&offx, &offy);
	  MINIMAP_START_X = offx+CHAR_SIZE*MINIMAP_CON_START_X;
	  LAGOMETER_START_X = resx-offx-LAGOMETER_SIZE;
 	#endif
	TCOD_console_set_keyboard_repeat(1, 0);
  trace("ASCIIWar has started.");

  trace("Parsing configs...");
	TCOD_parser_t game_parser = TCOD_parser_new();
	TCOD_parser_new_struct(game_parser, (char*)"game");
	TCOD_parser_run(game_parser,"game.cfg", &game_parser_listener);
	trace("Configs parsing done.");
	trace("Master server at: " + game_desc.master_server);

  client_gi.name = "asciiwar";

  #if RELEASE
  	TCOD_console_credits();
  #endif

  assert(_argc >= 4);
  game_desc.game_name             = (char*)argv[1];
  game_desc.player_count_per_team = atoi((char*)argv[2]);
  game_desc.team_count            = atoi((char*)argv[3]);

  // client game instance
	GI_Init(&client_gi, argc, argv);
  GI_ConnectToMasterServer(&client_gi);
	GI_AddState(&client_gi, AW_state_join);
	while(run && !TCOD_console_is_window_closed())
		GI_Update(&client_gi);

  trace("Deleting game instance...");
  GI_Close(&client_gi);

  trace(str("Execution time: " 
  	+ f2a(TCOD_sys_elapsed_seconds()/60) 
  	+ " minutes."));

	return 0;
}

