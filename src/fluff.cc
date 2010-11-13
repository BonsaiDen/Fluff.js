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

