#ifndef _ARRAY_H
#define _ARRAY_H

#include <stddef.h>

#define LUACC_ARRAY_INVALID_IDX -1

typedef struct
{
	void **data;

	size_t size;
	size_t len;
} array_t;

// array allocation & freeing
array_t *luacc_alloc_array(void); // allocates an array with a default size of 5
void luacc_free_array(array_t *arr);

// array management
void luacc_array_resize(array_t *arr, const size_t new_size);
void luacc_array_append(array_t *arr, const void *item);
void luacc_array_insert(array_t *arr, const array_t *arr0, const size_t len);
void luacc_array_raw_insert(array_t *arr, void *arr0, const size_t len);
void luacc_array_remove(array_t *arr, const size_t idx);
size_t luacc_array_find(array_t *arr, const void *item);

#endif // _ARRAY_H
