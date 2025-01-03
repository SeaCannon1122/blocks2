#include "game_client.h"

uint32_t game_client_run(struct game_client* game, uint8_t* resource_path) {

	application_create(game);

	resources_create(game, resource_path);

	settings_load(game);

	gui_menus_create(game);

	renderer_create(game);

	while (application_handle_events(game) == 0) {

		gui_menus_simulation_frame(game);

		if (gui_is_button_clicked(game->gui_menus_state.main.menu_handle, game->gui_menus_state.main.quit_game_button)) break;

		renderer_render(game);

		sleep_for_ms(16);
	}

	renderer_destroy(game);

	gui_menus_destroy(game);

	resources_destroy(game);

	application_destroy(game);

	return 0;
}