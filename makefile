


CFLAGS = -Wall
CC = gcc -std=c99
ECHO = echo "going to compile for target $@"
OS := $(shell uname)


ifneq (,$(findstring CYGWIN_NT,$(OS)))
EXT = .exe
else
EXT =
endif

PROG = ex3_q1$(EXT)
GIVEN = ex3_q1_given$(EXT)
TWIN = twin_prime$(EXT)
COUSIN = cousin_prime$(EXT)

all: $(GIVEN) $(TWIN) $(COUSIN) $(PROG) test

print:
	@echo MAIN_O: $(PROG)

clean:
	#rm -r $(PWD)/$(PROG)
	rm -vf *.o $(PROG) $(GIVEN) $(TWIN) $(COUSIN) *.log *.tmp

test:
	@echo going to run test...
	./$(PROG) 1000 2000

$(PROG): ex3_q1.c
	$(ECHO)
	$(CC) $(CFLAGS) $^ -o $@ ex3_q1_given

$(GIVEN): ex3_q1_given.c
	$(ECHO)
	$(CC) -c $(CFLAGS) $^ -o $@

$(TWIN): twin_prime.c
	$(ECHO)
	$(CC) $(CFLAGS) $^ -o $@ ex3_q1_given

$(COUSIN): cousin_prime.c
	$(ECHO)
	$(CC) $(CFLAGS) $^ -o $@ ex3_q1_given