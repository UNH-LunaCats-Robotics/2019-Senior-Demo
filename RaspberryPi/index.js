//c++ code
const cppFunctions = require('./build/Release/rpserver.node');

//serial port
const arduino = require('./modules/arduino.js');
var robot = arduino.robot;

//network
const sensorServer = require('./modules/server.js');
const serverSocket = require('./modules/serverSocket.js');

//tether
var controller = require('./modules/tether.js');

//tether means that the robot will drive from controller connection
console.log("Tethered Connection:", controller.isTethered);

serverSocket.connectSocket();
exports.server = sensorServer.app;

console.log("--------- C++ Function Examples ---------");
console.log("cppFunctions: ", cppFunctions);
console.log("Hello World: ", cppFunctions.helloWorld());
console.log("Add 2 + 3: ", cppFunctions.add(2,3));
console.log("getPoint: ", JSON.parse(cppFunctions.getPoint()));
const p = new cppFunctions.Point(1, 2, 3);
console.log("Point: ", p);
console.log("X: ", p.X(), " Y: ", p.Y(), " Z: ", p.Z());
console.log("-----------------------------------------");



