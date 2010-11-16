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
#include <SFML/Network.hpp>
#include <string>
#include "util.h"
#include "socket.h"

using namespace v8;


// Socket ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
Handle<Value> Socket::create(const Arguments& args) {
    HandleScope scope;
    if (args.IsConstructCall() && args.Length() == 2) {
        String::Utf8Value host(args[0]->ToString());
        Socket *socket = new Socket(string(*host), ToInt32(args[1]));
        
        if (!socket->hasTemplate()) {
            Persistent<ObjectTemplate> tmp = socket->createTemplate();
            tmp->Set(String::New("send"), FunctionTemplate::New(Socket::send));
            tmp->Set(String::New("close"), FunctionTemplate::New(Socket::close));            
        }
        return socket->wrap();
    
    } else {
        return Undefined();
    }
}

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
        
            // send every item, if one fails break and try again on the next frame
            for(unsigned int i = 0; i < sendQueue.size(); i++) {
                Persistent<String> data = sendQueue.at(i);
                int byteLength = data->Utf8Length();
                char *buffer = (char*)malloc(byteLength);
                data->WriteUtf8(buffer, -1);
                
                if (socket.Send(buffer, byteLength) == sf::Socket::Done) {
                    free(buffer);
                    data.Dispose();
                    sendQueue.erase(sendQueue.begin() + i);
                    i--;
                
                } else {
                    free(buffer);
                    break;
                }
            }
        }
    
        // Receive data
        char buf[1024];
        size_t received;
        sf::Socket::Status result = socket.Receive(buf, sizeof(buf), received);
        
        Handle<Value> args[1];
        switch (result) {
            case sf::Socket::Done:
                if (status == 2) {
                    status = 1;
                    args[0] = Boolean::New(true);
                    call("onConnect", args, 1);
                }
                args[0] = String::New(buf, received);
                call("onData", args, 1);
                break;
            
            case sf::Socket::Error:
                status = 3;
                socket.Close();
                call("onError", NULL, 0);
                break;
            
            case sf::Socket::NotReady:
                if (status == 2) {
                    status = 1;
                    args[0] = Boolean::New(true);
                    call("onConnect", args, 1);
                }
                break;
            
            case sf::Socket::Disconnected:
                close();
                break;
            
            default:
                break;
        }
    }
}

Handle<Value> Socket::send(const Arguments& args) {
    Socket *socket = WrappedClass::unwrap<Socket>(args.This());
    if (socket->status != 3 && args[0]->IsString()) {
        socket->sendQueue.push_back(Persistent<String>::New(args[0]->ToString()));
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

