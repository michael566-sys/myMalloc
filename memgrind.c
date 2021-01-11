#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "mymalloc.h"

// Workload A: malloc() 1 byte and immediately free it - do this 120 times
void workload_A(){

    for ( int j = 0 ; j < 120 ; j ++ ) {
         void * ptr = (void*) malloc (1);
         free (ptr) ; 
    }

}

// Workload B: malloc() 1 byte, store the pointer in an array - do this 120 times.
void workload_B(){

    void ** array [120]; 
    for ( int j = 0 ; j < 120 ; j ++ ) {
        void * ptr = (void*) malloc (1); 
        array[j]  = ptr; 
    }
    // Once you've malloc()ed 120 byte chunks, then free() the 120 1 byte pointers one by one.

    for ( int j = 0 ; j < 120 ; j ++ ) {
        free (array[j]);
    }
}


//Workload D:
// malloc 10, 20, 30, 40, 40 into memories with each returning a pointer
// free pointers containing 20, 30, 40 bytes in the memory.
// then malloc 110 into memory. 
// then free the rest of the pointers. 

void workload_D () {
    for ( int j = 0 ; j < 50 ; j ++ ) {
        void * a = (void* ) malloc (10);
        void * b = (void* ) malloc (20);
        void * c = (void* ) malloc (30);
        void * d = (void* ) malloc (40);
        void * e = (void* ) malloc (40);
        free (b); 
        free(d); 
        free (c); 

        b = (void*) malloc ( 110);
        free (b);
        free(a);
        free(e);
    }
}




