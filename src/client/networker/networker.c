#include "client/game_client.h"

void networker_thread_function(struct game_client* game) {

	game->networker_state.thread_active_flag = 2;
	while (game->networker_state.thread_active_flag != 1);

	while (game->networker_state.request_exit_flag == 0) {

		switch (game->networker_state.status)
		{
		
		case NETWORKER_STAUS_INACTIVE: {

			if (game->networker_state.request_flag_connect) {

				game->networker_state.ip_reading = 1;
				if (game->networker_state.ip_writing == 1) {
					game->networker_state.ip_reading = 0;
					continue;
				}
				uint8_t ip_nums[4] = {
					game->networker_state.ip_nums[0],
					game->networker_state.ip_nums[1],
					game->networker_state.ip_nums[2],
					game->networker_state.ip_nums[3]
				};
				uint16_t port = game->networker_state.port;
				game->networker_state.ip_reading = 0;
				
				uint8_t ip[15 + 1];
				sprintf(ip, "%d.%d.%d.%d", ip_nums[0], ip_nums[1], ip_nums[2], ip_nums[3]);

				uint32_t connect_return_value = networking_client_connect(ip, port, &game->networker_state.socket);

				if (connect_return_value == NETWORKING_SUCCESS) game->networker_state.status = NETWORKER_STATUS_CONNECTING;

			}

		} break;

		case NETWORKER_STATUS_CONNECTING: {

		} break;

		case NETWORKER_STATUS_CONNECTED: {

		} break;

		}

		sleep_for_ms(50);

	}

	game->networker_state.thread_active_flag = 0;

	return;
}
 
uint32_t networker_start(struct game_client* game) {

	game->networker_state.request_exit_flag = 0;

	game->networker_state.request_flag_connect = 0;
	game->networker_state.request_flag_abort_connection = 0;
	game->networker_state.status = NETWORKER_STAUS_INACTIVE;

	game->networker_state.ip_reading = 0;
	game->networker_state.ip_writing = 0;

	game->networker_state.thread_active_flag = 0;

	game->networker_thread_handle = create_thread(networker_thread_function, game);

	while (game->networker_state.thread_active_flag != 2);
	game->networker_state.thread_active_flag = 1;

	return 0;
}


uint32_t networker_stop(struct game_client* game) {

	game->networker_state.request_exit_flag = 1;

	join_thread(game->networker_thread_handle);

	return 0;
}