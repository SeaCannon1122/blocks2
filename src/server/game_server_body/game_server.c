#include "game_server.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "general/resource_manager.h"
#include "general/platformlib/platform.h"
#include "general/platformlib/networking.h"
#include "general/utils.h"

#include "game/networking_packets/networking_packets.h"

void log_chat_message(struct game_server* game, const char* format, ...) {
    char time_buffer[20];
    get_time_in_string(time_buffer);
    time_buffer[19] = '\0';

    va_list args;
    va_start(args, format);
    printf("%s [CHAT] ", time_buffer);
    vprintf(format, args);
    va_end(args);

    va_start(args, format);
    fprintf(game->chat_log_file, "%s [CHAT] ", time_buffer);
    vfprintf(game->chat_log_file, format, args);
    va_end(args);
    fflush(game->chat_log_file);
}

void log_debug_message(struct game_server* game, const char* format, ...) {
    char time_buffer[20];
    get_time_in_string(time_buffer);
    time_buffer[19] = '\0';

    va_list args;
    va_start(args, format);
    printf("%s [DEBUG] ", time_buffer);
    vprintf(format, args);
    va_end(args);

    va_start(args, format);
    fprintf(game->debug_log_file, "%s [DEBUG] ", time_buffer);
    vfprintf(game->debug_log_file, format, args);
    va_end(args);
    fflush(game->debug_log_file);
}

void new_game_server(struct game_server* game, char* resource_path) {
    game->resource_manager = new_resource_manager(resource_path);

    game->username_password_map = get_value_from_key(game->resource_manager, "passwords").ptr;

    struct key_value_map* settings_map = get_value_from_key(game->resource_manager, "settings").ptr;

    game->settings.max_clients = get_value_from_key(settings_map, "max_clients").i;
    game->settings.max_render_distance = get_value_from_key(settings_map, "max_render_distance").i;
    game->settings.max_message_length = get_value_from_key(settings_map, "max_message_length").i;

    game->clients_connected_count = 0;
    game->clients_length = game->settings.max_clients;
    game->clients = malloc(game->settings.max_clients * sizeof(struct client_on_server));

    for (int i = 0; i < game->settings.max_clients; i++) {
        game->clients[i].client_handle = NULL;
        for (int j = 0; j < 22 + 1; j++) game->clients[i].ip_address[j] = '\0';
        for (int j = 0; j < MAX_USERNAME_LENGTH + 1; j++) game->clients[i].username[j] = '\0';
    }

    int i = 0;
    for (; resource_path[i] != '\0'; i++);
    for (; resource_path[i] != '/' && i > 0; i--);
    if (resource_path[i] == '/') i++;

    for (int j = 0; j < i; j++) game->resource_folder_path[j] = resource_path[j];
    game->resource_folder_path[i] = '\0';
}

void logging_init(struct game_server* game) {
    char chat_log_file_path[1024];
    

}

