# Default settings.
override CFLAGS += -std=c11
O = o
RM = rm

# User commands.

default: tests

objects: trie.$(O)

tests: ../unit/unit.c trie.unit.c trie.c
	$(CC) $(CFLAGS) $(LDFLAGS) -I. -o $@ $^

clean:
	$(foreach OBJ, $(wildcard *.$(O)), $(RM) $(OBJ) $(\n))
	$(foreach BIN, $(wildcard tests),  $(RM) $(BIN) $(\n))

# Non-user commands.

trie.$(O): trie.c
	$(CC) -c $(CFLAGS) -o $@ $<

trie.c: trie.h
../unit/unit.c: ../unit/unit.h

.PHONY: default all clean

define \n


endef
