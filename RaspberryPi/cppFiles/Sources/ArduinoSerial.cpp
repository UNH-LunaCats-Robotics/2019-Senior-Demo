#include "ArduinoSerial.h"

//-------------------------------// INITIALIZATION //-------------------------------//

/** ArduinoSerial Constructor (Port Only)
 * @Port p - the port where the arduino is connected
 * @speed_t baud - the baud rate for communication.
 * 
 * - Establish the port location and baud rate 
 * 	 	with a default timeout of two seconds
 * - Calibrate the settings of the serial port
 */
ArduinoSerial::ArduinoSerial(Port p, BaudRate baud): port(p), baudRate(baud) {
    portStr = ports[p];
#ifdef DEBUG
	printf("Port: %s, Baud Rate: %d\n", ports[port].c_str(), baudRate);
#endif
}

/** ArduinoSerial Constructor (Port Only)
 * @Port p - the port where the arduino is connected
 * @speed_t baud - the baud rate for communication.
 *
 * - Establish the port location and baud rate
 *          with a default timeout of two seconds
 * - Calibrate the settings of the serial port
 */
ArduinoSerial::ArduinoSerial(string p, BaudRate baud): portStr(p), baudRate(baud) {
    port = custom;
#ifdef DEBUG
    printf("Port: %s, Baud Rate: %d\n", ports[port].c_str(), baudRate);
#endif
}

/**
 * Re-establish the previous port settings on destruction.
 */
ArduinoSerial::~ArduinoSerial(){
	resetPort();
}

void copyTermios( termios *tty_old, termios *tty  ) {
	tty_old->c_cc[VMIN] 	= tty->c_cc[VMIN];
	tty_old->c_cc[VTIME] = tty->c_cc[VTIME];
	tty_old->c_cflag 	= tty->c_cflag;
	tty_old->c_iflag 	= tty->c_iflag;
	tty_old->c_oflag 	= tty->c_oflag;
	tty_old->c_lflag 	= tty->c_lflag;
}

/** Initialize the Port
 * @bool force - force reinitialization after already initialized. 
 *  
 * Set the terminal serial port settings to allow us to perform 
 * reads and writes
 */
bool ArduinoSerial::initializePort(bool force) {
	//do not initialize port if already done unless forced
	if(!force && !isNotInitialized()) return false;
    
	termios tty;
	memset (&tty, 0, sizeof tty);

	USB = open( portStr.c_str(), O_RDWR| O_NOCTTY );
	
	//printf("USB Value: %d\n", USB);

	if(USB == -1) {
		throw invalid_argument("Cannot find serial port to open\n");
	}

	if(flock(USB, LOCK_EX | LOCK_NB) != 0) {
		close(USB);
		throw invalid_argument("Another process is using the port\n");
	}

	// Error Handling 
	if ( tcgetattr ( USB, &tty ) != 0 ) {
	    //std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
		close(USB);
		flock(USB, LOCK_UN); 
		throw invalid_argument("Error getting port settings\n");
	}

	// Save old tty parameters 
	if(initialized != true) {
		copyTermios(&tty_old, &tty);
	}

	// Set Baud Rate 
	cfsetospeed (&tty, baudRate);
	cfsetispeed (&tty, baudRate);

	//NOTE: We need to figure out how this number is retrieved
	//		it is currently the trailing settings from SerialMonitor
	tty.c_cflag = 2237;

	//these numbers are ok
	tty.c_iflag = IGNPAR;
	tty.c_oflag = 0;
	tty.c_lflag &= ~(ICANON|ECHO);

	tty.c_cflag     &=  ~CRTSCTS;           // no flow control
	tty.c_cc[VMIN]   =  0;                  // read doesn't block
	tty.c_cc[VTIME]  =  0.5;                // 0.5 seconds read timeout
	//tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

	// Make raw 
	cfmakeraw(&tty);

	if ( tcsetattr ( USB, TCSAFLUSH, &tty ) != 0) {
	   //std::cout << "Error " << errno << " from tcsetattr" << std::endl;
		close(USB);
		flock(USB, LOCK_UN);
		throw invalid_argument("Error setting new port settings");
	}
	/*
	//Set the modem to data terminal ready and ready to send
	if(ioctl(USB, TIOCMGET, &status_old) == -1) {
		tcsetattr(USB, TCSANOW, &tty_old);
		flock(USB, LOCK_UN);
		throw invalid_argument("Error Setting Port Status");
	}
	
	printf("Status is %x\n", status_old & TIOCM_RNG);
	
	
	int status = status_old;
	status |= TIOCM_DTR;    // turn on DTR 
	status |= TIOCM_RTS;    // turn on RTS 
	
	
	if(ioctl(USB, TIOCMSET, &status) == -1) {
		tcsetattr(USB, TCSANOW, &tty_old);
		flock(USB, LOCK_UN);
		throw invalid_argument("Error Setting Port Status");
	}
	*/

	tcflush(USB, TCIOFLUSH);
	
	initialized = true;

	return true;
}

