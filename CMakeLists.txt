# CMake project for Beachfront analyzer app and SDK lib

#Set 3P_DIR to the location of third-party, like /usr/local or /opt/3p
# (Note, I normally have a bunch of CMake ifdefs to locate it based on the OS)
set(3P_DIR /usr/local)

# Set 3P include and lib paths
set(CMAKE_INCLUDE_PATH ${3P_DIR}/include ${CMAKE_INCLUDE_PATH})
set(CMAKE_LIBRARY_PATH ${3P_DIR}/lib ${CMAKE_LIBRARY_PATH})

################################################################################
# Config3PLib -
#    Search macro for third-party headers and libraries
################################################################################
macro(Config3PLib name  header libname)
  find_path(${name}_INCLUDE ${header})
  find_library(${name}_LIB ${libname})
  if(${name}_INCLUDE AND ${name}_LIB)
#    set(3P_INCLUDES ${3P_INCLUDES} ${${name}_INCLUDE})
#    set(3P_LIBS ${3P_LIBS} ${${name}_LIB})
#    set(3P_DEPS ${3P_LIBS} )
    message(STATUS "Found ${name}: ${${name}_LIB}")
  else()
    message(STATUS "!Found ${name}: libname=${libname} header=${header}")
  endif()
endmacro( Config3PLib )

cmake_minimum_required(VERSION 2.8.12)

project(BF)

set(lib_modules 
    BFAnalyzer.cpp
)
set(app_modules 
    BFApp.cpp
)

add_library(bf_line_analyzer ${lib_modules})

add_executable(bf_la ${app_modules})

Config3PLib( GEOS geos.h geos )

include_directories( ${GEOS_INCLUDE} )

target_link_libraries(bf_la bf_line_analyzer ${GEOS_LIB})

