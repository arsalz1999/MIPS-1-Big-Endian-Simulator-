bin/mips_simulator: main.o memory.o simulator.o register.o
	mkdir -p bin
	g++ main.o memory.o simulator.o register.o -o bin/mips_simulator

main.o: main.cpp
	g++ -c main.cpp

simulator.o: simulator.cpp simulator.hpp
	g++ -c simulator.cpp

memory.o: memory.cpp memory.hpp
	g++ -c memory.cpp

register.o: register.cpp register.hpp
	g++ -c register.cpp

simulator: bin/mips_simulator

#run:
#	./simulator.exe

clean:
	rm *.o bin/mips_simulator -f
