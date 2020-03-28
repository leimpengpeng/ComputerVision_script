CXX = g++ -std=c++11
CFLAGS = -I/usr/include -I.
LDFLAGS = -lmosquittopp -lglut  -lGL -lGLU -lm 

main: 
	${CXX} ${CFLAGS} GLcountingEvent.cpp mqtt.cpp -o main ${LDFLAGS}
	
clean:
	rm -rf *.o
	rm -rf main
