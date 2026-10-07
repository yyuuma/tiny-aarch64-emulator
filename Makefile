CXX = clang++

CXXFLAGS = -Wall -Wextra -std=c++20

main:
	$(CXX) $(CXXFLAGS) main.cpp

clean:
	rm -rf *.out
