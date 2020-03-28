CXX = g++ -std=c++11
CFLAGS = -I/usr/include -I. `pkg-config --cflags opencv`
LDFLAGS = `pkg-config --libs opencv`

main: 
	${CXX} ${CFLAGS} readIpcam.cpp  -o readIpcam ${LDFLAGS}
	
clean:
	rm -rf *.o
	rm -rf readIpcam
