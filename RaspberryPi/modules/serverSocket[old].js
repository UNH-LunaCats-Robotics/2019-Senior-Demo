//documentation: https://socket.io/
var app = require('./server.js').app;
var commands = require('./commands.js').old;
var http = require('http').Server(app);
var io = require('socket.io')(http);
const rpserver = require('../build/Release/rpserver.node');

var port = 3002;

var cmd = commands.STOP;

//var str = "ZL:1";

//string format -> A:0
function setButtonCommand(str){
  var tmp = str.split(":", 2)
  var id = tmp[0]
  var on = tmp[1]
  
  if(on == 0){//key up, stop
    cmd = commands.STOP;
  }else{//key down
    switch(id){
    case 'Y': cmd = comands.BALL_SCREW_DN;
      break;
    case 'B': cmd = commands.BALL_SCREW_UP;
      break;
    case 'A': cmd = commands.TURN_AUGUR_CLOCKWISE;;
      break;
    case 'X': cmd = commands.TURN_AUGUR_COUNTER_CLOCKWISE;
      break;
      
    case "ZR": cmd = commands.RAISE_F;
      break;
    case 'R': cmd = commands.LOWER_F;
      break;
    case "ZL": cmd = commands.CONVEYOR_COLLECT;
      break;
    case 'L': cmd = commands.CONVEYOR_DUMP;
      break;
      
    //keep consistent with tether.js
    case "PLUS": cmd = commands.INCREASE_SPEED;
      break;
    case 'MINUS': cmd = commands.DECREASE_SPEED;
      break;
    
    //more cases go here
    }
  }
}

//string format -> L:45:0
function setAxisCommand(str){
  var tmp = str.split(":", 3)
  //tmp[0]
  var angle = tmp[1]
  var on = tmp[2]
  
  if(on == 0){//key up, stop
    cmd = commands.STOP;
  }
  else{
    if(angle >= 0 && angle < 45 || angle >= 315 && angle < 360){
      cmd = commands.FORWARD;
    }
    else if(angle >= 45 && angle <= 135){
      cmd = commands.LEFT;
    }
    else if(angle > 135 && angle < 225){
      cmd = commands.BACK;
    }
    else{
      cmd = commands.RIGHT;
    }
  }
}

io.on('connection', function(socket) {
    console.log('a user connected: '+socket);
    socket.on('subscribeToTimer', (interval) => {
        console.log('client is subscribing to timer with interval ', interval);
        setInterval(() => {
            socket.emit('timer', new Date());
        }, interval);
    });
    
    //button event: A button down -> A:0
    socket.on('button', (value) => {
        //update cmd
        setButtonCommand(value);
        console.log('button', value);
    });

    //joystick event: left joystick 45 -> L:45:0
    socket.on('joystick', (angle) => {
        //update cmd
        setAxisCommand(angle);
        console.log('joystick', angle);
     });

     var n = 0;
     setInterval( () => {
       var p = new rpserver.Point(n, n, 0);
       var res = p.X() + ":" + p.Y() + ":" + p.Z();
       socket.emit('lidar', res);
       n++;
       console.log("X: ", p.X(), " Y: ", p.Y(), " Z: ", p.Z());
     }, 1000);
});

var connectSocket = function(){
    io.listen(port);
    console.log("listening on port ", port);
}

var getCommand = function() {
    return cmd;
}

module.exports = {
    io: io,
    connectSocket: connectSocket,
    getCommand: getCommand
}