CXX=g++
all:
ifeq ($(OS),Windows_NT)
	$(CXX) -Wall -Wextra -O3 -static-libstdc++ -static-libgcc -static iqToSharp.cpp -o iqToSharp.exe
else
	$(CXX) -Wall -Wextra -O3 iqToSharp.cpp -o iqToSharp
endif
