CXX = g++
CXXFLAGS = --std=c++11 #-Wall -Werror # tutaj można dodawać inne flagi kompilatora
LIBS = -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -lSOIL -ldl  # tutaj można dodawać biblioteki

all: main.o Locomotive.o Camera.o Shader.o Box.o Cube.o Cylinder.o Cone.o PrimitiveObject.o PointLight.o
	$(CXX) main.o Locomotive.o Camera.o Shader.o Box.o Cube.o Cylinder.o Cone.o PrimitiveObject.o PointLight.o $(LIBS) -o test

main.o: main.cpp Locomotive.h Shader.h Camera.h Textures.h PrimitiveObject.h Box.h
	$(CXX) main.cpp -c $(CXXFLAGS) -o main.o

PrimitiveObject.o: PrimitiveObject.cpp PrimitiveObject.h
		$(CXX) PrimitiveObject.cpp -c $(CXXFLAGS) -o PrimitiveObject.o

Locomotive.o: Locomotive.cpp Locomotive.h PrimitiveObject.h Box.h Cube.h Cylinder.h Cone.h
	$(CXX) Locomotive.cpp -c $(CXXFLAGS) -o Locomotive.o

Camera.o: Camera.cpp Camera.h
	$(CXX) Camera.cpp -c $(CXXFLAGS) -o Camera.o

PointLight.o: PointLight.cpp PointLight.h
	$(CXX) PointLight.cpp -c $(CXXFLAGS) -o PointLight.o

Shader.o: Shader.cpp Shader.h
	$(CXX) Shader.cpp -c $(CXXFLAGS) -o Shader.o

Box.o: Box.cpp Box.h PrimitiveObject.h
		$(CXX) Box.cpp -c $(CXXFLAGS) -o Box.o

Cone.o: Cone.cpp Cone.h PrimitiveObject.h
		$(CXX) Cone.cpp -c $(CXXFLAGS) -o Cone.o

Cube.o: Cube.cpp Cube.h Box.h
	$(CXX) Cube.cpp -c $(CXXFLAGS) -o Cube.o

Cylinder.o: Cylinder.cpp Cylinder.h PrimitiveObject.h
	$(CXX) Cylinder.cpp -c $(CXXFLAGS) -o Cylinder.o

clean:
	rm -f *.o test
