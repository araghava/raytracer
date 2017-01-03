CC = g++
CFLAGS = -std=c++11 -Wall -g
LINKS = -pthread
TARGET = main

all: $(TARGET)

DEPS = acceleration/box.o \
  acceleration/bvh.o \
  core/matrix.o \
  core/geometry.o \
  core/constants.o \
  core/util.o \
	objects/objobject.o \
	objects/primitives/sphere.o \
	material/material.o \
	trace/screen.o \
	trace/world.o \
	trace/raytracer.o \
	trace/xmlparser.o \
  external/tinyxml2.o \

$(TARGET): clean $(DEPS) $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp -o raytrace $(DEPS) $(LINKS)

acceleration/box.o: acceleration/box.cpp acceleration/box.h
	$(CC) $(CFLAGS) -c acceleration/box.cpp -o acceleration/box.o
acceleration/bvh.o: acceleration/bvh.cpp acceleration/bvh.h
	$(CC) $(CFLAGS) -c acceleration/bvh.cpp -o acceleration/bvh.o
core/matrix.o: core/matrix.cpp core/matrix.h
	$(CC) $(CFLAGS) -c core/matrix.cpp -o core/matrix.o
core/geometry.o: core/geometry.cpp core/geometry.h
	$(CC) $(CFLAGS) -c core/geometry.cpp -o core/geometry.o
core/constants.o: core/constants.cpp core/constants.h
	$(CC) $(CFLAGS) -c core/constants.cpp -o core/constants.o
core/util.o: core/util.cpp core/util.h
	$(CC) $(CFLAGS) -c core/util.cpp -o core/util.o
material/material.o: material/material.cpp material/material.h
	$(CC) $(CFLAGS) -c material/material.cpp -o material/material.o
trace/screen.o: trace/screen.cpp trace/screen.h
	$(CC) $(CFLAGS) -c trace/screen.cpp -o trace/screen.o
trace/raytracer.o: trace/raytracer.cpp trace/raytracer.h
	$(CC) $(CFLAGS) -c trace/raytracer.cpp -o trace/raytracer.o
trace/world.o: trace/world.cpp trace/world.h
	$(CC) $(CFLAGS) -c trace/world.cpp -o trace/world.o
trace/xmlparser.o: trace/xmlparser.cpp trace/xmlparser.h
	$(CC) $(CFLAGS) -c trace/xmlparser.cpp -o trace/xmlparser.o
objects/objobject.o: objects/objobject.cpp objects/objobject.h
	$(CC) $(CFLAGS) -c objects/objobject.cpp -o objects/objobject.o
objects/primitives/sphere.o: objects/primitives/sphere.cpp objects/primitives/sphere.h
	$(CC) $(CFLAGS) -c objects/primitives/sphere.cpp -o objects/primitives/sphere.o

external/tinyxml2.o: external/tinyxml2.cpp external/tinyxml2.h
	$(CC) $(CFLAGS) -c external/tinyxml2.cpp -o external/tinyxml2.o

clean:
	rm -f raytrace *.o core/*.o trace/*.o objects/*.o acceleration/*.o objects/primitives/*.o external/*.o
