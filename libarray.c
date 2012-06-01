
#include <tw_array.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define ARRAY_INITIAL_SIZE  4
#define ARRAY_GROWTH        1.5f



static void assure_space(Array *array)
{
	if (array->count + 1 > (int)array->allocated)
	{
		array->allocated = (uint)(array->allocated * ARRAY_GROWTH);
        array->data = (char*)realloc(array->data, array->allocated * array->struct_size);
	}
}



Array *array_init(uint struct_size)
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


int array_in_range(Array *array, uint index)
{
	return (int)index <= array->count;
}


void *array_at(Array *array, uint index)
{
    assert(array_in_range(array, index));
    uint count = (index * array->struct_size);
    return (array->data + count);
}


void *array_end(Array *array)
{
	int count = (array->count * array->struct_size) - array->struct_size;
	return (array->data + count);
}


void array_insert(Array *array, const void *data, uint index)
{
    assert(array_in_range(array, index));

	assure_space(array);

	void *dest = array->data + ((index + 1) * array->struct_size);
	void *source = array_at(array, index);
	uint size = (array->count - index) * array->struct_size;

	//move everything after index right one element
	memmove(dest, source, size);

	//place the new data
	memcpy(array_at(array, index), data, array->struct_size);

	//then mark that we have added a new element
	array->count++;
}



void array_erase(Array *array, uint index)
{
    assert(array_in_range(array, index));
    
    //"destination" is the cell were erasing
    void *dest = array_at(array, index);
    //"source" is the cell after were erasing
    void *source = array->data + ((index + 1) * array->struct_size);
    //"size" is the size of the array after the cell were erasing
    uint size = (array->count - index) * array->struct_size;

    //shift everything after the cell were erasing into said cell
    memmove(dest, source, size);

    //were now tracking one less cell
    array->count--;
}


//Pass an Array of "uint"s as indecies into array to erase
void array_erase_list(Array *array, Array *index_array)
{
    int n;
	uint i;
	for (i = array->count; i > 0; i--)
	{
		for (n = 0; n < index_array->count; n++)
		{
			if (i == *(uint*)array_at(index_array, (uint)n))
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
		// memset(array_end(array), 0, array->struct_size);
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
void array_reserve(Array *array, uint size)
{
	array->allocated = size;
    array->data = (char*)realloc(array->data, array->allocated * array->struct_size);
}


//array_resize grows the actual count of items in the list
void array_resize(Array *array, uint size)
{
	array->count = size;
    if (array->count > (int)array->allocated)
    {
        array->allocated = array->count;
        array->data = (char*)realloc(array->data, array->allocated * array->struct_size);
    }
}