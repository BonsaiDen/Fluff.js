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
#include <string>
#include <math.h>
#include "graphics.h"
#include "game.h"
#include "fluff.h"

using namespace v8;
using namespace std;


// Graphics --------------------------------------------------------------------
// -----------------------------------------------------------------------------
const float PI = 3.14159;

inline int ToInt32(Handle<Value> i) {
    return i->Int32Value();
}

inline float ToFloat(Handle<Value> f) {
    return static_cast<float>(f->NumberValue());
}

bool CheckNumbers(const Arguments& args, int count) {
    if (args.Length() == count) {
        for(int i = 0; i < count; i++) {
            if (!args[i]->IsNumber()) {
                return false;
            }
        }
        return true;
        
    } else {
        return false;
    }
}

Handle<Value> GraphicsSetMode(const Arguments& args) {
    if (args.Length() == 4 && args[0]->IsNumber()
        && args[1]->IsNumber() && args[2]->IsString()
        && args[3]->IsNumber()) {
        
        String::Utf8Value title(args[2]->ToString());
        GameCreate(ToInt32(args[0]), ToInt32(args[1]), *title, ToInt32(args[3]));
    }
    return Undefined();
}

Handle<Value> GraphicsSetFPS(const Arguments& args) {
    if (args.Length() == 1) {
        gameFPS = ToInt32(args[0]);
        gameWindow.SetFramerateLimit(gameFPS);
    }
    return Undefined();
}


// Settings --------------------------------------------------------------------
Handle<Value> GraphicsSetBackgroundColor(const Arguments& args) {
    glClearColor(ToFloat(args[0]) / 255.f,
                 ToFloat(args[1]) / 255.f,
                 ToFloat(args[2]) / 255.f, 1.f);
    
    return Undefined();
}

Handle<Value> GraphicsClear(const Arguments& args) {
    glClear(GL_COLOR_BUFFER_BIT);
    return Undefined();
}

Handle<Value> GraphicsSetBlendMode(const Arguments& args) {
    String::Utf8Value mode(args[0]->ToString());
    string cmode = *mode;
    
    if (cmode.compare("additive") == 0) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    } else if (cmode.compare("default") == 0) {
        glDisable(GL_BLEND);
    }
    return Undefined();
}

Handle<Value> GraphicsSetLineSmoothing(const Arguments& args) {
    if (args[0]->IsTrue()) {
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    
    } else {
        glDisable(GL_LINE_SMOOTH);
    }
    return Undefined();
}

Handle<Value> GraphicsSetPolygonSmoothing(const Arguments& args) {
    if (args[0]->IsTrue()) {
        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
    
    } else {
        glDisable(GL_POLYGON_SMOOTH);
    }
    return Undefined();
}

Handle<Value> GraphicsSetLineWidth(const Arguments& args) {
    glLineWidth(ToFloat(args[0]));
    return Undefined();
}

Handle<Value> GraphicsSetColor(const Arguments& args) {
    glColor3f(ToFloat(args[0]) / 255.f,
              ToFloat(args[1]) / 255.f,
              ToFloat(args[2]) / 255.f);
    
    return Undefined();
}


// Shapes ----------------------------------------------------------------------
Handle<Value> GraphicsLine(const Arguments& args) {
    glBegin(GL_LINES);
    glVertex2f(ToFloat(args[0]), ToFloat(args[1]));
    glVertex2f(ToFloat(args[2]), ToFloat(args[3]));
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsTriangle(const Arguments& args) {
    glBegin(GL_LINE_LOOP);
    glVertex2f(ToFloat(args[0]), ToFloat(args[1]));
    glVertex2f(ToFloat(args[2]), ToFloat(args[3]));
    glVertex2f(ToFloat(args[4]), ToFloat(args[5]));
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsTriangleFilled(const Arguments& args) {
    glBegin(GL_TRIANGLES);
    glVertex2f(ToFloat(args[0]), ToFloat(args[1]));
    glVertex2f(ToFloat(args[2]), ToFloat(args[3]));
    glVertex2f(ToFloat(args[4]), ToFloat(args[5]));
    glVertex2f(ToFloat(args[0]), ToFloat(args[1]));
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsCircle(const Arguments& args) {
    glBegin(GL_LINE_LOOP);
    const float x = ToFloat(args[0]);
    const float y = ToFloat(args[1]);
    const float r = ToFloat(args[2]);
    
    const float pi = PI / (1.5f * r);
    for(int i = 0; i <= 3 * r; i++) {
        const float rad = pi * (float)i;
        glVertex2f(x + sin(rad) * r, y + cos(rad) * r);
    }
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsCircleFilled(const Arguments& args) {
    glBegin(GL_TRIANGLE_FAN);
    const float x = ToFloat(args[0]);
    const float y = ToFloat(args[1]);
    const float r = ToFloat(args[2]);
    
    glVertex2f(x, y) ;
    const float pi = PI / (1.5f * r);
    for(int i = 0; i <= 3 * r; i++) {
        const float rad = pi * (float)i;
        glVertex2f(x + sin(rad) * r, y + cos(rad) * r);
    }
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsRectangle(const Arguments& args) {
    const float x = ToFloat(args[0]);
    const float y = ToFloat(args[1]);
    const float w = ToFloat(args[2]);
    const float h = ToFloat(args[3]);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsRectangleFilled(const Arguments& args) {
    const float x = ToFloat(args[0]);
    const float y = ToFloat(args[1]);
    const float w = ToFloat(args[2]);
    const float h = ToFloat(args[3]);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsPolygon(const Arguments& args) {
    HandleScope scope;
    Handle<Array> points = Handle<Array>::Cast(args[0]);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < points->Length(); i += 2) {
        glVertex2f(ToFloat(points->Get(Number::New(i))),
                   ToFloat(points->Get(Number::New(i + 1))));
    }
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsPolygonFilled(const Arguments& args) {
    HandleScope scope;
    Handle<Array> points = Handle<Array>::Cast(args[0]);
    glBegin(GL_POLYGON);
    for(int i = 0; i < points->Length(); i += 2) {
        glVertex2f(ToFloat(points->Get(Number::New(i))),
                   ToFloat(points->Get(Number::New(i + 1))));
    }
    glEnd();
    return Undefined();
}


// Others ----------------------------------------------------------------------
Handle<Value> GraphicsRotate(const Arguments& args) {
    glRotatef(ToFloat(args[0]), 0.0, 0.0, 1.0);
    return Undefined();
}

Handle<Value> GraphicsTranslate(const Arguments& args) {
    glTranslatef(ToFloat(args[0]), ToFloat(args[1]), 0);
    return Undefined();
}

Handle<Value> GraphicsScale(const Arguments& args) {
    glScalef(ToFloat(args[0]), ToFloat(args[1]), 1);
    return Undefined();
}

Handle<Value> GraphicsPush(const Arguments& args) {
    gameStacks++;
    glPushMatrix();
    return Undefined();
}

Handle<Value> GraphicsPop(const Arguments& args) {
    if (gameStacks > 0) {
        glPopMatrix();
        gameStacks--;
    }
    return Undefined();
}

