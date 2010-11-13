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
#include <fstream>
#include "fluff.h"

using namespace v8;
using namespace std;


// JavaScript ------------------------------------------------------------------
// -----------------------------------------------------------------------------
void loadGame() {
    HandleScope scope;
    Handle<Script> script = loadScript("main.js");
    Handle<Value> result = script->Run();
}

Handle<Script> loadScript(const char *filename) {
    HandleScope scope;
    Handle<String> source = String::New("fluff.log('Could not find \"main.js\"')");
    ifstream file(filename);
    
    if (file.is_open()) {
        file.seekg(0, ios::end);
        ifstream::pos_type size = file.tellg();
        
        char *data = new char[size];
        file.seekg(0, ios::beg);
        file.read(data, size);
        file.close();
        source = String::New(data, size); 
        delete[] data;
    }
    
    if (source.IsEmpty()) {
        source = String::New("fluff.log('Error loading \"main.js\".')");
    }
    Handle<Script> script = Script::Compile(source, String::New(filename));
    return scope.Close(script);
}


// Tuff ------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool callFunction(const char *name, Handle<Value> *args, int argc) {
    HandleScope scope;
    Handle<Value> object = fluff->Get(String::NewSymbol(name));
    
    if (object->IsFunction()) {
        Handle<Function> func = Handle<Function>::Cast(object);
        func->Call(fluff, argc, args);
        return true;
    
    } else {
        return false;
    }
}

