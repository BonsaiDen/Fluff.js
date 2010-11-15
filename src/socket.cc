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

#define SOCKET_UNWRAP(obj) (Socket*)External::Unwrap(obj->ToObject()->GetInternalField(0));

#include <v8.h>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <string>
#include <iostream>
#include "socket.h"

using namespace std;





// Socket -------------------------------------------------------------------
// -----------------------------------------------------------------------------
Socket::Socket(string host, int port) {
    status = 0;
    this->host = host;
    this->port = port;
    sockets.push_back(this);
}

void Socket::connect() {
    if (status == 0) {
        const sf::IPAddress address(host);
        sf::Socket::Status result = socket.Connect(port, address, 2.0f);        
        if (result == sf::Socket::Done) {
            socket.SetBlocking(false);
            status = 2;
        
        } else {
            status = 3;
        }
    }
}

void Socket::call(const char *name, Handle<Value> *args, int argc) {
    HandleScope scope;
    Handle<Value> object = this->self->Get(String::NewSymbol(name));
    if (object->IsFunction()) {
        Handle<Function> func = Handle<Function>::Cast(object);
        func->Call(this->self, argc, args);
    }
}

void Socket::handle() {
    if ((status == 2 || status == 1) && socket.IsValid()) {
        HandleScope scope;
        
        // Send queued data
        if (status == 1) {
            for(unsigned int i = 0; i < sendQueue.size(); i++) {
                cout << "sending data" << endl;
                String::Utf8Value str(sendQueue.at(i));
            }
            sendQueue.clear();
        }
    
        // Receive data
        char buf[1024];
        size_t received;
        
        sf::Socket::Status result = socket.Receive(buf, sizeof(buf), received);
        switch (result) {
            case sf::Socket::Done:
                if (received > 0 && status == 1) {
                    Handle<Value> args[1];
                    args[0] = String::New(buf);
                    call("onData", args, 1);
                }
                break;
            
            case sf::Socket::Error:
                status = 3;
                socket.Close();
                call("onError", NULL, 0);
                cout << "Socket Error" << endl;
                break;
            
            case sf::Socket::NotReady:
                if (status == 2) {
                    status = 1;
                    Handle<Value> args[1];
                    args[0] = Boolean::New(true);
                    call("onConnect", args, 1);
                }
                break;
            
            case sf::Socket::Disconnected:
                close();
                cout << "Socket Disconnected" << endl;
                break;
            
            default:
                break;
        }
    }
}

Handle<Value> Socket::send(const Arguments& args) {
    Socket *socket = WrappedClass::unwrap<Socket>(args.This());
    if (socket->status != 3) {
        cout << "adding data" << endl;
        socket->sendQueue.push_back(args[0]->ToString());
        return Boolean::New(true);
    
    } else {
        return Boolean::New(false);
    }
}

Handle<Value> Socket::close(const Arguments& args) {
    Socket *socket = WrappedClass::unwrap<Socket>(args.This());
    if (socket->status == 1) {
        socket->close();
        return Boolean::New(true);
    
    } else {
        return Boolean::New(false);
    }
}

void Socket::close() {
    if (status != 3) {
        status = 3;
        socket.Close();
        call("onClose", NULL, 0);
    }
}

Handle<Value> Socket::create(const Arguments& args) {
    HandleScope scope;
    if (args.IsConstructCall()) {
        String::Utf8Value host(args[0]->ToString());
        Socket *socket = new Socket(string(*host), ToInt32(args[1]));
        
        Persistent<ObjectTemplate> tmp = socket->createTemplate();
        tmp->Set(String::New("send"), FunctionTemplate::New(Socket::send));
        return socket->wrap(tmp);
    
    } else {
        return Undefined();
    }
}

void Socket::destroy() {
    for(unsigned int i = 0; i < sockets.size(); i++) {
        Socket *csocket = sockets.at(i);
        if (csocket == this) {
            sockets.erase(sockets.begin() + i);
            delete this;
            break;
        }
    }
}

