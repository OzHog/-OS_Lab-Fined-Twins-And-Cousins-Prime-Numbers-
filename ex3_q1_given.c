
#include "ex3_q1_given.h"

//------------------------------------------------------------------------
// Global variables
sem_t* sem_twin;
sem_t* sem_cousin;


// ---------- get_random -----------
int get_random(int lo, int hi)
{
	static int first_time = 1;
	if (first_time) 
	{
		srand(time(NULL) * getpid());
		first_time = 0;
	}
	return ((rand() % (hi - lo +1)) + lo);
}

// ---------- is_prime -----------
bool is_prime(int num) // not really needed for this exercise
{
	int i; 
	for (i=2; i <= (int)sqrt(num); i++)
		if (num % i == 0 ) return false;
	return true;		
}

// ---------- short_sleep -----------
void short_sleep()
{
	int factor = getpid() %5 + 1;
	usleep(factor * SLEEP_FACTOR);
}

// ---------- TWIN or COUSIN primes functions -----------
enum TWIN_OR_COUSIN {TWIN = 2, COUSIN = 4};
bool is_twin_or_cousin(int num, int *n1, int *n2, enum TWIN_OR_COUSIN t_or_c)
{
  if (! is_prime(num))
    return false;
  int delta = (int) t_or_c;
  if (is_prime(num + delta))
  {
    *n1 = num;
    *n2 = num + delta;
    return true;
  }
  if ((num - delta) > 1 && (is_prime(num - delta)))
  {
    *n1 = num - delta;
    *n2 = num;
    return true;
  }
  return false;
}

bool is_twin_prime(int num, int *n1, int *n2)
{
  return is_twin_or_cousin(num, n1, n2, TWIN);
}

bool is_cousin_prime(int num, int *n1, int *n2)
{
  return is_twin_or_cousin(num, n1, n2, COUSIN);
}

void print_twin_primes(int p1, int p2)
{
  printf("TWIN Primes: %d, %d\n", p1, p2);
}
void print_cousin_primes(int p1, int p2)
{
  printf("COUSIN Primes: %d, %d\n", p1, p2);
}


//------------------------------------------------------------------------
void my_sleep()
{
  if (SLEEP_FACTOR == 0) 
    return;
  static int delay = 5;;
  delay++;
  delay = delay % 5 + getpid() % 20;
  usleep(SLEEP_FACTOR * delay);
}
//------------------------------------------------------------------------

//  Semaphore relaed functions
// sem_t* open_sem(char* sem_name, int count, bool unlink)
sem_t* open_sem(char* sem_name, int count)
{
  // if (unlink) {
  if (sem_unlink(sem_name) == 0)
    fprintf(stderr, "successul unlink of %s." "going to open with count=%d\n", sem_name, count);
    
	sem_t* sem_ptr = sem_open(sem_name, O_CREAT, S_IRWXU, count);
	if (sem_ptr == SEM_FAILED)
	{
		perror("failed to open semaphore");
    fprintf(stderr, "failed to open semaphore %s. Going to Exit.\n", sem_name);
		exit(EXIT_FAILURE);
	}

  return sem_ptr;
} 

void open_all_sem()
{
  sem_twin = open_sem("sem_twin", TWIN_COUNT);
  sem_cousin = open_sem("sem_cousin", COUSIN_COUNT);
}
  
bool is_done(sem_t* sem)
{
  if (sem_trywait(sem) == 0)
  {
    sem_post(sem);
    return false;
  }
  return true;
}

bool is_twin_done()
{
  return is_done(sem_twin);
}

bool  is_cousin_done()
{
  return is_done(sem_cousin);
}