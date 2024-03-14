CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

SRC_FILES = main.cpp
HEADER_FILES = SkipList.h

main: $(SRC_FILES) $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) $(SRC_FILES) -o bin/main

stress: test/StressTest.cpp $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) test/StressTest.cpp -o bin/stress

.PHONY: clean

clean:
	rm -f bin/main bin/stress
