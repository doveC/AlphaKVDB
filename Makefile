CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRC_FILES = main.cpp
HEADER_FILES = SkipList.h

main: $(SRC_FILES) $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) $(SRC_FILES) -o main

.PHONY: clean

clean:
	rm -f main
