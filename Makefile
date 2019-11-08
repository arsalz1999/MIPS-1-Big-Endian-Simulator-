bin/mips_simulator: main.o memory.o compiler.o register.o
	mkdir -p bin
	g++ main.o memory.o compiler.o register.o -o bin/mips_simulator

main.o: main.cpp
	g++ -c main.cpp

compiler.o: compiler.cpp compiler.hpp
	g++ -c compiler.cpp

memory.o: memory.cpp memory.hpp
	g++ -c memory.cpp

register.o: register.cpp register.hpp
	g++ -c register.cpp

simulator: bin/mips_simulator

#run:
#	./simulator.exe

clean:
	rm *.o bin/mips_simulator -f
