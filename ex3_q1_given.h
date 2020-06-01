#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>     // for exit()
#include <string.h>     // for strlen()
#include <sys/types.h>
#include <unistd.h>    // for sleep(), execvp(), pp()
#include <sys/wait.h>   // for wait()
#include <semaphore.h>
#include <fcntl.h>
#include <time.h> 

// function prototypes
bool is_prime(int);
bool is_twin_prime(int, int*, int*);
bool is_cousin_prime(int, int*, int*);
void print_twin_primes(int, int);
void print_cousin_primes(int, int);
int get_random(int lo, int hi);
void my_sleep();

//  Semaphore relaed functions
void open_all_sem();
bool is_twin_done();
bool is_cousin_done();
extern sem_t* sem_twin;
extern sem_t* sem_cousin;


#define SLEEP_FACTOR    1000  // used by my_sleep
#define PRIME_CHECKERS 4   // number of processes which check prime numbers
#define TWIN_COUNT     2
#define COUSIN_COUNT   6
