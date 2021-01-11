# myMalloc

Individual project from CS214 System Programming.
It implements malloc() and free() library calls for dynamic memory
allocation that detect common programming and usage errors.

# mymalloc function at mymalloc.c

- allocate memories in a 4096 bytes character array that simulates memory
- allocate memories with a metadata in front to store the size of the allocated memory, 
whether the memory is freed or not, and pointer to the next metadata
- detect error when not enough storage occurs
- merge adjacent freed memory blocks into one

#myfree function at mymalloc.c

- free allocated memory
- detect error of Free()ing addresses that are not pointers
- detect error of Free()ing pointers that were not allocated by malloc()
- detect error of Redundant free()ing of the same pointer

# Testcases:

Workload A

    malloc() 1 byte and immediately free it - do this 120 times
    
Workload B

    malloc() 1 byte, store the pointer in an array - do this 120 times.
    Once malloc()ed 120 byte chunks, then free() the 120 1 byte pointers one by one.
    
Workload C

    240 times, randomly choose between a 1 byte malloc() or free()ing one of the malloc()ed pointers
    Keep track of each operation so that eventually malloc() 120 bytes, in total
    Keep track of each operation so that eventually free() all pointers
    
Workload D

    malloc 10, 20, 30, 40, 40 into memories with each returning a pointer
    free pointers containing 20, 30, 40 bytes in the memory.
    then malloc 110 into memory. 
    then free the rest of the pointers. 

    Reason: Workload D is to test whether malloc can handle
    merging 3 consecutive freed memories. 
    Allocating 110 bytes also tests the edge case whereby 
    there is enough space to allocate bytes, but not enough
    space to split the space into two blocks.

Workload E

    randomly pick size from 1 to 100 for 35 times
    randomly pick between mallocing each randomly picked size 
    OR freeing one of the sizes
    (Note: each of the 35 mallocs store the pointers into an array)
    once sizes are picked an allocated 35 times, free until all sizes are freed. 

    Reason: developed on top of workload C. the purpose is to malloc 
    and free memories with random sizes. Since the maximum that the 35 times
    can pick is 35 * 100 = 3500, so 3500 + 35 * sizeof (Metadata) = 3500 + 35 * 16 = 4060. 
    Since 4060 < 4096, so there is enough space in the memory array to run Workload E. 

Workload F

    allocate full 4096 bytes with 32 malloc (8)
    free from two ends till freeing everything

    Reason: allocate all the memory space to check whether the memory is able to
    be fully loaded. 
    Freeing from two ends test for the edge cases of freeing the two ends, also 
    merging freed blocks from both sides
