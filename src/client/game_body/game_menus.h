#pragma once

#ifndef GAME_MENUS_H
#define GAME_MENUS_H

#include <stdbool.h>

#include "client/gui/menu.h"
#include "client/gui/char_font.h"

#include "general/keyvalue.h"

enum blocks_menus {
	MAIN_MENU = 0,
	OPTIONS_MENU = 1,
	JOIN_GAME_MENU = 3,
};

struct game_menus {
	struct {
		struct gui_character* this_is_not_minecraft_text;

		struct gui_character* join_game_text;
		bool join_game_button_state;
		bool join_game_button_enabled;

		struct gui_character* options_text;
		bool options_button_state;
		bool options_button_enabled;

		struct gui_character* quit_game_text;
		bool quit_game_button_state;
		bool quit_game_button_enabled;

		struct menu_scene menu;
	} main_menu;

	struct {
		struct gui_character* options_text;

		float fov_slider_state;
		struct gui_character* fov_text;

		float render_distance_slider_state;
		struct gui_character* render_distance_text;

		bool gui_scale_button_state;
		bool gui_scale_button_enabled;
		struct gui_character* gui_scale_text;

		bool done_button_state;
		bool done_button_enabled;
		struct gui_character* done_text;

		struct menu_scene menu;
	} options_menu;

	struct {
		struct gui_character* join_a_multiplayer_game_text;

		struct gui_character* ip_address_text;

		char ip_address_buffer[16 + 1];

		struct gui_character* port_text;

		char port_buffer[5 + 1];

		struct gui_character* join_game_text;
		bool join_game_button_state;
		bool join_game_button_enabled;

		bool back_button_state;
		bool back_button_enabled;
		struct gui_character* back_text;

		struct menu_scene menu;
	} join_game_menu;

};

struct game_client;

void init_game_menus(struct game_client* game);
#endif // !GAME_MENUS_H