cmake_minimum_required(VERSION 3.0)
project(midi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lrtmidi -pthread")

find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)
find_package(Boost REQUIRED)
find_package(Boost COMPONENTS python-py35 REQUIRED)

set(Boost_USE_STATIC_LIBS ON)

include_directories(${PYTHON_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

set(SOURCE_FILES src/instrument.h src/message.h src/message.cpp src/device.cpp src/device.h)
add_library(midi ${SOURCE_FILES})
target_link_libraries(midi rtmidi)

add_library(pymidi src/pymidi.cpp src/pymidi.h src/message.h)
target_link_libraries(pymidi ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} midi)

if (UNIX)
    set_target_properties(pymidi PROPERTIES PREFIX "")
    if (APPLE)
        set_target_properties(pymidi PROPERTIES SUFFIX ".so")
    endif ()
endif ()

add_executable(midi_sounds src/main.cpp)
target_link_libraries(midi_sounds midi)
