CXX = g++
CFLAGS = -I/usr/include -I.
LDFLAGS = -lmosquittopp

main: 
	${CXX} ${CFLAGS} main.cpp mqtt.cpp -o main ${LDFLAGS}
	
clean:
	rm -rf *.o
	rm -rf main