int main (int argc, char ** argv  ) {

 
//----------------------------------------------------------------------------------------------------------------------------------

    //variables that calculate the duration of operations for each program
    struct timeval begin, end;
    long long unsigned int time = 0 ; 
    // create a 2D array that stores all the runtime of each workload
    long long unsigned int all_run_time [6] [50]; 

    int i = 0 ; 
    int j = 0 ;
    
    // workloads: 
    // run all the workloads, one after the other, 50 times, with a for loop
    for ( i = 0 ; i < 50 ; i ++ ) {
        if (DEBUG2){
            printf ("Workload round %d\n\n", i );
        }

//------------------------------------------------------------------------
        // A: malloc() 1 byte and immediately free it - do this 120 times
        if (DEBUG2){
            printf ( "\nWorkload A:\n");
        }        gettimeofday(&begin, NULL);
        if (WLA){
            workload_A(); 
        }
       
        gettimeofday(&end, NULL);

        time = ((end.tv_sec * 1000000 + end.tv_usec) - (begin.tv_sec * 1000000 + begin.tv_usec));
            
        all_run_time [A][i] = time; 

        if (DEBUG2) {
            printf("Time taken for workload A is: %lld micro seconds\n", time);
        }
        // if (DEBUG){
        //     check_memory();
        // }

//------------------------------------------------------------------------

        // B: malloc() 1 byte, store the pointer in an array - do this 120 times.
        if (DEBUG2){
            printf ( "\nWorkload B:\n");
        }
        gettimeofday(&begin, NULL);
        if (WLB){
            workload_B();
        }

        gettimeofday(&end, NULL);


        time = ((end.tv_sec * 1000000 + end.tv_usec) - (begin.tv_sec * 1000000 + begin.tv_usec)); 
        all_run_time [B][i] = time; 

        if (DEBUG2) {
            printf("Time taken for workload B is: %lld micro seconds\n", time);
        }
        // if (DEBUG){
        //     check_memory();
        // }
//------------------------------------------------------------------------
        // C: 240 times, randomly choose between a 1 byte malloc() or free()ing one of the malloc()ed
        // pointers
        if (DEBUG2){
            printf ( "\nWorkload C:\n");
        }
        gettimeofday(&begin, NULL);
        // use wsC array of pointers to store 120 malloced pointers 
        void ** wsC [120]; 

        if (WLC) {
            for ( int k = 0 ; k < 120; k ++ ) {
                wsC[k] = NULL; 
            }
            int count_m = 0 ; 

            for ( j = 0 ; j < 240; j ++ ) {
                int r = rand()%2;
                if (DEBUG){
                    printf ( "random number: %d, \n",r);
                }
                    
                // if r == 1, means chose free(1)
                if (r) {
                    // check_empty() prevent free if have no malloc(1) being allocated 
                    if ( check_empty() ){
                        continue; 
                    }
                        int k = 0 ; 
                        // search for the first non-null poointer in wsC
                        for ( k = 0 ; k < 120; k ++ ){
                            if ( wsC[k] != NULL ) {
                                break; 
                            }
                        }
                    if (k < 120) {
                        if ( wsC[k] != NULL ) {
                            if (DEBUG){
                                printf("current freeing index: %d\n", k );
                            }
                            // free valid pointer
                            free (wsC[k]); 
                        }
                        wsC[k] = NULL;

                    }
                    
                }
                // if r == 0, means chose malloc(1) 
                else {
                    // keep track of the number of times of malloc(1) and make sure it happens exactly 120 times
                    if ( count_m < 120){
                        // store the pointer from malloc(1) into the wsC array
                        wsC[count_m] = (void*) malloc(1); 
                        if (DEBUG){
                            printf ("At memgrind.c: \n");
                            printf("current mallocing index: %d\n", count_m);
                            // check_memory();
                        }
                        count_m = count_m + 1 ; 
                    } 
                }
                
                if (DEBUG){
                    printf("\n");
                }
            // end of for loop
            }
        // end of WLC
        }
        gettimeofday(&end, NULL);

        time = ((end.tv_sec * 1000000 + end.tv_usec) - (begin.tv_sec * 1000000 + begin.tv_usec)); 
        all_run_time [C][i] = time; 

        // free any more pointers that hasn't been freed 
        for( int l = 0 ; l < 120 ; l ++ ) {
            if ( wsC[l] != NULL ) {
                if (DEBUG) {
                    printf ( "wsC index %d is not NULL\n", l);
                }
                free (wsC[l]);
            }
        }
        if (DEBUG2){
            printf("Time taken for workload C is: %lld micro seconds\n", time);
        }
        // if (DEBUG){
        //     // check_memory();
        // }
        // end of Workload C
//------------------------------------------------------------------------
        if (DEBUG2){
            printf ( "\nWorkload D:\n");
        }

        // malloc 10, 20, 30, 40, 40 into memories with each returning a pointer
        // free pointers containing 20, 30, 40 bytes in the memory.
        // then malloc 110 into memory. 
        // then free the rest of the pointers. 
        gettimeofday(&begin, NULL);
        if (WLD){
            workload_D(); 
            gettimeofday(&end, NULL);
        }

        time = ((end.tv_sec * 1000000 + end.tv_usec) - (begin.tv_sec * 1000000 + begin.tv_usec)); 
        all_run_time [D][i] = time; 

        if (DEBUG2) {
            printf("Time taken for workload D is: %lld micro seconds\n", time);
        }

        // if (DEBUG){
        //     check_memory();
        }
//------------------------------------------------------------------------
     
        // Workload E:
        if (DEBUG2){
            printf ( "\nWorkload E:\n");
        }
      
        // randomly pick size from 1 to 100 for 35 times
        // randomly pick between mallocing each randomly picked size and freeing one of the sizes
        // (Note: each of the 35 mallocs store the pointers into an array)
        // once sizes are picked and allocated 35 times, free until all sizes are freed. 
        gettimeofday(&begin, NULL);
        void ** wsE [35]; 

        if (WLE) {
            for ( int k = 0 ; k < 35; k ++ ) {
                wsE[k] = NULL; 
            }
            int count_m = 0 ; 

            for ( j = 0 ; j < 100; j ++ ) {
                // randomly pick size from 0 to 1 100 times
                int r = rand()%2;
                if (DEBUG){
                    printf ( "random number: %d, \n",r);
                }
                    
                // if r == 1, means chose free(1)
                if (r) {
                    if ( check_empty() ){
                        continue; 
                    }

                    int k = 0 ; 
                    for ( k = 0 ; k < 35; k ++ ){
                        if ( wsE[k] != NULL ) {
                        break; 
                        }
                    }
                    if (k < 35) {
                        if ( wsE[k] != NULL ) {
                            if (DEBUG){
                                printf("current freeing index: %d\n", k );
                            }
                                    
                            free (wsE[k]); 
                        }
                            wsE[k] = NULL;

                    }
                        
                }
                // if r == 0, means chose malloc(1) 
                else {
                    if ( count_m < 35){
                        int m = rand()%101;
                        wsE[count_m] = (void*) malloc(m); 
                            if (DEBUG){
                                printf ("At memgrind.c: \n");
                                printf("current mallocing index: %d\n", count_m);
                                // check_memory();
                            }
                        count_m = count_m + 1 ; 
                    } 
                }
                        
                if (DEBUG){
                    printf("\n");
                }
            }
        
        
            gettimeofday(&end, NULL);


            time = ((end.tv_sec * 1000000 + end.tv_usec) - (begin.tv_sec * 1000000 + begin.tv_usec)); 
            all_run_time [E][i] = time; 

            if (DEBUG) {
                for( int l = 0 ; l < 35 ; l ++ ) {
                    if ( wsE[l] != NULL ) {
                        printf ( "wsE index %d is not NULL\n", l);
                        free (wsE[l]);
                    }
                }
            }
                            
                            
            if (DEBUG2) {
                printf("Time taken for workload E is: %lld micro seconds\n", time);
            }                


            //  if (DEBUG){
            //     check_memory();
            // }
        }
//------------------------------------------------------------------------       
        // Workload F:
        if (DEBUG2){
            printf ( "\nWorkload F:\n");
        }
        gettimeofday(&begin, NULL);

        if (WLF) {

            void ** wlF [32]; 
            //allocate full 4096 bytes
            for ( int j = 0 ; j < 32 ; j ++ ){
                void * x = (void*)malloc (8);
                wlF[j] = x; 
                // if (DEBUG) {
                //     check_memory();
                // }
            }
            //free from two ends 
            int start = 0 ; 
            int last = 31; 
            while ( start < last ) {
                free (wlF[start]);
                free (wlF[last]); 
                start ++;
                last --; 
            }
            if (start == last) {
                free (wlF[last]); 
            }
    
            gettimeofday(&end, NULL);
            
            time = ((end.tv_sec * 1000000 + end.tv_usec) - (begin.tv_sec * 1000000 + begin.tv_usec)); 
            all_run_time [F][i] = time; 

            if (DEBUG2) {
                printf("Time taken for workload F is: %lld micro seconds\n", time);
            }

            // if (DEBUG){
            //     check_memory();
            // }
        }



        
    }// end of outer for loop that counts 50 times
   

    // calculate average:
    long long unsigned int sum_averge = 0 ; 
    char array [6] = "ABCDEF";
    for ( i = 0 ; i < 6; i ++ ) {
        for (j = 0 ; j < 50; j ++ ) {
            sum_averge = sum_averge + all_run_time [i][j] ;
        }
        printf( "Average run_time for workload %c is %lld micro seconds \n", array[i], sum_averge / 50 );
    }
    
