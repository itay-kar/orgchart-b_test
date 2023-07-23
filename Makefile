# Makefile for C++ project with debugging option

CXX=clang++-9
CXXVERSION=c++2a
SOURCE_PATH=sources
OBJECT_PATH=objects
CXXFLAGS=-std=$(CXXVERSION) -Werror -Wsign-conversion -I$(SOURCE_PATH)
TIDY_FLAGS=-extra-arg=-std=$(CXXVERSION) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=*
VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99

# Add debugging flags
DEBUG_FLAGS=-g -O0

SOURCES=$(wildcard $(SOURCE_PATH)/*.cpp)
HEADERS=$(wildcard $(SOURCE_PATH)/*.hpp)
OBJECTS=$(subst sources/,objects/,$(subst .cpp,.o,$(SOURCES)))

run: test

test: TestRunner.o StudentTest1.o StudentTest2.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $^ -o $@

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) --compile $< -o $@

$(OBJECT_PATH)/%.o: $(SOURCE_PATH)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) --compile $< -o $@

# Renana Rimon
StudentTest1.cpp:  
	curl https://raw.githubusercontent.com/renanarimon/cpp_5b_test/master/Test.cpp > $@

# Shauli Taragin
StudentTest2.cpp: 
	curl https://raw.githubusercontent.com/ShauliTaragin/Orgchart-A/main/Test.cpp > $@

tidy:
	clang-tidy $(SOURCES) $(HEADERS) $(TIDY_FLAGS) --

valgrind: test
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./test 2>&1 | { egrep "lost| at " || true; }

# New target for debugging
debug: test
	gdb ./test

clean:
	rm -f $(OBJECTS) *.o test* 
	rm -f StudentTest*.cpp