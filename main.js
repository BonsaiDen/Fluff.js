
var test = require('foo');


// Fluff Example program thingy ------------------------------------------------
// -----------------------------------------------------------------------------
fluff.onLoad = function() {
    fluff.graphics.setMode(480, 320);
    fluff.graphics.setBackgroundColor(0, 0, 0);
};

fluff.onUpdate = function(dt) {
    //fluff.log(dt);
    
    if (fluff.input.getMouseDown(0)) {
        fluff.log('mouse down!');
    }
    
    if (fluff.input.getMouseUp(0)) {
        fluff.log('mouse up!');
    }
    if (fluff.input.getMouseClicked(0)) {
        fluff.log('mouse clicked!');
    }
    
    if (fluff.input.getKeyDown(0)) {
        fluff.log('key down!');
    }
    
    if (fluff.input.getKeyUp(0)) {
        fluff.log('key up!');
    }
    if (fluff.input.getKeyPressed(0)) {
        fluff.log('key pressed!');
    }   
    
    //fluff.log(fluff.input.getMouseScrolled());
    
    if (fluff.input.hasMouse()) {
        //fluff.log(fluff.input.getMouseX());
    }
    
};

fluff.onDraw = function() {
    fluff.graphics.clear();
 //   for(var i = 0; i < 100; i++) {
        fluff.graphics.setColor(255, 0, 0);
        fluff.graphics.triangle(100, 0, 50, 200, 150, 200);
        fluff.graphics.setColor(0, 255, 0);
        fluff.graphics.circleFilled(200, 100, 50);
        fluff.graphics.setColor(255, 255, 0);
        fluff.graphics.setColor(0, 255, 255);
        fluff.graphics.rectangleFilled(10, 10, 100, 50);
//    }
};

fluff.onExit = function() {
};


// Events ----------------------------------------------------------------------
fluff.input.onEnter = function(e) {
  //  fluff.log('entered!');
};

fluff.input.onLeave = function(e) {
 //   fluff.log('left!');
};

fluff.input.onFocus = function(e) {
  //  fluff.log('focused!');
};

fluff.input.onBlur = function(e) {
//    fluff.log('blurred!');
};


