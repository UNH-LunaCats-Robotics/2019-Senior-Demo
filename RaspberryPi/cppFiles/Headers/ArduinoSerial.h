#ifndef ARDUINO_SERIAL
#define ARDUINO_SERIAL 

#include "config.h"

#include <sys/file.h>
#include <unistd.h>     // UNIX standard function definitions
#include <termios.h>    // POSIX terminal control definitions

#include <cstring>		// string function definitions
#include <unordered_map>
#include <cstdio>       // standard input / output functions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions

#include <chrono>
#include <thread> 
#include <mutex>
#include <condition_variable>

using namespace std;
typedef struct termios termios;

#define RS232_PORTNR  38

enum Port { custom = -1,
	ttyS0   = 0,  	ttyS1   = 1,  	ttyS2   = 2,  	ttyS3  = 3,   
	ttyS4   = 4,  	ttyS5   = 5,  	ttyS6   = 6,  	ttyS7  = 7,   
	ttyS8   = 8,  	ttyS9   = 9,  	ttyS10  = 10, 	ttyS11 = 11,
	ttyS12  = 12, 	ttyS13  = 13, 	ttyS14  = 14, 	ttyS15 = 15,
	ttyUSB0 = 16, 	ttyUSB1 = 17, 	ttyUSB2 = 18, 	tyUSB3 = 19, 
	ttyUSB4 = 20, 	ttyUSB5 = 21,
	ttyAMA0 = 22, 	ttyAMA1 = 23,
	ttyACM0 = 24, 	ttyACM1 = 25,
	rfcomm0 = 26, 	rfcomm1 = 27,
	ircomm0 = 28, 	ircomm1 = 29,
	cuau0   = 30, 	cuau1   = 31, 	cuau2   = 32, 	cuau3  = 33,
	cuaU0   = 30, 	cuaU1   = 31, 	cuaU2   = 32, 	cuaU3  = 33
};

enum BaudRate : speed_t {
	B_300 = (speed_t)B300,
	B_600 = (speed_t)B600,
	B_1200 = (speed_t)B1200,
	B_1800 = (speed_t)B1800,  
	B_2400 = (speed_t)B2400,
	B_4800 = (speed_t)B4800,
	B_9600 = (speed_t)B9600,
	B_19200 = (speed_t)B19200,
	B_38400 = (speed_t)B38400,
	B_57600 = (speed_t)B57600,
	B_115200 = (speed_t)B115200,
	B_230400 = (speed_t)B230400 
};

class ArduinoSerial{
private: 
	//all current possible ports to select from
	const string ports[RS232_PORTNR] = {
		"/dev/ttyS0",	"/dev/ttyS1",	"/dev/ttyS2",	"/dev/ttyS3",	
		"/dev/ttyS4",	"/dev/ttyS5",	"/dev/ttyS6",	"/dev/ttyS7",	
		"/dev/ttyS8",	"/dev/ttyS9",	"/dev/ttyS10",	"/dev/ttyS11",
		"/dev/ttyS12",	"/dev/ttyS13",	"/dev/ttyS14",	"/dev/ttyS15",	
		"/dev/ttyUSB0", "/dev/ttyUSB1",	"/dev/ttyUSB2",	"/dev/ttyUSB3",	
		"/dev/ttyUSB4",	"/dev/ttyUSB5",
		"/dev/ttyAMA0",	"/dev/ttyAMA1",	
		"/dev/ttyACM0",	"/dev/ttyACM1",
		"/dev/rfcomm0",	"/dev/rfcomm1",	
		"/dev/ircomm0",	"/dev/ircomm1",
		"/dev/cuau0",	"/dev/cuau1",	"/dev/cuau2",	"/dev/cuau3",
		"/dev/cuaU0",	"/dev/cuaU1",	"/dev/cuaU2",	"/dev/cuaU3"};

	//port descriptors
	int USB = -1; 				//port integer value
	Port port;				//port location
    string portStr;             //non traditional port
	BaudRate baudRate;
	termios tty_old;

	///int status_old = 0;			//old modem settings
	bool initialized = false;   //if initializaion has happened
	
	//timeout for readBytes
    chrono::seconds timeout = chrono::seconds(2);
	int static readBytes( int USBB, char* response, int buf_size, char terminator);
#ifndef WINDOWS
	int readBytes_wrapper( char* response, int buf_size, char terminator);
#endif 
	char readByte( int USBB );

	//prints error msg if in debug mode.
	bool isInitialized(); 	   
	bool isNotInitialized();

public:
	//create the connection
	ArduinoSerial(Port p, BaudRate baud);
	ArduinoSerial(string p, BaudRate baud);

	//ArduinoSerial(Port p, speed_t baud, chrono::seconds tout);
	~ArduinoSerial();

	//initialize the arduino serial connection port
	bool initializePort( bool force = false );

	//restore previous port settings on destruction
	bool resetPort();
	
	//perform read/write operations with arduino
	int readString( char* response, int buf_size, char terminator = '\n' );
	char readChar();
	bool writeString( const unsigned char* cmd );
	bool writeChar(char c);

	//set descriptor information
	void setTimeout(chrono::seconds s);
	bool setBaudRate( BaudRate baud );
	bool setPort( Port p );
    bool setPort( string p );

	//get descriptor information 
	bool getInitialized();
	int getUSB();
	string getPortStr();
    Port getPort();
	BaudRate getBaudRate();
	chrono::seconds getTimeout();

	void flushPort() {
		tcflush(USB, TCIOFLUSH);
	}

	bool operator<<(const unsigned char* cmd) {
		return writeString( cmd );
	}

	//NOTE: This assumes a buf_size of 1024 and a terminator of '\n'
	int operator>>(char* response) {
		return readString( response, 1024 );
	}
};
#endif
