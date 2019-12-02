#include <gtest/gtest.h>
#include "../Headers/ArduinoSerial.h"

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

typedef struct ArduinoSettings ArduinoSettings;

struct ArduinoSettings {
    string port;
    BaudRate baudRate;
    chrono::seconds timeout;
};

#define BUF_SIZE 1024

#ifdef MAC
string pp = "/dev/cu.usbmodem1451101";
#else
string pp = "/dev/ttyACM0";
#endif

ArduinoSettings settings{ pp, B_9600, chrono::seconds(2) };
ArduinoSerial serial( settings.port, settings.baudRate );

void resetPort() {
    serial.resetPort();
}

bool selfUploadArduinoCode = true;

//helper functions 
//try to minimize uploads to the arduino if you can.
void testRead( int speed, int count );
void testWrite( int speed, int count );

void initializeFiles();
void writeArduinoFile_w( int speed = 1000 );
void writeArduinoFile_wr( int speed = 1000 );
void writeArduinoFile_noTask(); 
void writeArduinoFile( string funcName, int delay, bool init = true);
void uploadChanges();

void waitForUser(string reason);

//-----------------------------------------------------------------//
//                              TESTS                              //
//-----------------------------------------------------------------//
TEST(ArduinoSerialTest_Initialization, ConnectToArduino) {
    //writeArduinoFile_w();
    initializeFiles();
    
    try {
        serial.initializePort();
    } catch( const exception &e) {
        printf("ERROR: %s\n\n", e.what());

        FAIL() << e.what();
    }

    assert(serial.getInitialized());
}

TEST(ArduinoSerialTest_Initialization, CantChangeSettingsWhenInitialized) {
    ASSERT_FALSE(serial.setBaudRate(B_115200));
    assert( serial.getBaudRate() != B_115200);
    
    ASSERT_FALSE(serial.setPort(ttyACM1));
    assert( serial.getPort() != ttyACM1 );
}

TEST(ArduinoSerialTest_Initialization, DisconnectFromArduino) {
    serial.resetPort();

    ASSERT_FALSE(serial.getInitialized());
}

TEST(ArduinoSerialTest_Initialization, CanChangeSettingsWhenNotInitialized) {
    try {
        ASSERT_TRUE(serial.setBaudRate(B_115200));
        assert( serial.getBaudRate() == B_115200);
    } catch(const exception &e) {
        FAIL() << e.what();
    }
    
    ASSERT_TRUE(serial.setPort(ttyACM1));
    assert( serial.getPort() == ttyACM1 );

    serial.setTimeout(chrono::seconds(10));
    assert( serial.getTimeout() == chrono::seconds(10) );
}

TEST(ArduinoSerialTest_Initialization, FailToConnectWithWrongPort) {
    try {
        serial.initializePort();
    } catch( const exception &e ) {
        printf("Successfully failed with reason: %s", e.what());
        ASSERT_FALSE(serial.getInitialized());
        return;
    }

    FAIL() << "Initialized with wrong settings";
}

TEST(ArduinoSerialTest_Initialization, ConnectWithCorrectSettings) {
    serial.setBaudRate(settings.baudRate);
    serial.setPort(settings.port);
    serial.setTimeout(settings.timeout);

    try {
        serial.initializePort();
    } catch( const exception &e) {
        FAIL() << e.what();
    }

    ASSERT_TRUE(serial.getInitialized());
}

//--------------------------- READING -----------------------------//

TEST(ArduinoSerialTest_Reading, InitializeArduinoForReading) {
    writeArduinoFile_w();
}

TEST(ArduinoSerialTest_Reading, ReadSingleByteFromArduino) {
    ASSERT_TRUE(serial.getInitialized());

    char c = 0;
    c = serial.readChar();
    assert(c != 0);
}

TEST(ArduinoSerialTest_Reading, ReadStringOfBytesFromArduino) {
    ASSERT_TRUE(serial.getInitialized());

    // Whole response
    char response[BUF_SIZE];
    memset(response, '\0', sizeof response);

    int n = serial.readString(response, BUF_SIZE);
    assert( n > 0 );
}

