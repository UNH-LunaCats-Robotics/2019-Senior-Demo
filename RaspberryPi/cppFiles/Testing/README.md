
# Testing with Google Test

This project uses [Google Test](https://github.com/google/googletest) to perform tests on the code.

If the code you want to make is for experimentation purposes rather than testing component code, please use another folder and add it to the .gitignore so we can keep the working directory clean.

## Creating Test Files

For each component that is created, there should be tests made for it. This helps make sure that the functionallity expected after writi

Here is an example of a test file:

```C++
File Name: ArduinoSerialTests/ArduinoSerialTest.cpp
//------------------------------//

//include the google testing repo
#include <gtest/gtest.h>

//include the class that you'd like to test
#include "../../Headers/ArduinoSerial.h"

//create individual tests within the test suite ArduinoSerialTest
TEST(ArduinoSerialTest, ConnectToArduino) {
    //put test code here
}

TEST(ArduinoSerialTest, WriteToArduino) {
    //put test code here
}

...

TEST(ArduinoSerialTest, DisconnectFromArduino) {
    //put test code here
}
```

Make sure that the test file is in the appropriate folder so you can also include dependent sources (for example, in this case, arduino code to perform tests against).

All of the tests will be executed when the executable created from cmake is ran after it is added to CMakeLists.txt.

## Running Test Files

Each file created has to be added to the executable in CMakeLists.txt. In order to add the test suite to the other test suites, add it to the list of .cpp files executed like so:

```cmake
package_add_test(${PROJECT_NAME}
    ArduinoSerialTest.cpp
    TestFile1.cpp
    TestFile2.cpp
    ...
)
```

If you would like to create a separate executable to run only your tests, then you can add these lines to CMakeLists.txt:

```cmake
#Add Separate Tests
package_add_test( <YOUR_TEST_NAME_HERE>
    TestFile1.cpp
    TestFile2.cpp
    ...
)
```
