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
#include <string>
#include <stdio.h>
#include "util.h"

using namespace v8;
using namespace std;


// JavaScript ------------------------------------------------------------------
// -----------------------------------------------------------------------------
Handle<Value> requireScript(const Arguments& args) {
    HandleScope scope;
    if (args.Length() == 1) {
        String::Utf8Value name(args[0]->ToString());
        return scope.Close(executeScript(loadScript(*name)));
    
    } else {
        return Undefined();
    }
}

Handle<Value> executeScript(Handle<Script> script) {
    HandleScope scope;
    TryCatch tryCatch;
    Handle<Value> result = script->Run();
    if (result.IsEmpty()) {
        handleException(&tryCatch);
        return Undefined();
    
    } else {
        return scope.Close(result);
    }
}

Handle<Script> loadScript(const char *name) {
    
    // Create filename
    string filename(name);
    filename.append(".js");
    
    // Handles
    HandleScope scope;
    Handle<Script> script;
    Handle<String> source;
    TryCatch tryCatch;
    
    // Load File
    ifstream file(filename.data());
    if (file.is_open()) {
        file.seekg(0, ios::end);
        ifstream::pos_type size = file.tellg();
        
        char *data = new char[size];
        file.seekg(0, ios::beg);
        file.read(data, size);
        file.close();
        source = String::New(data, size); 
        delete[] data;
    
    } else {
        printf("Error: Could not find module \"%s\"\n", name);
        script = Script::Compile(String::New("undefined"), String::New(filename.data()));
        return scope.Close(script);
    }
    
    // Wrap it!
    Handle<String> pre = String::New("(function() {var exports = {}; (function(exports, global) {\n");
    Handle<String> end = String::New("\n})(exports, global); return exports;})();");
    Handle<String> wrapped = String::Concat(String::Concat(pre, source), end);
    
    script = Script::Compile(wrapped, String::New(filename.data()));
    if (script.IsEmpty()) {
        printf("Error: Could not load module \"%s\"\n", name);
        handleException(&tryCatch);
        script = Script::Compile(String::New("undefined"), String::New(filename.data()));
    }
    return scope.Close(script);
}

Handle<Value> log(const Arguments& args) {
    bool first = true;
    for (int i = 0; i < args.Length(); i++) {
        HandleScope handle_scope;
        if (first) {
            first = false;
        
        } else {
            printf(" ");
        }
        
        String::Utf8Value str(args[i]);
        printf("%s", *str);
    }
    printf("\n");
    return Undefined();
}

void handleException(TryCatch* tryCatch) {
    HandleScope scope;
    String::Utf8Value exception(tryCatch->Exception());
    Handle<Message> message = tryCatch->Message();
    if (message.IsEmpty()) {
        printf("%s\n", *exception);
    
    } else {
    
        // Filename, line, message
        String::Utf8Value filename(message->GetScriptResourceName());
        int line = message->GetLineNumber();
        printf("%s:%i: %s\n", *filename, line, *exception);
        
        // Sourceline
        String::Utf8Value sourceLine(message->GetSourceLine());
        printf("%s\n", *sourceLine);
        
        int start = message->GetStartColumn(), end = message->GetEndColumn();
        for(int i = 0; i < start; i++) {
            printf(" ");
        }
        for(int i = start; i < end; i++) {
            printf("^");
        }
        printf("\n");
        
        // Stack Trace
        String::Utf8Value stackTrace(tryCatch->StackTrace());
        if (stackTrace.length() > 0) {
            printf("%s\n", *stackTrace);
        } 
    }
}

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