TEST(ArduinoSerialTest_Reading, FailToReadWhenNotInitialized) {
    serial.resetPort();
    ASSERT_FALSE(serial.getInitialized());

    char c = 'c'; //why would anyone do this though...
    c = serial.readChar();
    assert(c == 0);

    // Whole response
    char response[BUF_SIZE];
    memset(response, '\0', sizeof response);

    int n = serial.readString(response, BUF_SIZE);
    assert( n == -1 );

    try {
        serial.initializePort();
        ASSERT_TRUE(serial.getInitialized());
    } catch( const exception &e ) {
        FAIL() << e.what();
    }
}

TEST(ArduinoSerialTest_Reading, ReadStringOfBytesFromArduinoUsingOperator) {
    ASSERT_TRUE(serial.getInitialized());

    char response[BUF_SIZE];
    memset(response, '\0', sizeof response);

    int n = serial >> response;
    assert( n > 0 );
}

TEST(ArduinoSerialTest_Reading, ReadTimeoutSucceeedsWithCorrectTime) {
    serial.resetPort();
    ASSERT_FALSE(serial.getInitialized());

    writeArduinoFile_noTask();

    // Whole response
    char response[BUF_SIZE];
    memset(response, '\0', sizeof response);

    auto start = chrono::high_resolution_clock::now();
    int n = serial.readString(response, BUF_SIZE);
    auto finish = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = finish - start; 
    double tol = 0.1;

    printf("ellapsed time: %f\n", elapsed.count());

    assert( n == -1  &&
          ((settings.timeout.count()-tol) < elapsed.count()) &&
          ((settings.timeout.count()+tol) > elapsed.count()) );

    try {
        serial.initializePort();
        ASSERT_TRUE(serial.getInitialized());
    } catch( const exception &e ) {
        FAIL() << e.what();
    }
}

TEST(ArduinoSerialTest_Reading, ReadStringFromArduino_Slow) {
    testRead(1000, 10);
}

TEST(ArduinoSerialTest_Reading, ReadStringFromArduino_Moderate) {
    testRead(500, 20);
}

TEST(ArduinoSerialTest_Reading, ReadStringFromArduino_Fast) {
    testRead(100, 50);
}

TEST(ArduinoSerialTest_Reading, ReadStringFromArduino_NoDelay) {
    auto start = chrono::high_resolution_clock::now();
    testRead(0, 1000);
    auto finish = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = finish - start; 

    printf("ellapsed time: %f\n", elapsed.count());
}

void testRead( int speed, int count ) {
    ASSERT_TRUE(serial.getInitialized());
    
    // Whole response
    char response[BUF_SIZE];
    memset(response, '\0', sizeof response);

    writeArduinoFile_w(speed);

    int n = serial.readString(response, BUF_SIZE);
    assert( n != 0 && strstr(response, "init"));

    for(int i = 0; i < count; i++) {
        printf("\t%d: ",i);
        n = serial.readString(response, BUF_SIZE);
        assert( n != 0 && strstr(response, "Pong"));
    }
}

//-----------------------------------------------------------------//

//-------------------------// WRITING //---------------------------//
// unlike reading where we can check if it's successful or not, 
// there's no garuntee that the arduino picked up what was sent
// if it was sent before it started reading sadly. 

// we can check if its ready to read, however, by when it sends an 
// init message. 

TEST(ArduinoSerialTest_Writing, InitializeArduinoForWriting) {
    printf("\ninitializing...\n");

    // Whole response
    char response[BUF_SIZE];
    memset(response, '\0', sizeof response);

    writeArduinoFile_wr();
    ASSERT_TRUE(serial.getInitialized());

    int n = serial.readString(response, BUF_SIZE);
    assert( n != 0 && strstr(response, "init"));
    printf("ready to read written responce!\n\n");
}

TEST(ArduinoSerialTest_Writing, WriteSingleBiteToArduino) {
    ASSERT_TRUE(serial.getInitialized());

    ASSERT_TRUE( serial.writeChar('c') );
    char c = serial.readChar();
    assert(c == 'c');

    usleep(10000);

    //we only want to read this one char
    serial.flushPort();
}

