CC = g++
CFLAGS = -std=c++11 -Wall -g
LINKS = -pthread
TARGET = main

all: $(TARGET)

DEPS = acceleration/box.o \
  acceleration/bvh.o \
  core/vector3.o \
  core/constants.o \
  core/util.o \
	objects/objobject.o \
	objects/primitives/sphere.o \
	trace/screen.o \
	trace/world.o \
	trace/raytracer.o \
	trace/parser.o \

$(TARGET): clean $(DEPS) $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp -o raytrace $(DEPS) $(LINKS)

acceleration/box.o: acceleration/box.cpp acceleration/box.h
	$(CC) $(CFLAGS) -c acceleration/box.cpp -o acceleration/box.o
acceleration/bvh.o: acceleration/bvh.cpp acceleration/bvh.h
	$(CC) $(CFLAGS) -c acceleration/bvh.cpp -o acceleration/bvh.o
core/vector3.o: core/vector3.cpp core/vector3.h
	$(CC) $(CFLAGS) -c core/vector3.cpp -o core/vector3.o
core/constants.o: core/constants.cpp core/constants.h
	$(CC) $(CFLAGS) -c core/constants.cpp -o core/constants.o
core/util.o: core/util.cpp core/util.h
	$(CC) $(CFLAGS) -c core/util.cpp -o core/util.o
trace/screen.o: trace/screen.cpp trace/screen.h
	$(CC) $(CFLAGS) -c trace/screen.cpp -o trace/screen.o
trace/raytracer.o: trace/raytracer.cpp trace/raytracer.h
	$(CC) $(CFLAGS) -c trace/raytracer.cpp -o trace/raytracer.o
trace/world.o: trace/world.cpp trace/world.h
	$(CC) $(CFLAGS) -c trace/world.cpp -o trace/world.o
trace/parser.o: trace/parser.cpp trace/parser.h
	$(CC) $(CFLAGS) -c trace/parser.cpp -o trace/parser.o
objects/objobject.o: objects/objobject.cpp objects/objobject.h
	$(CC) $(CFLAGS) -c objects/objobject.cpp -o objects/objobject.o
objects/primitives/sphere.o: objects/primitives/sphere.cpp objects/primitives/sphere.h
	$(CC) $(CFLAGS) -c objects/primitives/sphere.cpp -o objects/primitives/sphere.o

clean:
	rm -f raytrace *.o core/*.o trace/*.o objects/*.o acceleration/*.o objects/primitives/*.o
