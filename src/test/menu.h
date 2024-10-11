#pragma once

#include "general/argb_image.h"
#include "general/platformlib/platform.h"

enum alignment {
	MENU_ALIGNMENT_LEFT,
	MENU_ALIGNMENT_RIGHT,
	MENU_ALIGNMENT_TOP,
	MENU_ALIGNMENT_BOTTOM,
	MENU_ALIGNMENT_MIDDLE,
};

enum menu_item_type {
	MENU_ITEM_LABEL,
	MENU_ITEM_IMAGE,
	MENU_ITEM_SLIDER,
	MENU_ITEM_TEXT_FIELD,
};

struct menu_label {
	int x;
	int y;
	char alignment_x;
	char alignment_y;
	char text_alignment_x;
	char text_alignment_y;
	struct gui_char* text;
};

struct menu_image {
	int x;
	int y;
	char alignment_x;
	char alignment_y;
	char image_alignment_x;
	char image_alignment_y;
	int image;
	int image_scalar;
};

struct menu_slider {
	float state;
	int x_min;
	int y_min;
	int x_max;
	int y_max;
	char alignment_x;
	char alignment_y;
	int texture_background;
	int texture_slider;
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
	char* selected;
	int font;
	char* field_visible;
};


struct menu_item {
	union {
		struct menu_label label;
		struct menu_image image;
		struct menu_slider slider;
		struct menu_text_field text_field;
	} items;
	int z;
	char menu_item_type;
};

void menu_frame(struct menu_item** menu_items, int menu_items_count, unsigned int* screen, int width, int height, int scale, void** resource_map);