void client_auth_thread_function(struct game_server* game) {


    while (game->running) {
        void* client_handle;
        for (int i = 0; (client_handle = server_accept(game->server_handle)) != NULL && i < 10; i++) {

            int client_index = 0;
            if (game->clients_connected_count >= game->settings.max_clients) client_index = game->settings.max_clients;
            else for (; client_index < game->settings.max_clients && game->clients[client_index].client_handle != NULL; client_index++);
            

            int packet_type = -1;
            struct networking_packet_client_auth auth_packet;
            int recieved;

            if (receive_data(client_handle, &packet_type, sizeof(int), NULL, 3000) == -2) {
                close_connection(client_handle);
                return;
            }

            if (packet_type != NETWORKING_PACKET_CLIENT_AUTH) {
                close_connection(client_handle);
                return;
            }

            if (receive_data(client_handle, &auth_packet, sizeof(struct networking_packet_client_auth), NULL, 3000) == -2) {
                close_connection(client_handle);
                return;
            }

            char* password = get_value_from_key(game->username_password_map, auth_packet.username).ptr;

            if (password == NULL) {
                int packet_type = NETWORKING_PACKET_NOT_AUTHORIZED_TO_JOIN;
                send_data(client_handle, &packet_type, sizeof(int));
                close_connection(client_handle);
                log_debug_message(game, "%s: not autherized to join\n", auth_packet.username);
            }
            else if (strcmp(auth_packet.password, password)) {
                int packet_type = NETWORKING_PACKET_INVALID_PASSWORD;
                send_data(client_handle, &packet_type, sizeof(int));
                close_connection(client_handle);
                log_debug_message(game, "%s: incorrect password\n", auth_packet.username);
            }
            else if (client_index == game->settings.max_clients) {
                int packet_type = NETWORKING_PACKET_SERVER_FULL;
                send_data(client_handle, &packet_type, sizeof(int));
                close_connection(client_handle);
            }
            else {
                int packet_type = NETWORKING_PACKET_LOGGED_IN;
                send_data(client_handle, &packet_type, sizeof(int));
                packet_type = NETWORKING_PACKET_SERVER_SETTINGS;
                send_data(client_handle, &packet_type, sizeof(int));
                struct networking_packet_server_settings server_settings = {
                    game->settings.max_render_distance,
                    game->settings.max_message_length,
                };
                send_data(client_handle, &server_settings, sizeof(struct networking_packet_server_settings));
                get_ip_address_and_port(client_handle, game->clients[client_index].ip_address, &game->clients[client_index].port);
                int j = 0;
                for (; auth_packet.username[j] != '\0'; j++) game->clients[client_index].username[j] = auth_packet.username[j];
                game->clients[client_index].username[j] = auth_packet.username[j];
                game->clients[client_index].client_handle = client_handle;
                game->clients_connected_count++;
                log_debug_message(game, "%s from %s:%u connected to the server\n", game->clients[client_index].username, game->clients[client_index].ip_address, game->clients[client_index].port);
            }
        }

        sleep_for_ms(100);
    }

    log_debug_message(game, "stopped accepting new clients\n");

}

void disconnect_client(struct game_server* game, int client_index) {
    close_connection(game->clients[client_index].client_handle);
    log_debug_message(game, "%s from %s:%u disconnected from the server\n", game->clients[client_index].username, game->clients[client_index].ip_address, game->clients[client_index].port);
    game->clients[client_index].client_handle = NULL;
    game->clients_connected_count--;
}

