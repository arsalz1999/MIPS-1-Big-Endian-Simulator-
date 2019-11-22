simulator: src/main.o src/memory.o src/simulator.o src/register.o
	mkdir -p bin
	g++ src/main.o src/memory.o src/simulator.o src/register.o -o bin/mips_simulator

testbench:
	mkdir -p bin
	echo '#!/bin/bash\npython3 testbench.py $$1' > bin/mips_testbench
	chmod +x bin/mips_testbench


main.o: src/main.cpp
	g++ -c src/main.cpp

simulator.o: src/simulator.cpp src/simulator.hpp
	g++ -c src/simulator.cpp

memory.o: src/memory.cpp src/memory.hpp
	g++ -c src/memory.cpp

register.o: src/register.cpp src/register.hpp
	g++ -c src/register.cpp

clean:
	rm *.o bin/mips_simulator -f
