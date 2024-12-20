#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "game/game_constants.h"
#include "game/networking_packets/networking_packets.h"
#include "game/chat.h"

#define CLIENT_CHAT_QUEUE_LENGTH 8

struct client_on_server {
	void* client_handle;
	char username[MAX_USERNAME_LENGTH + 1];
	char ip_address[22 + 1];
	unsigned short port;
	int next_packet_type;
	bool logged_in_from_another_location;
	bool just_logged_in;
};

struct game_server {
	char resource_folder_path[1024];
	FILE* debug_log_file;
	FILE* chat_log_file;
	struct key_value_map* resource_manager;
	struct key_value_map* username_password_map;
	void* server_handle;
	struct client_on_server* clients;
	int clients_connected_count;
	int clients_length;
	bool running;

	struct {
		int max_clients;
		int max_render_distance;
		int max_message_length;
	} settings;

};

void new_game_server(struct game_server* game, char* resource_path);

void run_game_server(struct game_server* game);

void delete_game_server(struct game_server* game);