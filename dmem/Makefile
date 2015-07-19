# Default settings.
ifeq ($(OS),)
OS := $(shell uname)
endif

ifeq ($(OS),Darwin)
override CFLAGS += -D_POSIX_C_SOURCE=200112
else
override CFLAGS += -std=c11
endif
O = o
RM = rm

# User commands.

default: tests

objects: dmem.$(O)

tests: ../unit/unit.c dmem.unit.c dmem.c
	$(CC) $(CFLAGS) $(LDFLAGS) -I. -o $@ $^

clean:
	$(foreach OBJ, $(wildcard *.$(O)), $(RM) $(OBJ) $(\n))
	$(foreach BIN, $(wildcard tests),  $(RM) $(BIN) $(\n))

# Non-user commands.

dmem.$(O): dmem.c
	$(CC) -c $(CFLAGS) -o $@ $<

dmem.c: dmem.h
../unit/unit.c: ../unit/unit.h

.PHONY: default all clean

define \n


endef
