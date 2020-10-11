flags=-Wno-deprecated -g -ggdb
comp=g++
prog=GVRP+MM

#El problema
config=Instancias/AB101.dat
resultados=Out.txt
semilla=123
restarts=1000
debug=0


comp:$(prog)

run:$(prog)
	./$(prog) $(config) $(resultados) $(semilla) $(restarts) $(debug)

$(prog): main.o
	$(comp) main.o -o $(prog) $(flags)

main.o: main.cpp includes.h
	$(comp) -c main.cpp $(flags)

clear:
	rm -f *~
	rm -f *.o
	rm -f $(prog)
	rm -f core*

