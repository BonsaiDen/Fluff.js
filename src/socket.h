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
#include <SFML/Network.hpp>
#include "wrap.h"

using namespace v8;
using namespace std;


// Socket ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
#ifndef SOCKET_H
#define SOCKET_H

class Socket: WrappedClass {
    private:
        vector< Persistent<String> > sendQueue;
        
        sf::SocketTCP socket;
        void call(const char *name, Handle<Value> *args, int argc);
    
    protected:
        void destroy();
    
    public:
        int status;
        
        Socket();
        void handle();
        void close();
        
        static Handle<Value> create(const Arguments& args);
        static Handle<Value> connect(const Arguments& args);
        static Handle<Value> send(const Arguments& args);
        static Handle<Value> close(const Arguments& args);

};

vector<Socket*> sockets;

#endif

