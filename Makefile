## location of cxxtest
CXXTEST_HOME=extern/cxxtest-4.4

## external libraries
EXTERN_INCLUDES=
EXTERN_SOURCES=
EXTERN_CXXFLAGS=



## various options for possible build configurations
## comment in one of the following sets or add your own

CXXFLAGS=
LDFLAGS=

## for optimization
CXXFLAGS:=$(CXXFLAGS) -O2
LDFLAGS:=$(LDFLAGS)

## for gdb or valgrind
#CXXFLAGS:=$(CXXFLAGS) -Og -ggdb -fno-inline -fno-omit-frame-pointer
#LDFLAGS:=$(LDFLAGS)

## for gprof
#CXXFLAGS:=$(CXXFLAGS) -O2 -pg
#LDFLAGS:=$(LDFLAGS) -pg



## the following should not need to change

## generic options
CXXFLAGS:=$(CXXFLAGS) -std=c++11 -Wall -Werror -pedantic-errors -Isrc $(EXTERN_CXXFLAGS)
LDFLAGS:=$(LDFLAGS) -std=c++11

## platform-specific options
ifeq ($(OS),Windows_NT)
	CXXFLAGS:=$(CXXFLAGS) -I/usr/include/SDL2
	LDFLAGS:=$(LDFLAGS) -L/usr/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	DOXYGEN=doxygen
else ifeq ($(shell sh -c 'uname'),Darwin)
	CXXFLAGS:=$(CXXFLAGS) -F/Library/Frameworks -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_image.framework/Headers -I/Library/Frameworks/SDL2_mixer.framework/Headers -I/Library/Frameworks/SDL2_ttf.framework/Headers
	LDFLAGS:=$(LDFLAGS) -F/Library/Frameworks -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework SDL2_ttf
	ifneq ($(wildcard /Applications/Doxygen.app/Contents/Resources/doxygen),)
		DOXYGEN=/Applications/Doxygen.app/Contents/Resources/doxygen
	else
		DOXYGEN=doxygen
	endif
else
	CXXFLAGS:=$(CXXFLAGS) $(shell sh -c 'sdl2-config --cflags')
	LDFLAGS:=$(LDFLAGS) $(shell sh -c 'sdl2-config --libs') -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	DOXYGEN=doxygen
endif

## files
EXECUTABLE_SOURCE_FILES=$(shell sh -c '/usr/bin/find src -name "main*.cpp"')
SOURCE_FILES=$(filter-out $(EXECUTABLE_SOURCE_FILES),$(shell sh -c '/usr/bin/find src $(EXTERN_SOURCES) -name "*.cpp"'))
HEADER_FILES=$(shell sh -c '/usr/bin/find src -name "*.hpp"')
OBJECT_FILES=$(SOURCE_FILES:%.cpp=build/obj/%.o)
EXECUTABLES=$(patsubst src/%.cpp,bin/%,$(EXECUTABLE_SOURCE_FILES))

## test files
CXXTEST_GEN=$(CXXTEST_HOME)/bin/cxxtestgen
CXXTEST_INCLUDE=$(CXXTEST_HOME)

TEST_FILES=$(shell sh -c '/usr/bin/find test -name "*.cxxtest.hpp"')
TEST_SOURCE_FILES=$(TEST_FILES:test/%.cxxtest.hpp=build/gen/%.cxxtest.cpp) build/gen/runner.cpp
TEST_OBJECT_FILES=$(TEST_SOURCE_FILES:.cpp=.o)

CXXFLAGS:=$(CXXFLAGS) -I$(CXXTEST_INCLUDE) $(EXTERN_INCLUDES:%=-I%)

## rules
all: $(EXECUTABLES) bin/test

$(EXECUTABLES): bin/%: build/obj/src/%.o $(OBJECT_FILES)
	mkdir -p $(dir $@)
	g++  $^ $(LDFLAGS) -o $@

build/obj/%.o: %.cpp $(HEADER_FILES) Makefile
	mkdir -p $(dir $@)
	g++ -c $< $(CXXFLAGS) -o $@

ifneq ($(wildcard $(CXXTEST_HOME)),)

bin/test: $(TEST_OBJECT_FILES) $(OBJECT_FILES)
	mkdir -p $(dir $@)
	g++  $^ $(LDFLAGS) -o $@

.PRECIOUS: build/gen/%.cxxtest.cpp
build/gen/%.cxxtest.cpp: test/%.cxxtest.hpp
	mkdir -p $(dir $@)
	$(CXXTEST_GEN) --part --error-printer $< -o $@

build/gen/runner.cpp:
	mkdir -p $(dir $@)
	$(CXXTEST_GEN) --root --error-printer -o $@

else

bin/test:
	@echo "Cannot find CxxTest to build tests."

endif

doc: $(HEADER_FILES) $(SOURCE_FILES) Makefile
	$(DOXYGEN)
	touch doc

clean:
	rm -rf bin/ build/ doc/ *.gcda *.gcno *.gcov gmon.out
