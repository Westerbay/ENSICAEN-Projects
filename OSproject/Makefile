CC = gcc
CPPFLAGS = -Wall -Wextra -O2 
CFLAGS = -I include

LIB_SHAREDUTILS = lib/libsharedutils.a

SRCFILES_CITIZEN = $(wildcard src/citizen_manager/*.c)
SRCFILES_EPIDEMIC = $(wildcard src/epidemic_sim/*.c)
SRCFILES_PRESS = $(wildcard src/press_agency/*.c)
SRCFILES_TIMER = $(wildcard src/timer/*.c)
SRCFILES_LAUNCHER = $(wildcard src/launcher/*.c)
SRCFILES_SHAREDUTILS = $(wildcard src/sharedutils/*.c)
SRCFILES_TESTUTIL = $(wildcard src/test_sharedutils/*.c)

OBJFILES_CITIZEN = $(patsubst src/%.c, obj/%.o, $(SRCFILES_CITIZEN))
OBJFILES_EPIDEMIC = $(patsubst src/%.c, obj/%.o, $(SRCFILES_EPIDEMIC))
OBJFILES_PRESS = $(patsubst src/%.c, obj/%.o, $(SRCFILES_PRESS))
OBJFILES_TIMER = $(patsubst src/%.c, obj/%.o, $(SRCFILES_TIMER))
OBJFILES_LAUNCHER = $(patsubst src/%.c, obj/%.o, $(SRCFILES_LAUNCHER))
OBJFILES_SHAREDUTILS = $(patsubst src/%.c, obj/%.o, $(SRCFILES_SHAREDUTILS))
OBJFILES_TESTUTIL = $(patsubst src/%.c, obj/%.o, $(SRCFILES_TESTUTIL))


.PHONY : all clean distclean exec test cleantest interface cleandev doc cleandoc


all : cleandev exec test venv interface

exec : lib/libsharedutils.a bin/timer bin/citizen_manager bin/press_agency bin/epidemic_sim  bin/launcher 
test : bin/test

cleandev:
	@rm -rf /dev/shm/os_project_memory
	@rm -rf /dev/mqueue/press_agency

interface:	
	python3 -m zipapp src/interface/ -o bin/interface.pyz

venv:
	python3 -m venv venv
	. venv/bin/activate && pip install pygame
	. venv/bin/activate && pip install posix_ipc
	
lib/libsharedutils.a : $(OBJFILES_SHAREDUTILS)
	@mkdir -p lib
	ar crv $@ $^

bin/test : $(OBJFILES_TESTUTIL) $(LIB_SHAREDUTILS)
	@mkdir -p bin
	$(CC) $^ -o $@ -lm

bin/timer : $(OBJFILES_TIMER) $(LIB_SHAREDUTILS)
	@mkdir -p bin
	$(CC) $^ -o $@ 
	
bin/citizen_manager : $(OBJFILES_CITIZEN) $(LIB_SHAREDUTILS)
	@mkdir -p bin
	$(CC) $^ -o $@ -lpthread 
	
bin/press_agency : $(OBJFILES_PRESS) $(LIB_SHAREDUTILS)
	@mkdir -p bin
	$(CC) $^ -o $@ 
	
bin/epidemic_sim : $(OBJFILES_EPIDEMIC) $(LIB_SHAREDUTILS)
	@mkdir -p bin
	$(CC) $^ -o $@ 
	
bin/launcher : $(OBJFILES_LAUNCHER) 
	@mkdir -p bin
	$(CC) $^ -o $@  
	
obj/press_agency/%.o : src/press_agency/%.c
	@mkdir -p obj
	@mkdir -p obj/press_agency
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c
	
obj/timer/%.o : src/timer/%.c
	@mkdir -p obj
	@mkdir -p obj/timer
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c
	
obj/epidemic_sim/%.o : src/epidemic_sim/%.c
	@mkdir -p obj
	@mkdir -p obj/epidemic_sim
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c
	
obj/citizen_manager/%.o : src/citizen_manager/%.c
	@mkdir -p obj
	@mkdir -p obj/citizen_manager
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c
	
obj/launcher/%.o : src/launcher/%.c
	@mkdir -p obj
	@mkdir -p obj/launcher
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c
	
obj/sharedutils/%.o : src/sharedutils/%.c
	@mkdir -p obj
	@mkdir -p obj/sharedutils
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c
	
obj/test_sharedutils/%.o : src/test_sharedutils/%.c
	@mkdir -p obj
	@mkdir -p obj/test_sharedutils
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

doc :
	doxygen doc/Doxyfile

clean:
	@rm -rf obj/

distclean: clean 
	@rm -rf bin
	@rm -rf lib
	@rm -rf venv
	
cleandoc:
	@rm -rf doc/html

cleantest:
	@rm -rf bin/test
	
	