TEST(ArduinoSerialTest_Writing, WriteStringOfBitesToArduino) {
    ASSERT_TRUE(serial.getInitialized());

    unsigned char cmd[] = "Lets test this messenger!";

    char response[BUF_SIZE];
    memset(response, '\0', sizeof response);

    ASSERT_TRUE( serial.writeString(cmd) );

    int n = serial.readString(response, BUF_SIZE);
    assert( n != 0 && strstr(response, (char*)cmd));
}

TEST(ArduinoSerialTest_Writing, WriteStringOfBytesToArduinoUsingOperator) {
    ASSERT_TRUE(serial.getInitialized());

    unsigned char cmd[] = "Lets test this messenger!";

    char response[BUF_SIZE];
    memset(response, '\0', sizeof response);

    ASSERT_TRUE( serial << cmd );
    int n = serial.readString(response, BUF_SIZE);
    assert( n != 0 && strstr(response, (char*)cmd));
}

TEST(ArduinoSerialTest_Writing, FailToReadWhenNotInitialized) {
    serial.resetPort();

    unsigned char cmd[] = "Lets test this messenger!";

    ASSERT_FALSE( serial.writeChar('c') );
    ASSERT_FALSE( serial.writeString(cmd) );

    try {
        serial.initializePort();
    } catch( const exception &e) {
        FAIL() << e.what();
    }
}

TEST(ArduinoSerialTest_Writing, WriteStringToArduino_Slow) {
    testWrite(1000, 10);
}

TEST(ArduinoSerialTest_Writing, WriteStringToArduino_Moderate) {
    testWrite(500, 20);
}

TEST(ArduinoSerialTest_Writing, WriteStringToArduino_Fast) {
    testWrite(100, 50);
}

TEST(ArduinoSerialTest_Writing, WriteStringToArduino_NoDelay) {
    auto start = chrono::high_resolution_clock::now();
    testWrite(0, 100);
    auto finish = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = finish - start; 

    printf("ellapsed time: %f\n", elapsed.count());
}

void testWrite( int speed, int count ) {
    ASSERT_TRUE(serial.getInitialized());
    
    unsigned char cmd[] = "Pong";

    // Whole response
    char response[BUF_SIZE];
    memset(response, '\0', sizeof response);

    writeArduinoFile_wr(speed);

    int n = serial.readString(response, BUF_SIZE);
    assert( n != 0 && strstr(response, "init"));

    for(int i = 0; i < count; i++) {
        ASSERT_TRUE( serial.writeString(cmd) );
        printf("\t%d: ",i);
        n = serial.readString(response, BUF_SIZE);
        assert( n != 0 && strstr(response, (char*)cmd));
    }
}

//*/
//-----------------------------------------------------------------//
/*
if(selfUploadArduinoCode) {
        //dangerous way to perform changes
        writeArduinoFile_w();
    }
    else { //have the user perform the changes 
        printf("Hello! Welcome to the Arduino Serial Tests.\n");
        printf("There are two ways to run this code: \n\t1. Through User Interactions(current)\n");
        printf("\t2. Have the test suite upload the correct file contents (DANGEROUS)\n\n");
        waitForUser("Please change the function in loop() to test_w() in ArduinoRead/ArduinoRead.ino, and upload the changes");    
    }
*/
int dirExists(const char *path)
{
    struct stat info;
    
    if(stat( path, &info ) != 0)
        return -1;
    else if(info.st_mode & S_IFDIR)
        return 1;
    else
        return 0;
}

void initializeFiles() {
    if(dirExists("./ArduinoCode/") != 1) {
        mkdir("ArduinoCode", 0777);
    }
    ofstream mkFile("./ArduinoCode/Makefile");
    
    if(!mkFile.is_open()) {
        printf("failed to make makefile");
        exit(-1);
    }
    
    mkFile <<
#ifdef MAC
    "ARDUINO_DIR = /Applications/Arduino.app/Contents/Java\n"   <<
#endif
#ifdef LINUX
    "ARDUINO_DIR = /usr/share/arduino\n"                        <<
#endif
    "ARDUINO_PORT = "<< settings.port << "\n\n"                 <<
    "USER_LIB_PATH = ./\n"                                      <<
    "BOARD_TAG = uno\n\n";
#ifdef MAC
    mkFile << "include /usr/local/opt/arduino-mk/Arduino.mk\n";
#endif
#ifdef LINUX
    mkFile << "include /usr/share/arduino/Arduino.mk";
#endif
    mkFile.close();
}

