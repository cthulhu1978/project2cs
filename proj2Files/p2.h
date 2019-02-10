#ifndef P2_H
#define P2_H



void heap_init(int num_pages_for_heap);

void *heap_alloc(int num_bytes_to_allocate);

void heap_free(void *pointer_to_area_to_free);



#endif
