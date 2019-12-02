//serial port
const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

//serial port
//const robot = new SerialPort('/dev/cu.usbmodem145101', { baudRate: 115200 }); //mac
var robot; //linux
var cmdParser;
var can_write_cmd = false;
var isConnected = false;

//each sensor aduino should be added here. 
//const sensorPorts = ['/dev/ttyACM1'];

var connectArd = function() {
    robot = new SerialPort('/dev/ttyACM0', { baudRate: 115200 }); //linux
    cmdParser = robot.pipe(new Readline({ delimiter: '\n' }));// Read the port data
    //stop process if arduino cannot be reached
    robot.on("error", function(err) {
        can_write_cmd = false;
        isConnected = false;
        reconnectArd();
    })

    robot.on("close", () => {
        console.log("!!!ERROR: LOST CONNECTION TO CONTROLLER!!!");
        can_write_cmd = false;
        isConnected = false;
        reconnectArd();
    });

    //just because it is open does not mean it is ready to recieve data
    robot.on("open", () => {
        console.log('robot serial port open');
        isConnected = true;
    });

    //print data recieved from arduino and init.
    var i = 0;
    cmdParser.on('data', data =>{
        console.log('got word from arduino:', data);
        
        //note that all strings will terminate with \r
        if(data === 'init\r') {
            cmd_can_read = true;   
        }
    /*  //ping pong
        if(cmd_can_read) {
            robot.write( i++ + ": hello from node", (err) => {
                if(err) {
                    return console.log("Error on write: ", err.message);
                }
                console.log("message written");
            }); 
        }
    */
    });
}

var reconnectArd = function() {
    setTimeout(function() {
        console.log("SEARCHING...");
        connectArd();
    }, 1000);
}

connectArd();

//skeleton of an idea//
var lastCommand = {
    cmd: "",
    success: "pending"
}

function writeToRobot(cmd) {
    
}
////////////////////////

function getConnected() {
    return isConnected;
}

function getCanWrite() {
    return can_write_cmd;
}

module.exports = {
    connectArd: connectArd,
    reconnectArd: reconnectArd,
    robot: robot,
    isConnected: getConnected,
    canWrite: getCanWrite
};