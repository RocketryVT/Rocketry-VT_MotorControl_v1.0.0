SRC = src/
INCLUDE = include/
OBJ = build/
BIN = bin/

CFLAGS = -std=c99 -Wall -Wpedantic -Wextra -g -I $(INCLUDE)
CPPFLAGS = -std=c++11 -Wall -Wpedantic -Wextra -g -I $(INCLUDE)

SRC_FILES = $(wildcard $(SRC)*.cpp)
OBJ_FILES = $(subst $(SRC),$(OBJ),$(subst .cpp,.o,$(SRC_FILES)))

.PHONY: all clean

all: $(OBJ_FILES)

clean:
	@rm -rf $(OBJ) $(BIN) >/dev/null 2>/dev/null || true

$(OBJ)%.o: $(SRC)%.cpp $(INCLUDE)%.h
	@mkdir -p $(OBJ) >/dev/null 2>/dev/null
	g++ $(CPPFLAGS) $< -c -o $@

