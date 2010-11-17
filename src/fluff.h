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
#include <SFML/Graphics.hpp>

using namespace v8;
using namespace std;

#ifndef FLUFF_H
#define FLUFF_H


// V8 --------------------------------------------------------------------------
Handle<Object> global;
Persistent<Object> fluff;
Persistent<Object> input;
Persistent<Object> network;


// Game ------------------------------------------------------------------------
sf::RenderWindow gameWindow;

// General
bool gameRunning;
bool gameReload;
int gameFPS;
bool gameFocus;
bool gameMouse;
int gameCursor;

// OpenGL
int gameStacks;
float gameBackColorR;
float gameBackColorG;
float gameBackColorB;
float gameBackColorA;

float gameColorR;
float gameColorG;
float gameColorB;
float gameColorA;

int gameBlendMode;

float gameFontRotation;
float gameFontScaleX;
float gameFontScaleY;

// Font
bool gameUseFonts;
string gameFont;
vector<sf::Font*> gameFontData;
vector<string*> gameFontNames;

// Keyboard
const int KEY_COUNT = 384;
int gameKeyReset;
int gameKeys[KEY_COUNT];
int gameKeysOld[KEY_COUNT];
bool gameKeyShift;
bool gameKeyControl;
bool gameKeyAlt;

// Mouse
const int BUTTON_COUNT = 10;
int gameMouseX;
int gameMouseY;
bool gameButtonReset;
int gameButtons[BUTTON_COUNT];
int gameButtonsOld[BUTTON_COUNT];
int gameMouseScrollOld;
int gameMouseScroll;

#endif

