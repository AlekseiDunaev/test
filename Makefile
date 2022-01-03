CC=gcc
FLAGS=-Wall -pedantic
INC=-Isrc/
CFLAGS=$(FLAGS) -c -g --std=gnu99 $(INC)
LFLAGS=$(FLAGS) -lncurses
DIR_GUARD=@mkdir -p $(@D)

# Sources and Objects
SOURCES=$(shell find src/ -type f -name "*.c")
OBJECTS=$(patsubst src/%.c,obj/%.o,$(SOURCES))
DEPS=$(patsubst src/%.c,deps/%.d,$(SOURCES))

# Main targets
.PHONY: all clean clean_all print

all: bin/main

#GTAGS: $(SOURCES)
#	gtags

clean:
	rm -rf obj/* bin/* src/*.gch GTAGS GPATH GRTAGS

clean_all:
	rm -rf bin/* obj/* deps/*

# --- Link Rule
bin/main: $(OBJECTS)
	$(DIR_GUARD)
	$(CC) $(OBJECTS) $(LFLAGS) -o bin/main

# --- Compile Rule
obj/%.o: src/%.c
	$(DIR_GUARD)
	$(CC) $(CFLAGS) $< -o $@

# --- Dependency Rule
deps/%.d: src/%.c
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -MM $< | sed -e 's/~\(.*\)\.o:/\1.d \1.o:/' > $@

print:
	@echo "DIR_GUARD:"
	@echo ""$(DIR_GUARD)""
	@echo ""
	@echo "SOURCES FILES:"
	@echo ""$(SOURCES)""
	@echo ""
	@echo "OBJECTS FILES:"
	@echo ""$(OBJECTS)""
	@echo ""
	@echo "DEPS:"
	@echo ""$(DEPS)""
	@echo ""
	@echo "MAKECMDGOALS:"
	@echo ""$(MAKECMDGOALS)""

ifneq "$(MAKECMDGOALS)" "clean_all"
-include $(DEPS)
endif

