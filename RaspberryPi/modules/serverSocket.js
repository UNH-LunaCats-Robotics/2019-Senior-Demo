//documentation: https://socket.io/
var app = require('./server.js').app;
var commands = require('./commands.js').cmds;
var ctrl = require('./commands').controller;
var http = require('http').Server(app);

const arduino = require('./arduino.js');
var robot = arduino.robot;
var io = require('socket.io')(http);
const rpserver = require('../build/Release/rpserver.node');

var port = 3002;
var connections = 0;

// assemble message and send to the robot
function sendToRobot(cmd, val){
  var action = cmd + ":" + val;
  //write to robot
  console.log("(remote control) sending message -> " + action);
  robot.write(action, (err) => {
            if(err) return console.log("Error on write: ", err.message);
            console.log("(remote control) message written -> " + action + "!");
        });
  //console.log("(remote control) message written -> " + action + "!");
}


//string format -> A:0
function sendButtonCommand(str){
  var tmp = str.split(":", 2)
  var cmd = tmp[0]
  var on = tmp[1]
  if(Object.values(commands).indexOf(cmd) == -1){
    console.log("??? Invalid command -> " + cmd + " ???");
  }else{
    sendToRobot(cmd, on);
  }
  
}

// string format -> L:45:0
// todo: need to confirm the format
function sendAxisCommand(str){
  var tmp = str.split(":", 3)
  //tmp[0]
  var angle = tmp[1]
  var on = tmp[2]

  sendToRobot(commands.MOVE, angle);
}

var connectSocket = function(){
  io.listen(port);
  console.log("listening on port ", port);
  
  //connection event
  io.on('connection', function(socket) {
    console.log('a user connected: '+ socket);
    connections++;
    //button event: A button down -> A:0
    socket.on('button', (value) => {
      sendButtonCommand(value);
    });

    //joystick event: left joystick 45 -> L:45:0
    socket.on('joystick', (angle) => {
      sendAxisCommand(angle);
    });

    //this function still works!
    var n = 0, inc = 9; 
    var x_i = inc, y_i = inc, z_i = 0.75;
    var x = 0,     y = 0,     z = 0;
    var obj = false;
    var testPoints = setInterval( () => {
      if(n < 1000 ) {
        if(x_i > 0 && x+x_i>370) x_i = -inc;
        if(x_i < 0 && x+x_i<5) x_i = inc;
        if(y_i > 0 && y+y_i>252) y_i = -inc;
        if(y_i < 0 && y+y_i<5) y_i = inc;
        if((z_i > 0 && z+z_i>12) || (z_i < 0 && z+z_i<-12)) z_i = -z_i;
        x+=x_i; y+=y_i; z+=z_i;

        var p = new rpserver.Point(x, y, z);
        var res = p.X() + ":" + p.Y() + ":" + p.Z();
        socket.emit('lidar', res);
        n++;
      }
      //console.log("n: ", n, " X: ", p.X(), " Y: ", p.Y(), " Z: ", p.Z());
    }, 20);  

    socket.on('disconnect', ()=>{
      console.log('user disconnected');
      connections--;
      clearInterval(testPoints);
    });
  });
}

io.on('connect_error', (err) => {
  console.log("something went wrong with the connection");
})

io.on('connect_timeout', () => { 
  console.log("connection timed out");
})

io.on('reconnect', (attempt) => {
  console.log("attempting to reconnect");
})

function getConnections() {
  return connections;
}

module.exports = {
    io: io,
    connectSocket: connectSocket,
    getConnections: getConnections
}