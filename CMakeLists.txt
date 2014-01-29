cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

IF(WIN32 OR win64)
   SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll)
ELSE()
   SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)
ENDIF()

find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED ) 

project(AverageFaces)

add_subdirectory(src)

include_directories(C:/opencv/build)
include_directories(${AverageFaces_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIRS}) 

file(GLOB_RECURSE AverageFaces_HEADERS include/*.h)
file(GLOB_RECURSE AverageFaces_SOURCES src/*.cpp)
set (AverageFaces_INCLUDE_DIRS "")
foreach (_headerFile ${AverageFaces_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND AverageFaces_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES AverageFaces_INCLUDE_DIRS)

include_directories(${AverageFaces_INCLUDE_DIRS})

add_executable (AverageFaces ${AverageFaces_SOURCES} ${AverageFaces_HEADERS} ${Boost_LIBRARIES})
target_link_libraries ( AverageFaces ${OpenCV_LIBS})