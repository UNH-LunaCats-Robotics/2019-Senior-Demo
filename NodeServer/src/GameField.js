import React from 'react';
import { startup } from './api';

class GridPoint extends React.Component {

    render() {
        return (
            <div className="GridPoint" style={{ 
                top: (388 - this.props.y) + "px", 
                left: (this.props.x) + "px",
                backgroundColor: this.props.color
             }}/>
        )
    }
}

class RobotPoint extends React.Component {
    render() {
        return (
            <div className="RobotPoint" style={{ 
                top: this.props.y + "px", 
                left: this.props.x + "px"
             }}/>
        )
    }
}

class GameField extends React.Component {

    state = {
        points: [],
        robotLoc: [0, 0, 0, 0],
        count: 0
    };

    inchesToFeet(v) {
        return v / 12.0;
    }

    feetToMeters(v) {
        return v * 0.3048;
    }

    centiToMeters(v) {
        return v / 100.0;
    }

    convert(v) {
        //v = this.inchesToFeet(v);
        //v = this.feetToMeters(v);
        //v = this.centiToMeters(v);
        //v = v * 380 / 5.4;
        //return Math.round(v);
        return v;
    }

    addPoint(p) {
        console.log(`Lidar Point: (${p[0]}, ${p[1]}, ${p[2]}) Key: ${this.state.count}`);
        var xp = this.convert(p[0]);
        var yp = this.convert(p[1]);
        var zp = this.convert(p[2]);
        if(xp < 0 || xp > 380 || yp < 0 || yp > 252) {
            console.log("Point out of bounds");
            return;
        }
        this.setState({points: this.state.points.push({
            key: this.state.count,
            x: xp,
            y: yp,
            z: zp
        })});
        this.setState({count: this.state.count + 1});
    }

    updatePosition(l) {
        console.log(`Robot Position: (${l[0]}, ${l[1]}, ${l[2]}) at ${l[3]}`);
        this.setState({robotLoc: [l[0], l[1], l[2], l[3]]});
        this.render();
    }

    test() {
        this.addPoint([0, 0, 4]);
        this.addPoint([92, 82, 13]);
        this.addPoint([134, 122, -42]);
        this.addPoint([380, 252, 30]);
        this.addPoint([385, 200, 30]);
        //0 <= x <= 380
        //0 <= y <= 252
    }

    constructor() {
        super();
        startup(this);
        //this.test();
    }

    getColor(z) {
        if(z >= 10)
            return "red";
        else if(z <= -10)
            return "blue";
        else
            return "lightgreen";
    }

    render() {
        return (
            <div className="GridBackground" style={{backgroundColor: "white"}}>
                {this.state.points.map( (inc) => 
                    {
                        return <GridPoint key = {inc.key} x = {inc.x} y = {inc.y} color = {this.getColor(inc.z)}/>
                    }
                )}
                <RobotPoint x= {this.state.robotLoc[0]} y = {this.state.robotLoc[1]}/>
            </div>
        )
    }
}

export default GameField;