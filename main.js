
var test = require('foo'); // require works!

var chr = String.fromCharCode;



// Fluff example program thingy ------------------------------------------------
// -----------------------------------------------------------------------------
var socket;
fluff.onLoad = function() {
    fluff.graphics.setMode(480, 320);
    fluff.graphics.setBackgroundColor(0, 0, 0);
   // fluff.graphics.setBlendMode('default');
    
    socket = new fluff.network.FluffSocket();
    
    socket.onConnect = function() {
        this.send('Hello Server from Fluff.js! ÄÖÜ' + chr(0) + 'foo' + chr(255));
    };
    
    socket.onMessage = function(msg) {
        fluff.log(msg);
    };
    
    socket.onError = function() {
        fluff.log('error');
    };
    
    socket.onClose = function() {
        fluff.log('closed');
    };
    socket.connect("127.0.0.1", 4731);
};

fluff.onUpdate = function(dt) {
    
    // Test Mode Switch
    if (fluff.input.getKeyPressed(49)) {
        fluff.graphics.setMode(480, 320);
    
    } else if (fluff.input.getKeyPressed(50)) {
        fluff.graphics.setMode(640, 480);
        fluff.graphics.setBackgroundColor(255, 0, 255);
    
    } else if (fluff.input.getKeyPressed(51)) {
        socket.close();
    }
    
    if (fluff.input.getMouseClicked(0)) {
        socket.send('Mouse clicked.. uh?');
    }
};

var alpha = 0;
fluff.onDraw = function() {
    fluff.graphics.clear();
//    fluff.graphics.setAlpha(alpha);
//    alpha += 0.01;
//    if (alpha > 1) {
//        alpha = 0;
//    }
//    for(var i = 0; i < 10; i++) {
//        fluff.graphics.push();
//        fluff.graphics.translate(Math.random() * 10, Math.random() * 10);
//        fluff.graphics.setColor(255, 0, 0);
//        fluff.graphics.triangle(100, 0, 50, 200, 150, 200);
//        fluff.graphics.setColor(0, 255, 0);
//        fluff.graphics.circleFilled(200, 100, 50);
//        fluff.graphics.setColor(255, 255, 0);
//        fluff.graphics.setColor(0, 255, 255);
//        fluff.graphics.rectangleFilled(10, 10, 100, 50);
//        fluff.graphics.pop();
//    }
};

fluff.onExit = function() {
};


// Events ----------------------------------------------------------------------
fluff.input.onEnter = function(e) {
   // fluff.log('entered!');
};

fluff.input.onLeave = function(e) {
  //  fluff.log('left!');
};

fluff.input.onFocus = function(e) {
   // fluff.log('focused!');
};

fluff.input.onBlur = function(e) {
   // fluff.log('blurred!');
};


