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
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "graphics.h"
#include "game.h"
#include "fluff.h"

using namespace v8;
using namespace std;


// Graphics --------------------------------------------------------------------
// -----------------------------------------------------------------------------
int ToInt32(Handle<Value> i) {
    return i->Int32Value();
}

Handle<Value> GraphicsSetMode(const Arguments& args) {
    HandleScope scope;
    if (args.Length() == 3 && args[0]->IsNumber()
        && args[1]->IsNumber() && args[2]->IsString()) {
        
        String::Utf8Value title(args[2]->ToString());
        GameCreate(ToInt32(args[0]), ToInt32(args[1]), *title);
        scope.Close(Boolean::New(true));
    
    } else {
        scope.Close(Boolean::New(false));
    }
}

Handle<Value> GraphicsSetFPS(const Arguments& args) {
    HandleScope scope;
    if (args.Length() == 1) {
        gameFPS = ToInt32(args[0]);
        gameWindow.SetFramerateLimit(gameFPS);
        scope.Close(Boolean::New(true));
    
    } else {
        scope.Close(Boolean::New(false));
    }
}


