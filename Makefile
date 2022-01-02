CC=gcc-9
TARGET=test
LIB=ncurses

.PHONY: run clean

$TARGET: *.c
	$(CC) -g3 -o $(TARGET) $(TARGET).c -l$(LIB)

run: $TARGET
	./$(TARGET)

r: run

clean:
	rm $(TARGET) *.o

