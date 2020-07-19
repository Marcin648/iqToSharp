CXX=g++
all:
ifeq ($(OS),Windows_NT)
	$(CXX) -Wl,--stack,8388608 -Wall -Wextra -O3 -static-libstdc++ -static-libgcc -static iqToSharp.cpp -o iqToSharp.exe
else
	$(CXX) -Wl,--stack,8388608 -Wall -Wextra -O3 iqToSharp.cpp -o iqToSharp
endif

debug:
ifeq ($(OS),Windows_NT)
	$(CXX) -g -Wl,--stack,8388608 -Wall -Wextra -O0 -static-libstdc++ -static-libgcc -static iqToSharp.cpp -o iqToSharp.exe
else
	$(CXX) -g -Wl,--stack,8388608 -Wall -Wextra -O0 iqToSharp.cpp -o iqToSharp
endif
