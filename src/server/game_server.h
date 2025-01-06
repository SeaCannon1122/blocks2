#pragma once

#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>

#include "general/resource_manager.h"
#include "general/platformlib/platform/platform.h"
#include "general/platformlib/networking/networking.h"

#include "resources/resources.h"

#include "game/game.h"

struct game_server {
	
	struct resource_state resource_state;

};

uint32_t game_server_run(struct game_server* game, uint8_t resource_path);

#endif // !GAME_SERVER_H




