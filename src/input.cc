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
#include "util.h"
#include "fluff.h"

using namespace v8;
using namespace std;


// Input ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool callSimpleEvent(const char *name) {
    HandleScope scope;
    Handle<Value> object = input->Get(String::NewSymbol(name));
    
    if (object->IsFunction()) {
        Handle<Function> func = Handle<Function>::Cast(object);
        func->Call(fluff, 0, NULL);
        return true;
    
    } else {
        return false;
    }
}

void resetButtons(bool old) {
    for(int i = 0; i < BUTTON_COUNT; i++) {
        gameButtons[i] = 0;
        if (old) {
            gameButtonsOld[i] = 0;
        }
    }
    gameMouseScroll = 0;
    if (old) {
        gameMouseScrollOld = 0;
    }
}

void resetKeys(bool old) {
    for(int i = 0; i < KEY_COUNT; i++) {
        gameKeys[i] = 0;
        if (old) {
            gameKeysOld[i] = 0;
        }
    }
    gameKeyShift = false;
    gameKeyControl = false;
    gameKeyAlt = false;
}

void resetInput() {
    resetButtons(true);
    resetKeys(true);
}

void updateInput() {
    for(int i = 0; i < BUTTON_COUNT; i++) {
        gameButtonsOld[i] = gameButtons[i];
        if (gameButtons[i] == 1) {
            gameButtons[i] = 2;
        }
    }
    for(int i = 0; i < KEY_COUNT; i++) {
        gameKeysOld[i] = gameKeys[i];
        if (gameKeys[i] == 1) {
            gameKeys[i] = 2;
        }
    }
    gameMouseScrollOld = gameMouseScroll;
    gameMouseScroll = 0;
}

// General ---------------------------------------------------------------------
Handle<Value> InputHasFocus(const Arguments& args) {
    return Boolean::New(gameFocus);
}

Handle<Value> InputHasMouse(const Arguments& args) {
    return Boolean::New(gameMouse);
}


// Mouse -----------------------------------------------------------------------
Handle<Value> InputGetButtonReset(const Arguments& args) {
    return Boolean::New(gameButtonReset);
}

Handle<Value> InputSetButtonReset(const Arguments& args) {
    gameButtonReset = args[0]->IsTrue();
    return Undefined();
}

Handle<Value> InputMouseDown(const Arguments& args) {
    int button = ToInt32(args[0]);
    return Boolean::New(gameButtons[button] > 0);
}

Handle<Value> InputMouseUp(const Arguments& args) {
    int button = ToInt32(args[0]); 
    return Boolean::New(gameButtons[button] == 0 && gameButtonsOld[button] > 0);
}

Handle<Value> InputMouseClicked(const Arguments& args) {
    int button = ToInt32(args[0]);
    return Boolean::New(gameButtons[button] == 1);
}

Handle<Value> InputMouseScrolled(const Arguments& args) {
    return Number::New(gameMouseScroll);
}

Handle<Value> InputMouseX(const Arguments& args) {
    return Number::New(gameMouseX);
}

Handle<Value> InputMouseY(const Arguments& args) {
    return Number::New(gameMouseY);
}


// Keyboard --------------------------------------------------------------------
Handle<Value> InputGetKeyReset(const Arguments& args) {
    return Boolean::New(gameKeyReset);
}

Handle<Value> InputSetKeyReset(const Arguments& args) {
    gameKeyReset = args[0]->IsTrue();
    return Undefined();
}

Handle<Value> InputKeyDown(const Arguments& args) {
    int key = ToInt32(args[0]);
    return Boolean::New(gameKeys[key] > 0);
}

Handle<Value> InputKeyUp(const Arguments& args) {
    int key = ToInt32(args[0]); 
    return Boolean::New(gameKeys[key] == 0 && gameKeysOld[key] > 0);
}

Handle<Value> InputKeyPressed(const Arguments& args) {
    int key = ToInt32(args[0]);
    return Boolean::New(gameKeys[key] == 1);
}

Handle<Value> InputShift(const Arguments& args) {
    return Boolean::New(gameKeyShift);
}

Handle<Value> InputControl(const Arguments& args) {
    return Boolean::New(gameKeyControl);
}

Handle<Value> InputAlt(const Arguments& args) {
    return Boolean::New(gameKeyAlt);
}






