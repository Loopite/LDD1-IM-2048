NOM_APPLICATION       = 2048
NOM_APPLICATION_TESTS = gametests

CXX_FLAGS              = -std=c++17 -Wall -Wextra
CPP_FLAGS              = -I ./libs/include/SDL2 -I ./libs/include/SDL2_ttf
LD_FLAGS               = -L ./libs/lib/SDL2 -L ./libs/lib/SDL2_ttf
LINKER_FLAGS           = -lSDL2 -lSDL2_ttf

all: $(NOM_APPLICATION) $(NOM_APPLICATION_TESTS)

$(NOM_APPLICATION): 2048.o modele.o
	$(CXX) $(CXX_FLAGS) $(CPP_FLAGS) $(LD_FLAGS) -o $(NOM_APPLICATION) 2048.o modele.o $(LINKER_FLAGS)

$(NOM_APPLICATION_TESTS): tests/tests.o modele.o
	$(CXX) $(CXX_FLAGS) -o $(NOM_APPLICATION_TESTS) tests/tests.o modele.o

2048.o: 2048.cpp modele.hpp
	$(CXX) $(CXX_FLAGS) $(CPP_FLAGS) -c 2048.cpp

modele.o: modele.cpp modele.hpp
	$(CXX) $(CXX_FLAGS) -c modele.cpp

tests/tests.o: tests/tests.cpp modele.hpp
	$(CXX) $(CXX_FLAGS) -c tests/tests.cpp -o tests/tests.o

clean:
	rm -f $(NOM_APPLICATION) $(NOM_APPLICATION_TESTS) *.o ./tests/*.o

.PHONY: all clean