void server_log_init(struct game_server* game) {
    time_t raw_time = time(NULL);
    struct tm* time_info = localtime(&raw_time);

    char chat_log_path[1024];
    char* chat_log_path_rel = get_value_from_key(game->resource_manager, "chat_dump").s;
    int i = 0;
    if (chat_log_path_rel[0] != '/')for (; game->resource_folder_path[i] != '\0'; i++) chat_log_path[i] = game->resource_folder_path[i];
    int j = 0;
    for (; chat_log_path_rel[j] != '\0'; j++) chat_log_path[j + i] = chat_log_path_rel[j];

    chat_log_path[i + j] = digit_to_char((time_info->tm_mon + 1) / 10);
    chat_log_path[i + j + 1] = digit_to_char((time_info->tm_mon + 1) % 10);
    chat_log_path[i + j + 2] = digit_to_char(time_info->tm_mday / 10);
    chat_log_path[i + j + 3] = digit_to_char(time_info->tm_mday % 10);
    chat_log_path[i + j + 4] = digit_to_char((time_info->tm_year + 1900) / 1000);
    chat_log_path[i + j + 5] = digit_to_char(((time_info->tm_year + 1900) / 100) % 10);
    chat_log_path[i + j + 6] = digit_to_char(((time_info->tm_year + 1900) / 10) % 10);
    chat_log_path[i + j + 7] = digit_to_char((time_info->tm_year + 1900) % 10);
    chat_log_path[i + j + 8] = digit_to_char(time_info->tm_hour / 10);
    chat_log_path[i + j + 9] = digit_to_char(time_info->tm_hour % 10);
    chat_log_path[i + j + 10] = digit_to_char(time_info->tm_min / 10);
    chat_log_path[i + j + 11] = digit_to_char(time_info->tm_min % 10);
    chat_log_path[i + j + 12] = digit_to_char(time_info->tm_sec / 10);
    chat_log_path[i + j + 13] = digit_to_char(time_info->tm_sec % 10);
    chat_log_path[i + j + 14] = '.';
    chat_log_path[i + j + 15] = 'l';
    chat_log_path[i + j + 16] = 'o';
    chat_log_path[i + j + 17] = 'g';
    chat_log_path[i + j + 18] = '\0';

    game->chat_log_file = fopen(chat_log_path, "w");

    char debug_log_path[1024];
    char* debug_log_path_rel = get_value_from_key(game->resource_manager, "debug_dump").s;
    i = 0;
    if (debug_log_path_rel[0] != '/') for (; game->resource_folder_path[i] != '\0'; i++) debug_log_path[i] = game->resource_folder_path[i];
    j = 0;
    for (; debug_log_path_rel[j] != '\0'; j++) debug_log_path[j + i] = debug_log_path_rel[j];

    debug_log_path[i + j] = digit_to_char((time_info->tm_mon + 1) / 10);
    debug_log_path[i + j + 1] = digit_to_char((time_info->tm_mon + 1) % 10);
    debug_log_path[i + j + 2] = digit_to_char(time_info->tm_mday / 10);
    debug_log_path[i + j + 3] = digit_to_char(time_info->tm_mday % 10);
    debug_log_path[i + j + 4] = digit_to_char((time_info->tm_year + 1900) / 1000);
    debug_log_path[i + j + 5] = digit_to_char(((time_info->tm_year + 1900) / 100) % 10);
    debug_log_path[i + j + 6] = digit_to_char(((time_info->tm_year + 1900) / 10) % 10);
    debug_log_path[i + j + 7] = digit_to_char((time_info->tm_year + 1900) % 10);
    debug_log_path[i + j + 8] = digit_to_char(time_info->tm_hour / 10);
    debug_log_path[i + j + 9] = digit_to_char(time_info->tm_hour % 10);
    debug_log_path[i + j + 10] = digit_to_char(time_info->tm_min / 10);
    debug_log_path[i + j + 11] = digit_to_char(time_info->tm_min % 10);
    debug_log_path[i + j + 12] = digit_to_char(time_info->tm_sec / 10);
    debug_log_path[i + j + 13] = digit_to_char(time_info->tm_sec % 10);
    debug_log_path[i + j + 14] = '.';
    debug_log_path[i + j + 15] = 'l';
    debug_log_path[i + j + 16] = 'o';
    debug_log_path[i + j + 17] = 'g';
    debug_log_path[i + j + 18] = '\0';

    game->debug_log_file = fopen(debug_log_path, "w");
}

void run_game_server(struct game_server* game) {

    server_log_init(game);

    log_debug_message(game, "Server starting...\n");
    show_console_window();



    game->server_handle = server_init(8080);
    if (!game->server_handle) {
        log_debug_message(game, "Failed to initialize server.\n");
        return;
        exit(1);
    }
    log_debug_message(game, "Server started on port 8080.\n");

    game->running = true;

    //create_thread();

    void* client_auth_thread = create_thread((void (*) (void*))client_auth_thread_function, game);

    while (game->running) {
        
        

        for (int i = 0; i < game->clients_length; i++) if (game->clients[i].client_handle != NULL) {

            for (int packet_count = 0; packet_count < 5; packet_count++) {
                int packet_type = -1;
                if(receive_data(game->clients[i].client_handle, &packet_type, sizeof(int), NULL, 0) == 0) {
                    disconnect_client(game, i);
                    break;
                }

                if (packet_type == -1) break;

                switch (packet_type) {


                case NETWORKING_PACKET_DISCONNECT: {
                    disconnect_client(game, i);
                    goto _connection_closed;
                } break;

                case 1: {

                } break;

                case 2: {

                } break;

                default:
                    disconnect_client(game, i);
                    
                } break;

            }
        _connection_closed:
            1 == 1;
        }

        sleep_for_ms(10);
        if (get_key_state(KEY_ESCAPE)) game->running = false;
    }

    join_thread(client_auth_thread);

    for (int i = 0; i < game->clients_length; i++) if (game->clients[i].client_handle != NULL) close_connection(game->clients[i].client_handle);

    log_debug_message(game, "\n\nserver_closing\n");
    server_close(game->server_handle);

    fclose(game->chat_log_file);
    fclose(game->debug_log_file);

}

void delete_game_server(struct game_server* game) {

}