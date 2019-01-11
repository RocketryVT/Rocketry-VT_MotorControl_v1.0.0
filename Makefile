CFLAGS = -std=c99 -Wall -Wpedantic -Wextra -g
CPPFLAGS = -std=c++11 -Wall -Wpedantic -Wextra -g

INCLUDE = -I include/
OBJ = build/


.PHONY: all clean

all: build/Assert.o

clean:
	@rm -rf build/ bin/ >/dev/null 2>/dev/null || true

$(OBJ)Assert.o: src/Assert.cpp include/Assert.h
	@mkdir -p build/ >/dev/null 2>/dev/null
	g++ $(CPPFLAGS) $^ -o $@ $(INCLUDE)

