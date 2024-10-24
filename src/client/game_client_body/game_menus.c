#include "game_menus.h"

#include <stdlib.h>
#include <time.h>

#include "general/platformlib/platform.h"
#include "general/keyvalue.h"
#include "general/utils.h"
#include "general/logging.h"
#include "client/gui/menu.h"
#include "game_client.h"
#include "game_client_networker.h"
#include "game/networking_packets/networking_packets.h"

void init_game_menus(struct game_client* game) {

	game->game_menus.active_menu = MAIN_MENU;



	game->resource_map[RESOURCE_TEXTURE_DIRT_MULTIBLE] = get_value_from_key(game->resource_manager, "texture_menu_background").ptr;
	game->resource_map[RESOURCE_TEXTURE_MENU_BUTTON] = get_value_from_key(game->resource_manager, "texture_menu_button").ptr;
	game->resource_map[RESOURCE_TEXTURE_MENU_BUTTON_HOVER] = get_value_from_key(game->resource_manager, "texture_menu_button_hover").ptr;
	game->resource_map[RESOURCE_TEXTURE_MENU_BUTTON_SHORT] = get_value_from_key(game->resource_manager, "texture_menu_button_short").ptr;
	game->resource_map[RESOURCE_TEXTURE_MENU_BUTTON_HOVER_SHORT] = get_value_from_key(game->resource_manager, "texture_menu_button_hover_short").ptr;


	game->game_menus.main_menu.background_image = (struct menu_image){
		MENU_ITEM_IMAGE,
		-1,
		0,
		0,
		ALIGNMENT_LEFT,
		ALIGNMENT_TOP,
		ALIGNMENT_LEFT,
		ALIGNMENT_TOP,
		0,
		RESOURCE_TEXTURE_DIRT_MULTIBLE,
		0,
		7,
	};



	game->game_menus.main_menu.this_is_not_minecraft_label = (struct game_menu_label){
		MENU_ITEM_LABEL,
		10,
		0,
		60,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_TOP,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		10000000,
		10000000,
		1,
		1,
		3,
		{0},
	};

	game->game_menus.main_menu.join_game_label = (struct game_menu_label){
		MENU_ITEM_LABEL,
		10,
		0,
		0,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		10000000,
		10000000,
		0,
		0,
		1,
		{0},
	};

	game->game_menus.main_menu.join_game_button_image = (struct menu_image){
		MENU_ITEM_IMAGE,
		1,
		0,
		0,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		1,
		RESOURCE_TEXTURE_MENU_BUTTON,
		RESOURCE_TEXTURE_MENU_BUTTON_HOVER,
		1,
	};

	game->game_menus.main_menu.options_label = (struct game_menu_label){
		MENU_ITEM_LABEL,
		10,
		-51,
		60,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		10000000,
		10000000,
		0,
		0,
		1,
		{0},
	};

	game->game_menus.main_menu.options_button_image = (struct menu_image){
		MENU_ITEM_IMAGE,
		1,
		-51,
		60,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		1,
		RESOURCE_TEXTURE_MENU_BUTTON_SHORT,
		RESOURCE_TEXTURE_MENU_BUTTON_HOVER_SHORT,
		1,
	};

	game->game_menus.main_menu.quit_game_label = (struct game_menu_label){
		MENU_ITEM_LABEL,
		10,
		51,
		60,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		10000000,
		10000000,
		0,
		0,
		1,
		{0},
	};

	game->game_menus.main_menu.quit_game_button_image = (struct menu_image){
		MENU_ITEM_IMAGE,
		1,
		51,
		60,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		ALIGNMENT_MIDDLE,
		1,
		RESOURCE_TEXTURE_MENU_BUTTON_SHORT,
		RESOURCE_TEXTURE_MENU_BUTTON_HOVER_SHORT,
		1,
	};

	pixel_char_convert_string_in(game->game_menus.main_menu.this_is_not_minecraft_label.text, main_menu_this_is_not_minecraft_label_text, 0xffffffff, 0xff000000, PIXEL_CHAR_SHADOW_MASK | RESOURCE_PIXEL_FONT_DEFAULT);
	pixel_char_convert_string_in(game->game_menus.main_menu.join_game_label.text, main_menu_join_game_label_text, 0xffffffff, 0xff000000, PIXEL_CHAR_SHADOW_MASK | RESOURCE_PIXEL_FONT_DEFAULT);
	pixel_char_convert_string_in(game->game_menus.main_menu.options_label.text, main_menu_options_label_text, 0xffffffff, 0xff000000, PIXEL_CHAR_SHADOW_MASK | RESOURCE_PIXEL_FONT_DEFAULT);
	pixel_char_convert_string_in(game->game_menus.main_menu.quit_game_label.text, main_menu_quit_game_label_text, 0xffffffff, 0xff000000, PIXEL_CHAR_SHADOW_MASK | RESOURCE_PIXEL_FONT_DEFAULT);

	game->game_menus.main_menu.menu = (struct main_menu_scene){
		-1,
		
		-1,
		-1,
		-1,
		0,

		-1,

		-1,
		-1,
		-1,

		8,
		{
		&game->game_menus.main_menu.this_is_not_minecraft_label.menu_item_type, 
		&game->game_menus.main_menu.join_game_label.menu_item_type,
		&game->game_menus.main_menu.join_game_button_image,
		&game->game_menus.main_menu.options_label.menu_item_type,
		&game->game_menus.main_menu.options_button_image,
		&game->game_menus.main_menu.quit_game_label.menu_item_type,
		&game->game_menus.main_menu.quit_game_button_image,
		&game->game_menus.main_menu.background_image
		},
	};

	game->game_menus.options_menu.background_image = (struct menu_image){
		MENU_ITEM_IMAGE,
		-1,
		0,
		0,
		ALIGNMENT_LEFT,
		ALIGNMENT_TOP,
		ALIGNMENT_LEFT,
		ALIGNMENT_TOP,
		0,
		RESOURCE_TEXTURE_DIRT_MULTIBLE,
		0,
		7,
	};

//
//	game->game_menus.options_menu.done_button_enabled = true;
//	game->game_menus.options_menu.done_button_state = false;
//	game->game_menus.options_menu.fov_slider_state = ((float)game->settings.fov - (float)game->constants.fov_min) / ((float)game->constants.fov_max - (float)game->constants.fov_min);
//	game->game_menus.options_menu.render_distance_slider_state = ((float)game->settings.render_distance - (float)game->constants.render_distance_min) / ((float)game->constants.render_distance_max - (float)game->constants.render_distance_min);
//	game->game_menus.options_menu.gui_scale_button_enabled = true;
//	game->game_menus.options_menu.gui_scale_button_state = false;
//	
//
//	convert_string_to_gui_string_in_buffer(default_font, "Options", 2, 0xffffffff, game->game_menus.options_menu.options_text, 8);
//	convert_string_to_gui_string_in_buffer(default_font, "FOV:    ", 1, 0xffffffff, game->game_menus.options_menu.fov_text, 9);
//
//	game->game_menus.options_menu.fov_text[5].value = (game->settings.fov < 100 ? '\x1f' : digit_to_char(game->settings.fov / 100));
//	game->game_menus.options_menu.fov_text[6].value = (game->settings.fov < 10 ? '\x1f' : digit_to_char((game->settings.fov / 10) % 10));
//	game->game_menus.options_menu.fov_text[7].value = digit_to_char(game->settings.fov % 10);
//
//	convert_string_to_gui_string_in_buffer(default_font, "Render Distance:    Chunks", 1, 0xffffffff, game->game_menus.options_menu.render_distance_text, 27);
//
//	game->game_menus.options_menu.render_distance_text[17].value = (game->settings.render_distance < 10 ? '\x1f' : digit_to_char(game->settings.render_distance / 10));
//	game->game_menus.options_menu.render_distance_text[18].value = digit_to_char(game->settings.render_distance % 10);
//
//	convert_string_to_gui_string_in_buffer(default_font, "GUI Scale:     ", 1, 0xffffffff, game->game_menus.options_menu.gui_scale_text, 16);
//
//	if (game->settings.gui_scale > 0) {
//		game->game_menus.options_menu.gui_scale_text[11].value = digit_to_char(game->settings.gui_scale);
//		game->game_menus.options_menu.gui_scale_text[12].value = '\x1f';
//		game->game_menus.options_menu.gui_scale_text[13].value = '\x1f';
//		game->game_menus.options_menu.gui_scale_text[14].value = '\x1f';
//	}
//	else {
//		game->game_menus.options_menu.gui_scale_text[11].value = 'A';
//		game->game_menus.options_menu.gui_scale_text[12].value = 'u';
//		game->game_menus.options_menu.gui_scale_text[13].value = 't';
//		game->game_menus.options_menu.gui_scale_text[14].value = 'o';
//	}
//
//	convert_string_to_gui_string_in_buffer(default_font, "Done", 1, 0xffffffff, game->game_menus.options_menu.done_text, 5);
//
//	add_menu_image(&game->game_menus.options_menu.menu, -1, 0, 0, ALIGNMENT_LEFT, ALIGNMENT_TOP, ALIGNMENT_LEFT, ALIGNMENT_TOP, background, 7);
//
//	add_menu_label(&game->game_menus.options_menu.menu, 2, -79, 60, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, game->game_menus.options_menu.fov_text, ALIGNMENT_MIDDLE); //fov
//	add_menu_label(&game->game_menus.options_menu.menu, 2, 79, 60, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, game->game_menus.options_menu.render_distance_text, ALIGNMENT_MIDDLE); //render distance
//	add_menu_label(&game->game_menus.options_menu.menu, 2, -79, 85, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, game->game_menus.options_menu.gui_scale_text, ALIGNMENT_MIDDLE); //gui_scale
//
//	add_menu_label(&game->game_menus.options_menu.menu, 0, 0, 25, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, game->game_menus.options_menu.options_text, ALIGNMENT_MIDDLE);
//
//
//	add_menu_slider(&game->game_menus.options_menu.menu, 1, &game->game_menus.options_menu.fov_slider_state, -154, 50, -4, 70, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, dark_texture, light_texture, 10);
//	add_menu_slider(&game->game_menus.options_menu.menu, 1, &game->game_menus.options_menu.render_distance_slider_state, 4, 50, 154, 70, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, dark_texture, light_texture, 10);
//
//	add_menu_button(&game->game_menus.options_menu.menu, 1, &game->game_menus.options_menu.gui_scale_button_state, -154, 75, -4, 95, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, light_texture, dark_texture, &game->game_menus.options_menu.gui_scale_button_enabled);
//
//	add_menu_label(&game->game_menus.options_menu.menu, 10, 0, -15, ALIGNMENT_MIDDLE, ALIGNMENT_BOTTOM, game->game_menus.options_menu.done_text, ALIGNMENT_MIDDLE);
//	add_menu_button(&game->game_menus.options_menu.menu, 2, &game->game_menus.options_menu.done_button_state, -100, -25, 100, -5, ALIGNMENT_MIDDLE, ALIGNMENT_BOTTOM, light_texture, dark_texture, &game->game_menus.options_menu.done_button_enabled);
//
//
//
//
//	game->game_menus.join_game_menu.menu.menu_items_count = 0;
//
//	game->game_menus.join_game_menu.join_game_button_enabled = false;
//	game->game_menus.join_game_menu.join_game_button_state = false;
//	game->game_menus.join_game_menu.back_button_state = false;
//	game->game_menus.join_game_menu.back_button_enabled = true;
//	for (int i = 0; i < sizeof(game->game_menus.join_game_menu.ip_address_buffer); i++) game->game_menus.join_game_menu.ip_address_buffer[i] = '\0';
//	game->game_menus.join_game_menu.ip_address_buffer_link = -1;
//	game->game_menus.join_game_menu.ip_address_box_selected = false;
//	game->game_menus.join_game_menu.ip_address_field_visible = true;
//	for (int i = 0; i < sizeof(game->game_menus.join_game_menu.port_buffer); i++) game->game_menus.join_game_menu.port_buffer[i] = '\0';
//	game->game_menus.join_game_menu.port_buffer_link = -1;
//	game->game_menus.join_game_menu.port_box_selected = false;
//	game->game_menus.join_game_menu.port_field_visible = true;
//	for (int i = 0; i < sizeof(game->game_menus.join_game_menu.username_buffer); i++) game->game_menus.join_game_menu.username_buffer[i] = '\0';
//	game->game_menus.join_game_menu.username_buffer_link = -1;
//	game->game_menus.join_game_menu.username_box_selected = false;
//	game->game_menus.join_game_menu.username_field_visible = true;
//	for (int i = 0; i < sizeof(game->game_menus.join_game_menu.password_buffer); i++) game->game_menus.join_game_menu.password_buffer[i] = '\0';
//	game->game_menus.join_game_menu.password_buffer_link = -1;
//	game->game_menus.join_game_menu.password_box_selected = false;
//	game->game_menus.join_game_menu.password_field_visible = true;
//
//	convert_string_to_gui_string_in_buffer(default_font, "Back", 1, 0xffffffff, game->game_menus.join_game_menu.back_text, 5);
//	convert_string_to_gui_string_in_buffer(default_font, "Join a multiplayer game", 2, 0xffffffff, game->game_menus.join_game_menu.join_a_multiplayer_game_text, 24);
//
//	convert_string_to_gui_string_in_buffer(default_font, "IP-address:", 1, 0xffa0a0a0, game->game_menus.join_game_menu.ip_address_text, 12);
//	convert_string_to_gui_string_in_buffer(default_font, "Port:", 1, 0xffa0a0a0, game->game_menus.join_game_menu.port_text, 6);
//	convert_string_to_gui_string_in_buffer(default_font, "Username:", 1, 0xffa0a0a0, game->game_menus.join_game_menu.username_text, 10);
//	convert_string_to_gui_string_in_buffer(default_font, "Password:", 1, 0xffa0a0a0, game->game_menus.join_game_menu.password_text, 10);
//
//	convert_string_to_gui_string_in_buffer(default_font, "Join Game", 1, 0xffffffff, game->game_menus.join_game_menu.join_game_text, 10);
//
//	add_menu_image(&game->game_menus.join_game_menu.menu, -1, 0, 0, ALIGNMENT_LEFT, ALIGNMENT_TOP, ALIGNMENT_LEFT, ALIGNMENT_TOP, background, 7);
//
//	add_menu_label(&game->game_menus.join_game_menu.menu, 0, 0, 25, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, game->game_menus.join_game_menu.join_a_multiplayer_game_text, ALIGNMENT_MIDDLE);
//
//	add_menu_label(&game->game_menus.join_game_menu.menu, 11, 0, -15, ALIGNMENT_MIDDLE, ALIGNMENT_BOTTOM, game->game_menus.join_game_menu.back_text, ALIGNMENT_MIDDLE);
//	add_menu_button(&game->game_menus.join_game_menu.menu, 10, &game->game_menus.join_game_menu.back_button_state, -100, -25, 100, -5, ALIGNMENT_MIDDLE, ALIGNMENT_BOTTOM, light_texture, dark_texture, &game->game_menus.join_game_menu.back_button_enabled);
//
//	add_menu_label(&game->game_menus.join_game_menu.menu, 2, -123, -48, ALIGNMENT_MIDDLE, ALIGNMENT_MIDDLE, game->game_menus.join_game_menu.ip_address_text, ALIGNMENT_LEFT);
//	add_menu_text_field(&game->game_menus.join_game_menu.menu, 2, game->game_menus.join_game_menu.ip_address_buffer,  -125, 45, -30, ALIGNMENT_MIDDLE, ALIGNMENT_MIDDLE, ALIGNMENT_LEFT, &game->game_menus.join_game_menu.ip_address_box_selected, default_font, &game->game_menus.join_game_menu.ip_address_field_visible );
//
//	add_menu_label(&game->game_menus.join_game_menu.menu, 2, 57, -48, ALIGNMENT_MIDDLE, ALIGNMENT_MIDDLE, game->game_menus.join_game_menu.port_text, ALIGNMENT_LEFT);
//	add_menu_text_field(&game->game_menus.join_game_menu.menu, 2, game->game_menus.join_game_menu.port_buffer, 55, 125, -30, ALIGNMENT_MIDDLE, ALIGNMENT_MIDDLE, ALIGNMENT_LEFT, &game->game_menus.join_game_menu.port_box_selected, default_font, &game->game_menus.join_game_menu.port_field_visible);
//
//	add_menu_label(&game->game_menus.join_game_menu.menu, 2, -123, -3, ALIGNMENT_MIDDLE, ALIGNMENT_MIDDLE, game->game_menus.join_game_menu.username_text, ALIGNMENT_LEFT);
//	add_menu_text_field(&game->game_menus.join_game_menu.menu, 2, game->game_menus.join_game_menu.username_buffer, -125, -5, 15, ALIGNMENT_MIDDLE, ALIGNMENT_MIDDLE, ALIGNMENT_LEFT, &game->game_menus.join_game_menu.username_box_selected, default_font, &game->game_menus.join_game_menu.username_field_visible);
//
//	add_menu_label(&game->game_menus.join_game_menu.menu, 2, 7, -3, ALIGNMENT_MIDDLE, ALIGNMENT_MIDDLE, game->game_menus.join_game_menu.password_text, ALIGNMENT_LEFT);
//	add_menu_text_field(&game->game_menus.join_game_menu.menu, 2, game->game_menus.join_game_menu.password_buffer, 5, 125, 15, ALIGNMENT_MIDDLE, ALIGNMENT_MIDDLE, ALIGNMENT_LEFT, &game->game_menus.join_game_menu.password_box_selected, default_font, &game->game_menus.join_game_menu.password_field_visible);
//
//
//	add_menu_label(&game->game_menus.join_game_menu.menu, 3, 0, 40, ALIGNMENT_MIDDLE, ALIGNMENT_MIDDLE, game->game_menus.join_game_menu.join_game_text, ALIGNMENT_MIDDLE);
//	add_menu_button(&game->game_menus.join_game_menu.menu, 2, &game->game_menus.join_game_menu.join_game_button_state, -100, 30, 100, 50, ALIGNMENT_MIDDLE, ALIGNMENT_MIDDLE, light_texture, dark_texture, &game->game_menus.join_game_menu.join_game_button_enabled);
//
//
//
//
//
//	game->game_menus.connection_waiting_menu.menu.menu_items_count = 0;
//
//	game->game_menus.connection_waiting_menu.back_button_enabled = true;
//	game->game_menus.connection_waiting_menu.back_button_state = false;
//
//	convert_string_to_gui_string_in_buffer(default_font, "Back", 1, 0xffffffff, game->game_menus.connection_waiting_menu.back_text, 5);
//	convert_string_to_gui_string_in_buffer(default_font, "                                                               ", 1, 0xffffffff, game->game_menus.connection_waiting_menu.networking_message_gui, 64);
//
//	for(int i = 0; i < sizeof(game->game_menus.connection_waiting_menu.networking_message); i++) game->game_menus.connection_waiting_menu.networking_message[i] = '\0';
//	for (int i = 0; i < sizeof(game->game_menus.connection_waiting_menu.networking_message); i++) game->game_menus.connection_waiting_menu.networking_message_gui[i].value = '\0';
//
//	add_menu_image(&game->game_menus.connection_waiting_menu.menu, -1, 0, 0, ALIGNMENT_LEFT, ALIGNMENT_TOP, ALIGNMENT_LEFT, ALIGNMENT_TOP, background, 7);
//
//	add_menu_label(&game->game_menus.connection_waiting_menu.menu, 1, 0, 0, ALIGNMENT_MIDDLE, ALIGNMENT_MIDDLE, game->game_menus.connection_waiting_menu.networking_message_gui, ALIGNMENT_MIDDLE);
//	
//	add_menu_label(&game->game_menus.connection_waiting_menu.menu, 11, 0, -15, ALIGNMENT_MIDDLE, ALIGNMENT_BOTTOM, game->game_menus.connection_waiting_menu.back_text, ALIGNMENT_MIDDLE);
//	add_menu_button(&game->game_menus.connection_waiting_menu.menu, 10, &game->game_menus.connection_waiting_menu.back_button_state, -100, -25, 100, -5, ALIGNMENT_MIDDLE, ALIGNMENT_BOTTOM, light_texture, dark_texture, &game->game_menus.connection_waiting_menu.back_button_enabled);
//
//
//
//
//	game->game_menus.chat_menu.message_link = -1;
//	for (int i = 0; i < sizeof(game->game_menus.chat_menu.message_buffer); i++) game->game_menus.chat_menu.message_buffer[i] = '\0';
//	game->game_menus.chat_menu.font = default_font;
//
//	game->game_menus.in_game_options_menu.menu.menu_items_count = 0;
//
//	game->game_menus.in_game_options_menu.back_to_game_button_enabled = true;
//	game->game_menus.in_game_options_menu.back_to_game_button_state = false;
//	game->game_menus.in_game_options_menu.disconnect_button_enabled = true;
//	game->game_menus.in_game_options_menu.disconnect_button_state = false;
//	game->game_menus.in_game_options_menu.fov_slider_state = ((float)game->settings.fov - (float)game->constants.fov_min) / ((float)game->constants.fov_max - (float)game->constants.fov_min);
//	game->game_menus.in_game_options_menu.render_distance_slider_state = ((float)game->settings.render_distance - (float)game->constants.render_distance_min) / ((float)game->constants.render_distance_max - (float)game->constants.render_distance_min);
//	game->game_menus.in_game_options_menu.gui_scale_button_enabled = true;
//	game->game_menus.in_game_options_menu.gui_scale_button_state = false;
//
//
//	convert_string_to_gui_string_in_buffer(default_font, "Options", 2, 0xffffffff, game->game_menus.in_game_options_menu.options_text, 8);
//	convert_string_to_gui_string_in_buffer(default_font, "FOV:    ", 1, 0xffffffff, game->game_menus.in_game_options_menu.fov_text, 9);
//
//	game->game_menus.in_game_options_menu.fov_text[5].value = (game->settings.fov < 100 ? '\x1f' : digit_to_char(game->settings.fov / 100));
//	game->game_menus.in_game_options_menu.fov_text[6].value = (game->settings.fov < 10 ? '\x1f' : digit_to_char((game->settings.fov / 10) % 10));
//	game->game_menus.in_game_options_menu.fov_text[7].value = digit_to_char(game->settings.fov % 10);
//
//	convert_string_to_gui_string_in_buffer(default_font, "Render Distance:    Chunks", 1, 0xffffffff, game->game_menus.in_game_options_menu.render_distance_text, 27);
//
//	game->game_menus.in_game_options_menu.render_distance_text[17].value = (game->settings.render_distance < 10 ? '\x1f' : digit_to_char(game->settings.render_distance / 10));
//	game->game_menus.in_game_options_menu.render_distance_text[18].value = digit_to_char(game->settings.render_distance % 10);
//
//	convert_string_to_gui_string_in_buffer(default_font, "GUI Scale:     ", 1, 0xffffffff, game->game_menus.in_game_options_menu.gui_scale_text, 16);
//
//	if (game->settings.gui_scale > 0) {
//		game->game_menus.in_game_options_menu.gui_scale_text[11].value = digit_to_char(game->settings.gui_scale);
//		game->game_menus.in_game_options_menu.gui_scale_text[12].value = '\x1f';
//		game->game_menus.in_game_options_menu.gui_scale_text[13].value = '\x1f';
//		game->game_menus.in_game_options_menu.gui_scale_text[14].value = '\x1f';
//	}
//	else {
//		game->game_menus.in_game_options_menu.gui_scale_text[11].value = 'A';
//		game->game_menus.in_game_options_menu.gui_scale_text[12].value = 'u';
//		game->game_menus.in_game_options_menu.gui_scale_text[13].value = 't';
//		game->game_menus.in_game_options_menu.gui_scale_text[14].value = 'o';
//	}
//
//	convert_string_to_gui_string_in_buffer(default_font, "Back to Game", 1, 0xffffffff, game->game_menus.in_game_options_menu.back_to_game_text, 13);
//	convert_string_to_gui_string_in_buffer(default_font, "Disconnect", 1, 0xffffffff, game->game_menus.in_game_options_menu.disconnect_text, 11);
//
//	add_menu_label(&game->game_menus.in_game_options_menu.menu, 2, -79, 85, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, game->game_menus.in_game_options_menu.fov_text, ALIGNMENT_MIDDLE); //fov
//	add_menu_label(&game->game_menus.in_game_options_menu.menu, 2, 79, 85, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, game->game_menus.in_game_options_menu.render_distance_text, ALIGNMENT_MIDDLE); //render distance
//	add_menu_label(&game->game_menus.in_game_options_menu.menu, 2, -79, 110, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, game->game_menus.in_game_options_menu.gui_scale_text, ALIGNMENT_MIDDLE); //gui_scale
//
//	add_menu_label(&game->game_menus.in_game_options_menu.menu, 0, 0, 25, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, game->game_menus.in_game_options_menu.options_text, ALIGNMENT_MIDDLE);
//
//	add_menu_label(&game->game_menus.in_game_options_menu.menu, 10, 0, 60, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, game->game_menus.in_game_options_menu.disconnect_text, ALIGNMENT_MIDDLE);
//	add_menu_button(&game->game_menus.in_game_options_menu.menu, 2, &game->game_menus.in_game_options_menu.disconnect_button_state, -100, 50, 100, 70, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, light_texture, dark_texture, &game->game_menus.in_game_options_menu.disconnect_button_enabled);
//
//	add_menu_slider(&game->game_menus.in_game_options_menu.menu, 1, &game->game_menus.in_game_options_menu.fov_slider_state, -154, 75, -4, 95, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, dark_texture, light_texture, 10);
//	add_menu_slider(&game->game_menus.in_game_options_menu.menu, 1, &game->game_menus.in_game_options_menu.render_distance_slider_state, 4, 75, 154, 95, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, dark_texture, light_texture, 10);
//
//	add_menu_button(&game->game_menus.in_game_options_menu.menu, 1, &game->game_menus.in_game_options_menu.gui_scale_button_state, -154, 100, -4, 120, ALIGNMENT_MIDDLE, ALIGNMENT_TOP, light_texture, dark_texture, &game->game_menus.in_game_options_menu.gui_scale_button_enabled);
//
//	add_menu_label(&game->game_menus.in_game_options_menu.menu, 10, 0, -15, ALIGNMENT_MIDDLE, ALIGNMENT_BOTTOM, game->game_menus.in_game_options_menu.back_to_game_text, ALIGNMENT_MIDDLE);
//	add_menu_button(&game->game_menus.in_game_options_menu.menu, 2, &game->game_menus.in_game_options_menu.back_to_game_button_state, -100, -25, 100, -5, ALIGNMENT_MIDDLE, ALIGNMENT_BOTTOM, light_texture, dark_texture, &game->game_menus.in_game_options_menu.back_to_game_button_enabled);
}

