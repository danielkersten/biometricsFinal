CC = g++
INCS = -I/usr/local/include/opencv -I/usr/local/include
LDFLAGS=-g
LDLIBS = -L/usr/local/lib -lopencv_highgui
CPPFLAGS=-g -D_REENTRANT

camCapture: camCapture.o CameraCapture.o
	$(CC) $(LDFLAGS) -o camCapture camCapture.o CameraCapture.o $(LDLIBS)

camCapture.o: camCapture.cpp CameraCapture.h
	$(CC) $(CPPFLAGS) $(INCS) -c camCapture.cpp

CameraCapture.o: CameraCapture.cpp CameraCapture.h
	$(CC) $(CPPFLAGS) $(INCS) -c CameraCapture.cpp
clean:
	rm *.o
