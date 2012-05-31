
#include "tw_array.h"

#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include <assert.h>


#define ARRAY_INITIAL_SIZE  4
#define ARRAY_GROWTH        1.5f



static void reconstruct_ptrs(Array* array)
{
    realloc(array->ptrs, array->count);
//
//    char* counter = (char*)array->data;
//    for (int i = 0; i < array->count; i++)
//    {
//        array->ptrs[i] = (char*)counter;
//        for (size_t cp = 0; cp < array->struct_size; cp++)
//            counter++;
//    }

}


static void assure_space(Array *array)
{
	if (array->count + 1 > (int)array->allocated)
	{
		array->allocated = (size_t)(array->allocated * ARRAY_GROWTH);
        array->data = (char*)realloc(array->data, array->allocated * array->struct_size);

        reconstruct_ptrs(array);
	}
}



Array *array_init(size_t struct_size)
{
	//start by creating the array struct
	Array *array = malloc(sizeof(Array));

	//init some variables
	array->struct_size = struct_size;
	array->data = 0;
	array->ptrs = 0;

	//then call clear to setup the arrays memory
	array_clear(array);

	return array;
}


void array_free(Array *array)
{
	//delete the array first
	if (array->data)
	{
	    free(array->ptrs);
		free(array->data);
	}

	//then free the array struct
	free(array);
}


int array_in_range(Array *array, size_t index)
{
    //NOTE: PUT AN ASSERT HERE
	return (int)index <= array->count;
}


void *array_at(Array *array, size_t index)
{
	if (array_in_range(array, index))
	{
		size_t count = (index * array->struct_size);
		return (array->data + count);
	}

	printf("(Array_At) out of bounds!\n");
	return 0;
}


void *array_end(Array *array)
{
	int count = (array->count * array->struct_size) - array->struct_size;
	return (array->data + count);
}


void array_insert(Array *array, const void *data, size_t index)
{
	if ((int)index > array->count)
	{
		printf("(Array_Insert) out of range!\n");
		return;
	}

	assure_space(array);

	void *dest = array->data + ((index + 1) * array->struct_size);
	void *source = array_at(array, index);
	size_t size = (array->count - index) * array->struct_size;

	//move everything after index right one element
	memmove(dest, source, size);

	//place the new data
	memcpy(array_at(array, index), data, array->struct_size);

	//then mark that we have added a new element
	array->count++;
}



void array_erase(Array *array, size_t index)
{
	if (!array_in_range(array, index))
	{
		printf("(Vector_Erase) out of bounds!\n");
		return;
	}
	else
	{
	    //"destination" is the cell were erasing
		void *dest = array_at(array, index);
		//"source" is the cell after were erasing
		void *source = array->data + ((index + 1) * array->struct_size);
		//"size" is the size of the array after the cell were erasing
		size_t size = (array->count - index) * array->struct_size;

        //shift everything after the cell were erasing into said cell
		memmove(dest, source, size);

        //were now tracking one less cell
		array->count--;
	}
}



void array_erase_list(Array *array, Array *index_array)
{
    int n;
	size_t i;
	for (i = array->count; i > 0; i--)
	{
		for (n = 0; n < index_array->count; n++)
		{
			if (i == *(size_t*)array_at(index_array, (size_t)n))
				array_erase(array, i);
		}
	}
}


void array_swap(Array *a, int index_1, int index_2)
{
	char *i1, *i2, *tmp;
	tmp = malloc(a->struct_size);
//	i1 = (char*)array_at(a, index_1);
//	i2 = (char*)array_at(a, index_2);
    i1 = &((char*)a->data)[index_1];
    i2 = &((char*)a->data)[index_2];

	memcpy(tmp, i1, a->struct_size);
	memcpy(i1, i2, a->struct_size);
	memcpy(i2, tmp, a->struct_size);

	free(tmp);
}


void array_push(Array *array, const void *data)
{
	array_insert(array, data, array->count);
}


void array_pop(Array *array)
{
	if (array->count > 0)
	{
		memset(array_end(array), 0, array->struct_size);
		array->count--;
	}
}


void array_clear(Array *array)
{
	if (array->data)
		free(array->data);

	array->count = 0;
	array->allocated = ARRAY_INITIAL_SIZE;
	array->data = (char*)calloc(ARRAY_INITIAL_SIZE, array->struct_size);
}


//array_reserve grows the reserved memory
//NOTE: UNTESTED
void array_reserve(Array *array, size_t size)
{
	array->allocated = size;
    array->data = (char*)realloc(array->data, array->allocated * array->struct_size);
}


//array_resize grows the actual count of items in the list
//NOTE: UNTESTED
void array_resize(Array *array, size_t size)
{
	array->count = size;
    if (array->count > (int)array->allocated)
    {
        array->allocated = array->count;
        array->data = (char*)realloc(array->data, array->allocated * array->struct_size);
    }
}


////- put an assert in the is_in_bounds function.
////- add search index to array struct
//
//void* array_search(Array* array, void* search_datem)
//{
//    array->search_index = 0;
//    //store sentinel
//}
//
////void* array_next(Array* array)
////{
////}