/** Reset the Port
 *  - Set the port back to its original configuration
 */
bool ArduinoSerial::resetPort() {
	// Make raw 
	cfmakeraw(&tty_old);

	// Flush Port, then applies attributes 
	tcflush( USB, TCIOFLUSH );
	if ( tcsetattr ( USB, TCSANOW, &tty_old ) != 0) {
	   //std::cout << "Error " << errno << " from tcsetattr" << std::endl;
#ifdef DEBUG
		printf("Error Setting Old Port Settings\n");
#endif
	}
	
	/*
	// Set the Port Status back to before
	if(ioctl(USB, TIOCMSET, &status_old) == -1) {
		printf("Error Setting Old Port Status");
	}
	*/
	close(USB);
	flock(USB, LOCK_UN);
	
	initialized = false;
	return true;
}


//-------------------------------// READING / WRITING //-------------------------------//

/** Read an array of bytes from the serial port
 * @int USBB		- Serial port to read from. 
 * @char* response 	- character array to read into
 * @int buf_size 	- max size of the responce character array
 * @char terminator - the terminating character to end the array at(\n, \r, etc)
 * @return int		- return length of string
 *  
 *  Keep reading characters from the serial port until 
 *  the terminating character has been reached. 
 */ 
int ArduinoSerial::readBytes( int USBB, char* response, int buf_size, char terminator) {
	//read 
	int n = -1, spot = 0;
	char buf = '\0';
	
	do {
		n = read( USBB, &buf, 1 );
		sprintf( &response[spot], "%c", buf );
		spot += n;
	} while( buf != terminator && n > 0);

	if(n < 0) return -1;

	return spot;
}


/** Performs readBytes in another thread with timeout
 * @char* response 	- character array to read into
 * @int buf_size 	- max size of the responce character array
 * @char terminator - the terminating character to end the array at(\n, \r, etc)
 * @return int		- return length of string
 *  
 *  Start the readBytes function in another thread until terminator is reached
 *  or if timeout is reached of which the thread is killed. 
 */ 
int ArduinoSerial::readBytes_wrapper( char* response, int buf_size, char terminator) {
	mutex m;
	condition_variable cv;

	int n = -1, usbTemp = USB;
	auto func = &ArduinoSerial::readBytes;

	thread t( [ &n, &response, &buf_size, &terminator, &usbTemp, func, &cv ](){
		n = func(usbTemp, response, buf_size, terminator);
		cv.notify_one();
		return;
	});
	
	auto tm_ = t.native_handle();
	t.detach();

	{
		unique_lock<mutex> l(m);
		if(cv.wait_for(l, timeout) == cv_status::timeout) {
			pthread_cancel(tm_);
			throw runtime_error("Timeout");
		}
	}

	return n;
}

/** Read a String sent from the Arduino
 * @char* response 	- character array to read into
 * @int buf_size 	- max size of the responce character array
 * @char terminator - the terminating character to end the array at(\n, \r, etc)
 * @return int		- return length of string
 *  
 *  Reads a character at a time from the serial port into the responce array
 *  until the terminator character is reached or it has reached the end of the array.
 */
