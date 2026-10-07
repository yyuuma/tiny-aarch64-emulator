CXX = g++

CXXFLAGS = -Wall -Wextra

main:
	$(CXX) $(CXXFLAGS) main.cpp

clean:
	rm -rf *.out
