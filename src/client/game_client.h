#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <stdint.h>
#include "general/platformlib/platform/platform.h"
#include "resources/resources.h"
#include "resources/settings.h"
#include "application/application.h"
#include "application/graphics.h"
#include "renderer/renderer.h"
#include "simulator/simulator.h"
#include "simulator/networker.h"

struct game_client {

	struct resource_state resource_state;
	struct settings_state settings_state;
	struct application_state application_state;
	struct graphics_state graphics_state;
	struct renderer renderer;
	struct simulator_state simulator_state;
	struct networker_state networker_state;

};


uint32_t run_game_client(struct game_client* game, uint8_t* resource_path);

#endif // !GAME_CLIENT_H