#include "resource_loader.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "keyvalue.h"
#include "stddef.h"


void* load_file(char* filename, int* size) {

    FILE* file = fopen(filename, "rb");
    if (file == NULL) return NULL;

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize);
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }
   
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);

    fclose(file);

    *size = fileSize;

    return buffer;
}

char* load_text_file(char* filename) {

    FILE* file = fopen(filename, "rb");
    if (file == NULL) return NULL;

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char* buffer_raw = (char*)malloc(fileSize);
    if (buffer_raw == NULL) {
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(buffer_raw, sizeof(char), fileSize, file);

    int carriage_return_count = 0;

    for (int i = 0; i < fileSize; i++) if (buffer_raw[i] == '\r') carriage_return_count++;

    char* buffer = (char*)malloc(fileSize + 1 - carriage_return_count);

    int i = 0;

    for (int raw_i = 0; raw_i < fileSize; raw_i++) {
        if (buffer_raw[raw_i] != '\r') {
            buffer[i] = buffer_raw[raw_i];
            i++;
        }
    }

    buffer[fileSize - carriage_return_count] = '\0';
    free(buffer_raw);

    fclose(file);

    return buffer;
}

int compare_resource_manager_entrys(const void* a, const void* b) {
    return strcmp(((struct key_value_map_entry*)a)->key, ((struct key_value_map_entry*)b)->key);
}

struct key_value_map* load_key_value_map(char* src) {

    char* data = load_text_file(src);
    if (data == NULL) return NULL;
    int pairs_count = 0;

    for (int i = 0; data[i] != '\0'; i++) if (data[i] == '\n') pairs_count++;

    struct key_value_map* map = malloc(sizeof(struct key_value_map) + sizeof(struct key_value_map_entry) * pairs_count);
    struct key_value_map_entry* mappings = (struct key_value_map_entry*)((long long)map + sizeof(struct key_value_map));

    map->mappings_count = pairs_count;
    map->mappings = mappings;

    int line_begin_index = 0;

    for (int i = 0; i < pairs_count; i++) {
        int j = line_begin_index;

        for (; data[j] != ' '; j++) mappings[i].key[j - line_begin_index] = data[j];
        mappings[i].key[j - line_begin_index] = '\0';
        j += 3;
        int value_start = j;

        char temp_value[MAX_VALUE_SIZE];

        for (; data[j] != '\n'; j++) temp_value[j - value_start] = data[j];
        temp_value[j - value_start] = '\0';


        if (data[value_start - 2] == 's') {
            for (int k = 0; k < j - value_start + 1; k++) mappings[i].value.s[k] = temp_value[k];
            mappings[i].value_type = VALUE_STRING;
        }
        else if (data[value_start - 2] == 'i') {
            mappings[i].value.i = string_to_int(temp_value, j - value_start);
            mappings[i].value_type = VALUE_INT;
        }
        else if (data[value_start - 2] == 'f') {
            mappings[i].value.f = string_to_float(temp_value, j - value_start);
            mappings[i].value_type = VALUE_FLOAT;
        }

        line_begin_index = j + 1;
    }

    qsort(map->mappings, map->mappings_count, sizeof(struct key_value_map_entry), compare_resource_manager_entrys);

    free(data);

    return map;

}