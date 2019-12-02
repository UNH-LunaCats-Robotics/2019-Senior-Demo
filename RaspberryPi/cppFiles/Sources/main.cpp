  
//#include "../Headers/ArduinoSerial.h"

#define BUF_SIZE 1024

int main() {
    /*
    string port = "/dev/ttyACM0";

#ifdef MAC
    port = "/dev/cu.usbmodem1451101";
#endif

    ArduinoSerial serial(port, B_9600);

    try {
        serial.initializePort();
        
        unsigned char cmd[] = "Lets test this messenger!";
        //printf("Writing %s\n", cmd);

        // Whole response
        char response[BUF_SIZE];
        memset(response, '\0', sizeof response);
        
        while(true) {
            //printf("Writing %s\n", cmd);
            serial.writeString(cmd);
            //printf("Reading Response... \n");
            serial.readString(response, BUF_SIZE);
        }
        
    } catch( const invalid_argument &e ) {
        printf("Error: %s", e.what());
    }
*/
	return 0;
}
