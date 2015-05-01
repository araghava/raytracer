CC = g++
CFLAGS = -O3 -Wall -std=c++11 -fopenmp
TARGET = main

all: $(TARGET)

DEPS = core/vector3.o \
    core/constants.o \
	objects/sphere.o \
	trace/screen.o \
	trace/world.o \
	trace/raytracer.o \

$(TARGET): clean $(DEPS) $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp -o raytrace $(DEPS)

core/vector3.o: core/vector3.cpp core/vector3.h
	$(CC) $(CFLAGS) -c core/vector3.cpp -o core/vector3.o
core/constants.o: core/constants.cpp core/constants.h
	$(CC) $(CFLAGS) -c core/constants.cpp -o core/constants.o
trace/screen.o: trace/screen.cpp trace/screen.h
	$(CC) $(CFLAGS) -c trace/screen.cpp -o trace/screen.o
trace/raytracer.o: trace/raytracer.cpp trace/raytracer.h
	$(CC) $(CFLAGS) -c trace/raytracer.cpp -o trace/raytracer.o
trace/world.o: trace/world.cpp trace/world.h
	$(CC) $(CFLAGS) -c trace/world.cpp -o trace/world.o
objects/object.o: objects/object.cpp objects/object.h
	$(CC) $(CFLAGS) -c objects/object.cpp -o objects/object.o

clean:
	rm -f raytrace *.o core/*.o trace/*.o objects/*.o
