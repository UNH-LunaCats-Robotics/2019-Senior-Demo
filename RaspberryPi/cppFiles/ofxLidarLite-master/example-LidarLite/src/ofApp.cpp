#include "ofApp.h"
#include <math.h>

//--------------------------------------------------------------
void ofApp::setup(){
	cout << "Here is the start of file" << endl;
	myLidarLite = LidarLite();
	myLidarLite.begin();

	// Exit if the lidar lite didn't initialize properly
	if (!myLidarLite.hasBegun()) ofApp::exit();

	// Print the hardware version of the Lidar Lite
	cout << "LIDAR Lite hardware version: " << myLidarLite.hardwareVersion() << endl;
	cout << "LIDAR Lite software version: " << myLidarLite.softwareVersion() << endl;

	wDistance = -1;
	myLidarLite.start();
}

//--------------------------------------------------------------
void ofApp::update(){
	int desread = myLidarLite.startDistanceRead();
	if(desread) {
		cout << "myLidarLite read something" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSleepMillis(100);

	cout << "Frame rate = " << std::fixed << std::setw( 5 ) << std::setprecision( 2 )
          << std::setfill( '0' ) <<ofGetFrameRate() << " Hz, ";

	// Read the distance
	int distance = myLidarLite.distance();
	cout << "Distance = " << distance << " cm, ";

	// Read the status (useful for debug)
	//int status = myLidarLite.status();
	//cout << myLidarLite.statusString(status);

	// Power user technique:
	// Weighting the new distance value by the measured signal strength
	// Helps eliminate noise created by the sun and by not detecting any objects
	int signalStrength = myLidarLite.signalStrength();
	int minSigStrength = 20; // signal strength minimum
	int fullSigStrength = 80; // signal strength that gets full weight
	float weight;
	if (signalStrength < minSigStrength) {
		// We're below the min signal strength
		// Set distance to -1 to indicate no object was found
		wDistance = -1; // Indicates no object was found
	} else {
		// Weight by the signal strength
		weight = ofMap(signalStrength, minSigStrength, fullSigStrength, 0.05f, 1.f, true);
		// Calculate the weighted distance
		wDistance = ((float) distance)*weight + ((float) wDistance)*(1-weight);
	}
	cout << "wDistance = " << wDistance << " cm, ";
	cout << "signalStrength = " << signalStrength << ", ";

	cout << endl;
}

// Get the straight distance
double ofApp::getXlength(double angle, double straightDistance){
	double pi = 3.14159265;
	double sinangle = sin (angle * pi / 180);
	cout << "The sin of angle is " << sinangle << " for strightDistance";
	double xlength = sinangle * straightDistance;
	return xlength;
}

// Get the straight distance
double ofApp::getHeight(double angle, double straightDistance){
	double pi = 3.14159265;
	double sinangle = cos (angle * pi / 180);
	cout << "The cos of angle is " << sinangle << " for strightDistance";
	double hlength = sinangle * straightDistance;
	return hlength;
}

// straightDistance is the distance from the lidar to the rock
// HereY is the Y of the current position
// UR means up right
double ofApp::getYforUR(double angle, double straightDistance, double hereY){
	double pi = 3.14159265;
	double result, cosnum;
	if(angle == 0 || angle == 360){
		result = straightDistance + hereY;
	}
	else if(angle < 90 && angle > 0){
		cosnum = cos(angle * pi /180);
		result = cosnum * straightDistance;
		result = result + hereY;
	}
	else if(angle == 90 || angle == 270){
		result = hereY;
	}
	else if(angle > 270 && angle < 360){
		angle = angle - 270;
		cosnum = cos(angle * pi /180);
		result = cosnum * straightDistance;
		result = result + hereY;
	}
	else if(angle > 90 && angle < 180){
		cosnum = cos(angle * pi /180);
		result = cosnum * straightDistance;
		result = hereY + result;
	}
	else{
		result = -1;
	}
	return result;
}

// straightDistance is the distance from the lidar to the rock
// HereX is the X of the current position
// UR means up right
double ofApp::getXforUR(double angle, double straightDistance, double hereX){
	double pi = 3.14159265;
	double result, cosnum;
	if(angle == 0 || angle == 360 || ngle == 180){
		result = hereX;
	}
	else if(angle < 90 && angle > 0){
		cosnum = sin(angle * pi /180);
		result = cosnum * straightDistance;
		result = result + hereX;
	}
	else if(angle == 90){
		result = hereX + straightDistance;
	}
	else if(angle == 270){
		result = hereX - straightDistance;
	}
	else if(angle > 270 && angle < 360){
		angle = angle - 270;
		cosnum = sin(angle * pi /180);
		result = cosnum * straightDistance;
		result = hereX - result;
	}
	else if(angle > 90 && angle < 180){
		cosnum = sin(angle * pi /180);
		result = cosnum * straightDistance;
		result = result - hereX;
	}
	else{
		result = -1;
	}
	return result;
}

// straightDistance is the distance from the lidar to the rock
// HereX is the X of the current position
// UL means up Left
double ofApp::getXforUL(double angle, double straightDistance, double hereX){
	double pi = 3.14159265;
	double result, cosnum;
	if(angle == 0 || angle == 360 || angle == 180){
		result = hereX;
	}
	else if(angle < 90 && angle > 0){
		cosnum = sin(angle * pi /180);
		result = cosnum * straightDistance;
		result = result + hereX;
	}
	else if(angle == 90){
		result = hereX + straightDistance;
	}
	else if(angle == 270){
		result = hereX - straightDistance;
	}
	else if(angle > 270 && angle < 360){
		angle = angle - 270;
		cosnum = sin(angle * pi /180);
		result = cosnum * straightDistance;
		result = hereX - result;
	}
	else if(angle > 180 && angle < 270){
		angle = angle - 180;
		cosnum = sin(angle * pi /180);
		result = cosnum * straightDistance;
		result = hereX - result;
	}
	else{
		result = -1;
	}
	return result;
}

// straightDistance is the distance from the lidar to the rock
// HereY is the Y of the current position
// UL means up Left
double ofApp::getYforUL(double angle, double straightDistance, double hereY){
	double pi = 3.14159265;
	double result, cosnum;
	if(angle == 0 || angle == 360){
		result = straightDistance + hereY;
	}
	else if(angle < 90 && angle > 0){
		cosnum = cos(angle * pi /180);
		result = cosnum * straightDistance;
		result = result + hereY;
	}
	else if(angle == 90 || angle == 270){
		result = hereY;
	}
	else if(angle > 270 && angle < 360){
		angle = angle - 270;
		cosnum = cos(angle * pi /180);
		result = cosnum * straightDistance;
		result = result + hereY;
	}
	else if(angle > 180 && angle < 270){
		cosnum = cos(angle * pi /180);
		result = cosnum * straightDistance;
		result = hereY - result;
	}
	else{
		result = -1;
	}
	return result;
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

void ofApp::exit(){
	// Stop it
	myLidarLite.stop();
}
