import React from 'react';
import Gamepad from 'react-gamepad'
//import { subscribeToTimer, startTimer } from '.././api';
import { sendEvent } from '.././api';

class Controller extends React.Component {
    state = {
        leftX: 20,
        leftY: 20,
        rightX: 20,
        rightY: 20,
        y: "lightgray",
        x: "lightgray",
        a: "lightgray",
        b: "lightgray",
        time: 'no timestamp yet'
    }

    /*constructor() {
        super();
        //startup(this);
    }*/

    getAngle(xL, yL) {
        if(xL === -0)
            xL = 0;
        if(yL === -0)
            yL = 0;

        //Get the angle relative to x and y
        var angle = 0;
        if(xL !== 0) 
            angle = (180 / Math.PI) * Math.atan(yL / xL);
        else if(yL < 0) 
            angle = 270;
        else if(yL > 0) 
            angle = 90;

        //Offset angle based on quadrant
        if(xL < 0)
            angle = angle + 180;
        else if(xL > 0 && yL < 0)
            angle = angle + 360;

        //Get rid of -0
        if(angle === -0)
            angle = 0;

        angle -= 90;
        if(angle < 0)
        angle += 360;

        return angle;
    }

    setButtonColor(buttonName, color) {
        if(buttonName === 'A') this.setState({ y: color});
        if(buttonName === 'Y') this.setState({x: color});
        if(buttonName === 'X') this.setState({a: color});
        if(buttonName === 'B') this.setState({b: color});
    }

    //LeftStickX, LeftStickY, RightStickX, RightStickY
    axisChangeHandler(axisName, value, previousValue) {
        if(axisName === 'LeftStickX') {
            this.setState({ leftX: (value*15) + 20})
        }
        if(axisName === 'LeftStickY') {
            this.setState({ leftY: -(value*15)+20})
        }
        if(axisName === 'RightStickX') {
            this.setState({ rightX: (value*15) + 20})
        }
        if(axisName === 'RightStickY') {
            this.setState({ rightY: -(value*15)+20})
        }
        var xL;
        var yL;

        if(axisName === 'LeftStickX' || axisName === 'LeftStickY') {
            xL = this.state.leftX - 20;
            yL = -(this.state.leftY - 20);
        }
        else {
            xL = this.state.rightX - 20;
            yL = -(this.state.rightY - 20);
        }

        var angle = this.getAngle(xL, yL);

        //How far the joystick is tilted
        var intensity = Math.max(Math.abs(xL), Math.abs(yL));
        var pushed = 0;
        if(intensity > 0)
            pushed = 1;
    
        console.log("Angle: ", angle, " Intensity: ", intensity);
        if(axisName === 'LeftStickX' || axisName === 'LeftStickY')
            sendEvent('joystick', `L:${pushed}:${angle}`);
        else
            sendEvent('joystick', `R:${pushed}:${angle}`);
    }

    //NOTE: Default Button Names do not match buttons on controller
    buttonDownHandler(buttonName) {
        console.log(buttonName, "down");
        this.setButtonColor(buttonName, "gray");
        sendEvent('button', `${buttonName}:1`);
    }

    buttonUpHandler(buttonName) {
        console.log(buttonName, "up");
        this.setButtonColor(buttonName, "lightgray");
        sendEvent('button', `${buttonName}:0`);
    }

    connectHandler(gamepadIndex) {
        console.log(`Gamepad ${gamepadIndex} connected !`)
    }

    disconnectHandler(gamepadIndex) {
        console.log(`Gamepad ${gamepadIndex} disconnected !`)
    }

    render() {
        return (
            <div className="d-inline-flex flex-row p-2" style={{backgroundColor: "yellow"}}>
                <Gamepad
                    onConnect={this.connectHandler}
                    onDisconnect={this.disconnectHandler}

                    onAxisChange={(axisName, value, previousValue) => {
                        this.axisChangeHandler(axisName, value, previousValue)
                    }}

                    onButtonDown={(buttonName) => {
                        this.buttonDownHandler(buttonName)
                    }}

                    onButtonUp={(buttonName) => {
                        this.buttonUpHandler(buttonName)
                    }}
                ><p></p></Gamepad>
                <div className = "JoystickRim m-2">
                    <div className="JoystickCircle" style={{ top: this.state.leftY + "px", left: this.state.leftX + "px" }}/>
                </div>
                
                <div className = "JoystickButton align-self-center p-0 ml-2" style={{backgroundColor: this.state.y}}>
                    <p className = "pt-1">Y</p>
                </div>
                <div className = "col my-2">
                    <div className = "JoystickButton p-0 row" style={{backgroundColor: this.state.x}}> 
                        <p className = "pt-1 ml-2 pl-1">X</p>
                    </div>
                    <div className = "JoystickButton p-0 row mt-4" style={{backgroundColor: this.state.b}}>
                        <p className = "pt-1 ml-2 pl-1">B</p>
                    </div>
                </div>
                <div className = "JoystickButton align-self-center mr-2" style={{backgroundColor: this.state.a}}> 
                    <p className = "pt-1">A</p>
                </div>

                <div className = "JoystickRim m-2">
                    <div className="JoystickCircle" style={{ top: this.state.rightY + "px", left: this.state.rightX + "px" }}/>
                </div>
            </div>
        )
    }
}

export default Controller;