void switch_game_menu(struct game_client* game, int menu) {
//
//	switch (game->game_menus.active_menu) {
//	
//	case JOIN_GAME_MENU: {
//
//		if (game->game_menus.join_game_menu.ip_address_buffer_link != -1) {
//			unlink_keyboard_parse_buffer(game->window, game->game_menus.join_game_menu.ip_address_buffer_link);
//			game->game_menus.join_game_menu.ip_address_buffer_link = -1;
//		}
//		if (game->game_menus.join_game_menu.port_buffer_link != -1) {
//			unlink_keyboard_parse_buffer(game->window, game->game_menus.join_game_menu.port_buffer_link);
//			game->game_menus.join_game_menu.port_buffer_link = -1;
//		}
//		if (game->game_menus.join_game_menu.username_buffer_link != -1) {
//			unlink_keyboard_parse_buffer(game->window, game->game_menus.join_game_menu.username_buffer_link);
//			game->game_menus.join_game_menu.username_buffer_link = -1;
//		}
//		if (game->game_menus.join_game_menu.password_buffer_link != -1) {
//			unlink_keyboard_parse_buffer(game->window, game->game_menus.join_game_menu.password_buffer_link);
//			game->game_menus.join_game_menu.password_buffer_link = -1;
//		}
//
//	} break;
//
//	case CHAT_MENU: {
//
//		if (game->game_menus.chat_menu.message_link != -1) unlink_keyboard_parse_buffer(game->window, game->game_menus.chat_menu.message_link);
//
//	} break;
//
//	}

	/*switch (menu) {
	
	case JOIN_GAME_MENU: {

	} break;

	case CHAT_MENU: {

	} break;

	}*/

	game->game_menus.active_menu = MAIN_MENU;
}