int ArduinoSerial::readString( char* response, int buf_size, char terminator ) {
	if(!isInitialized()) return -1;
	
	int n = -1;

	try {
		n = readBytes_wrapper(response, buf_size, terminator);

	} catch(runtime_error &e) {
#ifdef DEBUG
		printf("Read Request Timed Out\n");
#endif
		return n;
	}
	
#ifdef DEBUG
	if (n < 0) {
		//std::cout << "Error reading: " << strerror(errno) << std::endl;
		printf("Error Reading.\n");
	}
	else if (n == 0) {
		//std::cout << "Read nothing!" << std::endl;
		printf("Read nothing! \n");
	}
	else {
		printf("Received %i bytes: '%s'\n", n, (char *)response);
		//std::cout << "Response: " << response << std::endl;
	}
#endif

	return n;
}

/** Read a Byte
 * @int USBB 		Port to read from
 * @return char 	char read from serial
 * 
 * Start another thread and try to read a byte.
 * Timeout after x declared seconds, and kill the thread.
*/
char ArduinoSerial::readByte( int USBB ) {
	mutex m;
	condition_variable cv;

	char c;
    
    thread t( [ USBB, &c, &cv ](){
		read( USBB, &c, 1);
		cv.notify_one();
        return;
	});
	
    auto tm_ = t.native_handle();
	t.detach();

	{
		unique_lock<mutex> l(m);
        if(cv.wait_for(l, timeout) == cv_status::timeout){
            pthread_cancel(tm_);
            throw runtime_error("Timeout");
        }
	}

	return c;
}

/** Read a Character
 * @return char 	char read from serial
 * 
 * Read a character from the serial port with
 * a timeout of x given seconds. 
 */
char ArduinoSerial::readChar() {
	if(!isInitialized()) return '\0';

	try {
		return readByte(USB);
	} catch(runtime_error &e) {
#ifdef DEBUG
		printf("Read Request Timed Out\n");
#endif
		return '\0';
	}
}

/** Write a String to the Arduino 
 * @const unsigned char* cmd - command string to be sent to the arduino
 * @return write string success or fail
 * 
 * Write a string to the serial port for the arduino to recieve. 
 */
bool ArduinoSerial::writeString( const unsigned char* cmd  ) {
	if(!isInitialized()) return false;
	
	int n_written = 0, spot = 0;

#ifdef DEBUG
	printf("Sending the message '%s'\n", (char *)cmd);
#endif

	do {
		n_written = write( USB, &cmd[spot], 1 );
		spot += n_written;
	} while (cmd[spot-1] != '\0' && n_written > 0 );

	return true;
}

/** Write a Character
 * Write a character sent to the aduino
 */
bool ArduinoSerial::writeChar(char c) {
	if(!isInitialized()) return false;
	
	write( USB, &c, 1 );

	return true;
}

//-------------------------------// PRIVATE FUNCTIONS //-------------------------------//

/**
 * Check if the status of the serial port is initialized. (different error msg)
 */
bool ArduinoSerial::isInitialized() {
#ifdef DEBUG
	if(!initialized) printf("Port Not Initialized\n");
#endif
	return initialized;
}

/**
 * Check if the status of the serial port is not initialized. (different error msg)
 */
bool ArduinoSerial::isNotInitialized() {
#ifdef DEBUG
	if(initialized) printf("Port Already Initialized\n");
#endif
	return !initialized;
}

//-------------------------------// DESCRIPTOR FUNCTIONS //-------------------------------//

void ArduinoSerial::setTimeout(chrono::seconds s) {
	timeout = s;
}

bool ArduinoSerial::setBaudRate( BaudRate baud ) {
	if(!isNotInitialized()) return false;
	baudRate = baud;
	return true;
}

bool ArduinoSerial::setPort( Port p ) {
	if(!isNotInitialized()) return false;
	port = p;
    portStr = ports[p];
	return true;
}	

bool ArduinoSerial::setPort( string p ) {
    if(!isNotInitialized()) return false;
    port = custom;
    portStr = p;
    return true;
}

/** Get the status of the Arduino Serial Port */
bool ArduinoSerial::getInitialized() {
	return initialized;
}

/** Get the number representing the port */
int ArduinoSerial::getUSB(){
	return USB;
}

string ArduinoSerial::getPortStr() {
    return portStr;
}

/** Get the string path to the port */
Port ArduinoSerial::getPort(){
	return port;
}

chrono::seconds ArduinoSerial::getTimeout() {
	return timeout;
}

/** Get the baud rate used to establish a connection */
BaudRate ArduinoSerial::getBaudRate(){
	return baudRate;
}

