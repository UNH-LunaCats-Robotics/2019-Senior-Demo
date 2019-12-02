import openSocket from 'socket.io-client';
//const socket = openSocket('http://localhost:3002');
const socket = openSocket('http://10.0.10.11:3002');

function subscribeToTimer(cb) {
    console.log("hello");
    socket.on('timer', timestamp => cb(null, timestamp));
    socket.emit('subscribeToTimer', 1000);
}

function startTimer(interval) {
    console.log("Timer started. Commands being sent every ", interval);
    setInterval(() => {
        socket.emit('event', 0);
    }, interval);
}

function startup(controller) {
    console.log("Starting up");

    //Lidar point -> X:Y:Z
    socket.on('lidar', (point) => {
        var p = point.split(":");
        controller.addPoint(p);
    });

    //Robot location -> X:Y:Z:A
    socket.on('robot', (location) => {
        var l = location.split(":");
        controller.addPoint(l);
    });
}

function sendEvent(e, v) {
    console.log(`Event: ${e}   Value: ${v}`);
    socket.emit(e, v);
}

export {subscribeToTimer}
export {startTimer}
export {sendEvent}
export {startup}