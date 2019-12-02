import React from 'react';
import './App.scss';
import Controller from './ControlDisplay/Controller.js'
import GameField from './GameField.js'

class App extends React.Component {
  render() {
    return (
      <div className="App App-header">
        <Controller/>
        <GameField/>
      </div>
    );
  }
}

export default App;