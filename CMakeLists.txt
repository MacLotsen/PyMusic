cmake_minimum_required(VERSION 3.0)
project(libmidi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lrtmidi -pthread")

find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)
find_package(Boost REQUIRED)
find_package(Boost COMPONENTS python-py35 REQUIRED)

include_directories(${PYTHON_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

set(SOURCE_FILES src/player.cpp)
add_library(midi STATIC ${SOURCE_FILES})
target_link_libraries(midi rtmidi)

add_library(pymidi SHARED src/pymidi.cpp src/pymidi.h src/midi.h)
target_link_libraries(pymidi ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} midi)
if (UNIX)
    set_target_properties(pymidi PROPERTIES PREFIX "")
    if (APPLE)
        set_target_properties(pymidi PROPERTIES SUFFIX ".so")
    endif ()
endif ()

add_executable(testpymidi src/pmain.cpp)
target_link_libraries(testpymidi pymidi)

#file(COPY src/example.py DESTINATION .)
#add_custom_target(pyexample DEPENDS pymidi COMMAND ${PYTHON_EXECUTABLE} example.py)

add_executable(midi_sounds src/main.cpp)
target_link_libraries(midi_sounds midi)
