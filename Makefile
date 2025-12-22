CXX := g++
DEBUG_COMPILER_FLAGS := -std=c++20 -Wall -Wextra -Iinclude -g
RELEASE_COMPILER_FLAGS := -std=c++20 -Iinclude -O2 
export DEBUG_COMPILER_FLAGS
export RELEASE_COMPILER_FLAGS

COMPILER_FLAGS := $(RELEASE_COMPILER_FLAGS)
export COMPILER_FLAGS

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,obj/%.o,$(SRCS))

CLEX_OBJS := $(filter-out obj/tests.o obj/main.o, $(OBJS))


TESTS := bin/test
MAIN := bin/calculexdora


# Default rule
all: setup $(TESTS) $(MAIN)

tests: setup $(TESTS)
main: setup $(MAIN)

# Link
$(TESTS): $(CLEX_OBJS) obj/tests.o
	$(CXX) $(COMPILER_FLAGS) -o $@ $^

$(MAIN): $(CLEX_OBJS) obj/main.o
	$(CXX) $(COMPILER_FLAGS) -o $@ $^

# Compile each source file into obj/
obj/%.o: src/%.cpp
	$(CXX) $(COMPILER_FLAGS) -c $< -o $@

# Clean project
clean:
	rm -rf obj/*.o bin/*

# Optional: run program
run: $(MAIN)
	./$(MAIN)

setup:
	mkdir -p bin/ obj/

.PHONY: all clean run

