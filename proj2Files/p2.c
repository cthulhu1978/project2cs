#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#define PAGESIZE getpagesize()
#define ALIGNMENT 16
void * region;
void * heapStart;
unsigned char * heapEnd;
unsigned char * p;

typedef struct{
  struct block * next;
  char header;
  int size;
} block;

//will mask out last bit
//int y = (x & -2);
// Functions
void heap_init(int num_pages_for_heap)
{
    unsigned int numBytesToAllocate = (num_pages_for_heap * PAGESIZE);
    region = mmap(NULL, (PAGESIZE*num_pages_for_heap), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 );
    heapStart = region;
    heapEnd = (heapStart + numBytesToAllocate);
    // Initialize the block:
    memset(region, 0, numBytesToAllocate);
    // Set last 16 as t;
    printf("WORKING CODE/heap_init: region block from mmap: %p\n",region );
    printf("WORKING CODE/heap_init: heapStart: %p + numBytesToAllocate: %d = heapEnd: %p\n", heapStart, numBytesToAllocate, heapEnd );
    printf("\n" );
}
// returns a pointer to a chunck of data at least as big as asked for. alligned to a 16 byte boundary
// if num_bytes_to_allocate = zero return null/ if unsuccessful returns null and sets errno
void *heap_alloc(int num_bytes_to_allocate)
{
      // pads the data requested in case it is not a multiple of 16
    if(num_bytes_to_allocate % ALIGNMENT != 0){ num_bytes_to_allocate += num_bytes_to_allocate%16; }
    printf("ABOUT TO allocate: %d\n", num_bytes_to_allocate );

    p = region;
    printf("P is set to Start: %p\n" , p);
    printf("NUm bytes allocated: %d\n",num_bytes_to_allocate );
    return p;




}
// returns bl;ock to pool of free memory
void heap_free(void *pointer_to_area_to_free)
{
    return;
}
