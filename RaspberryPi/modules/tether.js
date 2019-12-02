var gamepad = require('gamepad');
var Move = require('./commands.js').cmds;
var ctrl = require('./commands').controller;

const arduino = require('./arduino.js');
var robot = arduino.robot;

//tethered
var isTethered = false;

//initialize gamepad for input reading
gamepad.init();

var state = {
    //axis
    left_x: 0,    left_y: 0, 
    right_x: 0,   right_y: 0,
    arrow_x: 0,   arrow_y: 0,
    //buttons
    y: false,
    b: false,
    a: false,
    x: false,
    L: false,
    R: false,
    ZL: false,
    ZR: false,
    joystick_left: false,
    joystick_right: false,
    plus: false,
    minus: false
}

function getBtnCommand(id) {
    switch(id) {
        case ctrl.Y:  return Move.BALL_SCREW_DN;
        case ctrl.B:  return Move.BALL_SCREW_UP;
        case ctrl.A:  return Move.TURN_AUGUR_CW;
        case ctrl.X:  return Move.TURN_AUGUR_CCW;

        case ctrl.L:  return Move.CONVEYOR_EMPTY;
        case ctrl.R:  return Move.LOWER_ACTUATOR;
        case ctrl.ZL: return Move.CONVEYOR_COLLECT;
        case ctrl.ZR: return Move.RAISE_ACTUATOR;

        case ctrl.MINUS: return Move.INCREASE_SPEED;
        case ctrl.PLUS:  return Move.DECREASE_SPEED;

        case ctrl.HOME: return Move.STOP;
        /*
        case ctrl.JOYSTICK_L: break;
        case ctrl.JOYSTICK_R: break;
        case ctrl.SQUARE: break;
        */
    }
}

function setButton(id, value) {
    switch(id) {
        case ctrl.Y:  state.y = value;     break;
        case ctrl.B:  state.b = value;     break;
        case ctrl.A:  state.a = value;     break;
        case ctrl.X:  state.x = value;     break;

        case ctrl.L:  state.L = value;     break;
        case ctrl.R:  state.R = value;     break;
        case ctrl.ZL: state.ZL = value;    break;
        case ctrl.ZR: state.ZR = value;    break;

        case ctrl.MINUS: state.minus = value; break;
        case ctrl.PLUS:  state.plus = value;  break;

        case ctrl.JOYSTICK_L: state.joystick_left = value;  break;
        case ctrl.JOYSTICK_R: state.joystick_right = value; break;

        case ctrl.HOME: /*home*/           break;
        case ctrl.SQUARE: /*square*/       break;
    }
}

function setAxis(id, val) {
    switch(id) {
        case ctrl.left_y: state.left_y = -val;   break;
        case ctrl.left_x: state.left_x = val;    break;
        case ctrl.right_y: state.right_y = -val; break;
        case ctrl.right_x: state.right_x = val;  break;
        case ctrl.arrow_y: state.arrow_y = -val; break;
        case ctrl.arrow_x: state.arrow_x = val;  break;
    }
}

function writeButton(id, val) {
    var cmd = getBtnCommand(id);
    if(cmd) {
        if(cmd == Move.STOP) {
            robot.write(cmd, (err) => {
                if(err) return console.error("Error on write: ", err.message);
                console.log(cmd+" - message written");
            });
        }
        var action = cmd + ":" + val;
        robot.write(action, (err) => {
            if(err) return console.log("Error on write: ", err.message);
            console.log(action+" - message written");
        });
    }
}

/*
function getCommand() {
    if(!isTethered) return Move.STOP;

    var moveCmd = Move.STOP;

    //axis
    if(state.left_y > tolerance) moveCmd = Move.FORWARD;
    if(state.left_y < -tolerance) moveCmd = Move.BACK;
    if(state.left_x > tolerance) moveCmd = Move.LEFT;
    if(state.left_x < -tolerance) moveCmd = Move.RIGHT;

    //buttons
    if(state.ZR) moveCmd = Move.RAISE_F;
    if(state.R) moveCmd = Move.LOWER_F;
    if(state.ZL) moveCmd = Move.CONVEYOR_COLLECT;
    if(state.L) moveCmd = Move.CONVEYOR_DUMP;

    if(state.y) moveCmd = Move.BALL_SCREW_DN;
    if(state.b) moveCmd = Move.BALL_SCREW_UP;
    if(state.a) moveCmd = Move.TURN_AUGUR_CLOCKWISE;
    if(state.x) moveCmd = Move.TURN_AUGUR_COUNTER_CLOCKWISE;

    //not sure what hat was so i used + and - buttons
    if(state.plus) moveCmd = Move.INCREASE_SPEED;
    if(state.minus) moveCmd = Move.DECREASE_SPEED;

    return moveCmd;
}
*/

function angle(cx, cy, ex, ey) {
    var dy = ey - cy;
    var dx = ex - cx;
    var theta = Math.atan2(dy, dx); // range (-PI, PI]
    //theta *= 180 / Math.PI; // rads to degs, range (-180, 180]
    //if (theta < 0) theta = 360 + theta; // range [0, 360)
    return theta;
  }

function writeAxis(axis) {
    if(axis == ctrl.left_y || axis == ctrl.left_x) {
        
    }
}

/* axis numbers
    Left JoyStick: [0,1]
        left y axis: 1
        left x axis: 0
    *
    Right Joystick: [2,3]
        right y axis: 3
        right x axis: 2
    * 
    Arrow Buttons: [4,5]
        arrow y axis: 5
        arrow x axis: 4
*/
//listen for move events on all gamepads
gamepad.on("move", function (id, axis, value) {
    setAxis(axis, value);
    if(value > 0.1 || -value > 0.1) {
        console.log("move", {
            id: id,
            axis: axis,
            value: value
        });
    }
});

//listen for button up events
gamepad.on("up", function(id, num) {
    setButton(num, false);
    writeButton(num, 0);
    console.log("up", {
        id: id,
        num: num
    });
});

/* num for buttons
    Four Main Buttons: [0-3]
        y: 0
        b: 1
        a: 2
        x: 3

    Center Buttons: [8,9],[12,13]
        +: 9
        home: 12
        square: 13
        -: 8

    Trigger Buttons: [4-7]
        L: 4
        R: 5
        ZL: 6
        ZR: 7

    Joystick Buttons: [10,11]
        left: 10
        right: 11
*/
gamepad.on('down', function(id, num) {
    setButton(num, true);
    writeButton(num, 1);

    console.log("down", {
        id: id,
        num: num
    });
});

//display connected controllers
console.log("Controllers Detected: ", gamepad.numDevices());
for(var i = 0, l = gamepad.numDevices(); i < l; i++) {
    console.log(i, gamepad.deviceAtIndex());
    isTethered = true;
}

//create intervals that will call the functions repeatedly
setInterval(gamepad.processEvents, 16);
setInterval(() => {
    gamepad.detectDevices();

    if(gamepad.numDevices() > 0) {
        if(!isTethered) console.log("controller connected!");
        isTethered = true;
    }
    else {
        if(isTethered) console.log("controller disconnected");
        isTethered = false;
    }
}, 500);

function getTethered() {
    return isTethered;
}

module.exports = {
    state: state,
    isTethered: getTethered
};