#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#define PAGESIZE getpagesize()
void * region;
void * heapStart;
// Functions
void heap_init(int num_pages_for_heap)
{
    // take pages and multiply x pagesize
    //printf("pagesize is: %d\n", PAGESIZE );
    int numBytesToAllocate = (num_pages_for_heap * PAGESIZE);
    //printf("num bytes required : %d\n", numBytesToAllocate );
    region = mmap(NULL, (PAGESIZE*num_pages_for_heap), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 );
    heapStart = region;
    printf("WORKING CODE/heap_init: region block from mmap: %p\n",region );
}

void *heap_alloc(int num_bytes_to_allocate)
{
    //int pages = 0;
    void * space;
    // int size = num_bytes_to_allocate;
    // if(size % PAGESIZE == 0){
    //   pages = size / PAGESIZE;
    // } else {
    //   pages = size / PAGESIZE + 1;
    // }
    printf("Allocate bytes: %d\n",num_bytes_to_allocate );
    space = mmap(region, num_bytes_to_allocate, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 );
    printf("WORKING CODE/heap_alloc: num bytes allocated %d at %p\n", num_bytes_to_allocate,space);
    printf("heapStart: %p  - current: %p = : %d\n", heapStart, space, (heapStart - space) );
    printf("How many pages: %d \n", (heapStart - space) / PAGESIZE );
    return space;
}

void heap_free(void *pointer_to_area_to_free)
{
    return;
}
