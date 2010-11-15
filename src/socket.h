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
#include <SFML/Network.hpp>

#include <string>
#include <iostream>

using namespace std;


// Socket -------------------------------------------------------------------
// -----------------------------------------------------------------------------
#ifndef Socket_H
#define Socket_H

class Socket {
    public:
        string host;
        int port;
        
        vector< Local<String> > sendQueue;
        
        // 0 = Waiting for connection, 1 = Connected, 2 = pending, 3 = closed
        int status;
        Persistent<Object> self;
        
        sf::SocketTCP socket;
        Socket(string host, int port);
        void connect();
        void handle();
        void call(const char *name, Handle<Value> *args, int argc);
        void close();
        
        static Handle<Value> send(const Arguments& args);
        static Handle<Value> close(const Arguments& args);
        
        static Handle<Value> wrap(const Arguments& args);
        static void unwrap(Persistent<Value> value, void *parameter);

};

vector<Socket*> sockets;

#endif





