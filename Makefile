CC = g++
CFLAGS = -Wall -Wextra -std=c++11
LDFLAGS = -lncurses

SRC = main.cpp

EXECUTABLE = ninja_game

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean