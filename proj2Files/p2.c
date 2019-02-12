#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#define PAGESIZE getpagesize()
#define ALIGNMENT 16
void * region;
int * heapStart;
int * end;
int * p;
int len;

typedef struct{
  struct block * next;
  char header;
  int size;
} block;

//will mask out last bit to zero
//int y = (x & -2);
// Functions
void heap_init(int num_pages_for_heap)
{
    unsigned int numBytesToAllocate = (num_pages_for_heap * PAGESIZE);
    region = mmap(NULL, (PAGESIZE*num_pages_for_heap), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 );
    heapStart = region;
    end = (heapStart + numBytesToAllocate);

    // Initialize the block:
    memset(region, 0, numBytesToAllocate);
    // Set last 16 as t;
    printf("WORKING CODE/heap_init: heapStart: %p + numBytesToAllocate: %d = end: %p\n", heapStart, numBytesToAllocate, end );
    printf("\n" );

}
// returns a pointer to a chunck of data at least as big as asked for. alligned to a 16 byte boundary
// if num_bytes_to_allocate = zero return null/ if unsuccessful returns null and sets errno
void *heap_alloc(int num_bytes_to_allocate)
{
      // pads the data requested in case it is not a multiple of 16
    if(num_bytes_to_allocate % ALIGNMENT != 0){ num_bytes_to_allocate += num_bytes_to_allocate%16; }
    //printf("ABOUT TO allocate: %d\n", num_bytes_to_allocate );
    len = num_bytes_to_allocate;
    p = heapStart;
    printf("pointer p %p\n", p );

    while( (p < end) && ((*p & 1) ||(*p <= len)) ) {
      printf("(*p & -2): %d\n", (*p & -2) );

      p = p + 100;//(*p & -2);
      printf("pointer p in while loop %p \n",p );
    }

    void * space = region;
    //printf("P is set to Start: %p\n" , p);
    //printf("NUm bytes allocated: %d\n",num_bytes_to_allocate );
    return space;




}
// returns bl;ock to pool of free memory
void heap_free(void *pointer_to_area_to_free)
{
    return;
}



void addBlock(int * p, int len){
  int newsize = ((len + 1) >> 1) << 1; // round up to even
  int oldsize = (*p & -2);// mask out low bit
  *p = newsize | 1;
  if (newsize < oldsize){
    *(p + newsize) = oldsize - newsize;
  }
}
