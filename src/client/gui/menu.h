#pragma once

#include <stdbool.h>

#include "general/argb_image.h"
#include "client/gui/char_font.h"

#define MAX_MENU_ITEMS 32

enum menu_item_type {
	MENU_ITEM_LABEL = 0,
	MENU_ITEM_IMAGE = 1,
	MENU_ITEM_BUTTON = 2,
	MENU_ITEM_SLIDER = 3,
	MENU_ITEM_TEXT_FIELD = 4,
};

struct menu_label {
	int x;
	int y;
	char alignment_x;
	char alignment_y;
	struct gui_character* text;
	char text_alignment;
};

struct menu_image {
	int x;
	int y;
	char alignment_x;
	char alignment_y;
	char image_alignment_x;
	char image_alignment_y;
	struct argb_image* image;
	int image_scalar;
};

struct menu_button {
	bool* state;
	int x_min;
	int y_min;
	int x_max;
	int y_max;
	char alignment_x;
	char alignment_y;
	struct argb_image* texture_enabled;
	struct argb_image* texture_disabled;
	bool* enabled;
};

struct menu_slider {
	float* state;
	int x_min;
	int y_min;
	int x_max;
	int y_max;
	char alignment_x;
	char alignment_y;
	struct argb_image* texture_background;
	struct argb_image* texture_slider;
	int slider_thickness;
};

struct menu_text_field {
	char* buffer;
	int x_min;
	int x_max;
	int y;
	char alignment_x;
	char alignment_y;
	char text_alignment;
	bool* selected;
	struct char_font* font;
	bool* field_visible;
};

struct menu_item {
	union {
		struct menu_label label;
		struct menu_image image;
		struct menu_button button;
		struct menu_slider slider;
		struct menu_text_field text_field;
	} items;
	int z;
	char menu_item_type;
};

struct menu_scene {
	struct menu_item menu_items[MAX_MENU_ITEMS];
	int menu_items_count;
};

void add_menu_label(struct menu_scene* scene, int z, int x, int y, char alignment_x, char alignment_y, struct gui_character* text, char text_alignment);
  
void add_menu_image(struct menu_scene* scene, int z, int x, int y, char alignment_x, char alignment_y, char image_alignment_x, char image_alignment_y, struct argb_image* image, int image_scalar);

void add_menu_button(struct menu_scene* scene, int z, bool* state, int x_min, int y_min, int x_max, int y_max, char alignment_x, char alignment_y, struct argb_image* texture_enabled, struct argb_image* texture_disabled, bool* enabled);

void add_menu_slider(struct menu_scene* scene, int z, float* state, int x_min, int y_min, int x_max, int y_max, char alignment_x, char alignment_y, struct argb_image* texture_background, struct argb_image* texture_slider, int slider_thickness);

void add_menu_text_field(struct menu_scene* scene, int z, char* buffer, int x_min, int x_max, int y, char alignment_x, char alignment_y, char text_alignment, bool* selected, struct char_font* font, bool* field_visible);

void menu_scene_frame(struct menu_scene* scene, int scale, unsigned int* screen, int width, int height, int mouse_x, int mouse_y, char click);

int menu_x(int x, int alignment, int scale, int width);

int menu_y(int y, int alignment, int scale, int height);