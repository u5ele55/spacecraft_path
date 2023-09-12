CC = g++ -Wall
EXECUTABLE = run.exe

SOURCES = main.cpp \
          utils\Vector.cpp \
		  spacecraft_motion\modeling\AbstractSolver.cpp \
		  spacecraft_motion\modeling\RK4Solver.cpp \
		  spacecraft_motion\system\SpacecraftGreenwichCS.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(SFML_FLAGS)

.cpp.o:
	$(CC) -c $< -o $@

clean: 
	del $(OBJECTS)