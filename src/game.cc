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
#include "input.h"
#include "fluff.h"
#include <iostream>
using namespace v8;
using namespace std;


// Game Runtime ----------------------------------------------------------------
// -----------------------------------------------------------------------------
void GameCreate(int width, int height, bool full, bool vsync, int fsaa) {
    if (gameWindow.IsOpened()) {
        gameWindow.Close();
    }
    
    sf::WindowSettings settings;
    settings.DepthBits = 8;
    settings.StencilBits = 8;
    settings.AntialiasingLevel = fsaa;
    
    unsigned long style = sf::Style::Close | (full ? sf::Style::Fullscreen : 0);
    gameWindow.Create(sf::VideoMode(width, height, 32), "", style, settings);
    
    gameWindow.EnableKeyRepeat(false);
    gameWindow.UseVerticalSync(vsync);
    gameWindow.SetFramerateLimit(gameFPS);
    gameRunning = true;
    GameInitGL();
}

void GameSetCaption(const char *caption) {
    // TODO modify SFML to support this -.-"
}

void GameInitGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, gameWindow.GetWidth(), gameWindow.GetHeight(), 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GameLoop() {
    HandleScope scope;
    Handle<Number> delta;
    Handle<Value> args[1];
    Handle<Value> vargs[0];
    
    resetInput();
    while (gameRunning) {
        GameEvents();
        
        delta = Number::New(gameWindow.GetFrameTime());
        args[0] = delta;
        callFunction("onUpdate", args, 1);
        callFunction("onDraw", vargs, 0);
        gameWindow.Display();
    }
    callFunction("onExit", vargs, 0);
    gameWindow.Close();
}

void GameEvents() {
    updateInput();
    
    sf::Event event;
    while (gameWindow.GetEvent(event)) {
        switch (event.Type) {
            
            // Keyboard
            case sf::Event::KeyPressed:
                if (event.Key.Code == sf::Key::Escape) {
                    gameRunning = false;
                }
                gameKeys[event.Key.Code] = 1;
                
                if (event.Key.Code == sf::Key::LControl
                    || event.Key.Code == sf::Key::RControl) {
                    
                    gameKeyControl = true;
                }
                if (event.Key.Code == sf::Key::LShift
                    || event.Key.Code == sf::Key::RShift) {
                    
                    gameKeyShift = true;
                }
                if (event.Key.Code == sf::Key::LAlt
                    || event.Key.Code == sf::Key::RAlt) {
                    
                    gameKeyAlt = true;
                }
                break;
            
            case sf::Event::KeyReleased:
                gameKeys[event.Key.Code] = 0;
                
                if (event.Key.Code == sf::Key::LControl
                    || event.Key.Code == sf::Key::RControl) {
                    
                    gameKeyControl = false;
                }
                if (event.Key.Code == sf::Key::LShift
                    || event.Key.Code == sf::Key::RShift) {
                    
                    gameKeyShift = false;
                }
                if (event.Key.Code == sf::Key::LAlt
                    || event.Key.Code == sf::Key::RAlt) {
                    
                    gameKeyAlt = false;
                }
                break;
            
            // Window
            case sf::Event::Closed:
                gameRunning = false;
                break;
            
            case sf::Event::GainedFocus:
                gameFocus = true;
                callSimpleEvent("onFocus");
                break;
            
            case sf::Event::LostFocus:
                gameFocus = false;
                if (gameButtonReset) {
                    resetButtons(false);
                }
                if (gameKeyReset) {
                    resetKeys(false);
                }
                callSimpleEvent("onBlur");
                break;
            
            // Mouse
            case sf::Event::MouseWheelMoved:
                gameMouseScroll = event.MouseWheel.Delta;
                break;
            
            case sf::Event::MouseMoved:
                gameMouseX = event.MouseMove.X;
                gameMouseY = event.MouseMove.Y;
                break;
            
            case sf::Event::MouseEntered:
                gameMouse = true;
                callSimpleEvent("onEnter");
                break;
            
            case sf::Event::MouseLeft:
                gameMouse = false;
                if (gameButtonReset) {
                    resetButtons(false);
                }
                callSimpleEvent("onLeave");
                break;
                
            case sf::Event::MouseButtonPressed:
                gameButtons[event.MouseButton.Button] = 1;
                break;
            
            case sf::Event::MouseButtonReleased:
                gameButtons[event.MouseButton.Button] = 0;
                break;
            
            default:
                break;
        }
    }
}

Handle<Value> GameExit(const Arguments& args) {
    gameRunning = false;
}

