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


// Fluff.js Simple Sockets -----------------------------------------------------
// -----------------------------------------------------------------------------
var chr = String.fromCharCode;

fluff.network.FluffSocket = function() {
    var that = this;
    var buffer = '';
    var messageSize = -1;
    
    // Socket
    var socket = new fluff.network.Socket();
    socket.onConnect = function() {
        that.onConnect();
    };
    socket.onData = function(data) {
        buffer += data;
        that.handleData();
    };
    
    socket.onError = function() {
        that.onError();
    };
    
    socket.onClose = function() {
        data = '';
        messageSize = -1;
        that.onClose();
    };
    
    // Methods
    this.connect = function(host, port) {
        socket.connect(host, port);
    };
    
    this.send = function(msg) {
        var size = msg.length;
        socket.send(chr((size >> 16) & 0xffff) + chr(size & 0xffff) + msg);
    };
    
    this.close = function() {
        data = '';
        messageSize = -1;  
        socket.close();
    };
    
    this.handleData = function() {
        var more = true;
        while (more) {
            more = false;
            var len = buffer.length;
            if (messageSize === -1) {
                if (len >= 2) {
                    messageSize = (buffer.charCodeAt(0) << 16) + buffer.charCodeAt(1);
                    buffer = buffer.substr(2);
                }
            }
            if (messageSize !== -1 && len >= messageSize) {
                that.onMessage(buffer.substr(0, messageSize));
                buffer = buffer.substr(messageSize);
                messageSize = -1;
                more = true;
            }
        }
    };
    
    // Handlers
    this.onMessage = function(msg) {};
    this.onConnect = function() {};
    this.onClose = function() {};
    this.onError = function() {}; 
};

