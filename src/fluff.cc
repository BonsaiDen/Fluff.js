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

#define EXPOSE(obj, jsname, func) obj->Set(String::New(jsname), FunctionTemplate::New(func))
#define OBJ_TEMPLATE(obj) Handle<ObjectTemplate> obj = ObjectTemplate::New()
#define SET(obj, name, data) obj->Set(String::New(name), data)
#define POBJECT(obj) Persistent<Object>::New(obj->NewInstance())
#define FUNC_TEMPLATE(obj, func) Handle<FunctionTemplate> obj = FunctionTemplate::New(func);

#include <v8.h>
#include "fluff.h"
#include "util.cc"
#include "input.cc"
#include "graphics.cc"
#include "game.cc"

using namespace v8;
using namespace std;


void setupFluff() {
    HandleScope scope;
    
    // Input
    OBJ_TEMPLATE(inputTemplate);
    EXPOSE(inputTemplate, "hasFocus", InputHasFocus);
    EXPOSE(inputTemplate, "hasMouse", InputHasMouse);
    
    EXPOSE(inputTemplate, "getButtonReset", InputGetButtonReset);
    EXPOSE(inputTemplate, "setButtonReset", InputSetButtonReset);
    
    EXPOSE(inputTemplate, "getMouseDown", InputMouseDown);
    EXPOSE(inputTemplate, "getMouseUp", InputMouseUp);
    EXPOSE(inputTemplate, "getMouseClicked", InputMouseClicked);
    
    EXPOSE(inputTemplate, "getMouseScrolled", InputMouseScrolled);
    
    EXPOSE(inputTemplate, "getMouseX", InputMouseX);
    EXPOSE(inputTemplate, "getMouseY", InputMouseY);
    
    EXPOSE(inputTemplate, "getKeyReset", InputGetKeyReset);
    EXPOSE(inputTemplate, "setKeyReset", InputSetKeyReset);
    
    EXPOSE(inputTemplate, "getKeyDown", InputKeyDown);
    EXPOSE(inputTemplate, "getKeyUp", InputKeyUp);
    EXPOSE(inputTemplate, "getKeyPressed", InputKeyPressed);
    
    EXPOSE(inputTemplate, "getControl", InputControl);
    EXPOSE(inputTemplate, "getShift", InputShift);
    EXPOSE(inputTemplate, "getAlt", InputAlt);
    
    // Graphics
    OBJ_TEMPLATE(graphicsTemplate);
    EXPOSE(graphicsTemplate, "setMode", GraphicsSetMode);
    EXPOSE(graphicsTemplate, "setFPS", GraphicsSetFPS);
    EXPOSE(graphicsTemplate, "getFPS", GraphicsGetFPS); 
    EXPOSE(graphicsTemplate, "setPosition", GraphicsSetPosition);
    EXPOSE(graphicsTemplate, "setMouse", GraphicsSetMouse);
    EXPOSE(graphicsTemplate, "getMouse", GraphicsGetMouse);
    
    EXPOSE(graphicsTemplate, "setBackgroundColor", GraphicsSetBackgroundColor);
    EXPOSE(graphicsTemplate, "clear", GraphicsClear);
    
    EXPOSE(graphicsTemplate, "setBlendMode", GraphicsSetBlendMode);
    EXPOSE(graphicsTemplate, "setColor", GraphicsSetColor);
    
    EXPOSE(graphicsTemplate, "setPolygonSmoothing", GraphicsSetPolygonSmoothing);
    
    EXPOSE(graphicsTemplate, "setLineWidth", GraphicsSetLineWidth);
    EXPOSE(graphicsTemplate, "setLineSmoothing", GraphicsSetLineSmoothing);
    
    EXPOSE(graphicsTemplate, "line", GraphicsLine);
    
    EXPOSE(graphicsTemplate, "circle", GraphicsCircle);
    EXPOSE(graphicsTemplate, "circleFilled", GraphicsCircleFilled);
    
    EXPOSE(graphicsTemplate, "triangle", GraphicsTriangle);
    EXPOSE(graphicsTemplate, "triangleFilled", GraphicsTriangleFilled);
    
    EXPOSE(graphicsTemplate, "rectangle", GraphicsRectangle);
    EXPOSE(graphicsTemplate, "rectangleFilled", GraphicsRectangleFilled);
    
    EXPOSE(graphicsTemplate, "polygon", GraphicsPolygon);
    EXPOSE(graphicsTemplate, "polygonFilled", GraphicsPolygonFilled);
    
    EXPOSE(graphicsTemplate, "rotate", GraphicsRotate);
    EXPOSE(graphicsTemplate, "translate", GraphicsTranslate);
    EXPOSE(graphicsTemplate, "scale", GraphicsScale);
    
    EXPOSE(graphicsTemplate, "push", GraphicsPush);
    EXPOSE(graphicsTemplate, "pop", GraphicsPop);
    EXPOSE(graphicsTemplate, "reset", GraphicsReset);
    
    // Input
    input = POBJECT(inputTemplate);
    
    // Fluff
    OBJ_TEMPLATE(fluffTemplate);
    EXPOSE(fluffTemplate, "log", log);
    EXPOSE(fluffTemplate, "exit", GameExit);
    fluff = POBJECT(fluffTemplate);
    
    SET(fluff, "graphics", POBJECT(graphicsTemplate));
    SET(fluff, "input", input);
    
    // Global
    FUNC_TEMPLATE(requireTemplate, requireScript);
    SET(global, "fluff", fluff);
    SET(global, "require", requireTemplate->GetFunction());
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
    callFunction("onLoad", NULL, 0);
    
    if (gameRunning) {
        GameLoop();
    }
    
    global.Clear();
    fluff.Clear();
    context.Dispose();
    return 0;
}

