#-----------------------# NOTE #-----------------------#
# if you wish to create an executable for only testing
#   one class, you can use the `add_executable` command
#   to create a new executable. 
#
# Make sure to clean it up after you are done, though, and 
#   add the tests to the main testing executable. 
#
# Testing Executables can be found after calling cmake in 
#   the folder `Testing`.
#------------------------------------------------------#
cmake_minimum_required (VERSION "3.10")

#title the project
project("AllTests" CXX)

#set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
#set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

set(CMAKE_BUILD_TYPE Debug)

macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} RobotSourceCode gtest gmock gtest_main)
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME} )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER Testing)
endmacro()

package_add_test(${PROJECT_NAME}
   ArduinoSerialTest.cpp
   ../Sources/ArduinoSerial.cpp
  [className]Tests/[className]Test.cpp
)

#[[

#Add Separate Tests
package_add_test( ${TEST_NAME} #replace with custom executable name
    TestFile1.cpp
    TestFile2.cpp
    ...
)

]]