void game_menus_frame(struct game_client* game) {

	unsigned int* pixels = game->render_state.pixels;
	int width = game->render_state.width;
	int height = game->render_state.height;

	struct point2d_int mousepos = window_get_mouse_cursor_position(game->window);
	mousepos.x /= game->settings.resolution_scale;
	mousepos.y /= game->settings.resolution_scale;

	int render_gui_scale = (game->settings.gui_scale != 0 ? clamp_int(game->settings.gui_scale, 1, (width / game->settings.resolution_scale - 350) / 350 + 1) : (width / game->settings.resolution_scale - 350) / 350 + 1);

	if (game->in_game_flag && (game->game_menus.active_menu == NO_MENU || game->game_menus.active_menu == CHAT_MENU || game->game_menus.active_menu == INGAME_OPTIONS_MENU)) {
		

	}

	switch (game->game_menus.active_menu) {

	case MAIN_MENU: {
		menu_scene_frame(&game->game_menus.main_menu.menu, pixels, width, height, render_gui_scale, game->resource_map, mousepos.x, mousepos.y, game->input_state.left_click);

		break;
	}

	case OPTIONS_MENU: {

		menu_scene_frame(&game->game_menus.options_menu.menu, pixels, width, height, render_gui_scale, game->resource_map, mousepos.x, mousepos.y, game->input_state.left_click);

		break;
	}

	//case JOIN_GAME_MENU: {

	//	menu_scene_frame(&game->game_menus.join_game_menu.menu, render_gui_scale, pixels, width, height, mousepos.x, mousepos.y, click);

	//	//ip address
	//	if (game->game_menus.join_game_menu.ip_address_box_selected && game->game_menus.join_game_menu.ip_address_buffer_link == -1) {
	//		game->game_menus.join_game_menu.ip_address_buffer_link = link_keyboard_parse_buffer(game->window, game->game_menus.join_game_menu.ip_address_buffer, sizeof(game->game_menus.join_game_menu.ip_address_buffer), string_length(game->game_menus.join_game_menu.ip_address_buffer) - 1);
	//	}
	//	else if (game->game_menus.join_game_menu.ip_address_box_selected == false && game->game_menus.join_game_menu.ip_address_buffer_link != -1) {
	//		unlink_keyboard_parse_buffer(game->window, game->game_menus.join_game_menu.ip_address_buffer_link);
	//		game->game_menus.join_game_menu.ip_address_buffer_link = -1;
	//	}
	//	//port
	//	if (game->game_menus.join_game_menu.port_box_selected && game->game_menus.join_game_menu.port_buffer_link == -1) {
	//		game->game_menus.join_game_menu.port_buffer_link = link_keyboard_parse_buffer(game->window, game->game_menus.join_game_menu.port_buffer, sizeof(game->game_menus.join_game_menu.port_buffer), string_length(game->game_menus.join_game_menu.port_buffer) - 1);
	//	}
	//	else if (game->game_menus.join_game_menu.port_box_selected == false && game->game_menus.join_game_menu.port_buffer_link != -1) {
	//		unlink_keyboard_parse_buffer(game->window, game->game_menus.join_game_menu.port_buffer_link);
	//		game->game_menus.join_game_menu.port_buffer_link = -1;
	//	}
	//	//username
	//	if (game->game_menus.join_game_menu.username_box_selected && game->game_menus.join_game_menu.username_buffer_link == -1) {
	//		game->game_menus.join_game_menu.username_buffer_link = link_keyboard_parse_buffer(game->window, game->game_menus.join_game_menu.username_buffer, sizeof(game->game_menus.join_game_menu.username_buffer), string_length(game->game_menus.join_game_menu.username_buffer) - 1);
	//	}
	//	else if (game->game_menus.join_game_menu.username_box_selected == false && game->game_menus.join_game_menu.username_buffer_link != -1) {
	//		unlink_keyboard_parse_buffer(game->window, game->game_menus.join_game_menu.username_buffer_link);
	//		game->game_menus.join_game_menu.username_buffer_link = -1;
	//	}
	//	//password
	//	if (game->game_menus.join_game_menu.password_box_selected && game->game_menus.join_game_menu.password_buffer_link == -1) {
	//		game->game_menus.join_game_menu.password_buffer_link = link_keyboard_parse_buffer(game->window, game->game_menus.join_game_menu.password_buffer, sizeof(game->game_menus.join_game_menu.password_buffer), string_length(game->game_menus.join_game_menu.password_buffer) - 1);
	//	}
	//	else if (game->game_menus.join_game_menu.password_box_selected == false && game->game_menus.join_game_menu.password_buffer_link != -1) {
	//		unlink_keyboard_parse_buffer(game->window, game->game_menus.join_game_menu.password_buffer_link);
	//		game->game_menus.join_game_menu.password_buffer_link = -1;
	//	}



	//	if (game->game_menus.join_game_menu.back_button_state || (get_key_state(KEY_ESCAPE) == 0b11 && is_window_selected(game->window))) {
	//		game->game_menus.join_game_menu.back_button_state = false;
	//		switch_game_menu(game, MAIN_MENU);

	//		break;

	//	}

	//	else if (game->game_menus.join_game_menu.join_game_button_state) {
	//		game->game_menus.join_game_menu.join_game_button_state = false;
	//		switch_game_menu(game, CONNECTION_WAITING_MENU);

	//		if (game->networker.status == NETWORK_INACTIVE) {
	//			game->networker.port = string_to_int(game->game_menus.join_game_menu.port_buffer, sizeof(game->game_menus.join_game_menu.port_buffer) - 1);
	//			for (int i = 0; i < 16; i++) game->networker.ip[i] = game->game_menus.join_game_menu.ip_address_buffer[i];
	//			for (int i = 0; i < MAX_USERNAME_LENGTH; i++) game->networker.username[i] = game->game_menus.join_game_menu.username_buffer[i];
	//			for (int i = 0; i < MAX_PASSWORD_LENGTH; i++) game->networker.password[i] = game->game_menus.join_game_menu.password_buffer[i];
	//			game->networker.request = CONNECT_TO_SERVER;
	//		}

	//		break;
	//	}

	//	if (string_length(game->game_menus.join_game_menu.ip_address_buffer) != 1 && string_length(game->game_menus.join_game_menu.port_buffer) != 1 && string_length(game->game_menus.join_game_menu.username_buffer) != 1) game->game_menus.join_game_menu.join_game_button_enabled = true;
	//	else game->game_menus.join_game_menu.join_game_button_enabled = false;


	//	break;
	//}

	//case CONNECTION_WAITING_MENU: {
	//	menu_scene_frame(&game->game_menus.connection_waiting_menu.menu, render_gui_scale, pixels, width, height, mousepos.x, mousepos.y, click);

	//	for (int i = 0; i <= CONECTION_CLOSE_MESSAGE_LENGTH; i++) game->game_menus.connection_waiting_menu.networking_message_gui[i].value = game->game_menus.connection_waiting_menu.networking_message[i];


	//	if (game->game_menus.connection_waiting_menu.back_button_state) {
	//		game->game_menus.connection_waiting_menu.back_button_state = false;
	//		game->game_menus.active_menu = JOIN_GAME_MENU;
	//		game->disconnect_flag = 2;
	//		break;
	//	}

	//	break;
	//}

	//case CHAT_MENU: {

	//	if (get_key_state(KEY_ENTER) == 0b11 && is_window_selected(game->window)) {
	//		switch_game_menu(game, NO_MENU);

	//		if (game->networker.should_send_chat_message == false) {
	//			for (int i = 0; i < MAX_CHAT_MESSAGE_LENGTH + 1; i++) game->networker.send_chat_message[i] = game->game_menus.chat_menu.message_buffer[i];
	//			game->networker.should_send_chat_message = true;

	//			for (int i = 0; i < sizeof(game->game_menus.chat_menu.message_buffer); i++) game->game_menus.chat_menu.message_buffer[i] = '\0';
	//		}

	//		
	//		game->game_menus.chat_menu.message_link = -1;
	//		break;
	//	}

	//	else if (get_key_state(KEY_ESCAPE) == 0b11 && is_window_selected(game->window)) {
	//		switch_game_menu(game, NO_MENU);
	//		game->game_menus.active_menu = NO_MENU;
	//		game->game_menus.chat_menu.message_link = -1;
	//		break;
	//	}


	//	if (game->game_menus.chat_menu.message_link == -1 && game->networker.should_send_chat_message == false) game->game_menus.chat_menu.message_link = link_keyboard_parse_buffer(game->window, game->game_menus.chat_menu.message_buffer, sizeof(game->game_menus.chat_menu.message_buffer), string_length(game->game_menus.chat_menu.message_buffer) - 1);


	//	//commandline background

	//	unsigned int top_a = 0x5f;
	//	unsigned int top_r = 0x00;
	//	unsigned int top_g = 0x00;
	//	unsigned int top_b = 0x00;

	//	for (int i = 0; i < width; i++) {
	//		for (int j = height - 1; j >= height - render_gui_scale * 10 - 1 && j >= 0; j--) {

	//			unsigned int* pixel = &pixels[i + width * j];

	//			


	//			*(unsigned char*)((long long)pixel + 2) = (unsigned char)((top_r * top_a + (255 - top_a) * (unsigned int)*(unsigned char*)((long long)pixel + 2)) / 255);
	//			*(unsigned char*)((long long)pixel + 1) = (unsigned char)((top_g * top_a + (255 - top_a) * (unsigned int)*(unsigned char*)((long long)pixel + 1)) / 255);
	//			*(unsigned char*)pixel = (unsigned char)((top_b * top_a + (255 - top_a) * (unsigned int)*(unsigned char*)pixel) / 255);

	//		}
	//	}

	//	if (game->game_menus.chat_menu.message_buffer[0] != '\0') {
	//		struct gui_character* text = convert_string_to_gui_string(game->game_menus.chat_menu.font, game->game_menus.chat_menu.message_buffer, 1, 0xffffffff);
	//		struct gui_character* text_shadow = convert_string_to_gui_string(game->game_menus.chat_menu.font, game->game_menus.chat_menu.message_buffer, 1, 0xff3b3b3b);

	//		int x = 3;

	//		print_gui_string(
	//			text_shadow,
	//			render_gui_scale,
	//			menu_x(x + 1, ALIGNMENT_LEFT, render_gui_scale, width),
	//			menu_y(-5 + 1, ALIGNMENT_BOTTOM, render_gui_scale, height),
	//			ALIGNMENT_LEFT,
	//			pixels,
	//			width,
	//			height
	//		);

	//		print_gui_string(
	//			text,
	//			render_gui_scale,
	//			menu_x(x, ALIGNMENT_LEFT, render_gui_scale, width),
	//			menu_y(-5, ALIGNMENT_BOTTOM, render_gui_scale, height),
	//			ALIGNMENT_LEFT,
	//			pixels,
	//			width,
	//			height
	//		);

	//		free(text);
	//		free(text_shadow);

	//	}


	//} break;

	//case INGAME_OPTIONS_MENU: {

	//	menu_scene_frame(&game->game_menus.in_game_options_menu.menu, render_gui_scale, pixels, width, height, mousepos.x, mousepos.y, click);

	//	game->settings.render_distance = (float)game->constants.render_distance_min + (game->game_menus.in_game_options_menu.render_distance_slider_state * ((float)game->constants.render_distance_max - (float)game->constants.render_distance_min));
	//	game->settings.fov = (float)game->constants.fov_min + (game->game_menus.in_game_options_menu.fov_slider_state * ((float)game->constants.fov_max - (float)game->constants.fov_min));

	//	if (game->game_menus.in_game_options_menu.back_to_game_button_state || (get_key_state(KEY_ESCAPE) == 0b11) && is_window_selected(game->window)) {
	//		game->game_menus.in_game_options_menu.back_to_game_button_state = false;
	//		game->game_menus.active_menu = NO_MENU;
	//		break;
	//	}

	//	if (game->game_menus.in_game_options_menu.disconnect_button_state) {
	//		game->game_menus.in_game_options_menu.disconnect_button_state = false;
	//		game->disconnect_flag = 2; 
	//		break;
	//	}

	//	else if (game->game_menus.in_game_options_menu.gui_scale_button_state) {
	//		game->game_menus.in_game_options_menu.gui_scale_button_state = false;
	//		game->settings.gui_scale = (game->settings.gui_scale + 1) % ((width + 350) / 350);

	//		if (game->settings.gui_scale > 0) {
	//			game->game_menus.in_game_options_menu.gui_scale_text[11].value = digit_to_char(game->settings.gui_scale);
	//			game->game_menus.in_game_options_menu.gui_scale_text[12].value = '\x1f';
	//			game->game_menus.in_game_options_menu.gui_scale_text[13].value = '\x1f';
	//			game->game_menus.in_game_options_menu.gui_scale_text[14].value = '\x1f';
	//		}
	//		else {
	//			game->game_menus.in_game_options_menu.gui_scale_text[11].value = 'A';
	//			game->game_menus.in_game_options_menu.gui_scale_text[12].value = 'u';
	//			game->game_menus.in_game_options_menu.gui_scale_text[13].value = 't';
	//			game->game_menus.in_game_options_menu.gui_scale_text[14].value = 'o';
	//		}
	//	}

	//	game->game_menus.in_game_options_menu.render_distance_text[17].value = (game->settings.render_distance < 10 ? '\x1f' : digit_to_char(game->settings.render_distance / 10));
	//	game->game_menus.in_game_options_menu.render_distance_text[18].value = digit_to_char(game->settings.render_distance % 10);

	//	game->game_menus.in_game_options_menu.fov_text[5].value = (game->settings.fov < 100 ? '\x1f' : digit_to_char(game->settings.fov / 100));
	//	game->game_menus.in_game_options_menu.fov_text[6].value = (game->settings.fov < 10 ? '\x1f' : digit_to_char((game->settings.fov / 10) % 10));
	//	game->game_menus.in_game_options_menu.fov_text[7].value = digit_to_char(game->settings.fov % 10);

	//	break;
	//}

	}

}