//----------------------------------------------------------------------------------------------------------------------------------


    // error checking: 
    // ERROR_CHECKING is disabled for submission purposes
    if (ERROR_CHECKING) {
    // 1. Detectable Errors
    // Your malloc() and free() implementation should be able to catch at least the following errors:

    // A: Free()ing addresses that are not pointers:
       // int x ;
        // free( (int*)x );
        printf ("Test Case A: \n\n");
        int x =  0;
        free( &x );

        printf ( "addess of ptr: %p\n\n", &x );


    // B: Free()ing pointers that were not allocated by malloc():
        printf ("Test Case B.1 \n\n");

        char * p = (char *)malloc( 200 );
        free( p + 10 );
        // - or -
        printf ("Test Case B.2 \n\n");

        int * x1 = NULL ;
        free( x1 );
        // - or - 
        printf ("Test Case B.3 \n\n");
        p = (char *)malloc( 200 );
        free ( x1 ) ; // x1 is not allocated 

    // C: Redundant free()ing of the same pointer:
       printf ("Test Case C.1 \n\n");

        p = (char*)malloc(100);
        free( p );
        free( p );
        // ... is an error, 
        
        
        //but:
        printf ("Test Case C.2 \n\n");
        p = (char *)malloc( 100 );
        free( p );                      // error here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        p = (char *)malloc( 100 );  
        free( p );
        // ... is perfectly valid, even if malloc() returned the same pointer both times.


    // D: Saturation of dynamic memory:
        printf ("Test Case D \n\n");
        char*px = NULL; 
        px = (char*)malloc(5000);
        // - or -
        px = (char*)malloc(4096);
        char * q = (char*)malloc(1);
        free(q);
        // ... your code must gracefully handle being asked for more memory than it can allocate.



    // E: did not free enough elements
        //Eg 1: linked list: only freed the head but didn't free the reset of the nodes

        //Eg 2: linked list: free rest of the node but never free the head  
 
    
    // F: memory is allocated on top of each other/ conflict
        printf ("Test case F \n\n");
          void * a = (void* )   malloc ( 10 ) ;
          void * b = (void* )  malloc (10);
          a = b; 
          free(a); // this one we may not catch 
         // may be cause problems when freeing 

    // E: lossing track of the allocated memory 
        printf ("Test case E \n\n");
     
            void * a1 = ( void* ) malloc (77); 
            a1 = NULL; 
            free ( a1 ); 
        
    
    // dont need to check: G: segmentation fault: access a memory location that does not exist or not allowed to access
        printf ("Test case G \n\n");
            char *  p2 = (char*) malloc ( sizeof (char));
            *p2 = 'x'; 
            printf (  "%c", *p2 ) ; 
            free (p2); 
   

        printf ("Test case H \n\n");
            char *  p3 = (char*) malloc ( sizeof (char));
            char * p4 = (char*) malloc ( sizeof (Metadata));
            free (p3);
            free(p4);

            printf ("Test case I \n\n");
            char * p5 = (char*) malloc ( sizeof (size_t));
            char * p6 = p5;
            free (p5);
            free(p6);  

            
            printf ("Test case J \n\n");
            char * p7 = (char*) malloc ( sizeof (size_t));
            p7 = p7-1; 
            free(p7);

            printf ("Test case K \n\n");
            char * p8 = (char*) malloc (0);
            free(p8);


        printf ("Test case L \n\n");
        char* y = NULL; 
        char *c = y ; 
        free (y);
        free (c); //--> error    




    }
//----------------------------------------------------------------------------------------------------------------------------------



    return 0 ; 
}