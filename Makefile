CC = g++ -Wall
EXECUTABLE = run.exe

SOURCES = main.cpp \
          utils\Vector.cpp \
		  spacecraft_motion\modeling\AbstractSolver.cpp \
		  spacecraft_motion\modeling\RK4Solver.cpp \
		  spacecraft_motion\system\SpacecraftGreenwichCS.cpp \
		  spacecraft_motion\system\SpacecraftECI.cpp

OBJECTS = $(SOURCES:.cpp=.o) $(SOFA_OBS)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ 

.cpp.o:
	$(CC) -c $< -o $@

clean: 
	del $(OBJECTS)