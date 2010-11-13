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

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "graphics.h"
#include "game.h"
#include "fluff.h"

using namespace v8;
using namespace std;


// Game Runtime ----------------------------------------------------------------
// -----------------------------------------------------------------------------
void GameCreate(int width, int height, const char *title, int aa) {
    sf::WindowSettings settings;
    settings.DepthBits = 8;
    settings.StencilBits = 8;
    settings.AntialiasingLevel = aa;
    gameWindow.Create(sf::VideoMode(width, height, 32), title,
                      sf::Style::Close, settings);
    
    gameWindow.SetFramerateLimit(gameFPS);
    
    gameWidth = width;
    gameHeight = height;
    gameRunning = true;
    
    GameInitGL();
}

void GameInitGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, gameWidth, gameHeight, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    
//    glEnable(GL_MULTISAMPLE_ARB);
//    
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    
//    glEnable(GL_POLYGON_SMOOTH);
//    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//    
//    glEnable(GL_LINE_SMOOTH);
//    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
}

void GameLoop() {
    HandleScope scope;
    Handle<Number> delta;
    Handle<Value> args[1];
    Handle<Value> vargs[0];

    while (gameRunning) {
        float elapsedTime = gameClock.GetElapsedTime();
        gameClock.Reset();
        GameEvents();
        
        delta = Number::New(elapsedTime);
        args[0] = delta;
        callFunction("onUpdate", args, 1);
        callFunction("onDraw", vargs, 0);
        gameWindow.Display();
    }
    callFunction("onExit", vargs, 0);
    gameWindow.Close();
}

void GameEvents() {
    sf::Event event;
    while (gameWindow.GetEvent(event)) {
        switch (event.Type) {
            
            // Window
            case sf::Event::Closed:
                gameRunning = false;
                break;
            
            case sf::Event::GainedFocus:
                gameFocus = true;
                break;
            
            case sf::Event::LostFocus:
                gameFocus = false;
                break;
            
            // Mouse
            case sf::Event::MouseMoved:
                break;
            
            case sf::Event::MouseEntered:
                gameMouse = true;
                break;
            
            case sf::Event::MouseLeft:
                gameMouse = false;
                break;
            
            default:
                break;
        }
    }
}

Handle<Value> GameExit(const Arguments& args) {
    gameRunning = false;
}

