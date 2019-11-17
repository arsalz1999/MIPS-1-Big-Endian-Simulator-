bin/mips_simulator: main.o memory.o simulate.o register.o
	mkdir -p bin
	g++ main.o memory.o simulate.o register.o -o bin/mips_simulator

main.o: main.cpp
	g++ -c main.cpp

simulate.o: simulate.cpp simulate.hpp
	g++ -c simulate.cpp

memory.o: memory.cpp memory.hpp
	g++ -c memory.cpp

register.o: register.cpp register.hpp
	g++ -c register.cpp

simulator: bin/mips_simulator

#run:
#	./simulator.exe

clean:
	rm *.o bin/mips_simulator -f
