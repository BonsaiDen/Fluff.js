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
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

using namespace v8;
using namespace std;

#ifndef FLUFF_H
#define FLUFF_H

// V8 --------------------------------------------------------------------------
Handle<Object> global;
Persistent<Object> fluff;


// Game ------------------------------------------------------------------------
sf::Window gameWindow;
sf::Clock gameClock;
bool gameRunning = false;
int gameFPS = 30;
bool gameFocus = false;
bool gameMouse = false;

int gameStacks = 0;
int gameWidth = 0;
int gameHeight = 0;


// JavaScript ------------------------------------------------------------------
Handle<Script> loadScript(const char *filename);
void loadGame();


// Fluff -----------------------------------------------------------------------
bool callFunction(const char *name, Handle<Value> *args, int argc);
void setupFluff();


// Objects ---------------------------------------------------------------------
Handle<Value> log(const Arguments& args);

#endif

