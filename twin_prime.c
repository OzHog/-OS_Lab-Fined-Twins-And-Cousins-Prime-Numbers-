
#include "ex3_q1_given.h"

#define READ 0
#define WRITE 1
#define N sizeof(int)

int main(int argc, char *argv[])
{
    int input_num, n1, n2;
    int nbytes;

    while ((nbytes = read(READ, &input_num, N)) > 0)
    {
        // printf("twin prime recieved: %d\n", input_num);
        if (is_twin_prime(input_num, &n1, &n2))
        {
            print_twin_primes(n1, n2);
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}