#-----------------------# NOTE #-----------------------#
# Make sure you build the project in a folder called 'build'.
#   If you change the name of the build folder, add it to .gitignore
#
# This file will be on the .gitignore after you clone it. 
#   If you wish to make global changes to this file, remove it from
#   .gitignore. 
#
# If you make platform-specific changes, add them in the 
#   appropriate if statement as to not mess with other platforms
#------------------------------------------------------#

#define the min version of cmake
cmake_minimum_required (VERSION "3.10")

#title the project
project("LunaCats" CXX)

#comment this out to get rid of debug comments
set(DEBUG 1)

#check if linux (includes raspberry pi)
if(UNIX AND NOT APPLE)
    #Check Build_Instructions/Linux/Linux.txt for how to run this project
    set(LINUX 1)
endif()

#check if windows
if(WIN32)
    # Check Build_Instructions/Windows/Windows.txt for how to run this project
	message(FATAL_ERROR "This code is not supported on Windows. Please use this link and set up a virtual Raspberry Pi Environment: https://raspberrytips.com/run-raspberry-in-virtual-machine/")
endif()

#check if mac
if(APPLE)
    # Check Build_Instructions/Mac/Mac.txt for how to run this project
	set(MAC 1)
endif()

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Headers/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Headers/config.h)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth=300")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0" )
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O3" )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++
 -dead_strip")
 endif()
#enable testing
option(PACKAGE_TESTS "Build the tests" ON)

include(FetchContent)


#add compiler flags
add_compile_options("-Wno-psabi")

#require cmake to be aware of pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#allows us to run tests
#enable_testing()

#create the libraries used by the main file.
add_library( RobotSourceCode STATIC Headers/config.h
    PixyCam/host/libpixyusb2/src/libpixyusb2.cpp
    PixyCam/host/libpixyusb2/include/libpixyusb2.h

    PixyCam/host/libpixyusb2/include/usblink.h
    PixyCam/host/libpixyusb2/src/usblink.cpp
    
    PixyCam/host/libpixyusb2/include/util.h
    PixyCam/host/libpixyusb2/src/util.cpp

    PixyCam/host/arduino/libraries/Pixy2/TPixy2.h
    
    PixyCam/common/inc/chirp.hpp
    PixyCam/common/src/chirp.cpp
    # Sources/ArduinoSerial.cpp	Headers/ArduinoSerial.h
#   Sources/[className].cpp     Headers/[className].h
#                               Headers/[declarations].h
)

target_include_directories( RobotSourceCode PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/Sources 
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers/Exported
    ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Exported
    ${CMAKE_CURRENT_SOURCE_DIR}/PixyCam/host/libpixyusb2/src
    ${CMAKE_CURRENT_SOURCE_DIR}/PixyCam/host/libpixyusb2/include
    ${CMAKE_CURRENT_SOURCE_DIR}/PixyCam/common/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/PixyCam/common/src
    ${CMAKE_CURRENT_SOURCE_DIR}/PixyCam/host/arduino/libraries/Pixy2
)

#create the executable for the project
add_executable(main Sources/main.cpp)



#add linked libraries to the compiled result 
target_link_libraries(main PUBLIC 
    RobotSourceCode
    Threads::Threads
)

#includes the googletest directory and Testing Directory
if(PACKAGE_TESTS)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.8.0
    )
    
    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()

    #add_subdirectory(Testing)

endif()

#keeps cache cleaner
mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

#keeping IDEs that support folders clean
#set_target_properties(gtest PROPERTIES FOLDER extern)
#set_target_properties(gtest_main PROPERTIES FOLDER extern)
#set_target_properties(gmock PROPERTIES FOLDER extern)
#set_target_properties(gmock_main PROPERTIES FOLDER extern)

