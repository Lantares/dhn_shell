main: main.o input.o scaner.o welcome.o paser.o run.o
	g++ -o main main.o input.o scaner.o welcome.o paser.o run.o

main.o: main.cpp input.h colour.h
	g++ -c main.cpp 

input.o: input.cpp input.h
	g++ -c input.cpp

scaner.o: scaner.cpp scaner.h colour.h
	g++ -c scaner.cpp

welcome.o: welcome.cpp welcome.h colour.h
	g++ -c welcome.cpp

paser.o: paser.cpp paser.h colour.h
	g++ -c paser.cpp

run.o: run.cpp run.h colour.h
	g++ -c run.cpp

clean:
	rm main main.o input.o scaner.o welcome.o paser.o run.o 
