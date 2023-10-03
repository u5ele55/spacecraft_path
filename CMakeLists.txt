cmake_minimum_required (VERSION 3.10)
project(spacecraft_path)

add_subdirectory(libs/sofa)

file(GLOB UTILS utils/*.cpp)
file(GLOB SPECECRAFT_MOTION_MODELING spacecraft_motion/modeling/*.cpp)
file(GLOB SPECECRAFT_MOTION_SYSTEM spacecraft_motion/system/*.cpp)
file(GLOB RADIOTELESCOPE radiotelescope/*.cpp)
file(GLOB OUTPUT output/*.cpp)

add_executable(run 
    main.cpp
    ${UTILS}
    ${SPECECRAFT_MOTION_MODELING}
    ${SPECECRAFT_MOTION_SYSTEM}
    ${RADIOTELESCOPE}
    ${OUTPUT}
    )

target_link_libraries(run PRIVATE sofa)