void waitForUser(string reason) {
    printf("%s\n\nPress any key to continue...", reason.c_str());
    getchar();
    printf("\n");
}

void writeArduinoFile_noTask() {
    writeArduinoFile( "//", 0, false );
    uploadChanges();
}

//An arduino file that only writes Pong every 1 second. 
void writeArduinoFile_w( int speed ) {
    writeArduinoFile( "test_w", speed );
    uploadChanges();
}

//An arduino file that writes what it reads
void writeArduinoFile_wr( int speed ) {
    writeArduinoFile( "test_wr", speed );
    uploadChanges();
}

int getBaudRate(BaudRate r) {
    switch(r) {
        case B_300: return 300;
        case B_600: return 600;
        case B_1200: return 1200;
        //case B_1800: return;
        case B_2400: return 2400;
        case B_4800: return 4800;
        case B_9600: return 9600;
        case B_19200: return 19200;
        case B_38400: return 38400;
        case B_57600: return 57600;
    }
    return 0;
}

void writeArduinoFile( string funcName, int delay, bool init ) {
    ofstream ardFile;

    ardFile.open("./ArduinoCode/ArduinoCode.ino");

    if(!ardFile.is_open()) {
        printf("ERROR: Could not write to arduino file\n");
        exit(-1);
    }

    ardFile <<  "void setup() {\n"                                  << 
                "    Serial.begin( "<< getBaudRate(settings.baudRate) <<" );\n"  <<
                "    Serial.setTimeout(20);\n";
    if(init) ardFile << "    Serial.println(\" init\");\n";
    ardFile <<  "}\n\n"                                             <<

                "void test_w() {\n"                                 <<
                "    Serial.println(\"Pong\");\n"                   <<
                "    delay( "<< delay << " );\n"                    <<
                "}\n\n"                                             <<

                "void test_wr() {\n"                                <<
                "    if (Serial.available() > 0) {\n"               <<
                "        Serial.println(Serial.readString());\n"    <<
                "        delay( "<< delay << " );\n"                <<
                "    }\n"                                           <<
                "}\n\n"                                             <<

                "void loop() {\n"                                   <<
                "    " << funcName << "();\n"                       <<
                "}";

    ardFile.close();
}

//---------------------------// NOTE //---------------------------//
//  ***DO NOT USE THIS FUNCTION OUTSIDE OF THE TEST DIRECTORY***  //   
//                                                                //
// It is not a stable function to depend on, and our code should  //
// not be dependent on this method. There is no garuntee this     //
// method will even work since the thread terminates as soon as   //
// 'make' is called, and is dependent on the file directory       //
// structure.                                                     //
//----------------------------------------------------------------//
void uploadChanges() {
    printf("\nWriting to Arduino...\n\n");
    serial.resetPort();

    if(system(NULL)) {
        printf("Command Processor Exists\n");
        system("make upload --directory=./ArduinoCode/ > ArduinoMakeLog.txt");
        printf("\nFinished writing to arduino.\n\n");
    }
    else {
        if(fork() == 0) {
            printf("Command Processor Does Not Exists\n");
            /*
            ofstream file("ArduinoMakeLog.txt");
            file << "do_upload"; 
            file.close();
            */
            if(execl("/usr/bin/make", 
                "make", 
                "--directory=./ArduinoCode/",
                "upload", NULL) == -1) 
            {
                printf("ERROR: did not use execl right\n");
            }

            exit(0);
        }
        int status;
        wait(&status);
        printf("\nFinished writing to arduino with exit status: %d\n\n", status);
    }
    
    try {
        serial.initializePort();
    } catch( const exception &e ) {
        FAIL() << e.what();
    }

/*
    ifstream file("ArduinoMakeLog.txt");
    char aWord[BUF_SIZE];
    while( file.good() ) {
        file >> aWord;
        if( file.good() && strcmp(aWord, "do_upload") == 0) {
            try {
                serial.initializePort();
            } catch( const exception &e ) {
                printf("Failed to reinitialize the port: %s", e.what());
            }

            file.close();
            return;
        }
    }

    FAIL() << "Failed to upload arduino code.";
*/
}
//----------------------------------------------------------------//
