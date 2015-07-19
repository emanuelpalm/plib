## Default settings.
override CFLAGS += -std=c11
O = o
RM = rm

## User commands.

default: tests

objects: unit.$(O)

tests: unit.unit.c unit.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	$(foreach OBJ, $(wildcard *.$(O)), $(RM) $(OBJ) $(\n))
	$(foreach BIN, $(wildcard tests),  $(RM) $(BIN) $(\n))

## Non-user commands.

unit.$(O): unit.c
	$(CC) -c $(CFLAGS) -o $@ $<

unit.c: unit.h

.PHONY: default all clean

define \n


endef
