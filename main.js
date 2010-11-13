

// Fluff Example program thingy ------------------------------------------------
// -----------------------------------------------------------------------------
fluff.onLoad = function() {
    fluff.log('loaded!');
    fluff.graphics.setMode(480, 320, 'Fluff Test', 4);
    fluff.graphics.setBackgroundColor(0, 0, 0);
};

fluff.onUpdate = function(dt) {
    //fluff.log(dt);
};

fluff.onDraw = function() {
    fluff.graphics.clear();
    for(var i = 0; i < 100; i++) {
        fluff.graphics.setColor(255, 0, 0);
        fluff.graphics.triangle(100, 0, 50, 200, 150, 200);
        fluff.graphics.setColor(0, 255, 0);
        fluff.graphics.circleFilled(200, 100, 50);
        fluff.graphics.setColor(255, 255, 0);
        fluff.graphics.setColor(0, 255, 255);
        fluff.graphics.rectangleFilled(10, 10, 100, 50);
    }
};

fluff.onExit = function() {
    fluff.log('exited!');
};

