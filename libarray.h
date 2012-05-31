#ifndef LIBARRAY_H_
#define LIBARRAY_H_

#ifndef uint
#define uint unsigned int
#endif

typedef struct Array
{
	char*   data;
	int     count;
	uint    struct_size;
	uint    allocated;
	char*   ptrs;
} Array;



extern Array* array_init(uint struct_size);
extern void   array_free(Array* array);

extern int    array_in_range(Array* array, uint index);
extern void*  array_at(Array* array, uint index);
extern void*  array_end(Array* array);

extern void   array_insert(Array* array, const void* data, uint index);
extern void   array_erase(Array* array, uint index);
extern void   array_erase_list(Array* array, Array* index_array);
extern void   array_swap(Array* a, int index_1, int index_2);

extern void   array_push(Array* array, const void* data);
extern void   array_pop(Array* array);

extern void   array_clear(Array* array);
extern void   array_reserve(Array* array, uint size);
extern void   array_resize(Array* array, uint size);

#endif
