#include "hedgelib/hl_list.h"
#include <string.h>
#include <stdlib.h>

void* hlINListReserve(void* HL_RESTRICT data,
    size_t size, size_t count, size_t capacity,
    size_t desiredCapacity)
{
    /* If capacity != 0, the list owns its data pointer. Just realloc. */
    if (capacity)
    {
        /* Reallocate the list's data buffer. */
        return hlRealloc(data, size * desiredCapacity);
    }

    /* If capacity == 0, the list does not own its data pointer. Alloc and copy. */
    else
    {
        void* newData;

        /* Allocate a new buffer for the list. */
        newData = hlAlloc(size * desiredCapacity);
        if (!newData) return NULL;

        /* Copy the existing data (if any) into the new buffer and return new buffer. */
        memcpy(newData, data, size * count);
        return newData;
    }
}

void* hlINListGrow(void* HL_RESTRICT data,
    size_t size, size_t count, size_t* HL_RESTRICT capacity)
{
    /* If capacity != 0, the list owns its data pointer. Just realloc. */
    if (*capacity)
    {
        /* Double the list's capacity. */
        *capacity *= 2;

        /* Reallocate the list's data buffer. */
        return hlRealloc(data, size * (*capacity));
    }

    /* If capacity == 0, the list does not own its data pointer. Alloc and copy. */
    else
    {
        void* newData;

        /* Set the list's new capacity. */
        *capacity = (count) ? (count * 2) : 1;

        /* Allocate a new buffer for the list. */
        newData = hlAlloc(size * (*capacity));
        if (!newData) return NULL;

        /* Copy the existing data (if any) into the new buffer and return new buffer. */
        memcpy(newData, data, size * count);
        return newData;
    }
}

void hlINListRemove(void* data, size_t size,
    size_t count, size_t index)
{
    /*
       Move everything after the value we specified "back" by one.

       Example:

       = Remove(1)
       Indices:     0 1 2 3
       Before:      A B C D
       After:       A C D
                      ^

       count:  3   (NOTE: We subtracted 1 from count before calling this function.)
       index:  1

       A B C D % % % %
         ^
        dst

       A B C D % % % %
         --^
       dst + size

       [We're moving count - index (2) values from dst + size to dst.]
    */
    HlU8* dst = (((HlU8*)data) + (size * index));
    memmove(dst, dst + size, size * (count - index));
}

static int hlINOffTableCompareOffsets(const void* a, const void* b)
{
    const size_t off1 = *(const size_t*)a;
    const size_t off2 = *(const size_t*)b;

    return ((off1 > off2) - (off1 < off2));
}

void hlOffTableSort(HlOffTable* offTable)
{
    /* Sort offsets in offset table. */
    qsort(offTable->data, offTable->count,
        sizeof(size_t), hlINOffTableCompareOffsets);
}
