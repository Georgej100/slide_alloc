CC:=gcc
IDIR:=-I./include/ -I./src/
CFLAGS:=-Wall -Wextra -Werror -lpthread $(IDIR)

SRCS:= $(wildcard src/*.c)
DEPS:= $(wildcard src/*.h)

LIBSLIDE:= bin/libslide.a

DEBUG_FLAGS:= -g -fsanitize=thread -O0 -DDEBUG=1
RELEASE_FLAGS:= -O2 -DRELEASE=1

BUILD ?= debug

ifeq ($(BUILD),release)
    CFLAGS += $(RELEASE_FLAGS)
	OBJS:= $(SRCS:src/%.c=build/obj/release/%.o)
	TMP_DIR:= ./build/obj/release bin
else
    CFLAGS += $(DEBUG_FLAGS)
	OBJS:= $(SRCS:src/%.c=build/obj/debug/%.o)
	TMP_DIR:= ./build/obj/debug bin
endif

.PHONY: all clean 

all: clean $(LIBSLIDE) app

build/obj/debug/%.o: src/%.c $(DEPS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/obj/release/%.o: src/%.c $(DEPS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBSLIDE): $(OBJS)
	@mkdir -p $(dir @a)
	ar rcs $(LIBSLIDE) $(OBJS)

app:
	$(CC) $(CFLAGS) apps/main.c $(LIBSLIDE) -o bin/a.out

clean:
	rm -rf $(TMP_DIR)
	mkdir $(TMP_DIR)
