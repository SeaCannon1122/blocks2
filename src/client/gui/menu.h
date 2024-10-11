#pragma once

#include "client/gui/argb_image.h"
#include "general/platformlib/platform.h"
#include "client/gui/pixel_char.h"

#ifndef ALIGNMENTS
#define ALIGNMENTS

#define ALIGNMENT_LEFT   0
#define ALIGNMENT_RIGHT  1
#define ALIGNMENT_TOP    2
#define ALIGNMENT_BOTTOM 3
#define ALIGNMENT_MIDDLE 5

#endif // !ALIGNMENTS

#define MAX_MENU_ITEMS 64

enum menu_item_type {
	MENU_ITEM_LABEL,
	MENU_ITEM_IMAGE,
	MENU_ITEM_SLIDER,
	MENU_ITEM_TEXT_FIELD,
};

struct menu_label {
	int menu_item_type;
	int z;
	int x;
	int y;
	int alignment_x;
	int alignment_y;
	int text_alignment_x;
	int text_alignment_y;
	int max_width;
	int max_rows;
	int selectable;
	int text_size;
	struct pixel_char text[];
};

struct menu_image {
	int menu_item_type;
	int z;
	int x;
	int y;
	int alignment_x;
	int alignment_y;
	int image_alignment_x;
	int image_alignment_y;
	int image;
	int image_scalar;
};

struct menu_slider {
	int menu_item_type;
	int z;
	float state;
	int x_min;
	int y_min;
	int x_max;
	int y_max;
	int alignment_x;
	int alignment_y;
	int texture_background;
	int texture_slider;
	int slider_thickness;
};

struct menu_text_field {
	int menu_item_type;
	int z;
	char* buffer;
	int x_min;
	int x_max;
	int y;
	int alignment_x;
	int alignment_y;
	int text_alignment;
	char* selected;
	int font;
	char* field_visible;
};

struct gui_menu {
	int select_label;
	int current_pos;
	int select_begin;
	int select_end;
	int selecting;

	int items_count;
	int* items[];
};

void menu_frame(struct gui_menu* menu, unsigned int* screen, int width, int height, int scale, const void** resource_map, int mouse_click, int mouse_x, int mouse_y);