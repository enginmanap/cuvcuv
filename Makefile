CXX = g++
 
#CXXFLAGS = -fopenmp -Wall -O3 -DUSE_FREEIMAGE_PNG
CXXFLAGS = -fopenmp -Wall -O3
LINKER   = g++ -o
LDFLAGS = -L./lib/ -lgomp -lfreeimage


EXE = rayTracer
BINARY_PATH=bin
OBJECT_PATH= obj
SOURCE_PATH=src

SOURCES  := $(wildcard $(SOURCE_PATH)/*.cpp)
INCLUDES := $(wildcard $(SOURCE_PATH)/*.h)
OBJECTS  := $(SOURCES:$(SOURCE_PATH)/%.cpp=$(OBJECT_PATH)/%.o)
rm       = rm -f

$(BINARY_PATH)/$(EXE): $(OBJECTS)
	@$(LINKER) $@ $(OBJECTS) $(LDFLAGS) 
	@echo "Linking complete!"

$(OBJECTS): $(OBJECT_PATH)/%.o : $(SOURCE_PATH)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@$(rm) $(BINARY_PATH)/$(EXE)
	@echo "Executable removed!"

