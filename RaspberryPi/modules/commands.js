var old = {
    FORWARD: "{\"c\":0}",  BACK: "{\"c\":1}",
    LEFT: "{\"c\":2}",     RIGHT: "{\"c\":3}",
    STOP: "{\"c\":4}",
    RAISE_F: "{\"c\":10}", 
    LOWER_F: "{\"c\":11}",
    INCREASE_SPEED: "{\"c\":12}",
    DECREASE_SPEED: "{\"c\":13}",
    CONVEYOR_COLLECT: "{\"c\":14}",
    CONVEYOR_DUMP: "{\"c\":15}",
    TURN_AUGUR_CLOCKWISE: "{\"c\":16}",
    TURN_AUGUR_COUNTER_CLOCKWISE: "{\"c\":17}",
    BALL_SCREW_UP: "{\"c\":18}",
    BALL_SCREW_DN: "{\"c\":19}"
}

/*
enum Commands {
  //BACKWARD: 0, FORWARD: 1, LEFT: 2, RIGHT: 3,
  MOVE: 1,
  UPACT: 10, DOWNACT: 11, 
  BALLDROP: 18, BALLUP: 19,
  CNVR_COLLECT: 14, CNVR_EMPTY: 15,
  AUGERTURN_CC: 16, AUGERTURN_CCW: 17,
  OFFSET_POS: 20, OFFSET_NEG: 21
};
*/

var cmds = {
    //BACKWARD: 0, FORWARD: 1, LEFT: 2, RIGHT: 3,
    MOVE:               'M',
    STOP:               'S',
    RAISE_ACTUATOR:     'ZR', 
    LOWER_ACTUATOR:     'R', 
    CONVEYOR_COLLECT:   'ZL', 
    CONVEYOR_EMPTY:     'L',
    TURN_AUGUR_CW:      'A', 
    TURN_AUGUR_CCW:     'X',
    BALL_SCREW_DN:      'Y', 
    BALL_SCREW_UP:      'B'
    //OFFSET_POS: 18, OFFSET_NEG: 19
}

var controller = {
    Y: 0, B: 1, A: 2, X: 3, 
    L: 4, ZL: 6,
    R: 5, ZR: 7,
    MINUS: 8, PLUS: 9,
    JOYSTICK_L: 10, JOYSTICK_R: 11,
    HOME: 12, SQUARE: 13,

    left_y: 0,  left_x: 1,
    right_y: 2, right_x: 3,
    arrow_y: 4, arrow_x: 5
};

module.exports = {
    old: old,
    cmds: cmds,
    controller: controller
};