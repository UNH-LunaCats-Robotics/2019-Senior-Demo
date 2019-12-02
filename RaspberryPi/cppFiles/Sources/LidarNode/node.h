#pragma once

#include "ofMain.h"
#include "LidarLite.hpp"
#include "ofApp.cpp"

class ofApp : public ofBaseApp{
	public:
    long botx;
    long boty;
    long botheight;
		long stridis;
		int whichLidar;

		void setup(bool fasti2c, char i2c);
		void node(long x, long y, long z);
		void getdistance();
		void update(long x, long y);
		//void draw();

    int gettypeofobstacle(long angle);
    double getXobstacle(long angle, int ulur);
		double getYobstacle(long angle, int ulur)


		ofApp myApp;
		LidarLite myLidarLite;
		int wDistance;
};
