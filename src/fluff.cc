/*
  
  Fluff.js
  Copyright (c) 2010 Ivo Wetzel.
  
  All rights reserved.
  
  Fluff.js is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Fluff.js is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  Fluff.js. If not, see <http://www.gnu.org/licenses/>.
  
*/

#include <v8.h>
#include "fluff.h"
#include "util.cc"
#include "objects.cc"
#include "graphics.cc"
#include "game.cc"

using namespace v8;
using namespace std;


void setupFluff() {
    HandleScope scope;
    
    // Graphics
    Handle<ObjectTemplate> graphicsTemplate = ObjectTemplate::New();
    graphicsTemplate->Set(String::New("setMode"), FunctionTemplate::New(GraphicsSetMode));
    graphicsTemplate->Set(String::New("setFPS"), FunctionTemplate::New(GraphicsSetFPS));
    
    graphicsTemplate->Set(String::New("setBackgroundColor"), FunctionTemplate::New(GraphicsSetBackgroundColor));
    graphicsTemplate->Set(String::New("clear"), FunctionTemplate::New(GraphicsClear));
    
    graphicsTemplate->Set(String::New("setBlendMode"), FunctionTemplate::New(GraphicsSetBlendMode));
    graphicsTemplate->Set(String::New("setColor"), FunctionTemplate::New(GraphicsSetColor));
    
    graphicsTemplate->Set(String::New("setPolygonSmoothing"), FunctionTemplate::New(GraphicsSetPolygonSmoothing));
    
    graphicsTemplate->Set(String::New("setLineWidth"), FunctionTemplate::New(GraphicsSetLineWidth));
    graphicsTemplate->Set(String::New("setLineSmoothing"), FunctionTemplate::New(GraphicsSetLineSmoothing));
    
    graphicsTemplate->Set(String::New("line"), FunctionTemplate::New(GraphicsLine));    
    
    graphicsTemplate->Set(String::New("circle"), FunctionTemplate::New(GraphicsCircle));
    graphicsTemplate->Set(String::New("circleFilled"), FunctionTemplate::New(GraphicsCircleFilled));
    
    graphicsTemplate->Set(String::New("triangle"), FunctionTemplate::New(GraphicsTriangle));
    graphicsTemplate->Set(String::New("triangleFilled"), FunctionTemplate::New(GraphicsTriangleFilled)); 
    
    graphicsTemplate->Set(String::New("rectangle"), FunctionTemplate::New(GraphicsRectangle));
    graphicsTemplate->Set(String::New("rectangleFilled"), FunctionTemplate::New(GraphicsRectangleFilled)); 
    
    graphicsTemplate->Set(String::New("polygon"), FunctionTemplate::New(GraphicsPolygon));
    graphicsTemplate->Set(String::New("polygonFilled"), FunctionTemplate::New(GraphicsPolygonFilled)); 
    
    graphicsTemplate->Set(String::New("rotate"), FunctionTemplate::New(GraphicsRotate));
    graphicsTemplate->Set(String::New("translate"), FunctionTemplate::New(GraphicsTranslate));
    graphicsTemplate->Set(String::New("scale"), FunctionTemplate::New(GraphicsScale));
    
    graphicsTemplate->Set(String::New("push"), FunctionTemplate::New(GraphicsTranslate));
    graphicsTemplate->Set(String::New("pop"), FunctionTemplate::New(GraphicsScale)); 
    
    // Tuff
    Handle<ObjectTemplate> fluffTemplate = ObjectTemplate::New();
    fluffTemplate->Set(String::New("log"), FunctionTemplate::New(log));
    fluffTemplate->Set(String::New("exit"), FunctionTemplate::New(GameExit));
    
    fluff = Persistent<Object>::New(fluffTemplate->NewInstance());
    fluff->Set(String::New("graphics"), Persistent<Object>::New(graphicsTemplate->NewInstance()));
    
    global->Set(String::New("fluff"), fluff);
}


// Main ------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    HandleScope scope;
    Persistent<Context> context = Context::New();
    Context::Scope contextScope(context);
    global = context->Global();
    setupFluff();
    
    loadGame();
    Handle<Value> args[0];
    callFunction("onLoad", args, 0);
    
    if (gameRunning) {
        GameLoop();
    }
    
    context.Dispose();
    return 0;
}

