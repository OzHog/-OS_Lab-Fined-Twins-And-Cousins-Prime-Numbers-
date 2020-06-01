#include "ex3_q1.h"

int main(int argc, char *argv[])
{
    int hi = atoi(argv[argc - 1]);
    int lo = atoi(argv[argc - 2]);

    open_pips_and_sem();

    //------create prime checkers------
    for (int i = 0; i < PRIME_CHECKERS; i++)
    {
        if (fork() == 0)
        {
            close_pips_by_type(PRIME_CHECKER, NO_USE);
            primes_checker(i);
            my_sleep();
            close_pips_by_type(PRIME_CHECKER, USE);
            exit(EXIT_SUCCESS);
        }
    }

    //------create twin producer------
    if (fork() == 0)
    {
        //change std input to be twin_pip
        dup2(twin_pip[READ_PIP], INPUT_STD);

        //close no use pips by twin producer
        close_pips_by_type(TWIN, NO_USE);

        producer(TWIN);
        my_sleep();

        exit(EXIT_SUCCESS);
    }

    //------create cousin producer------
    if (fork() == 0)
    {
        //chnage std input to be cousin_pip
        dup2(cousin_pip[READ_PIP], INPUT_STD);

        //close no needed pips by cousin producer
        close_pips_by_type(COUSIN, NO_USE);
        producer(COUSIN);
        my_sleep();

        exit(EXIT_SUCCESS);
    }

    //------random numbers producer------
    int rand_num = 0;

    close_pips_by_type(MAIN, NO_USE);

    while (!is_produce_done())
    {
        rand_num = get_random(lo, hi);
        write_to_pip_by_type(PRIME_CHECKER, rand_num);
        my_sleep();
    }

    close_pips_by_type(MAIN, USE);

    //waiting all child processes terminated
    for (int i = 0; i < 2 + PRIME_CHECKERS; i++)
        wait(NULL);

    printf("All child processes terminated. Exiting\n");

    return EXIT_SUCCESS;
}

void primes_checker(int checker_num)
{
    int nbytes;
    int num = 0;
    bool twin_done = false, cousin_done = false;
    bool did_write = false;

    while (!((twin_done = is_twin_done()) && (cousin_done = is_cousin_done())))
    {
        // my_sleep();
        if ((nbytes = read(random_nums_pip[READ_PIP], &num, N)) > 0)
        {
            if (is_prime(num))
            {
                if (!twin_done)
                    did_write = write_to_pip_by_type(TWIN, num);

                if (!cousin_done)
                    write_to_pip_by_type(COUSIN, num);
            }
        }
    }

    // printf("prime checker %d DONE\n", checker_num);
}

void producer(enum PROCESS type)
{
    int status;
    while (!is_produce_done_by_type(type))
    {
        if (fork() == 0)
        {
            if (type == TWIN)
                execv(TWIN_PRIME_PATH, NULL);
            else
                execv(COUSIN_PRIME_PATH, NULL);
        }
        else
        {
            wait(NULL);

            // -----check exit code of child-----
            // EXIT_SUCCESS = child find twin/cousin primes
            // EXIT_FAILURE = child didn't find twin/cousin primes and inputs end
            if (WEXITSTATUS(status) == EXIT_SUCCESS)
            {
                decrease_sem(type);
                my_sleep();
            }
        }
    }

    //     if (type == TWIN)
    //         printf("twin producer DONE\n");
    //     else
    //         printf("cousin producer DONE\n");
}

void open_pips_and_sem()
{
    open_all_sem();

    if (pipe(random_nums_pip) == -1)
    {
        fprintf(stderr, "cannot open random_nums_pip\n");
        exit(EXIT_FAILURE);
    }

    if (pipe(twin_pip) == -1)
    {
        fprintf(stderr, "cannot open twin_pip\n");
        exit(EXIT_FAILURE);
    }

    if (pipe(cousin_pip) == -1)
    {
        fprintf(stderr, "cannot open cousin_pip\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGPIPE, SIG_IGN);
    // signal(SIGPIPE, sigpipe_handler); // notify kernel about my signal handler
}

void close_pips_by_type(enum PROCESS type, enum CLOSE_PIP to_close)
{

    switch (type)
    {
    case PRIME_CHECKER:
        if (to_close == USE)
        {
            close(random_nums_pip[READ_PIP]);
            close(twin_pip[WRITE_PIP]);
            close(cousin_pip[WRITE_PIP]);
        }
        else // NO_USE
        {
            close(random_nums_pip[WRITE_PIP]);
            close(twin_pip[READ_PIP]);
            close(cousin_pip[READ_PIP]);
        }

        break;

    case TWIN:
    case COUSIN:
        if (to_close == NO_USE)
        {
            close(random_nums_pip[READ_PIP]);
            close(random_nums_pip[WRITE_PIP]);
            close(twin_pip[READ_PIP]);
            close(twin_pip[WRITE_PIP]);
            close(cousin_pip[READ_PIP]);
            close(cousin_pip[WRITE_PIP]);
        }

        break;

    case MAIN:
        if (to_close == USE)
        {
            close(random_nums_pip[WRITE_PIP]);
        }
        else
        {
            close(twin_pip[READ_PIP]);
            close(cousin_pip[READ_PIP]);
            close(random_nums_pip[READ_PIP]);
            close(cousin_pip[WRITE_PIP]);
            close(twin_pip[WRITE_PIP]);
        }

    default:
        break;
    }
}

void sigpipe_handler(int s)
{
    fprintf(stderr, "*** %s *** got signal sigpipe s=%d\n", __func__, s);
}

bool is_produce_done_by_type(enum PROCESS type)
{
    if (type == TWIN)
        return is_twin_done();
    else
        return is_cousin_done();
}

bool is_produce_done()
{
    return (is_twin_done() && is_cousin_done());
}

void decrease_sem(enum PROCESS type)
{
    if (type == TWIN)
        sem_wait(sem_twin);
    else
        sem_wait(sem_cousin);
}

//write to pip by enum PROCESS, if SUCCESS return true else false
bool write_to_pip_by_type(enum PROCESS pip, int value)
{
    switch (pip)
    {
    case PRIME_CHECKER:
        if (write(random_nums_pip[WRITE_PIP], &value, sizeof(value)) <= 0)
            return false;
        break;

    case TWIN:
        if (write(twin_pip[WRITE_PIP], &value, sizeof(value)) <= 0)
            return false;
        break;

    case COUSIN:
        if (write(cousin_pip[WRITE_PIP], &value, sizeof(value)) <= 0)
            return false;
        break;

    default:
        break;
    }

    return true;
}
