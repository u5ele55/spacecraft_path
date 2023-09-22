cmake_minimum_required (VERSION 3.10)
project(spacecraft_path)

add_subdirectory(libs/sofa)

add_executable(run 
    main.cpp
    utils/Vector.cpp
    utils/time.cpp
    utils/coordinates.cpp
    spacecraft_motion/modeling/AbstractSolver.cpp
    spacecraft_motion/modeling/RK4Solver.cpp
    spacecraft_motion/system/SpacecraftGreenwichCS.cpp
    spacecraft_motion/system/SpacecraftECI.cpp
    radiotelescope/TelescopeCreator.cpp
    radiotelescope/RadioTelescope.cpp
    radiotelescope/RadioTelescopeSystem.cpp
    )

target_link_libraries(run PRIVATE sofa)
