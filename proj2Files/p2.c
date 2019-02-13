#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#define PAGESIZE getpagesize()
#define ALIGNMENT 16
#define GET(p)       (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))
#define PACK(size, alloc) ((size) | (alloc))
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
void * region;
int * heapStart;
int * end;
int * p;
int len;
int * next;

//will mask out last bit to zero ->>>>  int y = (x & -2);
void heap_init(int num_pages_for_heap)
{
    int numBytesToAllocate = (num_pages_for_heap * PAGESIZE);
    region = mmap(NULL, (numBytesToAllocate), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 );
    heapStart = region;
    end = (heapStart + numBytesToAllocate);

    // Initialize memory region to zero
    for(int i = 0; i < numBytesToAllocate; i++){ *(heapStart+i) & 0; }

    // Initialize whole memory with size and unallocated:
    PUT(heapStart, PACK(numBytesToAllocate, 0));

    printf("WORKING CODE/heap_init: heapStart: %p + numBytesToAllocate: %d = end: %p\n", heapStart, numBytesToAllocate, end );
    printf("\n" );

}

// returns a pointer to a chunck of data at least as big as asked for. alligned to a 16 byte boundary
// if num_bytes_to_allocate = zero return null/ if unsuccessful returns null and sets errno
void *heap_alloc(int num_bytes_to_allocate)
{
      // pads the data requested in case it is not a multiple of 16
    if(num_bytes_to_allocate % ALIGNMENT != 0){
        len += num_bytes_to_allocate%16;
      }

    p = heapStart;

    printf("GET_SIZE p: %d\n",GET_SIZE(p) );
    printf("GET_ALLOC p: %d\n",GET_ALLOC(p) );

    //First FIt algorithm???
    while( (p < end)
            &&  ((*p & 1)
            ||  (*p <= len)) )
    {
      p = p + (*p & -2);
      //printf("P at time 50-> %p :  %d\n",p, *p );
    }
    printf("Heapstart:%p : heapend: %p\n",heapStart, end );
    return p;
}

// returns bl;ock to pool of free memory
void heap_free(void *pointer_to_area_to_free)
{
    return;
}


// allocate a free block if needs are less than required
void addBlock(int * p, int len){
  int newsize = ((len + 1) >> 1) << 1; // round up to even
  int oldsize = (*p & -2);// mask out low bit
  *p = newsize | 1; // set new length
  if (newsize < oldsize){ // set length in remaining part of the block
    *(p + newsize) = oldsize - newsize;
  }
}

void free_block(int * p){
  *p = *p & -2; // clear allocated flag
  next = p + *p; // find next block
  if( (*next & 1) == 0){
    *p = *p + *next; // add to    this block if not allocated
  }
}
