CXX = g++
# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install
# SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
#SDL_LIB = -L/usr/lib/x86_64-linux-gnu -lSDL2 -lSDL2_image -lSDL2_ttf
SDL_LIB = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf
#SDL_INCLUDE = -I/usr/include

SDL_INCLUDE = `sdl2-config --cflags`
# You may need to change -std=c++11 to -std=c++0x if your compiler is a bit older
#CXXFLAGS = -Wall -c -ggdb -std=c++11 $(SDL_INCLUDE) -DUSE_FREEIMAGE_PNG
CXXFLAGS = -fopenmp -Wall -O3 -c 
LDFLAGS = -L./lib/ -lgomp -lfreeimage
EXE = rayTracer
BINARY_PATH=bin/
SOURCE_PATH=src/

all: $(BINARY_PATH)$(EXE)

$(BINARY_PATH)$(EXE): $(BINARY_PATH)MaterialReader.o  $(BINARY_PATH)Film.o $(BINARY_PATH)Octree.o $(BINARY_PATH)Light.o $(BINARY_PATH)Transform.o $(BINARY_PATH)Mat4f.o $(BINARY_PATH)Primitive.o $(BINARY_PATH)Vec4f.o $(BINARY_PATH)Triangle.o $(BINARY_PATH)Ray.o $(BINARY_PATH)RayTracer.o $(BINARY_PATH)Sphere.o $(BINARY_PATH)Vec3f.o $(BINARY_PATH)Camera.o $(BINARY_PATH)Scene.o $(BINARY_PATH)SimpleFileReader.o $(BINARY_PATH)FileReader.o $(BINARY_PATH)main.o 
	$(CXX) $^ $(LDFLAGS) -o $@

$(BINARY_PATH)main.o: $(SOURCE_PATH)main.cpp 
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@
	
$(BINARY_PATH)FileReader.o: $(SOURCE_PATH)FileReader.cpp $(SOURCE_PATH)FileReader.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@
	
$(BINARY_PATH)SimpleFileReader.o: $(SOURCE_PATH)SimpleFileReader.cpp $(SOURCE_PATH)SimpleFileReader.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@
	
$(BINARY_PATH)Scene.o: $(SOURCE_PATH)Scene.cpp $(SOURCE_PATH)Scene.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@
	
$(BINARY_PATH)Camera.o: $(SOURCE_PATH)Camera.cpp $(SOURCE_PATH)Camera.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BINARY_PATH)Vec3f.o: $(SOURCE_PATH)Vec3f.cpp $(SOURCE_PATH)Vec3f.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@
	
$(BINARY_PATH)Vec4f.o: $(SOURCE_PATH)Vec4f.cpp $(SOURCE_PATH)Vec4f.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@
	
$(BINARY_PATH)Mat4f.o: $(SOURCE_PATH)Mat4f.cpp $(SOURCE_PATH)Mat4f.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BINARY_PATH)Sphere.o: $(SOURCE_PATH)Sphere.cpp $(SOURCE_PATH)Sphere.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@
	
$(BINARY_PATH)RayTracer.o: $(SOURCE_PATH)RayTracer.cpp $(SOURCE_PATH)RayTracer.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@
	
$(BINARY_PATH)Ray.o: $(SOURCE_PATH)Ray.cpp $(SOURCE_PATH)Ray.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BINARY_PATH)Triangle.o: $(SOURCE_PATH)Triangle.cpp $(SOURCE_PATH)Triangle.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@
	
$(BINARY_PATH)Primitive.o: $(SOURCE_PATH)Primitive.cpp $(SOURCE_PATH)Primitive.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@
	
$(BINARY_PATH)Transform.o: $(SOURCE_PATH)Transform.cpp $(SOURCE_PATH)Transform.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@
	
$(BINARY_PATH)Light.o: $(SOURCE_PATH)Light.cpp $(SOURCE_PATH)Light.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BINARY_PATH)Octree.o: $(SOURCE_PATH)Octree.cpp $(SOURCE_PATH)Octree.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BINARY_PATH)Film.o: $(SOURCE_PATH)Film.cpp $(SOURCE_PATH)Film.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BINARY_PATH)MaterialReader.o: $(SOURCE_PATH)MaterialReader.cpp $(SOURCE_PATH)MaterialReader.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@


clean:
	rm -rf $(BINARY_PATH)/*.o $(BINARY_PATH)/rayTracer.exe 
