CC:=gcc
IDIR:=./include/
CFLAGS:=-Wall -Wextra -Werror -I$(IDIR)

SRCS:= $(wildcard src/*.c)
OBJS:= $(SRCS:src/%.c=build/obj/%.o)
DEPS:= $(wildcard src/*.h)

LIBSLIDE:= bin/libslide.a

DEBUG_FLAGS:= -g -O0 -DDEBUG=1
RELEASE_FLAGS:= -O2 -DRELEASE=1

BUILD ?= debug

ifeq ($(BUILD),release)
    CFLAGS += $(RELEASE_FLAGS)
else
    CFLAGS += $(DEBUG_FLAGS)
endif

.PHONY: all clean 

all: clean $(LIBSLIDE) app

build/obj/%.o: src/%.c $(DEPS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBSLIDE): $(OBJS)
	@mkdir -p $(dir @a)
	ar rcs $(LIBSLIDE) $<

app:
	$(CC) $(CFLAGS) apps/main.c $(LIBSLIDE) -o bin/a.out

clean:
	rm -rf build bin
	mkdir build bin
