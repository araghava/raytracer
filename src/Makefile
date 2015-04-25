CC = g++
CFLAGS = -O3 -Wall -std=c++11
TARGET = main

all: $(TARGET)

DEPS = core/vector3.o \
	trace/screen.o \
	objects/sphere.o \
	trace/raytracer.o \

$(TARGET): clean $(DEPS) $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp -o raytrace $(DEPS)

core/vector3.o: core/vector3.cpp core/vector3.h
	$(CC) $(CFLAGS) -c core/vector3.cpp -o core/vector3.o
trace/screen.o: trace/screen.cpp trace/screen.h
	$(CC) $(CFLAGS) -c trace/screen.cpp -o trace/screen.o
trace/raytracer.o: trace/raytracer.cpp trace/raytracer.h
	$(CC) $(CFLAGS) -c trace/raytracer.cpp -o trace/raytracer.o
objects/sphere.o: objects/sphere.cpp objects/sphere.h
	$(CC) $(CFLAGS) -c objects/sphere.cpp -o objects/sphere.o

clean:
	rm -f raytrace *.o core/*.o trace/*.o objects/*.o
