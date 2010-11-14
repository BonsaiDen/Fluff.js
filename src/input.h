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

using namespace v8;
using namespace std;


// Input ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool callSimpleEvent(const char *name);
void resetButtons(bool old);
void resetInput();
void updateInput();


// General ---------------------------------------------------------------------
Handle<Value> InputHasFocus(const Arguments& args);
Handle<Value> InputHasMouse(const Arguments& args);


// Mouse -----------------------------------------------------------------------
Handle<Value> InputGetButtonReset(const Arguments& args);
Handle<Value> InputSetButtonReset(const Arguments& args);

Handle<Value> InputMouseDown(const Arguments& args);
Handle<Value> InputMouseUp(const Arguments& args);
Handle<Value> InputMouseClicked(const Arguments& args);
Handle<Value> InputMouseScrolled(const Arguments& args);

Handle<Value> InputMouseX(const Arguments& args);
Handle<Value> InputMouseY(const Arguments& args);


// Keyboard --------------------------------------------------------------------
Handle<Value> InputGetKeyReset(const Arguments& args);
Handle<Value> InputSetKeyReset(const Arguments& args);

Handle<Value> InputKeyDown(const Arguments& args);
Handle<Value> InputKeyUp(const Arguments& args);
Handle<Value> InputKeyPressed(const Arguments& args);

Handle<Value> InputShift(const Arguments& args);
Handle<Value> InputControl(const Arguments& args);
Handle<Value> InputAlt(const Arguments& args);



