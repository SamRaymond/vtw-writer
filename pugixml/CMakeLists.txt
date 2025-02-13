project(pugixml)

cmake_minimum_required(VERSION 2.8.11)


set(HEADERS pugixml.hpp pugiconfig.hpp)
set(SOURCES ${HEADERS} pugixml.cpp)

add_library(pugixml ${SOURCES})

# Enable C++11 long long for compilers that are capable of it
if(NOT ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} STRLESS 3.1 AND ";${CMAKE_CXX_COMPILE_FEATURES};" MATCHES ";cxx_long_long_type;")
	target_compile_features(pugixml PUBLIC cxx_long_long_type)
endif()

set_target_properties(pugixml PROPERTIES VERSION 1.8 SOVERSION 1)

