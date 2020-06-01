#ifndef EX3_Q1_H_
#define EX3_Q1_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "ex3_q1_given.h"

int random_nums_pip[2];
int twin_pip[2];
int cousin_pip[2];

enum PROCESS
{
    PRIME_CHECKER,
    TWIN,
    COUSIN,
    MAIN
};
enum CLOSE_PIP
{
    USE,
    NO_USE
};

void primes_checker(int checker_num);
void open_pips_and_sem();
void close_pips_by_type(enum PROCESS type, enum CLOSE_PIP to_close);
void sigpipe_handler(int s);
void producer(enum PROCESS type);
bool is_produce_done_by_type(enum PROCESS producer);
bool is_produce_done();
void decrease_sem(enum PROCESS type);
bool write_to_pip_by_type(enum PROCESS pip, int value);

#define N sizeof(int)
#define READ_PIP 0
#define WRITE_PIP 1
#define TWIN_PRIME_PATH "./twin_prime"
#define COUSIN_PRIME_PATH "./cousin_prime"
#define INPUT_STD 0
#define OUTPUT_STD 1

#endif