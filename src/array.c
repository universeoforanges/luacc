#include <luacc/array.h>
#include <luacc/log.h>

#include <stdlib.h>
#include <string.h>

array_t *luacc_alloc_array(void)
{
	array_t *arr = (array_t *) malloc(sizeof(array_t));

	if (!arr)
	{
		luacc_log(LUACC_LOG_LEVEL_VERBOSE, "luacc_alloc_array() warning: memory allocation failure");
		return NULL;
	}
	
	arr->size = 5;
	arr->data = malloc(sizeof(void *) * arr->size);
	arr->len = 0;

	if (!arr->data)
	{
		luacc_log(LUACC_LOG_LEVEL_VERBOSE, "luacc_alloc_array() warning: memory allocation failure");
		return NULL;
	}

	return arr;
}

void luacc_free_array(array_t *arr)
{
	if (!arr)
	{
		luacc_log(LUACC_LOG_LEVEL_VERBOSE, "luacc_free_array() warning: attempt to free a null array");
		return;
	}

	free(arr->data);
	free(arr);
}

void luacc_array_append(array_t *arr, const void *item)
{
	if (arr->len >= arr->size)
	{
		arr->len *= 2;
		arr->data = realloc(arr->data, sizeof(void*) * arr->len);

		if (!arr->data)
		{
			luacc_log(LUACC_LOG_LEVEL_VERBOSE, "luacc_array_append() warning: memory allocation failure during array resize");
			return;
		}
	}

	arr->data[arr->len++] = item;
}

void luacc_array_remove(array_t *arr, const size_t idx)
{
	if (idx >= arr->len || idx <= 0)
	{
		luacc_log(LUACC_LOG_LEVEL_VERBOSE, "luacc_array_remove() warning: invalid array index");
		return;
	}

	arr->len--;
	memmove(&arr->data[idx], &arr->data[idx + 1], sizeof(void*) * (arr->len - idx));
}

size_t luacc_array_find(array_t *arr, const void *item)
{
	for (int i = 0; i < arr->len; i++)
		if (arr->data[i] == item) return i;
	
	return LUACC_ARRAY_INVALID_IDX; // -1
}
