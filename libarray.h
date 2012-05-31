#ifndef LIBARRAY_H_
#define LIBARRAY_H_

#include "stdio.h"

typedef struct Array
{
	char*   data;
	int     count;
	size_t  struct_size;
	size_t  allocated;
	char*   ptrs;
} Array;



extern Array* array_init(size_t struct_size);
extern void   array_free(Array* array);

extern int    array_in_range(Array* array, size_t index);
extern void*  array_at(Array* array, size_t index);
extern void*  array_end(Array* array);

extern void   array_insert(Array* array, const void* data, size_t index);
extern void   array_erase(Array* array, size_t index);
extern void   array_erase_list(Array* array, Array* index_array);
extern void   array_swap(Array* a, int index_1, int index_2);

extern void   array_push(Array* array, const void* data);
extern void   array_pop(Array* array);

extern void   array_clear(Array* array);
extern void   array_reserve(Array* array, size_t size);
extern void   array_resize(Array* array, size_t size);

#endif
