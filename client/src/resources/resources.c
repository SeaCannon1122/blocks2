#include "minec_client.h"

#include <stb_image/stb_image.h>

#include "resources_defines.h"

struct resource_pack_declaration
{
	uint32_t priority;
	uint8_t* path;
};

int compare_resource_pack_declaration_priority(
	const struct resource_pack_declaration* decl0, 
	const struct resource_pack_declaration* decl1
) 
{
	if (decl0->priority < decl1->priority) return -1;
	if (decl0->priority > decl1->priority) return 1;
	return 0;
}

void _resources_load_paths_of_resource_type(struct minec_client* client, void* paths_hashmap, uint8_t* resource_pack_path, uint8_t* sub_layout_file_path)
{
	uint8_t* layout_file_path_components[] = { resource_pack_path, sub_layout_file_path };
	uint8_t* layout_file_path = string_allocate_joined_string(client->string_allocator, layout_file_path_components, 2);

	size_t file_length;
	void* file_data = minec_client_load_file(layout_file_path, &file_length);

	string_free(client->string_allocator, layout_file_path);

	if (file_data != NULL)
	{
		void* pack_paths_hashmap = hashmap_new(1024, 2);

		hashmap_read_yaml(pack_paths_hashmap, file_data, file_length);
		free(file_data);

		struct hashmap_iterator it;
		hashmap_iterator_start(&it, pack_paths_hashmap);

		struct hashmap_multi_type* val;
		uint8_t* key;

		while (val = hashmap_iterator_next_key_value_pair(&it, &key)) if (val->type == HASHMAP_VALUE_STRING)
		{
			uint8_t* resource_path_components[] = { resource_pack_path, val->data._string };
			uint8_t* resource_path = string_allocate_joined_string(client->string_allocator, resource_path_components, 2);

			hashmap_set_value(paths_hashmap, key, resource_path, HASHMAP_VALUE_STRING);

			string_free(client->string_allocator, resource_path);
		}

		hashmap_delete(pack_paths_hashmap);
	}
}

void resources_create(struct minec_client* client) {

	uint32_t resource_packs_count = hashmap_get_key_count(client->settings_state.resource_pack_paths_hashmap);
	struct resource_pack_declaration* paths = string_allocate(client->string_allocator, sizeof(struct resource_pack_declaration) * resource_packs_count);

	{
		struct hashmap_iterator it;
		hashmap_iterator_start(&it, client->settings_state.resource_pack_paths_hashmap);

		struct hashmap_multi_type* val;
		uint8_t* key;

		for (uint32_t i = 0; val = hashmap_iterator_next_key_value_pair(&it, &key); i++)
		{
			paths[i].priority = val->data._int;
			paths[i].path = key;
		}
	}

	qsort(paths, resource_packs_count, sizeof(struct resource_pack_declaration), compare_resource_pack_declaration_priority);

	void* texture_paths_hashmap = hashmap_new(1024, 2);
	void* font_paths_hashmap = hashmap_new(1024, 2);

	for (uint32_t i = 0; i < resource_packs_count; i++)
	{
		_resources_load_paths_of_resource_type(client, texture_paths_hashmap, paths[i].path, RESOURCE_PACK_TEXTURES_FILE_PATH);
		_resources_load_paths_of_resource_type(client, font_paths_hashmap, paths[i].path, RESOURCE_PACK_FONTS_FILE_PATH);
	}

	//textures
	{
		client->resources_state.texture_count = 0;
		uint32_t texture_declaration_count = hashmap_get_key_count(texture_paths_hashmap);

		client->resources_state.textures = malloc(sizeof(struct resources_texture) * texture_declaration_count + 8);

		client->resources_state.texture_token_id_hashmap = hashmap_new((uint32_t)((float)texture_declaration_count * 1.2f) + 1, 2);

		struct hashmap_iterator it;
		hashmap_iterator_start(&it, texture_paths_hashmap);

		struct hashmap_multi_type* val;
		uint8_t* key;

		while (val = hashmap_iterator_next_key_value_pair(&it, &key)) if (val->type == HASHMAP_VALUE_STRING)
		{
			uint32_t comp;
			uint32_t width;
			uint32_t height;
			uint32_t* data;

			if (data = stbi_load(val->data._string, &width, &height, &comp, 4))
			{
				client->resources_state.textures[client->resources_state.texture_count].data = data;
				client->resources_state.textures[client->resources_state.texture_count].width = width;
				client->resources_state.textures[client->resources_state.texture_count].height = height;

				hashmap_set_value(client->resources_state.texture_token_id_hashmap, key, &client->resources_state.texture_count, HASHMAP_VALUE_INT);
				client->resources_state.texture_count++;
			}
		}
	}

	//fonts
	{
		client->resources_state.pixelchar_font_count = 0;
		uint32_t font_declaration_count = hashmap_get_key_count(font_paths_hashmap);

		client->resources_state.pixelchar_fonts = malloc(sizeof(struct resources_pixelchar_font) * font_declaration_count + 8);

		client->resources_state.pixelchar_font_token_id_hashmap = hashmap_new((uint32_t)((float)font_declaration_count * 1.2f) + 1, 2);

		struct hashmap_iterator it;
		hashmap_iterator_start(&it, font_paths_hashmap);

		struct hashmap_multi_type* val;
		uint8_t* key;

		while (val = hashmap_iterator_next_key_value_pair(&it, &key)) if (val->type == HASHMAP_VALUE_STRING)
		{
			size_t size;
			void* data;

			if (data = minec_client_load_file(val->data._string, &size))
			{
				client->resources_state.pixelchar_fonts[client->resources_state.pixelchar_font_count].font_file_data = data;
				client->resources_state.pixelchar_fonts[client->resources_state.pixelchar_font_count].font_file_data_size = size;

				hashmap_set_value(client->resources_state.pixelchar_font_token_id_hashmap, key, &client->resources_state.pixelchar_font_count, HASHMAP_VALUE_INT);
				client->resources_state.pixelchar_font_count++;
			}
		}
	}


	hashmap_delete(texture_paths_hashmap);
	hashmap_delete(font_paths_hashmap);

	string_free(client->string_allocator, paths);
}


void resources_destroy(struct minec_client* client) {

	for (uint32_t i = 0; i < client->resources_state.texture_count; i++) free(client->resources_state.textures[i].data);
	free(client->resources_state.textures);
	hashmap_delete(client->resources_state.texture_token_id_hashmap);

	for (uint32_t i = 0; i < client->resources_state.pixelchar_font_count; i++) free(client->resources_state.pixelchar_fonts[i].font_file_data);
	free(client->resources_state.pixelchar_fonts);
	hashmap_delete(client->resources_state.pixelchar_font_token_id_hashmap);
}
